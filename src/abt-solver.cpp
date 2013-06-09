/*
 * abt-solver.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: soroush
 */

#include "abt-solver.h"
#include <algorithm>

const int agentCount = 4;

using namespace AIT;
using namespace std;
using namespace zmq;
using namespace protocols::csp;
using namespace protocols::csp::abt;

ABT_Solver::ABT_Solver(const std::string& host_, const unsigned short& port_,
		const std::string& serverHost_,
		const unsigned short& serverResponderPort_,
		const unsigned short& serverPublisherPort_, const AgentID& id_) :
		id(id_), address(host_), port(port_), serverAddress(serverHost_), serverResponderPort(
				serverResponderPort_), serverPublisherPort(
				serverPublisherPort_), context(2), listener(context, ZMQ_PULL), serverRquest(
				context, ZMQ_REQ), serverBroadcast(context, ZMQ_SUB), end(false) {
	initializeDomain();
}

ABT_Solver::~ABT_Solver() {
	serverBroadcast.close();
	serverRquest.close();
	listener.close();
	context.close();
}

void ABT_Solver::ABT() {
	value = 0; // FIXME fix this
	getAgentList();
	checkAgentView();
	while (!end) {
		P_Message m = getMessage();
		switch (m.type()) {
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
	// Let's listen to other agents:
	stringstream addressName;
	this->address = "127.0.0.1"; //Socket::getIP();
	addressName << "udp://" << this->address << ":*";
	try {
		listener.bind(addressName.str().data());
	} catch (zmq::error_t &e) {
		cerr << e.what() << endl;
	}
	size_t len = 255;
	char endpoint[len];
	listener.getsockopt(ZMQ_LAST_ENDPOINT, &endpoint, &len);
	char * token, *last;
	token = strtok(endpoint, ":");
	while (token != NULL) {
		last = token;
		token = strtok(NULL, ":");
	}
	this->port = static_cast<unsigned int>(atoi(last));
	_INFO("Socket successfully created. Now listening on %s:%d",
			this->address.data(), this->port);
	// Now we'll connect to server (monitor agent)
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
			_ERROR("Server didn't reply.");
		}
	} else {
		_ERROR("Unable to send introduction message.");
	}
}

void ABT_Solver::checkAgentView() {
	_INFO("Checking Agent View...");
	if (!consistent(this->value)) {
		this->value = chooseValue();
		if (this->value != 0) {
			_INFO("Sending value to all succeeding agents...");
			for (const auto& agent : succeeding) {
				_INFO("Sending value to [%d,%s:%d]...",
						agent->id(), agent->host().c_str(), agent->port());
				sendMessageOK(agent->id());
			}
		} else {
			_INFO("Can't find any consistent value. Backtracking...");
			backtrack();
		}
	}
}

void ABT_Solver::processInfo(const P_Message& m) {
	updateAgentView(m.assignment());
	checkAgentView();
}

int ABT_Solver::chooseValue() {
	for (auto &v : domain) {
		_INFO("Trying to find a consistent value...");
		bool eliminated = false;
		eliminated = any_of(noGoodStore.begin(), noGoodStore.end(),
				[&](P_Nogood &ngd) {
					return (ngd.rhs().id()==this->id and ngd.rhs().value()==v);
				});
		if (!eliminated) {
			if (consistent(v)) {
				_INFO("A new value has been assigned: %d", v);
				return v;
			} else {
				int culpritID = findCulprit(v);
				int culpirtValue = findCulpritsValue(culpritID);
				P_Nogood ngd;
				P_CompoundAssignment lhs;
				P_Assignment *lhs_value = ngd.mutable_lhs()->add_assignments();
				lhs_value->set_id(culpritID);
				lhs_value->set_value(culpirtValue);

				ngd.mutable_rhs()->set_id(this->id);
				ngd.mutable_rhs()->set_value(v);

				noGoodStore.push_back(ngd);
			}
		}
	}
	return 0;
}

void ABT_Solver::backtrack() {
	P_Nogood newNogood = solve();
	if (newNogood.lhs().assignments_size() == 0) {
		end = true;
		sendMessageSTP();
	} else {
		P_Message nogoodMessage;
		nogoodMessage.set_allocated_nogood(&newNogood);
		sendMessageNGD(newNogood.rhs().id(), nogoodMessage);
		newNogood.mutable_rhs()->clear_value();
		updateAgentView(newNogood.rhs());
		checkAgentView();
	}
}

