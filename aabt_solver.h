#include <iostream>
#include <vector>
using namespace std;

class aabt_solver {
public:
	static int n; //problem size
	struct assignment {
		int id;
		int time_stamp;
		int value;
	};

	typedef std::vector<assignment> CompoundAssignment;

	struct nogood {
		CompoundAssignment LHS;
		assignment RHS;
	};

	typedef std::vector<int> order;
	typedef std::vector<int> Termination_value;
	typedef std::vector<nogood> nogood_store;

	struct explanation {
		int id;
		CompoundAssignment LHS;
		int RHS;
	};

	enum MessageType {
		OK = 1, NOGOOD = 2, ORDER = 3, STOP = 4
	};
	struct message {
		MessageType msg_type;
		int sender_id;
		assignment vi;
		explanation ei;
		order oi;
		Termination_value tvi;
		nogood ng;

	};
	struct CVOrderData {
		assignment a;
		order o;
		Termination_value tv;
		std::vector<explanation> E;
	};

private:

	int ID;
	vector<int> my_initial_domain;
	static vector<int> current_domains_size;
	static vector<int> initial_domains_size;
	assignment my_assignment;
	CompoundAssignment Agent_View;
	nogood_store my_nogood_store;
	order my_order;
	Termination_value my_temination_value;
	explanation my_exp;
	std::vector<explanation> E;
	bool end;

	void Agile_ABT();
	void ProcessInfo(message msg);
	void ProcessOrder(message msg);
	void ResolveConflict(message msg);
	void CheckOrder(order, Termination_value);
	void CheckAgentView();
	void UpdateAgentView(CompoundAssignment);
	void BackTrack();
	int chooseValue();
	vector<explanation> UpdateExplanations(std::vector<explanation>, nogood,
			assignment);
	order ComputeOrder(std::vector<explanation>);
	CVOrderData ChooseVariableOrder(vector<explanation>, nogood);
	nogood Solve(nogood_store);
	bool Coherent(nogood, CompoundAssignment);
	bool Coherent(nogood);
	bool Coherent(explanation);
	bool Compatible(nogood);
	bool Compatible(explanation);
	bool Compatible(nogood, order);
	message getMsg();
	CompoundAssignment union_func(assignment, CompoundAssignment);
	CompoundAssignment union_func(CompoundAssignment, CompoundAssignment);
	void setRhs(nogood, assignment);
	bool Consistent(assignment, CompoundAssignment);
	bool Consistent(assignment, assignment);
	bool exp_is_valid(explanation);

};
