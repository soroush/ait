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

#include "global.h"
#include "common_csp.h"
#include "common_async.h"
#include "abt-socket.h"
#include "abt.pb.h"

namespace AIT {

class ABT_EndPoint: public protocols::csp::abt::P_EndPoint, public Socket {
public:
	ABT_EndPoint(const protocols::csp::abt::P_EndPoint& ep,
			zmq::context_t& context);
};

class ABT_Solver {

public:

	typedef protocols::csp::abt::P_CommunicationProtocol CommunicationPacket;
	typedef protocols::csp::abt::P_Message Message;

	ABT_Solver(const std::string&, const unsigned short&, const std::string&,
			const unsigned short&, const unsigned short&, const AgentID&);
	virtual ~ABT_Solver();

	void connect();
	void ABT();
	void checkAgentView();
	void chooseValue();
	void backtrack();
	void processInfo(const Message&); // OK
	void updateAgentView(const Assignment&);
	bool coherent(const protocols::csp::P_CompoundAssignment& nogood,
			const protocols::csp::P_CompoundAssignment& assign);
	void resolveConflict(const Message&);
	void checkAddLink(const Message&);
	void setLink(const Message&);
	bool consistent(const int&);
	void sendMessage(const AgentID&, const Message&);
	Message getMessage();

private:
	struct AgentIdentifier {
		AgentIdentifier(const AgentID&, const std::string&,
				const unsigned short&, zmq::context_t&);
		AgentID id;
		std::string host;
		unsigned short port;
		Socket socket;
	};
	int value;
	void getAgentList();
	AgentID id;
	std::list<protocols::csp::abt::P_EndPoint*> preceding; // Γ+
	std::list<protocols::csp::abt::P_EndPoint*> succeeding; // Γ-
	std::vector<protocols::csp::abt::P_EndPoint> everybody;
//	std::list<std::vector<AgentIdentifier>::iterator> preceding; // Γ+
//	std::list<std::vector<AgentIdentifier>::iterator> succeeding; // Γ-
//	std::vector<AgentIdentifier> everybody;
	CompoundAssignment myAgentView;
	std::list<protocols::csp::abt::P_ABT_Nogood> NoGoodStore;

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