void ABT_Solver::updateAgentView(const P_Assignment& assignment) {
	if (assignment.has_value()) {
		agentView.add(assignment);
		remove_if(noGoodStore.begin(), noGoodStore.end(),
				[&](P_Nogood ngd)->bool {return !this->coherent(ngd,agentView);});
	} else {
		// Clear item from agentView with specified ID
		remove_if(agentView.mutable_assignments()->begin(),
				agentView.mutable_assignments()->end(),
				[&](P_Assignment a)->bool {return a.id()==assignment.id();});
	}
}

void ABT_Solver::resolveConflict(const P_Message& msg) {
	CompoundAssignment myselfAssignment;
	myselfAssignment.add(this->id, this->value);

	CompoundAssignment totalView;
	totalView.add(this->agentView);
	totalView.add(this->id, this->value);

	if (coherent(msg.nogood(), totalView)) {
		checkAddLink(msg);
		noGoodStore.push_back(msg.nogood());
		this->value = 0; // FIXME
		checkAgentView();
	} else if (coherent(msg.nogood(), myselfAssignment)) {
		sendMessageOK(msg.sender());
	}
}

bool ABT_Solver::consistent(const int& v) {
	_INFO("Checking consistency of (%d,%d)...", this->id, v);
	if (v == 0)
		return false;
	for (int i = 0; i < agentView.assignments_size(); ++i) {
		AgentID x_j = agentView.assignments(i).id();
		int v_j = agentView.assignments(i).value();
		if (v_j == v or (abs(x_j - id) == abs(v_j - v))) {
			return false;
		}
	}
	_INFO("Consistency check successful");
	return true;
}

P_Message ABT_Solver::getMessage() {
	_INFO("Waiting for message from an agent...");
	message_t message;
	listener.recv(&message);
	P_Message x;
	x.ParseFromArray(message.data(), message.size());
	_INFO("New message has been received from %d.", x.sender());
	return x;
}

void ABT_Solver::setLink(const P_Message& message) {
	bool repeated = any_of(succeeding.begin(), succeeding.end(),
			[&](const std::vector<ABT_EndPoint>::iterator i)->bool
			{	return i->id() == message.sender();});
	if (!repeated) {
		auto index = find_if(everybody.begin(), everybody.end(),
				[&](const P_EndPoint& agent)->bool
				{	return agent.id()==message.sender();});
		succeeding.push_back(index);
		_INFO("Agent `%d' added to succeeding list.", index->id());
	}
}

void ABT_Solver::checkAddLink(const P_Message& message) {
	for (int i = 0; i < message.nogood().lhs().assignments_size(); ++i) {
		int id = message.nogood().lhs().assignments(i).id();
		int value = message.nogood().lhs().assignments(i).value();
		if (!any_of(succeeding.begin(), succeeding.end(),
				[&](const std::vector<ABT_EndPoint>::iterator i)->bool
				{	return i->id()==id;})) {
			P_Message adl;
			adl.set_type(P_MessageType::T_ADDLINK);
			adl.set_sender(this->id);
			sendMessageADL(id);
			P_Assignment assign;
			assign.set_id(id);
			assign.set_value(value);
			updateAgentView(assign);
		}
	}
}

