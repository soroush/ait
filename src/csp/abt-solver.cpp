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

#include "abt-solver.h"
#include "assignment.h"

using namespace std;
using namespace zmq;
using namespace AIT::CSP;
using namespace AIT::protocols::csp;
using namespace AIT::protocols::csp::abt;

ABT_Solver::ABT_Solver(const std::string& serverHost_,
        const unsigned short& serverResponderPort_,
        const unsigned short& serverPublisherPort_, const std::string& name_,
        const std::string& xcspFile) :
        name(name_), serverAddress(serverHost_), serverResponderPort(
                serverResponderPort_), serverPublisherPort(
                serverPublisherPort_), context(2), listener(
                new Socket(context, ZMQ_PULL)), serverRquest(context, ZMQ_REQ), serverBroadcast(
                context, ZMQ_SUB), end(false) {
    this->parseFromFile(xcspFile);
    sem_init(&agentReadyLock, 0, 0);
    sem_init(&messageCount, 0, 0);
    pthread_mutex_init(&this->messageRW, NULL);
    this->priority = this->instance->variableIndex(this->name);
    this->me = this->instance->variable(this->name);
    this->agentCount = this->instance->variables().size();
    cout << this->priority << " of " << this->agentCount << ',' << this->name
            << endl;
    this->prune();
}

ABT_Solver::~ABT_Solver() {
    serverBroadcast.close();
    serverRquest.close();
    listener->close();
    context.close();
    delete listener;
    //delete[] assignedAgents;
}

void ABT_Solver::ABT() {
    // TODO: Compute Γ+ and Γ- from problem instance
    this->connect();
    value = 0; // FIXME fix this
    me->unset();
    getAgentList();
    checkAgentView();
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
            break;
        default:
            end = true;
            break;
        }
    }
}

