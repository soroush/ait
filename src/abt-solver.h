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
#include <semaphore.h>
#include <stdlib.h>

#include "global.h"
#include "common_async.h"
#include "compound-assignment.h"
#include "assignment.h"
#include "abt-socket.h"
#include "abt-nogood.h"
#include "abt-message.h"

namespace AIT {

class ABT_EndPoint: public protocols::csp::abt::P_EndPoint {
public:
	ABT_EndPoint(const protocols::csp::abt::P_EndPoint& ep,
			zmq::context_t& context);
	Socket* socket() const;
private:
	Socket* socket_;
};

class ABT_Solver {

public:
	ABT_Solver(const std::string&, const unsigned short&, const std::string&,
			const unsigned short&, const unsigned short&, const AgentID&);
	virtual ~ABT_Solver();

	void connect();
	void ABT();
	void checkAgentView();
	int chooseValue();
	void backtrack();
	void processInfo(const protocols::csp::abt::P_Message); // OK
	void updateAgentView(const protocols::csp::P_Assignment);
	bool coherent(const protocols::csp::P_CompoundAssignment nogood,
			const protocols::csp::P_CompoundAssignment assign);
	void resolveConflict(const protocols::csp::abt::P_Message);
	void checkAddLink(const protocols::csp::abt::P_Message);
	void setLink(const protocols::csp::abt::P_Message);
	bool consistent(const int&);
	void sendMessageOK(const AgentID&);
	void sendMessageNGD(const AgentID&, protocols::csp::abt::P_Message);
	void sendMessageSTP();
	void sendMessageADL(const AgentID&);
	protocols::csp::abt::P_Message getMessage();

private:
	int value;
	void getAgentList();
	int findCulprit(const int& v);
	int findLastCulprit();
	int findCulpritsValue(const int& culpirtsID);
	protocols::csp::P_CompoundAssignment solve();
	void sendMessage(const AgentID&, const protocols::csp::abt::P_Message);
	void initializeDomain();
	void add(const protocols::csp::P_CompoundAssignment&);
	void add(const protocols::csp::abt::P_Nogood&);

	AgentID id;
	std::list<std::vector<ABT_EndPoint>::iterator> preceding; // Γ+
	std::list<std::vector<ABT_EndPoint>::iterator> succeeding; // Γ-
	std::vector<ABT_EndPoint> everybody;
	protocols::csp::P_CompoundAssignment agentView;
	std::list<protocols::csp::abt::P_Nogood> noGoodStore;
	std::vector<int> domain;

	std::string address;
	unsigned short port;
	std::string serverAddress;
	unsigned short serverResponderPort;
	unsigned short serverPublisherPort;

	static void* _messageReader(void* param);
	std::queue<protocols::csp::abt::P_Message> messageQueue;
	pthread_mutex_t messageRW;
	sem_t messageCount;
	sem_t agentReadyLock;
	pthread_t messageReader;

	zmq::context_t context;
	Socket *listener;
	Socket serverRquest;
	Socket serverBroadcast;

	bool end;
};

}
#endif /* ABT_SOLVER_H_ */