bool AIT::ABT_Solver::coherent(const P_Nogood& nogood,
		const P_CompoundAssignment& assign) {
//	for (int j = 0; j < assign.assignments_size(); ++j) {
//		for (int i = 0; i < nogood.lhs().assignments_size(); ++i) {
//			if (nogood.lhs().assignments(i).id()
//					== assign.assignments(j).id()) {
//				if (nogood.lhs().assignments(i).value()
//						!= assign.assignments(j).value())
//					return false;
//			}
//			if (nogood.rhs().id() != assign.assignments(j).id()
//					or nogood.rhs().value() != assign.assignments(j).value())
//				return false;
//		}
//	}
//	return true;

	for (const auto &a : assign.assignments()) {
		for (const auto &n : nogood.lhs().assignments()) {
			if (a.id() == n.id() and a.value() != n.value())
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
	_INFO( "I'm done.");
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
	}
}

ABT_EndPoint::ABT_EndPoint(const protocols::csp::abt::P_EndPoint& ep,
		zmq::context_t& context) :
		socket_(new Socket(context, ZMQ_PUSH)) {
	this->CopyFrom(ep);
}

int ABT_Solver::findCulprit(const int& v) {
	AgentID lastCulpirt = 0;
	for (int i = 0; i < agentView.assignments_size(); ++i) {
		if (agentView.assignments(i).id() < this->id) { // Always true
			AgentID x_j = agentView.assignments(i).id();
			int v_j = agentView.assignments(i).value();
			if (v_j == this->value or (abs(x_j - id) == abs(v_j - value))) { // culprit
				lastCulpirt = (x_j > lastCulpirt) ? x_j : lastCulpirt; // and last
			}
		}
	}
	return lastCulpirt;
}

int AIT::ABT_Solver::findCulpritsValue(const int& culpirtsID) {
	for (int i = 0; i < agentView.assignments_size(); ++i) {
		if (agentView.assignments(i).id() == culpirtsID)
			return agentView.assignments(i).value();
	}
	return 0;
}

void AIT::ABT_Solver::sendMessageOK(const AgentID& agent) {
	P_Message ok;
	ok.set_sender(this->id);
	ok.set_type(P_MessageType::T_OK);
	ok.mutable_assignment()->set_id(this->id);
	ok.mutable_assignment()->set_value(this->value);
	_INFO("Sending OK message...");
	sendMessage(agent, ok);
}

void AIT::ABT_Solver::sendMessageNGD(const AgentID& agent, P_Message& ngd) {
	ngd.set_sender(this->id);
	ngd.set_type(P_MessageType::T_NOGOOD);
	_INFO("Sending NGD message...");
	sendMessage(agent, ngd);
}

void AIT::ABT_Solver::sendMessageSTP() {
	P_Message stop;
	stop.set_type(P_MessageType::T_STOP);
	stop.set_sender(this->id);
	_INFO("Sending STP message...");
	sendMessage(0, stop);
}

void ABT_Solver::sendMessageADL(const AgentID& agent) {
	P_Message adl;
	adl.set_sender(this->id);
	adl.set_type(P_MessageType::T_ADDLINK);
	_INFO("Sending ADL message...");
	sendMessage(agent, adl);
}

//void ABT_Solver::updateAgentView(const P_CompoundAssignment& assignments) {
//	agentView.add(assignments);
//	remove_if(NoGoodStore.begin(), NoGoodStore.end(),
//			[&](P_Nogood ngd)->bool {return !this->coherent(ngd,agentView);});
//}
//
//void ABT_Solver::updateAgentView(const protocols::csp::abt::P_Nogood& ngd) {
//	updateAgentView(ngd.lhs());
//	for (int i = 0; i < agentView.assignments_size(); ++i) {
//		if (agentView.assignments(i).id() == ngd.rhs().id()) {
//			agentView.mutable_assignments()->DeleteSubrange(i, 1);
//		}
//	}
//}

P_Nogood ABT_Solver::solve() {
	P_Nogood newNogood;
	for (const auto& ngd : this->noGoodStore) {
		for (const auto& assign : ngd.lhs().assignments()) {
			P_Assignment* newAssignment =
					newNogood.mutable_lhs()->add_assignments();
			newAssignment->CopyFrom(assign);
		}
	}
	return newNogood;
}

void ABT_Solver::sendMessage(const AgentID& agent, const P_Message& message) {
	if (agent == 0) {
		_INFO("Sending message to everybody");
		for (const auto &ep : this->everybody) {
			if (ep.id() == agent) {
				ep.socket()->sendMessage(message);
				return;
			}
		}
	} else {
		_INFO("Sending message to %d", agent);
		for (const auto &ep : this->everybody) {
			if (ep.id() == agent)
				ep.socket()->sendMessage(message);
		}
	}
}

Socket* AIT::ABT_EndPoint::socket() const {
	return this->socket_;
}

void AIT::ABT_Solver::initializeDomain() {
	for (int i = 0; i < agentCount; ++i) {
		this->domain.push_back(i + 1);
	}
}

