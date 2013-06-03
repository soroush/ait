/*
 * abt-solver.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: soroush
 */

#include "abt-solver.h"

using namespace AIT;
using namespace std;
using namespace zmq;
using namespace protocols::csp::abt;

inline ABT_Solver::ABT_Solver(const std::string& host_,
		const unsigned short& port_, const std::string& serverHost_,
		const unsigned short& serverResponderPort_,
		const unsigned short& serverPublisherPort_) :
		address(host_), port(port_), serverAddress(serverHost_), serverResponderPort(
				serverResponderPort_), serverPublisherPort(
				serverPublisherPort_), context(2), listener(context, ZMQ_PULL), serverRquest(
				context, ZMQ_REQ), serverBroadcast(context, ZMQ_SUB) {
}

inline ABT_Solver::~ABT_Solver() {
	// delete this->myValue;
	serverBroadcast.close();
	serverRquest.close();
	listener.close();
	context.close();
}

inline void ABT_Solver::solve() {
	myValue = 0; // FIXME fix this
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

inline void ABT_Solver::connect() {
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
			}
		} else {
			_ERROR("Server didn't reply.");
		}
	} else {
		_ERROR("Unable to send introduction message.");
	}
}

inline void ABT_Solver::checkAgentView() {
	if (!consistent(myValue)) {
		chooseValue(myValue);
		if (myValue != 0) { // FIXME
			for (const auto& agent : succeeding) {
				sendMessage(agent->id(), Message());
			}
		} else {
			backtrack();
		}
	} // end if !consistent
}

inline void ABT_Solver::processInfo(const Message& m) {
	updateAgentView(m.ok_data().assignment());
	checkAgentView();
}

inline void ABT_Solver::chooseValue(const int& value) {
}

inline void ABT_Solver::backtrack() {
}

inline void ABT_Solver::updateAgentView(
		const P_Assignment& assignment) {
}

inline void ABT_Solver::resolveConflict(const Message& m) {
}

inline bool ABT_Solver::consistent(const int& value) {
}

inline void ABT_Solver::sendMessage(const AgentID& i, const Message& m) {
}

ABT_Solver::Message ABT_Solver::getMessage() {
	message_t message;
	listener.recv(&message);
	Message x;
	x.ParseFromArray(message.data(), message.size());
	return x;
}

inline void ABT_Solver::setLink(const Message& m) {
}

inline void ABT_Solver::getAgentList() {
	CommunicationPacket packet;
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
		_ERROR(
				"Unable to get agent list from monitor.\nTerminating silently. Goodbye.");
	}
	_INFO( "Waiting for all agents to came online...");
	this->serverBroadcast.recvMessage(packet);
	_INFO( "I'm done.");
//	if (packet.type() == CP_MessageType::T_LIST) {
//		for (int i = 0; i < packet.others_size(); ++i) {
//			this->everybody.push_back(packet.others(i));
//		}
//		for (auto& agent : everybody) {
//			if (agent.id() < this->id)
//				preceding.push_back(&agent);
//			else if (agent.id() > this->id)
//				succeeding.push_back(&agent);
//			else
//				_ERROR("Somebody has same ID with me! ()\n")
//		}
//	}
}

