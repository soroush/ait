#include <iostream>
#include <vector>
#include "aabt-assignment.h"
#include "aabt-explanation.h"

using namespace std;

namespace AIT {

struct AABT_Message;
struct AABT_Nogood;
struct CVOrderData;

class AABT_Solver {
public:
	int n; //problem size

	typedef std::vector<AABT_Assignment> CompoundAssignment;

	typedef std::vector<int> order;
	typedef std::vector<int> Termination_value;
	typedef std::vector<AABT_Nogood> nogood_store;

public:

	int ID;
	vector<int> my_initial_domain;
	vector<int> current_domains_size;
	vector<int> initial_domains_size;
	AABT_Assignment my_assignment;
	CompoundAssignment Agent_View;
	nogood_store my_nogood_store;
	order my_order;
	Termination_value my_temination_value; // = initial domain
	AABT_Explanation my_exp;
	std::vector<AABT_Explanation> E;
	bool end;

	void Agile_ABT();
	void ProcessInfo(const AABT_Message& msg);
	void ProcessOrder(const AABT_Message& msg);
	void ResolveConflict(const AABT_Message& msg);
	void CheckOrder(const order&, const Termination_value&);
	void CheckAgentView();
	void UpdateAgentView(const CompoundAssignment&);
	void BackTrack();
	int chooseValue();
	vector<AABT_Explanation> UpdateExplanations(const std::vector<AABT_Explanation>&,
			AABT_Nogood&, const AABT_Assignment&);
	order ComputeOrder(const std::vector<AABT_Explanation>&);
	CVOrderData ChooseVariableOrder(const vector<AABT_Explanation>&, AABT_Nogood&);
	AABT_Nogood Solve(const nogood_store&);
	bool Coherent(const AABT_Nogood&, const CompoundAssignment&);
	bool Coherent(const AABT_Nogood&);
	bool Coherent(const AABT_Explanation&);
	bool Compatible(const AABT_Nogood&);
	bool Compatible(const AABT_Explanation&);
	bool Compatible(const AABT_Nogood&, const order&);
	AABT_Message getMsg();
	CompoundAssignment union_func(const AABT_Assignment&, const CompoundAssignment&);
	CompoundAssignment union_func(const CompoundAssignment&,
			const CompoundAssignment&);
	void setRhs(AABT_Nogood&, const AABT_Assignment&);
	bool Consistent(const AABT_Assignment&, const CompoundAssignment&);
	bool Consistent(const AABT_Assignment&, const AABT_Assignment&);
	bool exp_is_valid(const AABT_Explanation&);
};

} // namespace AIT