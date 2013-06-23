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
		assignment() {
			id = 0;
			time_stamp = 0;
			value = 0;
		}
		~assignment() {
		}
		assignment& operator=(const assignment& a) {
			id = a.id;
			time_stamp = a.time_stamp;
			value = a.value;
			return *this;
		}
	};

	typedef std::vector<assignment> CompoundAssignment;

	struct nogood {
		CompoundAssignment LHS;
		assignment RHS;

		nogood() :
				RHS() {
		}
		~nogood() {
		}
		nogood& operator=(const nogood& ng) {
			for (const auto& x : ng.LHS)
				LHS.push_back(x);
			RHS = ng.RHS;
			return *this;
		}
	};

	typedef std::vector<int> order;
	typedef std::vector<int> Termination_value;
	typedef std::vector<nogood> nogood_store;

	struct explanation {
		int id;
		CompoundAssignment LHS;
		int RHS;
		explanation() {
			id = 0;
			RHS = 0;
		}
		~explanation() {
		}
		explanation& operator=(const explanation& e1) {
			for (const auto& x : e1.LHS)
				LHS.push_back(x);
			RHS = e1.RHS;
			id = e1.id;
			return *this;
		}
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
		message() :
				vi(), ei(), ng() {

			sender_id = 0;

		}
		~message() {
		}
		message& operator=(const message& m1) {
			msg_type = m1.msg_type;
			sender_id = m1.sender_id;
			vi = m1.vi;
			ei = m1.ei;
			ng = m1.ng;
			for (const auto& x : m1.oi)
				oi.push_back(x);
			for (const auto& x : m1.tvi)
				tvi.push_back(x);
			return *this;

		}

	};
	struct CVOrderData {
		assignment a;
		order o;
		Termination_value tv;
		std::vector<explanation> E;
		CVOrderData():a() {}
		~CVOrderData() {
		}
		CVOrderData& operator=(const CVOrderData& c1) {
			a = c1.a;
			for (const auto& x : c1.o)
				o.push_back(x);
			for (const auto& x : c1.E)
				E.push_back(x);
			for (const auto& x : c1.tv)
				tv.push_back(x);

		}

	};

public:

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
	void ProcessInfo(const message& msg);
	void ProcessOrder(const message& msg);
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