void ABT_Solver::connect() {
    pthread_create(&this->messageReader, NULL, &ABT_Solver::_messageReader,
            static_cast<void*>(this));
    sem_wait(&this->agentReadyLock);
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
    _INFO("Successfully connected to broadcasting channel of "
            "monitor agent at %s:%d, (%s)", this->serverAddress.data(),
            this->serverPublisherPort, addressName.str().data());
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
                        "Monitor agent accepted connection. Current priority is: %d",
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

void ABT_Solver::checkAgentView() {
    if (!consistent(this->value, this->agentView)) {
        cout << "checkAgentView: !consistent" << endl;
        this->value = chooseValue();
        // FIXME: Remove 0=unset logic
        if (this->value != 0) {
                cout << "SUC: " << succeeding.size() << endl;
            for (const auto& agent : succeeding) {
                cout << "Sending message to: " << agent->name() << endl;
                sendMessageOK(agent->priority());
            }
        } else {
            backtrack();
        }
    }
}

void ABT_Solver::processInfo(const Message& m) {
    cout << "OK received";
    updateAgentView(m.assignment);
    checkAgentView();
}

int ABT_Solver::chooseValue() {
    for (auto &possibleValue : this->me->getDomain()->getValues()) {
        bool eliminated = false;
        eliminated =
                any_of(noGoodStore.begin(), noGoodStore.end(),
                        [&](const Nogood &ngd) {
                            return (ngd.rhs.id==this->priority and ngd.rhs.value==possibleValue);
                        });
        if (!eliminated) {
            if (consistent(possibleValue, agentView)) {
                _INFO("A new value has been assigned: %d", possibleValue);
                // FIXME: Remove return and update to current type
                me->setValue(possibleValue);
                return possibleValue;
            } else {
                me->setValue(possibleValue);
                Variable* culprit = findCulprit(possibleValue);
                me->unset();
                Nogood ngd;
                ngd.lhs.items.insert(
                        Assignment(
                                this->instance->variableIndex(
                                        culprit->getName()),
                                culprit->getValue()));
                ngd.rhs.id = this->priority;
                ngd.rhs.value = possibleValue;
                noGoodStore.push_back(ngd);
            }
        }
    }
    me->unset();
    return 0;
}

void ABT_Solver::backtrack() {
    CompoundAssignment newNogood = solve();
    if (newNogood.items.empty()) {
        end = true;
        sendMessageSTP();
    } else {
        Message nogoodMessage;
        nogoodMessage.nogood = newNogood;

        AgentID lastCulpirt = findLastCulprit();
        sendMessageNGD(lastCulpirt, nogoodMessage);
        Assignment clearCulpirt(lastCulpirt, 0);
        updateAgentView(clearCulpirt);
        checkAgentView();
    }
}

void ABT_Solver::updateAgentView(const Assignment& assignment) {
    // Remove old value of given id in agentView
    for (auto it = agentView.items.begin(); it != agentView.items.end();) {
        if (it->id == assignment.id) {
            agentView.items.erase(it++);
            break;
        } else {
            ++it;
        }
    }
    // Add new value
    if (assignment.value != 0) {
        agentView.items.insert(assignment);
        //this->agentViewX[assignment.id] = assignment.value;
    }
    // Add new value to problem structures (generic CSP definitions)
    // FIXME: Use global names instead of IDs or use a global name-to-id map.
    const auto& x = getByPriority(assignment.id).name();
    this->instance->variable(x)->setValue(assignment.value);
    // Remove invalid nogoods
    noGoodStore.remove_if(
            [&](Nogood& ngd)->bool {return !coherent(ngd.lhs,agentView);});
    //this->assignedAgents[assignment.id] = true;
}

void ABT_Solver::resolveConflict(const Message& msg) {
    CompoundAssignment myselfAssignment;
    myselfAssignment.items.insert(Assignment(this->priority, this->value));

    CompoundAssignment totalView;
    totalView = agentView;
    totalView.items.insert(Assignment(this->priority, this->value));

    if (coherent(msg.nogood, totalView)) {
        //checkAddLink(msg);
        add(msg.nogood);
        // FIXME: Remove value and convert to xurrent structures
        this->value = 0;
        me->unset();
        checkAgentView();
    } else if (coherentSelf(msg.nogood, myselfAssignment)) {
        sendMessageOK(msg.sender);
    } else {
    }
}

P_Message ABT_Solver::getMessage() {
    sem_wait(&messageCount);
    pthread_mutex_lock(&this->messageRW);
    P_Message x = this->messageQueue.front();
    this->messageQueue.pop();
    pthread_mutex_unlock(&this->messageRW);
    return x;
}

void ABT_Solver::setLink(const Message& message) {
    bool repeated = any_of(succeeding.begin(), succeeding.end(),
            [&](const std::vector<EndPoint>::iterator i)->bool
            {   return i->priority() == message.sender;});
    if (!repeated) {
        auto index = find_if(everybody.begin(), everybody.end(),
                [&](const P_EndPoint& agent)->bool
                {   return agent.priority()==message.sender;});
        succeeding.push_back(index);
    }
}

void ABT_Solver::checkAddLink(const Message& message) {
    for (const auto& assignment : message.nogood.items) {
        if (!any_of(succeeding.begin(), succeeding.end(),
                [&](const std::vector<EndPoint>::iterator i)->bool
                {   return i->priority()==assignment.id;})) {
            sendMessageADL(priority);
            updateAgentView(assignment);
        }
    }
}

bool ABT_Solver::coherent(const CompoundAssignment& nogood,
        const CompoundAssignment& assign) {
    for (const auto &n : nogood.items) {
        bool found = false;
        for (const auto &a : assign.items) {
            if (n.id == a.id and n.value != a.value)
                return false;
            else if (n.id == a.id and n.value == a.value)
                found = true;
        }
        if (!found)
            return false;
    }
    return true;
}

bool ABT_Solver::coherentSelf(const CompoundAssignment& nogood,
        const CompoundAssignment& assign) {
    for (const auto &n : nogood.items) {
        for (const auto &a : assign.items) {
            if (n.id == a.id and n.value != a.value)
                return false;
        }
    }
    return true;
}

void ABT_Solver::getAgentList() {
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
            _INFO("New agent introduced by server:\n"
                    "\tID:        %d\n"
                    "\tHost:      %s\n"
                    "\tVariable:  %s\n"
                    "\tPort:      %d", listPacket.others(i).priority(),
                    listPacket.others(i).host().c_str(),
                    listPacket.others(i).name().c_str(),
                    listPacket.others(i).port());
        }
        // TODO: Generalize this to solve not only fully-connected problems
        cout << "My priority is:" << this->priority << endl;
        for (auto i = everybody.begin(); i < everybody.end(); ++i) {
            cout << "Checking priority of :" << i->priority() << endl;
            if (i->priority() < this->priority)
                preceding.push_back(i);
            else if (i->priority() > this->priority)
                succeeding.push_back(i);
        }
        for (auto &agentEndPoint : everybody) {
            stringstream address;
            address << "tcp://";
            address << agentEndPoint.host() << ":" << agentEndPoint.port();
            agentEndPoint.socket()->connect(address.str().c_str());
        }
    }
}

