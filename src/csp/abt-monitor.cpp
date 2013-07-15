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

#include "../global.h"
#include "abt-monitor.h"
#include <sstream>
#include <algorithm>

using namespace AIT::CSP;
using namespace AIT::protocols::csp::abt;
using namespace zmq;
using namespace std;

ABT_Monitor::ABT_Monitor(const std::string& host,
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
			bool repeated =
					std::any_of(agents.begin(), agents.end(),
							[&](P_EndPoint i) {return (requestPacket.identity().id()==static_cast<int>(i.id()));});
			if (repeated) {
				_ERROR(
						"An agent with ID number `%d'is already registered. Ignoring this request",
						requestPacket.identity().id());
				P_CommunicationProtocol nackPacket;
				nackPacket.set_type(CP_MessageType::ERR_REPEATED_ID);
				this->responser.sendMessage(nackPacket);
			} else {
				agents.push_back(requestPacket.identity());
				P_CommunicationProtocol ackPacket;
				ackPacket.set_type(CP_MessageType::T_INTRODUCE_ACK);
				this->responser.sendMessage(ackPacket);
				++counter;
			}
		} else if (requestPacket.type() == CP_MessageType::T_REQUEST_LIST) {
			P_CommunicationProtocol requestListAck;
			requestListAck.set_type(CP_MessageType::T_REQUEST_ACK);
			this->responser.sendMessage(requestListAck);
			++counter;
		}
	} // end of while

	// Send list on broadcast channel
	P_CommunicationProtocol listPacket;
	listPacket.set_type(CP_MessageType::T_LIST);
	for (auto endPoint : this->agents) {
		auto item = listPacket.add_others();
		item->CopyFrom(endPoint);
	}
	this->publisher.sendMessage(listPacket);
}

void ABT_Monitor::run() {
}

