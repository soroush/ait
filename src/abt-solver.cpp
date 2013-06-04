/*
 * abt-solver.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: soroush
 */

#include "abt-solver.h"
#include <algorithm>

using namespace AIT;
using namespace std;
using namespace zmq;
using namespace protocols::csp::abt;

ABT_Solver::ABT_Solver(const std::string& host_, const unsigned short& port_,
		const std::string& serverHost_,
		const unsigned short& serverResponderPort_,
		const unsigned short& serverPublisherPort_, const AgentID& id_) :
		id(id_), address(host_), port(port_), serverAddress(serverHost_), serverResponderPort(
				serverResponderPort_), serverPublisherPort(
				serverPublisherPort_), context(2), listener(context, ZMQ_PULL), serverRquest(
				context, ZMQ_REQ), serverBroadcast(context, ZMQ_SUB) {
}

ABT_Solver::~ABT_Solver() {
	// delete this->myValue;
	serverBroadcast.close();
	serverRquest.close();
	listener.close();
	context.close();
}

void ABT_Solver::ABT() {
	value = 0; // FIXME fix this
	bool end = false;
	getAgentList();
	checkAgentView();
	while (!end) {
		Message m = getMessage();
		switch (m.type()) {
		case ABT_MessageType::T_OK:
			processInfo(m);
			break;
		case ABT_MessageType::T_NOGOOD:
			resolveConflict(m);
			break;
		case ABT_MessageType::T_ADDLINK:
			setLink(m);
			break;
		case ABT_MessageType::T_STOP:
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
	this->address = Socket::getIP();
	addressName << "tcp://" << this->address << ":*";
	try {
		listener.bind(addressName.str().data());
	} catch (zmq::error_t e) {
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
	_INFO("Socket successfully created. Now listening on %s:%d",
			this->address.data(), this->port);
	this->port = static_cast<unsigned int>(atoi(last));
	// Now we'll connect to server (monitor agent)
	addressName.str(string());
	addressName << "tcp://" << this->serverAddress << ':'
			<< this->serverResponderPort;
	try {
		serverRquest.connect(addressName.str().data());
	} catch (zmq::error_t e) {
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
	} catch (zmq::error_t e) {
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
	if (!consistent(value)) {
		chooseValue();
		if (value != 0) {
			for (const auto& agent : succeeding) {
				Message message;
				message.set_sender(this->id);
				message.set_type(ABT_MessageType::T_OK);
				P_Assignment assign;
				assign.set_id(this->id);
				assign.set_value(this->value);
				message.set_allocated_assignment(&assign);
				sendMessage(agent->id(), message); // FIXME
			}
		} else {
			backtrack();
		}
	} // end if !consistent
}

void ABT_Solver::processInfo(const Message& m) {
	updateAgentView(m.assignment());
	checkAgentView();
}

void ABT_Solver::chooseValue() {
}

void ABT_Solver::backtrack() {
}

void ABT_Solver::updateAgentView(const P_Assignment& assignment) {
	myAgentView.add(assignment);
	remove_if(NoGoodStore.begin(), NoGoodStore.end(),
			[&](P_ABT_Nogood ngd)->bool {return !this->coherent(ngd.lhs(),myAgentView);});
}

void ABT_Solver::resolveConflict(const Message& m) {
}

bool ABT_Solver::consistent(const int& value) {
}

void ABT_Solver::sendMessage(const AgentID& id, const Message& message) {
}

ABT_Solver::Message ABT_Solver::getMessage() {
	message_t message;
	listener.recv(&message);
	Message x;
	x.ParseFromArray(message.data(), message.size());
	return x;
}

void ABT_Solver::setLink(const Message& message) {
	bool repeated = any_of(succeeding.begin(), succeeding.end(),
			[&](const P_EndPoint* i)->bool
			{	return i->id() == message.sender();});
	if (!repeated) {
		auto index = find_if(everybody.begin(), everybody.end(),
				[&](const P_EndPoint& agent)->bool
				{	return agent.id()==message.sender();});
		succeeding.push_back(&(*index));
		_INFO("Agent `%d' added to succeeding list.", index->id());
		//FIXME
	}
}

void ABT_Solver::checkAddLink(const Message& message) {
	for (int i = 0; i < message.nogood().lhs().assignments_size(); ++i) {
		int id = message.nogood().lhs().assignments(i).id();
		int value = message.nogood().lhs().assignments(i).value();
		if (!any_of(succeeding.begin(), succeeding.end(),
				[&](const P_EndPoint* i)->bool {return i->id()==id;})) {
			Message adl;
			adl.set_type(ABT_MessageType::T_ADDLINK);
			adl.set_sender(this->id);
			sendMessage(id, adl);
			Assignment assign;
			assign.set_id(id);
			assign.set_value(value);
			updateAgentView(assign);
		}
	}
}

void ABT_Solver::getAgentList() {
	_INFO( "Sending Request List to monitor agent ...");
	CommunicationPacket requestList;
	requestList.set_type(CP_MessageType::T_REQUEST_LIST);
	requestList.set_id(this->id);
	this->serverRquest.sendMessage(requestList);

	CommunicationPacket requestListAck;
	this->serverRquest.recvMessage(requestListAck);
	if (requestListAck.type() == CP_MessageType::T_REQUEST_ACK) {
		_INFO( "Monitor has accepted to send list of agents.");
	} else {
		_ERROR( "Unable to get agent list from monitor.\n"
		"\tTerminating silently. Goodbye.");
	}

	_INFO( "Waiting for all agents to came online...");
	CommunicationPacket listPacket;
	this->serverBroadcast.recvMessage(listPacket);
	_INFO( "I'm done.");
	if (listPacket.type() == CP_MessageType::T_LIST) {
		for (int i = 0; i < listPacket.others_size(); ++i) {
			this->everybody.push_back(listPacket.others(i));
			_INFO("New agent introduced by server:\n"
			"\tID:     %d\n"
			"\tHost:   %s\n"
			"\tPort:   %d",
					listPacket.others(i).id(), listPacket.others(i).host().c_str(), listPacket.others(i).port());
		}
		for (auto agent : everybody) {
			if (agent.id() < this->id)
				preceding.push_back(&agent);
			else if (agent.id() > this->id)
				succeeding.push_back(&agent);
		}
	}
}

ABT_Solver::AgentIdentifier::AgentIdentifier(const AgentID& agent_,
		const std::string& host_, const unsigned short & port_,
		zmq::context_t& context_) :
		id(agent_), host(host_), port(port_), socket(Socket(context_, ZMQ_PUSH)) {
}

ABT_EndPoint::ABT_EndPoint(const protocols::csp::abt::P_EndPoint& ep,
		zmq::context_t& context) :
		Socket(context, ZMQ_PUSH) {
	this->CopyFrom(ep);
}