void ABT_Solver::sendMessageOK(const AgentID& agent) {
    Message ok;
    ok.sender = this->priority;
    ok.type = P_MessageType::T_OK;
    ok.assignment = Assignment(this->priority, this->value);
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
    } else {
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
        } else {
            ngd.rhs = a;
        }
    }
    noGoodStore.push_back(ngd);
}

void ABT_Solver::printNGS() {
    stringstream ss;
    ss << "NogoodStore: ";
    for (const auto& ngd : noGoodStore) {
        for (const auto& a : ngd.lhs.items)
            ss << '(' << a.id << ',' << a.value << "),";
        ss << "=>" << ngd.rhs.id << "!=" << ngd.rhs.value << endl
                << "                                        ";
    }
    _INFO("%s", ss.str().c_str());
}

void ABT_Solver::printAV() {
    stringstream ss;
    ss << "Agent View: ";
    for (const auto& a : agentView.items) {
        ss << '(' << a.id << ',' << a.value << "),";
    }
    _INFO("%s", ss.str().c_str());
}

bool ABT_Solver::consistent(const int& possibleValue,
        const CompoundAssignment& ca) {
    int previousValue = me->getValue();
    me->setValue(possibleValue);
    for (const auto& constraint : this->myConstraints) {
//        if (all_of(constraint->Scope().begin(), constraint->Scope().end(),
//                [&](const Variable* v)->bool
//                {   return v->isSet();})) {
        if (!constraint->satisfies()) {
            me->setValue(previousValue);
            return false;
        }
//        }
    }
    me->setValue(previousValue);
    return true;
}

int ABT_Solver::getValueOf(const int& agent, bool& find) {
    // TODO: Completely get rid of agentView
    for (const auto& a : this->agentView.items) {
        if (a.id == agent) {
            find = true;
            return a.value;
        }
    }
    find = false;
    return 0;
}

Variable* ABT_Solver::findCulprit(const int& v) {
    string maxName;
    int max = 0;
    for (const auto& constraint : this->myConstraints) {
        if (!constraint->satisfies()) {
            cout << constraint->getName() << " is not sat" << endl;
            for (const auto& variable : constraint->Scope()) {
                if (variable != me) {
                    const EndPoint& ptr = this->getByName(variable->getName());
                    if (ptr.priority() > max) {
                        maxName = ptr.name();
                        max = ptr.priority();
                        //cout << "Current Culprit: " << maxName << " " << max << endl;
                    }
                }
            }
        }
    }
//    cout << "Final Culprit: " << maxName << " "
//            << this->instance->variable(maxName)->getValue() << endl;
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

void* ABT_Solver::_messageReader(void* param) {
    // Let's listen to other agents:
    _INFO("Get ready to listen to other agents...");
    ABT_Solver* solver = (reinterpret_cast<ABT_Solver*>(param));
    solver->listener = new Socket(solver->context, ZMQ_PULL);
    stringstream addressName;
    solver->address = "127.0.0.1";
    addressName << "tcp://" << solver->address << ":*";
    try {
        solver->listener->bind(addressName.str().data());
    } catch (zmq::error_t &e) {
        cerr << e.what() << endl;
    }
    size_t len = 255;
    char endpoint[len];
    solver->listener->getsockopt(ZMQ_LAST_ENDPOINT, &endpoint, &len);
    char * token, *last;
    token = strtok(endpoint, ":");
    while (token != NULL) {
        last = token;
        token = strtok(NULL, ":");
    }
    solver->port = static_cast<unsigned int>(atoi(last));
    _INFO("Socket successfully created. Now listening on %s:%d",
            solver->address.data(), solver->port);
    sem_post(&solver->agentReadyLock);
    while (true) {
        P_Message message;
        solver->listener->recvMessage(message);
        pthread_mutex_lock(&(solver->messageRW));
        solver->messageQueue.push(message);
        pthread_mutex_unlock(&(solver->messageRW));
        sem_post(&solver->messageCount);
    }
    return 0;
}

const ABT_Solver::EndPoint& ABT_Solver::getByName(const std::string& name) {
    for (const auto& ep : everybody) {
        if (ep.name() == name) {
            return ep;
        }
    }
    std::stringstream ss;
    ss << "There is no such EndPoint: `" << name << "'";
    throw std::out_of_range { ss.str() };
}

const ABT_Solver::EndPoint& ABT_Solver::getByPriority(const size_t& priority) {
    for (const auto& ep : everybody) {
        if (ep.priority() == priority) {
            return ep;
        }
    }
}
