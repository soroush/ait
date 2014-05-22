/*
 AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.

 Copyright (c) 2012,2013 Soroush Rabiei <soroush-r@users.sf.net>,
 Roya Ghasemzadeh <ghasemzadeh.roya1@gmail.com>

 AIT is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 See the file COPYING included with this distribution for more
 information.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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

#include "common_async.h"
#include "compound-assignment.h"
#include "abt-socket.h"
#include "csp-solver.h"
#include "../global.h"

namespace AIT {
namespace CSP {

class LIBRARY_API ABT_Solver: public CSP_Solver {
	friend class AABT_Solver;
public:
	ABT_Solver(const std::string&, const unsigned short&, const unsigned short&,
			const AgentID&, const size_t&);
	virtual ~ABT_Solver();
	void ABT();
	virtual void prepareProblem()=0;
	/**
	 * This method extracts all constraints that current agent is evolved in
	 * and stores pointers to them in a vector of Constraints.
	 */
	void prune();

protected:
	class EndPoint: public protocols::csp::abt::P_EndPoint {
	public:
		EndPoint(const protocols::csp::abt::P_EndPoint& ep,
				zmq::context_t& context);
		Socket* socket() const;
	private:
		Socket* socket_;
	};

	struct Message {
		Message();
		Message(const Message& other);
		~Message();
		Message& operator =(const Message& other);
		operator protocols::csp::abt::P_Message() const;
		void readFromProtocol(const protocols::csp::abt::P_Message&);

		protocols::csp::abt::P_MessageType type;
		AgentID sender;
		Assignment assignment;
		CompoundAssignment nogood;
	};

	struct Nogood {
		Nogood();
		Nogood(const Nogood& other);
		Nogood(const CompoundAssignment& lhs, const Assignment& rhs);
		~Nogood();
		CompoundAssignment lhs;
		Assignment rhs;
		bool operator ==(const Nogood& other) const;
		bool operator !=(const Nogood& other) const;
		Nogood& operator =(const Nogood& other);
		operator protocols::csp::abt::P_Nogood();
	};
	virtual bool consistent(const int&, const CompoundAssignment&);
	int getValueOf(const int&, bool&);
	virtual int findCulprit(const int& v) = 0;
	virtual int findLastCulprit() = 0;
	virtual int findCulpritsValue(const int& culpirtsID) = 0;

	void connect();
	void checkAgentView();
	int chooseValue();
	void backtrack();
	void processInfo(const Message&);
	void updateAgentView(const Assignment&);
	bool coherent(const CompoundAssignment& nogood,
			const CompoundAssignment& assign);
	bool coherentSelf(const CompoundAssignment& nogood,
			const CompoundAssignment& assign);
	void resolveConflict(const Message&);
	void checkAddLink(const Message&);
	void setLink(const Message&);
	void sendMessageOK(const AgentID&);
	void sendMessageNGD(const AgentID&, Message&);
	void sendMessageSTP();
	void sendMessageADL(const AgentID&);
	// helper functions:
	void getAgentList();
	CompoundAssignment solve();
	void sendMessage(const AgentID&, const Message&);
	protocols::csp::abt::P_Message getMessage();
	void add(const CompoundAssignment&);
	void printNGS();
	void printAV();

	size_t agentCount;

	AgentID id;
	std::list<std::vector<EndPoint>::iterator> preceding; // Γ+
	std::list<std::vector<EndPoint>::iterator> succeeding; // Γ-
	std::vector<EndPoint> everybody;
	std::list<Nogood> noGoodStore;
	// FIXME: Integrate other parts with new design: Copy domain from problem definition
	std::vector<int> domain;
	CompoundAssignment agentView;
	std::vector<int> agentViewX;
	bool* assignedAgents;
	int value;

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
	std::forward_list<Constraint> myConstraints;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* ABT_SOLVER_H_ */
