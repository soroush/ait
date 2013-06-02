/*
 * abt-monitor.cpp
 *
 *  Created on: May 21, 2013
 *      Author: soroush
 */

#include "global.h"
#include "abt-monitor.h"
#include <sstream>

/* namespace AIT */

using namespace AIT;
using namespace zmq;
using namespace std;

AIT::ABT_Monitor::ABT_Monitor(const std::string& host,
		const unsigned short & responserPort,
		const unsigned short & publisherPort, const size_t& agentCount) :
		host_(host), portNumberResponser(responserPort), portNumberPublisher(
				publisherPort), context(2), responser(context, ZMQ_REP), publisher(
				context, ZMQ_PUB), agentCount_(agentCount) {
}

ABT_Monitor::ABT_Monitor() :
		context(2), responser(context, ZMQ_REP), publisher(context, ZMQ_PUB) {
}

ABT_Monitor::~ABT_Monitor() {
	this->responser.close();
	this->publisher.close();
	this->context.close();
}

void ABT_Monitor::start() {
	using namespace protocols::ABT;
	// Listen for incoming requests
	stringstream address;
	address << "tcp://*:" << this->portNumberResponser;
	try {
		this->responser.bind(address.str().data());
	} catch (zmq::error_t* e) {
		cerr << e->what() << endl;
	}
	// Open broadcast channel
	address.str("");
	address << "tcp://*:" << this->portNumberPublisher;
	try {
		_INFO("Binding to %s:%d broadcast end point.",
				"*", this->portNumberPublisher)
		this->publisher.bind(address.str().data());
	} catch (zmq::error_t e) {
		_ERROR("Unable to bind to broadcast channel at %s:%d",
				"*", this->portNumberPublisher)
		cerr << e.what() << endl;
	}
	_INFO("Successfully bound to broadcast channel.");

	size_t counter = 0;
	while (counter != 2 * this->agentCount_) {
		P_CommunicationProtocol requestPacket;
		this->responser.recvMessage(requestPacket);
		if (requestPacket.type() == CP_MessageType::T_INTRODUCE) {
			_INFO(
					"New agent subscribed. This information is introduced by new agent"
					"\n\tHost          : %s"
					"\n\tListener Port : %d"
					"\n\tAgent ID      : %d",
					requestPacket.identity().host().data(), requestPacket.identity().port(), requestPacket.identity().id());
			agents.push_back(requestPacket.identity());
			P_CommunicationProtocol ackPacket;
			ackPacket.set_type(CP_MessageType::T_INTRODUCE_ACK);
			this->responser.sendMessage(ackPacket);
		} else if (requestPacket.type() == CP_MessageType::T_REQUEST_LIST) {
			// TODO Check if an agent with this ID is already connected or not
			P_CommunicationProtocol requestListAck;
			requestListAck.set_type(CP_MessageType::T_REQUEST_ACK);
			this->responser.sendMessage(requestListAck);
		}
		++counter;
	} // end of while
	  // TODO: Encode `agents' into a message and send it to everybody
	P_CommunicationProtocol listPacket;
	listPacket.set_type(CP_MessageType::T_LIST);
	for (auto endPoint : this->agents) {
		auto item = listPacket.add_others();
		item->CopyFrom(endPoint);
	}
	cout << this->publisher.sendMessage(listPacket);
}

void AIT::ABT_Monitor::run() {
}

