/*
 * abt-endpoint.cpp
 *
 *  Created on: Jun 23, 2013
 *      Author: soroush
 */

#include "abt-endpoint.h"
#include "abt-socket.h"

using namespace AIT;
using namespace zmq;
using namespace protocols::csp;
using namespace protocols::csp::abt;

Socket* ABT_EndPoint::socket() const {
	return (this->socket_);
}

ABT_EndPoint::ABT_EndPoint(const protocols::csp::abt::P_EndPoint& ep,
		zmq::context_t& context) :
		socket_(new Socket(context, ZMQ_PUSH)) {
	this->CopyFrom(ep);
}
