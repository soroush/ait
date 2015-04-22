/*
 AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.

 Copyright (c) 2012,2013 Soroush Rabiei <soroush-r@users.sf.net>,
 Roya Ghasemzadeh <ghasemzadeh.roya1@gmail.com>

 AIT is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 See the file COPYING included with this distribution for more
 information.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <config.h>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <cmath>
#include <fstream>
#include <config.h>
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
#define BOOST_ALL_DYN_LINK
#define DSO
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/format.hpp>
#endif
#include "abt-solver.hpp"
#include "assignment.hpp"
#include "abt-solver.h"

using namespace std;
using namespace zmq;
using namespace AIT::CSP;
using namespace AIT::protocols::csp;
using namespace AIT::protocols::csp::abt;
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;
boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
#endif

ABT_Solver::ABT_Solver(const string& serverHost_,
        const unsigned short& serverResponderPort_,
        const unsigned short& serverPublisherPort_, const string& name_,
        const string& xcspFile, const string& logfile) :
        name(name_),
        serverAddress(serverHost_),
        serverResponderPort(serverResponderPort_),
        serverPublisherPort(serverPublisherPort_),
        context(2),
        listener { context, ZMQ_PULL },
        serverRquest { context, ZMQ_REQ },
        serverBroadcast { context, ZMQ_SUB },
        end(false) {
    int linger = 0;
    listener.get_zmq_socket().setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    serverRquest.get_zmq_socket().setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    serverBroadcast.get_zmq_socket().setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    this->parseFromFile(xcspFile);
    this->priority = this->instance->variableIndex(this->name);
    this->me = this->instance->variable(this->name);
    this->sequences.assign(this->instance->variables().size(), 0);
    this->prune();
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    logging::add_common_attributes();
    logging::register_simple_formatter_factory<logging::trivial::severity_level,
            char>("Severity");
    if (logfile == "std::cout") {
        logging::register_simple_formatter_factory<
                logging::trivial::severity_level, char>("Severity");
        logging::add_console_log(std::cout, boost::log::keywords::format =
                "[%TimeStamp%][%Severity%]: %Message%");
    }
    else {
        logging::add_file_log(keywords::file_name = logfile,
                keywords::auto_flush = true, keywords::format =
                        "[%TimeStamp%][%Severity%]: %Message%");
    }
#endif
}

ABT_Solver::~ABT_Solver() {
    // FIXME: Close socket gracefuly
    // context.close();
}

void ABT_Solver::ABT() {
    me->unset();
    this->connect();
    this->getAgentList();
    this->m_sentinel = std::thread { &ABT_Solver::_sentinel, this };
    this->checkAgentView();
    while (!end) {
        Message m;
        m.readFromProtocol(getMessage());
        switch (m.type) {
        case P_MessageType::T_OK:
            processInfo(m);
            break;
        case P_MessageType::T_NOGOOD:
            resolveConflict(m);
            break;
        case P_MessageType::T_ADDLINK:
            setLink(m);
            break;
        case P_MessageType::T_STOP:
            end = true;
            finished = false;
            break;
        case P_MessageType::T_FINISH:
            end = true;
            finished = true;
            break;
        default:
            end = true;
            finished = false;
            break;
        }
    }
    this->m_sentinel.join();
    this->messageReader.join();
}

void ABT_Solver::connect() {
    messageReader = thread(&ABT_Solver::_messageReader, this);
    this->agentReadyLock.wait();
    // Now we'll connect to server (monitor agent)
    stringstream addressName;
    addressName.str(string());
    addressName << "tcp://" << this->serverAddress << ':'
            << this->serverResponderPort;
    try {
        serverRquest.get_zmq_socket().connect(addressName.str().data());
    }
    catch (zmq::error_t &e) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, error)<< "Unable to connect to monitor. Exiting gracefully";
        BOOST_LOG_SEV(lg, error) << e.what();
#endif
        return;
    }
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, trace)<<
    boost::format("Successfully connected to monitor agent at %1%:%2%.") % this->serverAddress.data() % this->serverResponderPort;
    // Now connect to broadcast port of monitor
#endif
    addressName.str(string());
    addressName << "tcp://" << this->serverAddress << ':'
            << this->serverPublisherPort;
    try {
        serverBroadcast.get_zmq_socket().connect(addressName.str().data());
        serverBroadcast.get_zmq_socket().setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }
    catch (zmq::error_t &e) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, error)<< "Unable to connect to broadcast channel of monitor agent.";
        BOOST_LOG_SEV(lg, error) << e.what() << endl;
#endif
        return;
    }
#include <config.h>
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< "Successfully connected to broadcasting channel "
    "of monitor agent on "
    << this->serverAddress << ':'
    << this->serverPublisherPort << ", "
    << addressName.str();
#endif
    // Send agent information to server
    P_CommunicationProtocol introPacket;
    introPacket.set_type(CP_MessageType::T_INTRODUCE);
    introPacket.mutable_identity()->set_host(this->address);
    introPacket.mutable_identity()->set_port(this->port);
    introPacket.mutable_identity()->set_name(this->name);
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< "Sending introduction message to monitor...";
#endif
    if (this->serverRquest.sendMessage(introPacket)) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, info)<<
        "Introduction message sent to monitor agent. Waiting for reply...";
#endif
        P_CommunicationProtocol introAckPacket;
        if (this->serverRquest.recvMessage(introAckPacket)) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
            BOOST_LOG_SEV(lg, info) << "Message received from monitor";
#endif
            if (introAckPacket.type() == CP_MessageType::T_INTRODUCE_ACK) {
                this->priority = introAckPacket.priority();
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
                BOOST_LOG_SEV(lg, info) <<
                "Monitor agent accepted connection. Current priority is: "
                << this->priority;
#endif
            }
            else if (introAckPacket.type()
                    == CP_MessageType::ERR_REPEATED_ID) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
                BOOST_LOG_SEV(lg, error)
                <<"An agent with current name ("
                << this->name
                <<") is already registered. Exiting gracefully";
#endif
                return;
            }
        }
        else {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
            BOOST_LOG_SEV(lg, error) << "Server didn't reply. Exiting gracefully";
#endif
        }
    }
    else {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, error) << "Unable to send introduction message.";
#endif
    }
}

void ABT_Solver::checkAgentView(const int& priority_) {
    printAV();
    if (!consistent()) {
        if (me->isSet()) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
            BOOST_LOG_SEV(lg, info)<<
            boost::format(
                    "Current value (%1%) is not consistent with recently "
                    "updated agent view.") % this->me->getValue();
#endif
        }
        else {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
            BOOST_LOG_SEV(lg, info) << "No value has been assigned. Trying to find a value";
#endif
        }

        chooseValue();
        if (me->isSet()) {
            if (priority_ != 0) {
                sendMessageOK_Monitor(priority_, true);
            }
            for (const auto& agent : m_gammaPlus) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
                BOOST_LOG_SEV(lg, info) <<
                    boost::format("Sending ok message to %1%, %2%=%3%")
                % agent->priority() % this->priority % this->me->getValue();
#endif
                sendMessageOK(agent->priority());
            }
        }
        else {
            backtrack();
        }
    }
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< "Current value is consistent with agent view. Doing nothing";
#endif
    if (priority_ != 0) {
        sendMessageOK_Monitor(priority_, true);
    }
}

void ABT_Solver::processInfo(const Message& m) {
    this->sequences[m.assignment.id - 1] = m.sequence;
    sendMessageOK_Monitor(m.assignment.id, false);
    // TODO: Add assignment here
    this->priority2Variable[m.assignment.id]->setValue(m.assignment.value);
    updateAgentView();
    checkAgentView(m.assignment.id);
}

void ABT_Solver::chooseValue() {
    me->unset();
    for (auto &possibleValue : this->me->getDomain()->getValues()) {
        bool eliminated = false;
        eliminated =
                any_of(noGoodStore.begin(), noGoodStore.end(),
                        [&](const Nogood &ngd) {
                            return (ngd.rhs.id==this->priority and ngd.rhs.value==possibleValue);
                        });
        if (!eliminated) {
            if (consistentGammaMinus(possibleValue)) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
                BOOST_LOG_SEV(lg, info)<< boost::format("A new value has been assigned: %1%") % possibleValue;
#endif
                me->setValue(possibleValue);
                sequences[this->priority - 1]++;
                sendMessageOK_Monitor(this->priority, true);
                return;
            }
            else {
                Variable* culprit = findCulprit(possibleValue);
                Nogood ngd;
                ngd.lhs.items.insert(
                        Assignment(variable2Priority[culprit],
                                culprit->getValue()));
                ngd.rhs.id = this->priority;
                ngd.rhs.value = possibleValue;
                noGoodStore.push_back(ngd);
            }
        }
    }
}

void ABT_Solver::backtrack() {
    CompoundAssignment newNogood = solve();
    if (newNogood.items.empty()) {
        end = true;
        sendMessageSTP();
    }
    else {
        Message nogoodMessage;
        nogoodMessage.nogood = newNogood;

        AgentID lastCulpirt = findLastCulprit();
        sendMessageNGD(lastCulpirt, nogoodMessage);
        this->priority2Variable[lastCulpirt]->unset();
        updateAgentView();
        checkAgentView();
    }
}

void ABT_Solver::updateAgentView() {
    // Remove invalid nogoods
    noGoodStore.remove_if([&](Nogood& ngd)->bool {return !coherent(ngd.lhs);});
}

void ABT_Solver::resolveConflict(const Message& msg) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< boost::format("Nogood received: %1%")% msg.sender;
#endif
    printNG(msg.nogood);
    if (coherentGammaMinus(msg.nogood)) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, info) << boost::format("Received nogood was coherent.");
#endif
        checkAddLink(msg);
        add(msg.nogood);
        me->unset();
        checkAgentView();
    }
    else if (coherentSelf(msg.nogood)) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, info) << "Received nogood was coherent with current assignment.";
#endif
        sendMessageOK(msg.sender);
    }
    else {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, info) << boost::format(
                "Received nogood was not coherent with me and my agent view. Dropping message.");
#endif
    }
}

P_Message ABT_Solver::getMessage() {
    this->messageCount.wait();
    this->messageRW.lock();
    P_Message x = this->messageQueue.front();
    this->messageQueue.pop();
    this->messageRW.unlock();
    return x;
}

void ABT_Solver::setLink(const Message& message) {
    bool repeated = any_of(m_gammaPlus.begin(), m_gammaPlus.end(),
            [&](const std::vector<EndPoint>::iterator i)->bool
            {   return i->priority() == message.sender;});
    if (!repeated) {
        auto index = find_if(everybody.begin(), everybody.end(),
                [&](const P_EndPoint& agent)->bool
                {   return agent.priority()==message.sender;});
        m_gammaPlus.push_back(index);
    }
}

void ABT_Solver::checkAddLink(const Message& message) {
    for (const auto& assignment : message.nogood.items) {
        if (!any_of(m_gammaMinus.begin(), m_gammaMinus.end(),
                [&](const std::vector<EndPoint>::iterator i)->bool
                {   return i->priority()==assignment.id;})) {
            sendMessageADL(assignment.id);
            auto x = std::find_if(everybody.begin(), everybody.end(),
                    [&](const EndPoint& e)->bool {
                        return e.priority()== assignment.id;});
            m_gammaMinus.push_back(x);
            priority2Variable[assignment.id]->setValue(assignment.value);
        }
    }
}

bool ABT_Solver::coherent(const CompoundAssignment& nogood) {
    for (const auto &n : nogood.items) {
        if (n.id <= this->priority) {
            if (!priority2Variable[n.id]->isSet())
                return false;
            else if (priority2Variable[n.id]->getValue() != n.value) {
                return false;
            }
        }
    }
    return true;
}

bool ABT_Solver::coherentGammaMinus(const CompoundAssignment& nogood) {
    for (const auto &n : nogood.items) {
        auto foundGamma = std::find_if(this->m_gammaMinus.begin(),
                this->m_gammaMinus.end(),
                [&](const std::vector<EndPoint>::iterator i)->bool
                {   return n.id == i->priority();});
        if (foundGamma != m_gammaMinus.end()) {
            if (!priority2Variable[n.id]->isSet())
                return false;
            else if (priority2Variable[n.id]->getValue() != n.value) {
                return false;
            }
        }
    }
    return true;
}

bool ABT_Solver::coherentSelf(const CompoundAssignment& nogood) {
    if (!this->me->isSet()) {
        return false;
    }
    for (const auto &n : nogood.items) {
        if (n.id == this->priority and n.value != me->getValue())
            return false;
    }
    return true;
}

void ABT_Solver::getAgentList() {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< boost::format("Sending request to to monitor agent to get agent list...");
#endif
    P_CommunicationProtocol requestList;
    requestList.set_type(CP_MessageType::T_REQUEST_LIST);
    requestList.set_priority(this->priority);
    this->serverRquest.sendMessage(requestList);

    P_CommunicationProtocol requestListAck;
    this->serverRquest.recvMessage(requestListAck);
    if (requestListAck.type() == CP_MessageType::T_REQUEST_ACK) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, info) << boost::format("Monitor has accepted to send list of agents.");
#endif
    }
    else {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
        BOOST_LOG_SEV(lg, error) << "Unable to get agent list from monitor. Exiting gracefully.";
#endif
    }
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info) << "Waiting for all agents to came online...";
#endif
    P_CommunicationProtocol listPacket;
    this->serverBroadcast.recvMessage(listPacket);
    if (listPacket.type() == CP_MessageType::T_LIST) {
        for (int i = 0; i < listPacket.others_size(); ++i) {
            int p = listPacket.others(i).priority();
            this->everybody.push_back(
                    EndPoint {listPacket.others(i), context});
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
            BOOST_LOG_SEV(lg, info) << boost::format(
                    "New agent introduced by server:\n"
                    "\tID:        %1%\n"
                    "\tHost:      %2%\n"
                    "\tVariable:  %3%\n"
                    "\tPort:      %4%")
            % listPacket.others(i).priority()
            % listPacket.others(i).host()
            % listPacket.others(i).name()
            % listPacket.others(i).port();
#endif
        }
        for (auto i = everybody.begin(); i < everybody.end(); ++i) {
            Variable* var = this->instance->variable(i->name());
            this->priority2Variable.insert(
                    pair<size_t, Variable*>(i->priority(), var));
            this->variable2Priority.insert(
                    pair<Variable*, size_t>(var, i->priority()));
            bool foundAConstraint = false;
            for (const auto& constraint : this->myConstraints) {
                if (any_of(constraint->Scope().begin(),
                                constraint->Scope().end(), [&](const Variable* v)->bool
                                {   return v == var;})) {
                    foundAConstraint = true;
                    break;
                }
            }
            if (i->priority() < this->priority and foundAConstraint)
            m_gammaMinus.push_back(i);
            else if (i->priority() > this->priority and foundAConstraint)
            m_gammaPlus.push_back(i);
            else if (i->priority() == this->priority)
            currentAgent = i;
        }
        for (auto &agentEndPoint : everybody) {
            stringstream address;
            address << "tcp://";
            address << agentEndPoint.host() << ":" << agentEndPoint.port();
            agentEndPoint.socket()->get_zmq_socket().connect(address.str().c_str());
        }
    }
}

void ABT_Solver::sendMessageOK(const AgentID& agent) {
    Message ok;
    ok.sender = this->priority;
    ok.type = P_MessageType::T_OK;
    ok.assignment = Assignment { this->priority, me->getValue() };
    ok.sequence = this->sequences[this->priority - 1];
    sendMessage(agent, ok);
}

void ABT_Solver::sendMessageNGD(const AgentID& agent, Message& ngd) {
    ngd.sender = this->priority;
    ngd.type = P_MessageType::T_NOGOOD;
    sendMessage(agent, ngd);
}

void ABT_Solver::sendMessageSTP() {
    Message stop;
    stop.type = P_MessageType::T_STOP;
    stop.sender = this->priority;
    sendMessage(0, stop);
}

void ABT_Solver::sendMessageADL(const AgentID& agent) {
    if (agent == this->priority)
        return;
    Message adl;
    adl.sender = this->priority;
    adl.type = P_MessageType::T_ADDLINK;
    sendMessage(agent, adl);
}

CompoundAssignment ABT_Solver::solve() {
    CompoundAssignment newNogood;
    for (const auto& ngd : this->noGoodStore) {
        for (const auto& a : ngd.lhs.items) {
            newNogood.items.insert(a);
        }
    }
    return newNogood;
}

void ABT_Solver::sendMessage(const AgentID& agent, const Message& message) {
    if (agent == this->priority)
        return;
    if (agent == 0) {
        for (const auto &ep : this->everybody) {
            stringstream address;
            ep.socket()->sendMessage(message);
            return;
        }
    }
    else {
        for (const auto &ep : this->everybody) {
            if (ep.priority() == agent) {
                ep.socket()->sendMessage(message);
            }
        }
    }
}

void ABT_Solver::add(const CompoundAssignment& ca) {
    Nogood ngd;
    for (const auto& a : ca.items) {
        if (a.id != this->priority) {
            ngd.lhs.items.insert(a);
        }
        else {
            ngd.rhs = a;
        }
    }
    noGoodStore.push_back(ngd);
}

void ABT_Solver::printNGS() {
    stringstream ss;
    ss << "Current Nogood Store is: " << endl;
    for (const auto& ngd : noGoodStore) {
        for (const auto& a : ngd.lhs.items)
            ss << '(' << a.id << ',' << a.value << "),";
        ss << "=>" << ngd.rhs.id << "!=" << ngd.rhs.value << endl
                << "                                        ";
    }
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< ss.str();
#endif
}

void ABT_Solver::printNG(const CompoundAssignment& ca) {
    stringstream ss;
    ss << "Nogood:" << endl;
    for (const auto& a : ca.items) {
        ss << a.id << " = " << a.value << endl
                << "                                        ";
    }
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< ss.str();
#endif
}

void ABT_Solver::printAV() {
    stringstream ss;
    ss << "Agent View:" << endl;
    for (const auto& v : this->instance->variables()) {
        if (v->isSet())
            ss << "    > " << v->getName() << '=' << v->getValue() << endl;
        else
            ss << "    > " << v->getName() << '=' << "<unassigned>" << endl;
    }
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< "%s", ss.str();
#endif
}

bool ABT_Solver::consistentGammaMinus(const int& possibleValue) {
    me->setValue(possibleValue);
    for (const auto& constraint : this->myConstraints) {
        bool allSet = all_of(constraint->Scope().begin(),
                constraint->Scope().end(), [&](const Variable* v)->bool
                {   return v->isSet();});
        bool allInGammaMinus = all_of(constraint->Scope().begin(),
                constraint->Scope().end(), [&](const Variable* v)->bool
                {   return findInGammaMinus(v) || v==me;});

        if (allSet and allInGammaMinus) {
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
            BOOST_LOG_SEV(lg, info)<< "Found a constraint to check against";
#endif
            if (!constraint->satisfies()) {
                return false;
            }
        }
    }
    return true;
}

bool ABT_Solver::consistent(const int& possibleValue) {
    me->setValue(possibleValue);
    for (const auto& constraint : this->myConstraints) {
        bool allSet = all_of(constraint->Scope().begin(),
                constraint->Scope().end(), [&](const Variable* v)->bool
                {   return v->isSet();});
        if (allSet) {
            if (!constraint->satisfies()) {
                return false;
            }
        }
    }
    return true;
}

bool ABT_Solver::consistent() {
    if (!me->isSet()) {
        return false;
    }
    else {
        return this->consistent(this->me->getValue());
    }
}

Variable* ABT_Solver::findCulprit(const int& v) {
    me->setValue(v);
    string maxName;
    int max = 0;
    for (const auto& constraint : this->myConstraints) {
        if (all_of(constraint->Scope().begin(), constraint->Scope().end(),
                [&](const Variable* v)->bool
                {   return v->isSet();})) {
            if (!constraint->satisfies()) {
                for (const auto& variable : constraint->Scope()) {
                    if (variable != me) {
                        if (variable2Priority[variable] > max) {
                            maxName = variable->getName();
                            max = variable2Priority[variable];
                            //cout << "Current Culprit: " << maxName << " " << max << endl;
                        }
                    }
                }
            }
        }
    }
//    cout << "Final Culprit: " << maxName << " "
//            << this->instance->variable(maxName)->getValue() << endl;
    me->unset();
    return this->instance->variable(maxName);
}

int ABT_Solver::findLastCulprit() {
    AgentID maximum = 0;
    for (const auto& nogood : noGoodStore) {
        for (const auto& item : nogood.lhs.items) {
            maximum = item.id > maximum ? item.id : maximum;
        }
    }
    return maximum;
}

void ABT_Solver::prune() {
    for (const auto& c : this->instance->constraints()) {
        for (const auto& v : c->Scope()) {
            if (v->getName() == this->name) {
                myConstraints.push_front(c.get());
                break;
            }
        }
    }
}

void ABT_Solver::_messageReader() {
    // Let's listen to other agents:
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< "Get ready to listen to other agents...";
#endif
    //solver->listener = Socket(solver->context, ZMQ_PULL);
    stringstream addressName;
    this->address = "127.0.0.1";
    addressName << "tcp://" << this->address << ":*";
    try {
        this->listener.get_zmq_socket().bind(addressName.str().data());
    }
    catch (zmq::error_t &e) {
        cerr << e.what() << endl;
    }
    size_t len = 255;
    char endpoint[len];
    // FIXME: Use older versions instead
    // this->listener.getsockopt(ZMQ_LAST_ENDPOINT, &endpoint, &len);
    char * token, *last;
    token = strtok(endpoint, ":");
    while (token != NULL) {
        last = token;
        token = strtok(NULL, ":");
    }
    this->port = static_cast<unsigned int>(atoi(last));
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info) <<
    boost::format("Socket successfully created. Now listening on %1%:%2%")%
    this->address% this->port;
    this->agentReadyLock.notify();
#endif
    while (true) {
        P_Message message;
        this->listener.recvMessage(message);
        this->messageRW.lock();
        this->messageQueue.push(message);
        this->messageRW.unlock();
        this->messageCount.notify();
        if (message.type() == P_MessageType::T_STOP
                || message.type() == P_MessageType::T_FINISH) {
            return;
        }
    }
}

void ABT_Solver::_sentinel() {
    P_Message messagePack;
    this->serverBroadcast.recvMessage(messagePack);
    if (messagePack.type() == P_MessageType::T_FINISH
            or messagePack.type() == P_MessageType::T_STOP) {
        this->currentAgent->socket()->sendMessage(messagePack);
    }
}

void ABT_Solver::sendMessageOK_Monitor(const int& p, const bool& approved) {
    // Send value to monitor:
    Message okMessage;
    okMessage.type = P_MessageType::T_OK_MONITOR;
    okMessage.sender = this->priority;
    okMessage.sequence = this->sequences[p - 1];
    okMessage.approved = approved;
    Assignment a;
    a.id = p;
    if (p == this->priority)
        a.value = this->me->getValue();
    else
        a.value = 0;
    okMessage.assignment = a;
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< boost::format("Sending OK_MONITOR: (%1%, %2%, %3%).")
    % okMessage.sender % okMessage.sequence % okMessage.assignment.id;
#endif
    this->serverRquest.sendMessage(okMessage);
    P_Message ack;
    this->serverRquest.recvMessage(ack);
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
    BOOST_LOG_SEV(lg, info)<< "OK_MONITOR has been acknowledged now.";
#endif
}

bool ABT_Solver::findInGammaMinus(const Variable* v) {
    size_t p = variable2Priority[v];
    for (const auto& gm : this->m_gammaMinus) {
        if (gm->priority() == p)
            return true;
    }
    return false;
}

/* ********************************** C API **********************************
 *
 * This is C wrapper around C++ implementation API.
 *
 * **************************************************************************/

C_ABT_Solver* abt_solver_create(char* serverHost,
        const unsigned short serverResponderPort,
        const unsigned short serverPublisherPort, const char* name,
        const char* xcspFile, const char* logFile) {
    return reinterpret_cast<C_ABT_Solver*>(new ABT_Solver(serverHost,
            serverResponderPort, serverPublisherPort, name, xcspFile, logFile));
}

void abt_solver_destrory(C_ABT_Solver* s) {
    delete reinterpret_cast<ABT_Solver*>(s);
}

void abt_solver_solve(C_ABT_Solver* s) {
    reinterpret_cast<ABT_Solver*>(s)->ABT();
}
