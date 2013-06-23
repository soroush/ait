/*
 * abt-endpoint.h
 *
 *  Created on: Jun 23, 2013
 *      Author: soroush
 */

#ifndef ABT_ENDPOINT_H_
#define ABT_ENDPOINT_H_

#include "abt.pb.h"
#include "zmq.hpp"

namespace AIT {

class Socket;

class ABT_EndPoint: public protocols::csp::abt::P_EndPoint {
public:
	ABT_EndPoint(const protocols::csp::abt::P_EndPoint& ep,
			zmq::context_t& context);
	Socket* socket() const;
private:
	Socket* socket_;
};

} /* namespace AIT */
#endif /* ABT_ENDPOINT_H_ */
