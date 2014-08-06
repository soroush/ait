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

#include <algorithm>
#include <stdexcept>
#include <memory>

#include "cc-abt-solver.h"
#include "assignment.h"

using namespace std;
using namespace zmq;
using namespace AIT::CSP;
using namespace AIT::protocols::csp;
using namespace AIT::protocols::csp::ccabt;

CC_ABT_Solver::CC_ABT_Solver(const std::string& serverHost_,
        const unsigned short& serverResponderPort_,
        const unsigned short& serverPublisherPort_, const std::string& name_,
        const std::string& xcspFile) :
        name(name_), serverAddress(serverHost_), serverResponderPort(
                serverResponderPort_), serverPublisherPort(
                serverPublisherPort_), context(2), listener { context, ZMQ_PULL }, serverRquest {
                context, ZMQ_REQ }, serverBroadcast { context, ZMQ_SUB }, end(
                false) {
    int linger = 0;
    listener.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    serverRquest.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    serverBroadcast.setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
    this->parseFromFile(xcspFile);
    this->priority = this->instance->variableIndex(this->name);
    this->me = this->instance->variable(this->name);
    this->sequences.assign(this->instance->variables().size(), 0);
    this->prune();
}

CC_ABT_Solver::~CC_ABT_Solver() {
//    pthread_mutex_destroy(&this->messageRW);
//    sem_destroy(&agentReadyLock);
//    sem_destroy(&messageCount);
    serverBroadcast.close();
    serverRquest.close();
    listener.close();
//    for (const auto& a : this->everybody) {
//        a.socket()->close();
//    }
    context.close();
}

