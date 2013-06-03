/*
 * abt-solver.h
 *
 *  Created on: Apr 30, 2013
 *      Author: soroush
 */

#ifndef ABT_SOLVER_H_
#define ABT_SOLVER_H_

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <queue>
#include <pthread.h>
#include <stdlib.h>
#include <zmq.hpp>

#include "global.h"
#include "common_csp.h"
#include "common_async.h"
#include "abt-socket.h"
#include "abt.pb.h"

namespace AIT {

class ABT_Solver {

public:

	typedef protocols::csp::abt::P_CommunicationProtocol CommunicationPacket;
	typedef protocols::csp::abt::P_Message Message;

	ABT_Solver(const std::string&, const unsigned short&, const std::string&,
			const unsigned short&, const unsigned short&);
	virtual ~ABT_Solver();

	void connect();
	void solve();
	void checkAgentView();
	void chooseValue(const int&);
	void backtrack();
	void processInfo(const Message&); // OK
	void updateAgentView(const Assignment&);
	bool coherent(const Nogood& nogood, const Assignment& assign);
	void resolveConflict(const Message&);
	void checkAddLink(const Message&);
	void setLink(const Message&);
	bool consistent(const int&);
	void sendMessage(const AgentID&, const Message&);
	Message getMessage();

private:
	int myValue;
	void getAgentList();
	AgentID id;
	std::list<protocols::csp::abt::P_EndPoint*> preceding; // Γ+
	std::list<protocols::csp::abt::P_EndPoint*> succeeding; // Γ-
	std::vector<protocols::csp::abt::P_EndPoint> everybody;
	CompoundAssignment myAgentView;

	std::string address;
	unsigned short port;
	std::string serverAddress;
	unsigned short serverResponderPort;
	unsigned short serverPublisherPort;

	zmq::context_t context;
	Socket listener;
	Socket serverRquest;
	Socket serverBroadcast;
};

}
#endif /* ABT_SOLVER_H_ */
