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
#include "abt.pb.h"
#include "aabt.pb.h"

namespace AIT {

class Socket: public zmq::socket_t {
public:
	Socket(zmq::context_t &context_, int type_);
	Socket(zmq::socket_t&& rhs);
	virtual ~Socket();
	// Common
	size_t sendMessage(const protocols::csp::abt::P_CommunicationProtocol);
	size_t recvMessage(protocols::csp::abt::P_CommunicationProtocol&);
	// ABT
	size_t sendMessage(const protocols::csp::abt::P_Message);
	size_t recvMessage(protocols::csp::abt::P_Message&);
	// AABT
	size_t sendMessage(const protocols::csp::aabt::P_Message);
	size_t recvMessage(protocols::csp::aabt::P_Message&);
	static std::string getIP();
};

} /* namespace AIT */
#endif /* SOCKET_H_ */
