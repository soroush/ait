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
				serverPublisherPort_), context(2), serverRquest(context,
				ZMQ_REQ), serverBroadcast(context, ZMQ_SUB), end(false) {
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
		P_Message m = getMessage();
		switch (m.type()) {
		case P_MessageType::T_OK:
			_INFO("OK message received from %d : (%d,%d).",
					m.sender(), m.assignment().id(), m.assignment().value());
			processInfo(m);
			break;
		case P_MessageType::T_NOGOOD:
			_INFO("NGD message received from %d.", m.sender());
			resolveConflict(m);
			break;
		case P_MessageType::T_ADDLINK:
			_INFO("ADL message received from %d.", m.sender());
			setLink(m);
			break;
		case P_MessageType::T_STOP:
			_INFO("STP message received from %d.", m.sender());
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

void ABT_Solver::processInfo(const P_Message m) {
	updateAgentView(m.assignment());
	checkAgentView();
}

int ABT_Solver::chooseValue() {
	for (auto &v : domain) {
		_INFO("Trying to find a consistent value...");
		/* */
		bool eliminated = false;
		eliminated = any_of(noGoodStore.begin(), noGoodStore.end(),
				[&](const P_Nogood &ngd) {
					return (ngd.rhs().id()==this->id and ngd.rhs().value()==v);
				});
		if (!eliminated) {
			if (consistent(v)) {
				_INFO("A new value has been assigned: %d", v);
				return v;
			} else {
				int culpritID = findCulprit(v);
				int culpirtValue = findCulpritsValue(culpritID);
				/* TEST FIXME Erase this*/
				cout << "CULPIRT" << culpritID << ':' << culpirtValue << endl;
				/* EOT */
				P_Nogood ngd;
				P_Assignment *lhs_value = ngd.mutable_lhs()->add_assignments();
				lhs_value->set_id(culpritID);
				lhs_value->set_value(culpirtValue);

				ngd.mutable_rhs()->set_id(this->id);
				ngd.mutable_rhs()->set_value(v);

				// Add to nogoodStore only of not exists
				noGoodStore.push_back(ngd);
			}
		}
	}
	return 0;
}

void ABT_Solver::backtrack() {
	P_CompoundAssignment newNogood = solve();
	/* TEST CODE ==== FIXME Erase this */
	for (const auto& a : agentView.assignments()) {
		cout << "AV: (" << a.id() << ',' << a.value() << ')' << endl;
	}
	for (const auto& a : newNogood.assignments()) {
		cout << "NGD: (" << a.id() << ',' << a.value() << ')' << endl;
	}
	for (const auto& a : noGoodStore) {
		for (const auto& ngd : a.lhs().assignments())
			cout << "NGDS: (" << ngd.id() << ',' << ngd.value() << ')' << "=> "
		<< a.rhs().id() << "!=" << a.rhs().value() << endl;
	}
	/* END OF TEST CODE */
	if (newNogood.assignments_size() == 0) {
		end = true;
		sendMessageSTP();
	} else {
		P_Message nogoodMessage;
		nogoodMessage.set_allocated_nogood(&newNogood);
		AgentID lastCulpirt = findLastCulprit();
		sendMessageNGD(lastCulpirt, nogoodMessage);
		P_Assignment clearCulpirt;
		clearCulpirt.set_id(lastCulpirt);
		updateAgentView(clearCulpirt);
		nogoodMessage.release_assignment();
		nogoodMessage.release_nogood();
		checkAgentView();
	}
}

void ABT_Solver::updateAgentView(const P_Assignment assignment) {
	if (assignment.has_value()) {
		_INFO("updateAgentView(%d,%d):%d",
				assignment.id(), assignment.value(), assignment.has_value());
		// remove old value of given assignment
		bool found = false;
		for (auto& a : *agentView.mutable_assignments()) {
			if (a.id() == assignment.id()) {
				a.set_value(assignment.value());
				found = true;
				break;
			}
		}
		// add new value
		if (!found) {
			P_Assignment* newAssign = agentView.add_assignments();
			newAssign->CopyFrom(assignment);
		}
		// remove invalid nogoods
		for (auto i = noGoodStore.begin(); i != noGoodStore.end();) {
			if (coherent(i->lhs(), agentView))
				++i;
			else
				i = noGoodStore.erase(i);
		}
	} else {
		// Clear item from agentView with specified ID

		for (int i = 0; i < agentView.assignments_size(); ++i) {
			if (agentView.assignments(i).id() == assignment.id())
				agentView.mutable_assignments()->DeleteSubrange(i, 1);
		}
		// remove invalid nogoods
		for (auto i = noGoodStore.begin(); i != noGoodStore.end();) {
			if (coherent(i->lhs(), agentView))
				++i;
			else
				i = noGoodStore.erase(i);
		}
	}
}

void ABT_Solver::resolveConflict(const P_Message msg) {
	P_CompoundAssignment myselfAssignment;
	P_Assignment* assign = myselfAssignment.add_assignments();
	assign->set_id(this->id);
	assign->set_value(this->value);

	P_CompoundAssignment totalView;
	totalView.CopyFrom(agentView);
	P_Assignment* a2 = totalView.add_assignments();
	a2->set_id(this->id);
	a2->set_value(this->value);

	if (coherent(msg.nogood(), totalView)) {
		_INFO("Received NGD was coherent with total view");
		checkAddLink(msg);
		add(msg.nogood());
		this->value = 0; // FIXME
		checkAgentView();
	} else if (coherent(msg.nogood(), myselfAssignment)) {
		_INFO("Received NGD was coherent with myself view");
		sendMessageOK(msg.sender());
	}else{
		_INFO("Received NGD was not coherent.");
	}
}

bool ABT_Solver::consistent(const int& v) {
	_INFO("Checking consistency of (%d,%d)", this->id, v);
	if (v == 0)
		return false;
	for (const auto& a : agentView.assignments()) {
		/* WARNING : VERY STUPID CODE AHEAD! */
		int delta_a = a.id() - this->id;
		int delta_v = a.value() - v;
		if (delta_a < 0)
			delta_a *= -1;
		if (delta_v < 0)
			delta_v *= -1;
		/* END OF VERY SRUPID CODE */
		if (a.value() == v or delta_a == delta_v) {
			_INFO("(%d,%d) <=> (%d,%d)", this->id, v, a.id(), a.value());
			return false;
		}
	}
	return true;
}

P_Message ABT_Solver::getMessage() {
	_INFO("Waiting for message queue ...");
	sem_wait(&messageCount);
	pthread_mutex_lock(&this->messageRW);
	P_Message x = this->messageQueue.front();
	this->messageQueue.pop();
	pthread_mutex_unlock(&this->messageRW);
	return x;
}

void ABT_Solver::setLink(const P_Message message) {
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

void ABT_Solver::checkAddLink(const P_Message message) {
	for (const auto& assignment : message.nogood().assignments()) {
		if (!any_of(succeeding.begin(), succeeding.end(),
				[&](const std::vector<ABT_EndPoint>::iterator i)->bool
				{	return i->id()==assignment.id();})) {
			sendMessageADL(id);
			P_Assignment assign;
			assign.set_id(id);
			assign.set_value(value);
			updateAgentView(assign);
		}
	}
}

bool AIT::ABT_Solver::coherent(const P_CompoundAssignment nogood,
		const P_CompoundAssignment assign) {
	for (const auto &n : nogood.assignments()) {
		bool found = false;
		for (const auto &a : assign.assignments()) {
			if (a.id() == n.id() and a.value() != n.value())
				return false;
			else if (a.id() == n.id() and a.value() == n.value())
				found = true;
		}
//		if (!found)
//			return false;
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
		for (auto &agent : everybody) {
			stringstream address;
			address << "tcp://";
			address << agent.host() << ":" << agent.port();
			agent.socket()->connect(address.str().c_str());
		}
		/* TEST CODE */
		for (const auto &agent : succeeding) {
			cout << "SUC: " << agent->id() << endl;
		}
		for (const auto &agent : preceding) {
			cout << "PRE: " << agent->id() << endl;
		}
		/* END OF TEST */
	}
}

ABT_EndPoint::ABT_EndPoint(const protocols::csp::abt::P_EndPoint& ep,
		zmq::context_t& context) :
		socket_(new Socket(context, ZMQ_PUSH)) {
	this->CopyFrom(ep);
}

int ABT_Solver::findCulprit(const int& v) {
	AgentID lastCulpirt = 0;
	for (const auto& a : agentView.assignments()) {
		int delta_a = (a.id() - id); // FIXME Think about :|
		int delta_v = (a.value() - v);
		if (delta_a < 0)
			delta_a *= -1;
		if (delta_v < 0)
			delta_v *= -1;
		cout << "CULPIRT CHECK" << '(' << a.id() << ',' << a.value() << ')'
				<< endl;
		cout << "CULPIRT VALS" << '(' << delta_a << ',' << delta_v << ')'
				<< endl;
		if (a.value() == v or delta_a == delta_v) { // culprit
			lastCulpirt = (a.id() > lastCulpirt) ? a.id() : lastCulpirt; // and last
		}
	}
	return lastCulpirt;
}

int AIT::ABT_Solver::findCulpritsValue(const int& culpirtsID) {
	for (const auto& a : agentView.assignments()) {
		if (a.id() == culpirtsID)
			return a.value();
	}
	return 0;
}

void AIT::ABT_Solver::sendMessageOK(const AgentID& agent) {
	P_Message ok;
	ok.set_sender(this->id);
	ok.set_type(P_MessageType::T_OK);
	ok.mutable_assignment()->set_id(this->id);
	ok.mutable_assignment()->set_value(this->value);
	_INFO("Sending OK message to %d", agent);
	sendMessage(agent, ok);
}

void AIT::ABT_Solver::sendMessageNGD(const AgentID& agent, P_Message ngd) {
	ngd.set_sender(this->id);
	ngd.set_type(P_MessageType::T_NOGOOD);
	_INFO("Sending NGD message to %d", agent);
	sendMessage(agent, ngd);
}

void AIT::ABT_Solver::sendMessageSTP() {
	P_Message stop;
	stop.set_type(P_MessageType::T_STOP);
	stop.set_sender(this->id);
	_INFO("Sending STP message.");
	sendMessage(0, stop);
}

void ABT_Solver::sendMessageADL(const AgentID& agent) {
	if (agent == this->id)
		return;
	P_Message adl;
	adl.set_sender(this->id);
	adl.set_type(P_MessageType::T_ADDLINK);
	_INFO("Sending ADL message to %d", agent);
	sendMessage(agent, adl);
}

P_CompoundAssignment ABT_Solver::solve() {
	CompoundAssignment newNogood;
	for (const auto& ngd : this->noGoodStore) {
		for (const auto& assign : ngd.lhs().assignments()) {
			bool found = false;
			for (auto& a : *newNogood.mutable_assignments()) {
				if (a.id() == assign.id()) {
					a.set_value(assign.value());
					found = true;
					break;
				}
			}
			// didn't find any assignment with same ID:
			if (!found) {
				P_Assignment* newAssignment = newNogood.add_assignments();
				newAssignment->CopyFrom(assign);
			}
		}
	}
	return newNogood;
}

void ABT_Solver::sendMessage(const AgentID& agent, const P_Message message) {
	if(agent == this->id)
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
				//_INFO("ACTUAL SEND: %d, %d, %d",
				//		agent, message.ByteSize(), ep.ByteSize());
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

void ABT_Solver::add(const P_CompoundAssignment& ca) {
	P_Nogood ngd;
	for (const auto& a : ca.assignments()) {
		if (a.id() != this->id) {
			P_Assignment* assign = ngd.mutable_lhs()->add_assignments();
			assign->CopyFrom(a);
		} else {
			ngd.mutable_rhs()->set_id(this->id);
			ngd.mutable_rhs()->set_value(this->value);
		}
	}
	noGoodStore.push_back(ngd);
}

int ABT_Solver::findLastCulprit() {
	AgentID lastID = 0;
	for (const auto& ngd : noGoodStore) {
		for (const auto& a : ngd.lhs().assignments()) {
			if (a.id() > lastID)
				lastID = a.id();
		}
	}
	return lastID;
}

void* ABT_Solver::_messageReader(void* param) {
	// Let's listen to other agents:
	ABT_Solver* solver = (reinterpret_cast<ABT_Solver*>(param));
	solver->listener = new Socket(solver->context, ZMQ_PULL);
	stringstream addressName;
	solver->address = "127.0.0.1"; //Socket::getIP();
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

Socket* ABT_EndPoint::socket() const {
	return (this->socket_);
}

