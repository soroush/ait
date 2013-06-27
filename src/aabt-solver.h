#include <iostream>
#include <vector>
#include <queue>
#include <forward_list>
#include <pthread.h>
#include <semaphore.h>
#include "aabt-assignment.h"
#include "aabt-explanation.h"
#include "abt-socket.h"
#include "abt-endpoint.h"
#include "aabt.pb.h"
#include "common_async.h"

namespace AIT {

struct AABT_Message;
struct AABT_Nogood;
struct CVOrderData;

class AABT_Solver {
public:
	int n; //problem size

	typedef std::vector<AABT_Assignment> CompoundAssignment;

	typedef std::forward_list<AABT_Nogood> nogood_store;

public:

	int id;
	std::forward_list<std::forward_list<ABT_EndPoint>::iterator> preceding; // Γ+
	std::forward_list<std::forward_list<ABT_EndPoint>::iterator> succeeding; // Γ-
	std::forward_list<ABT_EndPoint> everybody;
	std::vector<int> initialDomain;
	std::vector<int> currentDomainsSize;
	std::vector<int> initialDomainsSize;
	AABT_Assignment assignment;
	CompoundAssignment agentView;
	nogood_store nogoodStore;
	std::vector<int> order;
	std::vector<int> teminationValue; // = initial domain
	AABT_Explanation explanation;
	std::forward_list<AABT_Explanation> E;
	bool end;

	void Agile_ABT();
	void ProcessInfo(const AABT_Message& msg);
	void ProcessOrder(const AABT_Message& msg);
	void ResolveConflict(const AABT_Message& msg);
	void CheckOrder(const std::vector<int>&, const std::vector<int>&);
	void CheckAgentView();
	void UpdateAgentView(const CompoundAssignment&);
	void BackTrack();
	int chooseValue();
	std::forward_list<AABT_Explanation> UpdateExplanations(
			const std::forward_list<AABT_Explanation>&, AABT_Nogood&,
			const AABT_Assignment&);
	std::vector<int> ComputeOrder(const std::forward_list<AABT_Explanation>&);
	CVOrderData ChooseVariableOrder(const std::forward_list<AABT_Explanation>&,
			AABT_Nogood&);
	AABT_Nogood Solve(const nogood_store&);
	bool Coherent(const AABT_Nogood&, const CompoundAssignment&);
	bool Coherent(const AABT_Nogood&);
	bool Coherent(const AABT_Explanation&);
	bool Compatible(const AABT_Nogood&);
	bool Compatible(const AABT_Explanation&);
	bool Compatible(const AABT_Nogood&, const std::vector<int>&);
	AABT_Message getMsg();
	CompoundAssignment union_func(const AABT_Assignment&,
			const CompoundAssignment&);
	CompoundAssignment union_func(const CompoundAssignment&,
			const CompoundAssignment&);
	void setRhs(AABT_Nogood&, const AABT_Assignment&);
	bool Consistent(const AABT_Assignment&, const CompoundAssignment&);
	bool Consistent(const AABT_Assignment&, const AABT_Assignment&);
	bool exp_is_valid(const AABT_Explanation&);

private:
	void sendMessageOK(const AgentID&);
	void sendMessageNGD(const AgentID&, AABT_Message&);
	void sendMessageORD(const AgentID&);
	void sendMessageSTP();
	void getAgentList();
	void sendMessage(const AgentID&, const AABT_Message&);
	protocols::csp::aabt::P_Message getMessage();

	// Networking staff
	std::string address;
	unsigned short port;
	std::string serverAddress;
	unsigned short serverResponderPort;
	unsigned short serverPublisherPort;

	static void* _messageReader(void* param);
	std::queue<protocols::csp::aabt::P_Message> messageQueue;
	pthread_mutex_t messageRW;
	sem_t messageCount;
	sem_t agentReadyLock;
	pthread_t messageReader;

	zmq::context_t context;
	Socket *listener;
	Socket serverRquest;
	Socket serverBroadcast;
};

} // namespace AIT