void CC_ABT_Solver::ABT() {
    me->unset();
    this->connect();
    this->getAgentList();
    this->m_sentinel = std::thread { &CC_ABT_Solver::_sentinel, this };
    this->checkAgentView();
    while (!end) {
        Message m = getMessage();
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

void CC_ABT_Solver::connect() {
    messageReader = thread(&CC_ABT_Solver::_messageReader, this);
    this->agentReadyLock.wait();
    // Now we'll connect to server (monitor agent)
    stringstream addressName;
    addressName.str(string());
    addressName << "tcp://" << this->serverAddress << ':'
            << this->serverResponderPort;
    try {
        serverRquest.connect(addressName.str().data());
    } catch (zmq::error_t &e) {
        _ERROR("Unable to connect to monitor.\n"
                "\t\tTerminating process\n"
                "\t\tSee below details for more information:\n")
        cerr << e.what() << endl;
        return;
    }
    _INFO("Successfully connected to monitor agent at %s:%d",
            this->serverAddress.data(), this->serverResponderPort);

    // Now connect to broadcast port of monitor
    addressName.str(string());
    addressName << "tcp://" << this->serverAddress << ':'
            << this->serverPublisherPort;
    try {
        serverBroadcast.connect(addressName.str().data());
        serverBroadcast.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    } catch (zmq::error_t &e) {
        _ERROR("Unable to connect to broadcast channel of monitor agent.\n"
                "\t\tTerminating process\n"
                "\t\tSee below details for more information:")
        cerr << e.what() << endl;
        return;
    }
    _INFO(
            "Successfully connected to broadcasting channel of " "monitor agent at %s:%d, (%s)",
            this->serverAddress.data(), this->serverPublisherPort,
            addressName.str().data());
    // Send agent information to server
    P_CommunicationProtocol introPacket;
    introPacket.set_type(CP_MessageType::T_INTRODUCE);
    introPacket.mutable_identity()->set_host(this->address);
    introPacket.mutable_identity()->set_port(this->port);
    introPacket.mutable_identity()->set_name(this->name);
    _INFO("Sending introduction message to monitor...");
    if (this->serverRquest.sendMessage(introPacket)) {
        _INFO(
                "Introduction message sent to monitor agent. Waiting for reply...");
        P_CommunicationProtocol introAckPacket;
        if (this->serverRquest.recvMessage(introAckPacket)) {
            _INFO("Message received from monitor");
            if (introAckPacket.type() == CP_MessageType::T_INTRODUCE_ACK) {
                this->priority = introAckPacket.priority();
                _INFO(
                        "Monitor agent accepted connection. Current priority is: %zd",
                        this->priority);
            } else if (introAckPacket.type()
                    == CP_MessageType::ERR_REPEATED_ID) {
                _ERROR(
                        "An agent with current name (`%s') is already registered.\n"
                                "\t\tMonitor agent has ignored this request.\n"
                                "\t\tExiting silently.", this->name.c_str());
                return;
            }
        } else {
            _ERROR("Server didn't reply. Terminating silently");
        }
    } else {
        _ERROR("Unable to send introduction message.");
    }
}

void CC_ABT_Solver::checkAgentView(const int& priority_) {
    printAV();
    if (!consistent()) {
        if (me->isSet()) {
            _INFO(
                    "Current value %d is not consistent with recently updated agent view. "
                            "Trying to choose a new one.", this->me->getValue());
        } else {
            _INFO("No value has been assigned. Trying to find a value");
        }

        chooseValue();
        if (me->isSet()) {
            this->sequences[this->priority-1]++;
            if (priority_ != 0) {
                sendMessageOK_Monitor(priority_, true);
            }
            for (const auto& agent : m_gammaPlus) {
                sendMessageOK(agent->priority());
            }
        } else {
            backtrack();
        }
    }
    if (priority_ != 0) {
        sendMessageOK_Monitor(priority_, true);
    }
}

void CC_ABT_Solver::processInfo(const Message& m) {
    this->sequences[m.assignment.id - 1] = m.sequence;
    sendMessageOK_Monitor(m.assignment.id, false);
    // TODO: Add assignment here
    this->priority2Variable[m.assignment.id]->setValue(m.assignment.value);
    updateAgentView();
    checkAgentView(m.assignment.id);
}

void CC_ABT_Solver::chooseValue() {
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
                _INFO("A new value has been assigned: %d", possibleValue);
                me->setValue(possibleValue);
                sequences[this->priority - 1]++;
                sendMessageOK_Monitor(this->priority, true);
                return;
            } else {
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

void CC_ABT_Solver::backtrack() {
    CompoundAssignment newNogood = solve();
    if (newNogood.items.empty()) {
        end = true;
        sendMessageSTP();
    } else {
        Message nogoodMessage;
        nogoodMessage.nogood = newNogood;

        AgentID lastCulpirt = findLastCulprit();
        sendMessageNGD(lastCulpirt, nogoodMessage);
        this->priority2Variable[lastCulpirt]->unset();
        updateAgentView();
        checkAgentView();
    }
}

void CC_ABT_Solver::updateAgentView() {
    // Remove invalid nogoods
    noGoodStore.remove_if([&](Nogood& ngd)->bool {return !coherent(ngd.lhs);});
}

void CC_ABT_Solver::resolveConflict(const Message& msg) {
    _INFO("Nogood received: %d", msg.sender);
//    printNG(msg.nogood);
    if (coherentGammaMinus(msg)) {
        _INFO("Received nogood was coherent.");
        checkAddLink(msg);
        add(msg.nogood);
        me->unset();
        checkAgentView();
    } else if (coherentSelf(msg.nogood)) {
        _INFO("Received nogood was coherent with current assignment.");
        sendMessageOK(msg.sender);
    } else {
        _INFO(
                "Received nogood was not coherent with me and my agent view. Dropping message.");
    }
}

CC_ABT_Solver::Message CC_ABT_Solver::getMessage() {
    this->messageCount.wait();
    this->messageRW.lock();
    if (this->messageQueue.size() == 0) {
        throw std::logic_error {
                "Queue is empty. Semaphore may not worked correctly." };
    }
    Message x = this->messageQueue.front();
    this->messageQueue.pop_front();
    this->messageRW.unlock();
    return x;
}

void CC_ABT_Solver::setLink(const Message& message) {
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

void CC_ABT_Solver::checkAddLink(const Message& message) {
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

bool CC_ABT_Solver::coherent(const CompoundAssignment& nogood) {
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

bool CC_ABT_Solver::coherentGammaMinus(const Message& msg) {
    // CAUTION: This code is tricky. Please consider that i:0->this->priority
    // scans all items before current agent, plus current agent
    for (int i = 0; i < this->priority; ++i) {
        if (msg.tv[i] == 0) {
            continue;
        } else if (msg.tv[i] < this->sequences[i]) {
            return false;
        }
    }
    return true;
}

bool CC_ABT_Solver::coherentSelf(const CompoundAssignment& nogood) {
    if (!this->me->isSet()) {
        return false;
    }
    for (const auto &n : nogood.items) {
        if (n.id == this->priority and n.value != me->getValue())
            return false;
    }
    return true;
}

void CC_ABT_Solver::getAgentList() {
    _INFO("Sending request to to monitor agent to get agent list...");
    P_CommunicationProtocol requestList;
    requestList.set_type(CP_MessageType::T_REQUEST_LIST);
    requestList.set_priority(this->priority);
    this->serverRquest.sendMessage(requestList);

    P_CommunicationProtocol requestListAck;
    this->serverRquest.recvMessage(requestListAck);
    if (requestListAck.type() == CP_MessageType::T_REQUEST_ACK) {
        _INFO("Monitor has accepted to send list of agents.");
    } else {
        _ERROR("Unable to get agent list from monitor.\n"
                "\tTerminating silently. Goodbye.");
    }

    _INFO("Waiting for all agents to came online...");
    P_CommunicationProtocol listPacket;
    this->serverBroadcast.recvMessage(listPacket);
    if (listPacket.type() == CP_MessageType::T_LIST) {
        for (int i = 0; i < listPacket.others_size(); ++i) {
            int p = listPacket.others(i).priority();
            this->everybody.push_back(
                    EndPoint { listPacket.others(i), context });
            _INFO(
                    "New agent introduced by server:\n" "\tID:        %d\n" "\tHost:      %s\n" "\tVariable:  %s\n" "\tPort:      %d",
                    listPacket.others(i).priority(),
                    listPacket.others(i).host().c_str(),
                    listPacket.others(i).name().c_str(),
                    listPacket.others(i).port());
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
            agentEndPoint.socket()->connect(address.str().c_str());
        }
    }
}

void CC_ABT_Solver::sendMessageOK(const AgentID& agent) {
    Message ok;
    ok.sender = this->priority;
    ok.type = P_MessageType::T_OK;
    ok.assignment = Assignment { this->priority, me->getValue() };
    ok.sequence = this->sequences[this->priority - 1];
    ok.tv = this->sequences;
    sendMessage(agent, ok);
}

void CC_ABT_Solver::sendMessageNGD(const AgentID& agent, Message& ngd) {
    ngd.sender = this->priority;
    ngd.type = P_MessageType::T_NOGOOD;
    ngd.sequence = this->sequences[this->priority - 1];
    ngd.tv = this->sequences;
    sendMessage(agent, ngd);
}

void CC_ABT_Solver::sendMessageSTP() {
    Message stop;
    stop.type = P_MessageType::T_STOP;
    stop.sender = this->priority;
    sendMessage(0, stop);
}

void CC_ABT_Solver::sendMessageADL(const AgentID& agent) {
    if (agent == this->priority)
        return;
    Message adl;
    adl.sender = this->priority;
    adl.type = P_MessageType::T_ADDLINK;
    sendMessage(agent, adl);
}

CompoundAssignment CC_ABT_Solver::solve() {
    CompoundAssignment newNogood;
    for (const auto& ngd : this->noGoodStore) {
        for (const auto& a : ngd.lhs.items) {
            newNogood.items.insert(a);
        }
    }
    return newNogood;
}

void CC_ABT_Solver::sendMessage(const AgentID& agent, const Message& message) {
    if (agent == this->priority)
        return;
    if (agent == 0) {
        for (const auto &ep : this->everybody) {
            stringstream address;
            ep.socket()->sendMessage(message);
            return;
        }
    } else {
        for (const auto &ep : this->everybody) {
            if (ep.priority() == agent) {
                ep.socket()->sendMessage(message);
            }
        }
    }
}

void CC_ABT_Solver::add(const CompoundAssignment& ca) {
    Nogood ngd;
    for (const auto& a : ca.items) {
        if (a.id != this->priority) {
            ngd.lhs.items.insert(a);
        } else {
            ngd.rhs = a;
        }
    }
    noGoodStore.push_back(ngd);
}

void CC_ABT_Solver::printNGS() {
    stringstream ss;
    ss << "NGS: ";
    for (const auto& ngd : noGoodStore) {
        for (const auto& a : ngd.lhs.items)
            ss << '(' << a.id << ',' << a.value << "),";
        ss << "=>" << ngd.rhs.id << "!=" << ngd.rhs.value << endl
                << "                                        ";
    }
    _INFO("%s", ss.str().c_str());
}

void CC_ABT_Solver::printNG(const CompoundAssignment& ca) {
    stringstream ss;
    ss << "NG: ";
    for (const auto& a : ca.items) {
        ss << a.id << " = " << a.value << endl
                << "                                        ";
    }
    _INFO("%s", ss.str().c_str());
}

void CC_ABT_Solver::printAV() {
    stringstream ss;
    ss << "Agent View:   " << endl;
    for (const auto& v : this->instance->variables()) {
        if (v->isSet())
            ss << "    > " << v->getName() << '=' << v->getValue() << endl;
        else
            ss << "    > " << v->getName() << '=' << "<unassigned>" << endl;
    }
    _INFO("%s", ss.str().c_str());
}

bool CC_ABT_Solver::consistentGammaMinus(const int& possibleValue) {
    me->setValue(possibleValue);
    for (const auto& constraint : this->myConstraints) {
        bool allSet = all_of(constraint->Scope().begin(),
                constraint->Scope().end(), [&](const Variable* v)->bool
                {   return v->isSet();});
        bool allInGammaMinus = all_of(constraint->Scope().begin(),
                constraint->Scope().end(), [&](const Variable* v)->bool
                {   return findInGammaMinus(v) || v==me;});

        if (allSet and allInGammaMinus) {
            if (!constraint->satisfies()) {
                return false;
            }
        }
    }
    return true;
}

bool CC_ABT_Solver::consistent(const int& possibleValue) {
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

bool CC_ABT_Solver::consistent() {
    if (!me->isSet()) {
        return false;
    } else {
        return this->consistent(this->me->getValue());
    }
}

Variable* CC_ABT_Solver::findCulprit(const int& v) {
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

int CC_ABT_Solver::findLastCulprit() {
    AgentID maximum = 0;
    for (const auto& nogood : noGoodStore) {
        for (const auto& item : nogood.lhs.items) {
            maximum = item.id > maximum ? item.id : maximum;
        }
    }
    return maximum;
}

void CC_ABT_Solver::prune() {
    for (const auto& c : this->instance->constraints()) {
        for (const auto& v : c->Scope()) {
            if (v->getName() == this->name) {
                myConstraints.push_front(c.get());
                break;
            }
        }
    }
}

void CC_ABT_Solver::_messageReader() {
    // Let's listen to other agents:

    _INFO("Get ready to listen to other agents...");
    stringstream addressName;
    this->address = "127.0.0.1";
    addressName << "tcp://" << this->address << ":*";
    try {
        this->listener.bind(addressName.str().data());
    } catch (zmq::error_t &e) {
        cerr << e.what() << endl;
    }
    size_t len = 255;
    char endpoint[len];
    this->listener.getsockopt(ZMQ_LAST_ENDPOINT, &endpoint, &len);
    char * token, *last;
    token = strtok(endpoint, ":");
    while (token != NULL) {
        last = token;
        token = strtok(NULL, ":");
    }
    this->port = static_cast<unsigned int>(atoi(last));
    _INFO("Socket successfully created. Now listening on %s:%d",
            this->address.data(), this->port);
    this->agentReadyLock.notify();
    while (true) {
        P_Message p_message;
        this->listener.recvMessage(p_message);
        Message message;
        message.readFromProtocol(p_message);
        if (validate(message)) {
            this->messageRW.lock();
            this->messageQueue.push_back(message);
            this->messageRW.unlock();
            this->messageCount.notify();
        }
        if (message.type == P_MessageType::T_STOP
                || message.type == P_MessageType::T_FINISH) {
            return;
        }
    }
}

void CC_ABT_Solver::_sentinel() {
    P_Message messagePack;
    this->serverBroadcast.recvMessage(messagePack);
    if (messagePack.type() == P_MessageType::T_FINISH
            or messagePack.type() == P_MessageType::T_STOP) {
        this->currentAgent->socket()->sendMessage(messagePack);
    }
}

void CC_ABT_Solver::sendMessageOK_Monitor(const int& p, const bool& approved) {
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
    this->serverRquest.sendMessage(okMessage);
    P_Message ack;
    this->serverRquest.recvMessage(ack);
}

bool CC_ABT_Solver::validate(const Message& message) {
    switch (message.type) {
    case P_MessageType::T_OK:
        if (message.sequence <= this->sequences[message.sender - 1]
                and message.sequence != 0) {
            return false;
        } else {
            if (message.tv.size() != this->sequences.size()) {
                stringstream ss;
                ss << "Size of time vector of message must be "
                        "same as current time vector. Sizes are: "
                        << message.tv.size() << ',' << this->sequences.size();
                throw std::logic_error { ss.str() };
                return false;
            }
            auto mi = message.tv.begin();
            auto s = this->sequences.begin();
            while (mi != message.tv.end()) {
                if (*mi != 0 and *mi < *s) {
                    _INFO(
                            "Dropping message due to inconsistent timing: %d(%d) < %d(%d)",
                            std::distance(message.tv.begin(), mi), *mi,
                            std::distance(this->sequences.begin(), s), *s);
                    return false;
                }
                mi++;
                s++;
            }
            return true;
        }

    case P_MessageType::T_NOGOOD: {
        if (message.tv.size() != this->sequences.size()) {
            stringstream ss;
            ss << "Size of time vector of message must be "
                    "same as current time vector. Sizes are: "
                    << message.tv.size() << ',' << this->sequences.size();
            throw std::logic_error { ss.str() };
            return false;
        }
        auto mi = message.tv.begin();
        auto s = this->sequences.begin();
        while (mi != message.tv.end()) {
            if (*mi != 0 or *mi < *s) {
                _INFO(
                        "Dropping NOGOOG due to inconsistent timing: %d(%d) < %d(%d)",
                        std::distance(message.tv.begin(), mi), *mi,
                        std::distance(this->sequences.begin(), s), *s);
                return false;
            }
            mi++;
            s++;
        }
        return true;
    }
        break;
    default:
        return true;
        break;
    }
}

bool CC_ABT_Solver::findInGammaMinus(const Variable* v) {
    size_t p = variable2Priority[v];
    for (const auto& gm : this->m_gammaMinus) {
        if (gm->priority() == p)
            return true;
    }
    return false;
}
