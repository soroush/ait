/*
 * abt-socket.h
 *
 *  Created on: May 24, 2013
 *      Author: soroush
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <zmq.hpp>
#include <string>
#include "protocols.pb.h"

namespace AIT {

class Socket: public zmq::socket_t {
public:
	Socket(zmq::context_t &context_, int type_);
	Socket(zmq::socket_t&& rhs);
	virtual ~Socket();
	size_t sendMessage(const protocols::ABT::P_CommunicationProtocol&);
	size_t sendMessage(const protocols::ABT::P_Message&);
	size_t recvMessage(protocols::ABT::P_CommunicationProtocol&);
	size_t recvMessage(protocols::ABT::P_Message&);
	static std::string getIP();
};

} /* namespace AIT */
#endif /* SOCKET_H_ */
