/*
 * abt-solver.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: soroush
 */

#include <algorithm>

#include "abt-solver.h"
#include "abt-endpoint.h"
#include "abt-nogood.h"
#include "abt-message.h"
#include "assignment.h"

using namespace AIT;
using namespace std;
using namespace zmq;
using namespace protocols::csp;
using namespace protocols::csp::abt;

ABT_Solver::ABT_Solver(const std::string& serverHost_,
		const unsigned short& serverResponderPort_,
		const unsigned short& serverPublisherPort_, const AgentID& id_,
		const size_t& agentCount_) :
		id(id_), serverAddress(serverHost_), serverResponderPort(
				serverResponderPort_), serverPublisherPort(
				serverPublisherPort_), context(2), serverRquest(context,
				ZMQ_REQ), serverBroadcast(context, ZMQ_SUB), end(false), agentCount(
				agentCount_) {
	initializeDomain();
	sem_init(&agentReadyLock, 0, 0);
	sem_init(&messageCount, 0, 0);
	pthread_mutex_init(&this->messageRW, NULL);
}

ABT_Solver::~ABT_Solver() {
	serverBroadcast.close();
	serverRquest.close();
	listener->close();
	context.close();
}

void ABT_Solver::ABT() {
	value = 0; // FIXME fix this
	getAgentList();
	checkAgentView();
	while (!end) {
		ABT_Message m;
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
	_INFO( "Successfully connected to broadcasting channel of "
	"monitor agent at %s:%d, (%s)",
			this->serverAddress.data(), this->serverPublisherPort, addressName.str().data());
	// Send agent information to server
	P_CommunicationProtocol introPacket;
	introPacket.set_type(CP_MessageType::T_INTRODUCE);
	introPacket.set_allocated_identity(new P_EndPoint());
	introPacket.mutable_identity()->set_host(this->address);
	introPacket.mutable_identity()->set_id(this->id);
	introPacket.mutable_identity()->set_port(this->port);
	_INFO("Sending introduction message to monitor...");
	if (this->serverRquest.sendMessage(introPacket)) {
		_INFO(
				"Introduction message sent to monitor agent. Waiting for reply...");
		P_CommunicationProtocol introAckPacket;
		if (this->serverRquest.recvMessage(introAckPacket)) {
			_INFO("Message received from monitor");
			if (introAckPacket.type() == CP_MessageType::T_INTRODUCE_ACK) {
				_INFO( "Monitor agent accepted connection.");
			} else if (introAckPacket.type()
					== CP_MessageType::ERR_REPEATED_ID) {
				_ERROR( "An agent with ID number `%d'is already registered.\n"
				"\t\tMonitor agent has ignored this request.\n"
				"\t\tExiting silently.", this->id);
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
	if (!consistent(this->value)) {
		this->value = chooseValue();
		if (this->value != 0) {
			for (const auto& agent : succeeding) {
				sendMessageOK(agent->id());
			}
		} else {
			backtrack();
		}
	}
}

void ABT_Solver::processInfo(const ABT_Message& m) {
	updateAgentView(m.assignment);
	checkAgentView();
}

int ABT_Solver::chooseValue() {
	for (auto &v : domain) {
		bool eliminated = false;
		eliminated = any_of(noGoodStore.begin(), noGoodStore.end(),
				[&](const ABT_Nogood &ngd) {
					return (ngd.rhs.id==this->id and ngd.rhs.value==v);
				});
		if (!eliminated) {
			if (consistent(v)) {
				_INFO("A new value has been assigned: %d", v);
				return v;
			} else {
				int culpritID = findCulprit(v);
				int culpirtValue = findCulpritsValue(culpritID);
				ABT_Nogood ngd;
				ngd.lhs.items.insert(Assignment(culpritID, culpirtValue));
				ngd.rhs.id = this->id;
				ngd.rhs.value = v;
				noGoodStore.push_back(ngd);
			}
		}
	}
	return 0;
}

void ABT_Solver::backtrack() {
	CompoundAssignment newNogood = solve();
	if (newNogood.items.empty()) {
		end = true;
		sendMessageSTP();
	} else {
		ABT_Message nogoodMessage;
		nogoodMessage.nogood = newNogood;

		AgentID lastCulpirt = findLastCulprit();
		sendMessageNGD(lastCulpirt, nogoodMessage);
		Assignment clearCulpirt(lastCulpirt, 0);
		updateAgentView(clearCulpirt);
		checkAgentView();
	}
}

void ABT_Solver::updateAgentView(const Assignment& assignment) {
	// remove old value of given id in assignment
	for (auto it = agentView.items.begin(); it != agentView.items.end();) {
		if (it->id == assignment.id) {
			agentView.items.erase(it++);
			break;
		} else {
			++it;
		}
	}
	// add new value
	if (assignment.value != 0)
		agentView.items.insert(assignment);
	// remove invalid nogoods
	auto x =
			std::remove_if(noGoodStore.begin(), noGoodStore.end(),
					[&](ABT_Nogood& ngd)->bool {return !coherent(ngd.lhs,agentView);});
	noGoodStore.erase(x, noGoodStore.end());
}

void ABT_Solver::resolveConflict(const ABT_Message& msg) {
	CompoundAssignment myselfAssignment;
	myselfAssignment.items.insert(Assignment(this->id, this->value));

	CompoundAssignment totalView;
	totalView = agentView;
	totalView.items.insert(Assignment(this->id, this->value));

	if (coherent(msg.nogood, totalView)) {
		checkAddLink(msg);
		add(msg.nogood);
		this->value = 0; // FIXME
		checkAgentView();
	} else if (coherentSelf(msg.nogood, myselfAssignment)) {
		sendMessageOK(msg.sender);
	} else {
	}
}

bool ABT_Solver::consistent(const int& v) {
	if (v == 0)
		return false;
	for (const auto& a : agentView.items) {
		/* WARNING : VERY STUPID CODE AHEAD! */
		int delta_a = a.id - this->id;
		int delta_v = a.value - v;
		if (delta_a < 0)
			delta_a *= -1;
		if (delta_v < 0)
			delta_v *= -1;
		/* END OF VERY SRUPID CODE */
		if (a.value == v or delta_a == delta_v) {
			return false;
		}
	}
	return true;
}

P_Message ABT_Solver::getMessage() {
	sem_wait(&messageCount);
	pthread_mutex_lock(&this->messageRW);
	P_Message x = this->messageQueue.front();
	this->messageQueue.pop();
	pthread_mutex_unlock(&this->messageRW);
	return x;
}

void ABT_Solver::setLink(const ABT_Message& message) {
	bool repeated = any_of(succeeding.begin(), succeeding.end(),
			[&](const std::vector<ABT_EndPoint>::iterator i)->bool
			{	return i->id() == message.sender;});
	if (!repeated) {
		auto index = find_if(everybody.begin(), everybody.end(),
				[&](const P_EndPoint& agent)->bool
				{	return agent.id()==message.sender;});
		succeeding.push_back(index);
	}
}

void ABT_Solver::checkAddLink(const ABT_Message& message) {
	for (const auto& assignment : message.nogood.items) {
		if (!any_of(succeeding.begin(), succeeding.end(),
				[&](const std::vector<ABT_EndPoint>::iterator i)->bool
				{	return i->id()==assignment.id;})) {
			sendMessageADL(id);
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
	_INFO( "Sending Request List to monitor agent ...");
	P_CommunicationProtocol requestList;
	requestList.set_type(CP_MessageType::T_REQUEST_LIST);
	requestList.set_id(this->id);
	this->serverRquest.sendMessage(requestList);

	P_CommunicationProtocol requestListAck;
	this->serverRquest.recvMessage(requestListAck);
	if (requestListAck.type() == CP_MessageType::T_REQUEST_ACK) {
		_INFO( "Monitor has accepted to send list of agents.");
	} else {
		_ERROR( "Unable to get agent list from monitor.\n"
		"\tTerminating silently. Goodbye.");
	}

	_INFO( "Waiting for all agents to came online...");
	P_CommunicationProtocol listPacket;
	this->serverBroadcast.recvMessage(listPacket);
	if (listPacket.type() == CP_MessageType::T_LIST) {
		for (int i = 0; i < listPacket.others_size(); ++i) {
			this->everybody.push_back(
					ABT_EndPoint(listPacket.others(i), context));
			_INFO("New agent introduced by server:\n"
			"\tID:     %d\n"
			"\tHost:   %s\n"
			"\tPort:   %d",
					listPacket.others(i).id(), listPacket.others(i).host().c_str(), listPacket.others(i).port());
		}
		for (auto i = everybody.begin(); i < everybody.end(); ++i) {
			if (i->id() < this->id)
				preceding.push_back(i);
			else if (i->id() > this->id)
				succeeding.push_back(i);
		}
		for (auto &agent : everybody) {
			stringstream address;
			address << "tcp://";
			address << agent.host() << ":" << agent.port();
			agent.socket()->connect(address.str().c_str());
		}
	}
}

int ABT_Solver::findCulprit(const int& v) {
	AgentID lastCulpirt = 0;
	for (const auto& a : agentView.items) {
		int delta_a = (a.id - id); // FIXME Think about :|
		int delta_v = (a.value - v);
		if (delta_a < 0)
			delta_a *= -1;
		if (delta_v < 0)
			delta_v *= -1;
		if (a.value == v or delta_a == delta_v) { // culprit
			lastCulpirt = (a.id > lastCulpirt) ? a.id : lastCulpirt; // and last
		}
	}
	return lastCulpirt;
}

int AIT::ABT_Solver::findCulpritsValue(const int& culpirtsID) {
	for (const auto& a : agentView.items) {
		if (a.id == culpirtsID)
			return a.value;
	}
	return 0;
}

void AIT::ABT_Solver::sendMessageOK(const AgentID& agent) {
	ABT_Message ok;
	ok.sender = this->id;
	ok.type = P_MessageType::T_OK;
	ok.assignment = Assignment(this->id, this->value);
	sendMessage(agent, ok);
}

void AIT::ABT_Solver::sendMessageNGD(const AgentID& agent, ABT_Message& ngd) {
	ngd.sender = this->id;
	ngd.type = P_MessageType::T_NOGOOD;
	sendMessage(agent, ngd);
}

void AIT::ABT_Solver::sendMessageSTP() {
	ABT_Message stop;
	stop.type = P_MessageType::T_STOP;
	stop.sender = this->id;
	sendMessage(0, stop);
}

void ABT_Solver::sendMessageADL(const AgentID& agent) {
	if (agent == this->id)
		return;
	ABT_Message adl;
	adl.sender = this->id;
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

void ABT_Solver::sendMessage(const AgentID& agent, const ABT_Message& message) {
	if (agent == this->id)
		return;
	if (agent == 0) {
		for (const auto &ep : this->everybody) {
			stringstream address;
			ep.socket()->sendMessage(message);
			return;
		}
	} else {
		for (const auto &ep : this->everybody) {
			if (ep.id() == agent) {
				ep.socket()->sendMessage(message);
			}
		}
	}
}

void AIT::ABT_Solver::initializeDomain() {
	for (int i = 0; i < agentCount; ++i) {
		this->domain.push_back(i + 1);
	}
}

void ABT_Solver::add(const CompoundAssignment& ca) {
	ABT_Nogood ngd;
	for (const auto& a : ca.items) {
		if (a.id != this->id) {
			ngd.lhs.items.insert(a);
		} else {
			ngd.rhs = a;
		}
	}
	noGoodStore.push_back(ngd);
}

int ABT_Solver::findLastCulprit() {
	AgentID lastID = 0;
	for (const auto& ngd : noGoodStore) {
		for (const auto& a : ngd.lhs.items) {
			if (a.id > lastID)
				lastID = a.id;
		}
	}
	return lastID;
}

void AIT::ABT_Solver::printNGS() {
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

void AIT::ABT_Solver::printAV() {
	stringstream ss;
	ss << "Agent View: ";
	for (const auto& a : agentView.items) {
		ss << '(' << a.id << ',' << a.value << "),";
	}
	_INFO("%s", ss.str().c_str());
}

void* ABT_Solver::_messageReader(void* param) {
	// Let's listen to other agents:
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
