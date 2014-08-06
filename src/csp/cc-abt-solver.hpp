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

#ifndef CC_ABT_SOLVER_H_
#define CC_ABT_SOLVER_H_

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <queue>
#include <thread>
#include <mutex>
#include <stdlib.h>

#include "common_async.h"
#include "compound-assignment.h"
#include "abt-socket.h"
#include "csp-solver.h"
#include "../global.h"

namespace AIT {
namespace CSP {

class LIBRARY_API CC_ABT_Solver: public CSP_Solver {
public:
    CC_ABT_Solver(const std::string& serverHost,
            const unsigned short& serverResponderPort,
            const unsigned short& serverPublisherPort, const std::string& name,
            const std::string& xcspFile);
    virtual ~CC_ABT_Solver();
    void ABT();
    /**
     * This method extracts all constraints that current agent is evolved in
     * and stores pointers to them in a vector of Constraints.
     */
    void prune();

protected:
    class EndPoint: public protocols::csp::ccabt::P_EndPoint {
    public:
        EndPoint(const protocols::csp::ccabt::P_EndPoint& ep,
                zmq::context_t& context);
        ~EndPoint();
        Socket* socket() const;
    private:
        Socket* socket_;
    };

    struct Message {
        Message();
        Message(const Message& other);
        ~Message();
        Message& operator =(const Message& other);
        operator protocols::csp::ccabt::P_Message() const;
        void readFromProtocol(const protocols::csp::ccabt::P_Message&);

        protocols::csp::ccabt::P_MessageType type;
        int sender;
        Assignment assignment;
        CompoundAssignment nogood;
        int sequence;
        bool approved;
        std::vector<int> tv;
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
        operator protocols::csp::ccabt::P_Nogood();
    };
    bool consistentGammaMinus(const int&);
    bool consistent(const int&);
    bool consistent();
    Variable* findCulprit(const int& v);
    virtual int findLastCulprit();

    void connect();
    void checkAgentView(const int& priority=0);
    void chooseValue();
    void backtrack();
    void processInfo(const Message&);
    void updateAgentView();
    bool coherent(const CompoundAssignment& nogood);
    bool coherentGammaMinus(const Message& nogood);
    bool coherentSelf(const CompoundAssignment& nogood);
    void resolveConflict(const Message&);
    void checkAddLink(const Message&);
    void setLink(const Message&);
    void sendMessageOK(const AgentID&);
    void sendMessageOK_Monitor(const int& priority, const bool& approved);
    void sendMessageNGD(const AgentID&, Message&);
    void sendMessageSTP();
    void sendMessageADL(const AgentID&);
    // helper functions:
    void getAgentList();
    CompoundAssignment solve();
    void sendMessage(const AgentID&, const Message&);
    Message getMessage();
    void add(const CompoundAssignment&);
    void printNGS();
    void printNG(const CompoundAssignment&);
    void printAV();

    size_t priority;
    std::string name;
    std::list<std::vector<EndPoint>::iterator> m_gammaMinus; // Γ-
    std::list<std::vector<EndPoint>::iterator> m_gammaPlus; // Γ+
    std::vector<EndPoint>::iterator currentAgent;
    std::vector<EndPoint> everybody;
    std::list<Nogood> noGoodStore;

    std::string address;
    unsigned short port;
    std::string serverAddress;
    unsigned short serverResponderPort;
    unsigned short serverPublisherPort;

    void _messageReader();
    void _sentinel();
    bool validate(const Message&);
    std::list<Message> messageQueue;
    std::mutex messageRW;
    Semaphore messageCount;
    Semaphore agentReadyLock;
    std::thread messageReader;
    std::thread m_sentinel;

    zmq::context_t context;
    Socket listener;
    Socket serverRquest;
    Socket serverBroadcast;

    bool end, finished;
    Variable* me;
    std::forward_list<Constraint*> myConstraints;
    std::map<size_t, Variable*> priority2Variable;
    std::map<const Variable*, size_t> variable2Priority;
private:
    std::vector<int> sequences;
    bool findInGammaMinus(const Variable* v);
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* CC_ABT_SOLVER_H_ */
