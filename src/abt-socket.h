/*
 * abt-socket.h
 *
 *  Created on: May 24, 2013
 *      Author: soroush
 */

#ifndef ABT_SOCKET_H_
#define ABT_SOCKET_H_

#include <zmq.hpp>
#include <string>
#include "protocols.pb.h"

namespace AIT {

class ABT_Socket: public zmq::socket_t {
public:
	ABT_Socket(zmq::context_t &context_, int type_);
	ABT_Socket(zmq::socket_t&& rhs);
	virtual ~ABT_Socket();
	size_t sendMessage(const ABT_CommunicationProtocol&);
	size_t sendMessage(const ABT_Message&);
	size_t recvMessage(ABT_CommunicationProtocol&);
	size_t recvMessage(ABT_Message&);
	static std::string getIP();
};

} /* namespace AIT */
#endif /* ABT_SOCKET_H_ */
