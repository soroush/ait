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
#include "abt-socket.h"

namespace AIT {

class ABT_EndPoint;
struct ABT_Nogood;
struct ABT_Message;
struct Assignment;

class ABT_Solver {

public:
	ABT_Solver(const std::string&, const unsigned short&, const unsigned short&,
			const AgentID&, const size_t&);
	virtual ~ABT_Solver();

	void connect();
	void ABT();
	void checkAgentView();
	int chooseValue();
	void backtrack();
	void processInfo(const ABT_Message&);
	void updateAgentView(const Assignment&);
	bool coherent(const CompoundAssignment& nogood,
			const CompoundAssignment& assign);
	bool coherentSelf(const CompoundAssignment& nogood,
			const CompoundAssignment& assign);
	void resolveConflict(const ABT_Message&);
	void checkAddLink(const ABT_Message&);
	void setLink(const ABT_Message&);
	bool consistent(const int&);
	void sendMessageOK(const AgentID&);
	void sendMessageNGD(const AgentID&, ABT_Message&);
	void sendMessageSTP();
	void sendMessageADL(const AgentID&);
	protocols::csp::abt::P_Message getMessage();

private:
	int value;
	// helper functions:
	void getAgentList();
	int findCulprit(const int& v);
	int findLastCulprit();
	int findCulpritsValue(const int& culpirtsID);
	CompoundAssignment solve();
	void sendMessage(const AgentID&, const ABT_Message&);
	void initializeDomain();
	void add(const CompoundAssignment&);
	void printNGS();
	void printAV();

	int agentCount;

	AgentID id;
	std::list<std::vector<ABT_EndPoint>::iterator> preceding; // Γ+
	std::list<std::vector<ABT_EndPoint>::iterator> succeeding; // Γ-
	std::vector<ABT_EndPoint> everybody;
	CompoundAssignment agentView;
	std::list<ABT_Nogood> noGoodStore;
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
