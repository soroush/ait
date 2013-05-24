/*
 * abt-monitor.h
 *
 *  Created on: May 21, 2013
 *      Author: soroush
 */

#ifndef ABT_MONITOR_H_
#define ABT_MONITOR_H_

#include <string>
#include <vector>
#include <zmq.hpp>
#include "common_async.hpp"
#include "abt-solver.hpp"

namespace AIT {

class ABT_Monitor {
public:
	ABT_Monitor();
	ABT_Monitor(const std::string& host, const unsigned short& responserPort,
			const unsigned short& publisherPort, const size_t& agentCount=2);
	virtual ~ABT_Monitor();
	void start();
private:
	unsigned short portNumberResponser, portNumberPublisher;
	std::string host_;
	zmq::context_t context;
	zmq::socket_t publisher;
	zmq::socket_t responser;
	size_t agentCount_;
	std::vector<ABT_CommunicationProtocol_AgentIdentity> agents;
};

} /* namespace AIT */
#endif /* ABT_MONITOR_H_ */
