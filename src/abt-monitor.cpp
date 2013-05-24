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
	stringstream address;
	address << "tcp://*:" << this->portNumberResponser;
	try {
		this->responser.bind(address.str().data());
	} catch (zmq::error_t* e) {
		cerr << e->what() << endl;
	}
	while (this->agents.size() < this->agentCount_) {
		message_t message;
		this->responser.recv(&message);
		_INFO("Receive a new message");
		ABT_CommunicationProtocol packet;
		packet.ParseFromArray(message.data(), message.size());
		if (packet.type()
				== ABT_CommunicationProtocol_MessageType_T_INTRODUCE) {
			_INFO(
					"New agent subscribed. This information is introduced by new agent"
					"\n\tHost          : %s"
					"\n\tListener Port : %d"
					"\n\tAgent ID      : %d",
					packet.identity().host().data(), packet.identity().port(), packet.identity().id());
			agents.push_back(packet.identity());
		}
		ABT_CommunicationProtocol ackPacket;
		ackPacket.set_type(ABT_CommunicationProtocol_MessageType_T_ACK);
		size_t dataLenght = ackPacket.ByteSize();
		message_t ackMessage(dataLenght);
		ackPacket.SerializeToArray(ackMessage.data(), dataLenght);
		this->responser.send(ackMessage);
	}
	address.str("");
	address << "tcp://*:" << this->portNumberPublisher;
	this->publisher.bind(address.str().data());
	message_t message;
	// TODO: Encode `agents' into a message and send it to everybody
	this->publisher.send(message);
}
