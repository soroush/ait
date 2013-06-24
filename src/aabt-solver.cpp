/*
 * aabt-assignment.h
 *
 *  Created on: Jun 21, 2013
 *      Author: roya
 */

#include <iostream>
#include <vector>
#include  <algorithm>
#include "aabt-solver.h"
#include "aabt-message.h"
#include "aabt-order.h"

using namespace std;
using namespace AIT;


struct incoming {
	int n;
	int id;
	std::vector<int> effective;
};

void AABT_Solver::Agile_ABT() {
	for (int i = 0; i < n; ++i)
		my_temination_value.push_back(1000);
	end = false;
	my_assignment.time_stamp = 0;
	my_assignment.value = 0;

	CheckAgentView();
	while (!end) {
		AABT_Message msg = getMsg();
		switch (msg.msg_type) {
		case AABT_MessageType::OK:
			ProcessInfo(msg);
			break;
		case AABT_MessageType::NOGOOD:
			ResolveConflict(msg);
			break;
		case AABT_MessageType::ORDER:
			ProcessOrder(msg);
			break;
		case AABT_MessageType::STOP:
			end = true;
			break;
		default:
			break;
		}
	}
}

void AABT_Solver::ProcessInfo(const AABT_Message& msg) {
	CheckOrder(my_order, msg.tvi);

	UpdateAgentView(union_func(msg.vi, msg.ei.LHS));
<<<<<<< Updated upstream:src/aabt-solver.cpp
	if (Coherent(msg.ei) && Compatible(msg.ei)) // TODO ask Roya
=======
	if (Coherent(msg.ei) && Compatible(msg.ei))
>>>>>>> Stashed changes:src/aabt_solver.cpp
		E.push_back(msg.ei);
	CheckAgentView();
}

void AABT_Solver::ProcessOrder(const AABT_Message& msg) {
	CheckOrder(msg.oi, msg.tvi);
	CheckAgentView();
}

void AABT_Solver::ResolveConflict(const AABT_Message& msg) {
	CheckOrder(msg.oi, msg.tvi);
	UpdateAgentView(union_func(msg.vi, msg.ng.LHS));

	if (Coherent(msg.ng, union_func(my_assignment, Agent_View))) {
		if (Compatible(msg.ng, my_order)) {

			my_nogood_store.push_back(msg.ng);
			my_assignment.value = 0;
			CheckAgentView();
		} else if (msg.ng.RHS.value == my_assignment.value) {
			AABT_Message m;
			m.ei = my_exp;
			m.vi = my_assignment;
			m.oi = my_order;
			m.tvi = my_temination_value;
			//send msg ok to msg.sender
		}
	}
}

void AABT_Solver::CheckOrder(const AABT_Solver::order& o,
		const AABT_Solver::Termination_value& tv) {
	bool is_stronger = true;
	auto i = tv.begin();
	auto j = my_temination_value.begin();
	for (; i != tv.end() and j != my_temination_value.end(); i++, j++)
		if (*i >= *j)
			is_stronger = false;
	if (is_stronger) {
		my_temination_value = tv;
		my_order = o;
	}

	for (nogood_store::iterator j = my_nogood_store.begin();
			j != my_nogood_store.end(); j++)
		if (!Coherent(*j, Agent_View))
			my_nogood_store.erase(j);

	for (vector<AABT_Explanation>::iterator z = E.begin(); z != E.end(); z++)
		if (!Coherent(*z))
			E.erase(z);
}

void AABT_Solver::CheckAgentView() {
	if (!Consistent(my_assignment, Agent_View)) {
		my_assignment.value = chooseValue();
		if (my_assignment.value > 0) {

			AABT_Message m;
			m.msg_type = AABT_MessageType::OK;
			m.vi = my_assignment;
			m.ei = my_exp;
			m.oi = my_order;
			m.tvi = my_temination_value;
			for (order::iterator i = my_order.begin();; i++) {
				if (*i == my_assignment.id)
					break;
				//sendmsg ok to agent i
			}

		} else
			BackTrack();
	}
//	else //if my_order was modified
	{
		AABT_Message m;
		m.msg_type = AABT_MessageType::OK;
		m.vi = my_assignment;
		m.ei = my_exp;
		m.oi = my_order;
		m.tvi = my_temination_value;
		for (order::iterator i = my_order.begin();; i++) {
			if (*i == my_assignment.id)
				break;
			//sendmsg ok to agent i
		}
	}
}

<<<<<<< Updated upstream:src/aabt-solver.cpp
void AABT_Solver::UpdateAgentView(const AABT_Solver::CompoundAssignment& a) {
	for (const auto& i : a) {
		for (auto& j : Agent_View) {
			if (i.id == j.id) {
				if (i.time_stamp > j.time_stamp)
					j = i;
			}
		}
	}
	auto toBeRemoved =
			std::remove_if(my_nogood_store.begin(), my_nogood_store.end(),
					[&](const AABT_Nogood& ngd) {return !Coherent(ngd, this->Agent_View);});
	my_nogood_store.erase(toBeRemoved, my_nogood_store.end());
	auto removedExplanation = std::find_if(E.begin(), E.end(),
			[&](const AABT_Explanation& exp) {return !Coherent(exp);});
	E.erase(removedExplanation, E.end());
=======
void aabt_solver::UpdateAgentView(aabt_solver::CompoundAssignment a) {

	for (CompoundAssignment::iterator i = a.begin(); i != a.end(); i++)

		for (CompoundAssignment::iterator j = Agent_View.begin();
				j != Agent_View.end(); j++)
			if (i->id == j->id)
				if (i->time_stamp > j->time_stamp)
					j=i;
	for (nogood_store::iterator j = my_nogood_store.begin();
			j != my_nogood_store.end(); j++)
		if (!Coherent(*j, Agent_View))
			my_nogood_store.erase(j);

	for (vector<explanation>::iterator z = E.begin(); z != E.end(); z++)
		if (!Coherent(*z))
			E.erase(z);
>>>>>>> Stashed changes:src/aabt_solver.cpp
}

void AABT_Solver::BackTrack() {
	AABT_Nogood ng = Solve(my_nogood_store);
	if (ng.LHS.empty()) {
		end = true;
		AABT_Message m;
		m.msg_type = AABT_MessageType::STOP; //send message stop to system
	}
	CVOrderData cvo = ChooseVariableOrder(E, ng);
	bool is_stronger = true;
	for (Termination_value::iterator i = cvo.tv.begin(), j =
			my_temination_value.begin();
			i != cvo.tv.end(), j != my_temination_value.end(); i++, j++)
		if (i > j)
			is_stronger = false;
	if (is_stronger) {
		my_temination_value = cvo.tv;
		my_order = cvo.o;
		E = cvo.E;
		setRhs(ng, cvo.a);
		AABT_Message m1;
		m1.msg_type = AABT_MessageType::NOGOOD;
		m1.ng = ng;
		m1.oi = my_order;
		m1.tvi = my_temination_value;
		//send ngd message to cvo.assignment.id
		for (vector<AABT_Explanation>::iterator i = E.begin(); i != E.end(); i++)
			if (i->id == cvo.a.id)
				E.erase(i);
		AABT_Message m;
		m.msg_type = AABT_MessageType::ORDER;
		m.oi = my_order;
		m.tvi = my_temination_value;
		//broadcast message order
	}

	else {
		setRhs(ng, cvo.a);
		AABT_Message m;
		m.msg_type = AABT_MessageType::NOGOOD;
		m.oi = my_order;
		m.tvi = my_temination_value;
		//sendmsg nogood to Ak;
	}
	cvo.a.value = 0;
	CompoundAssignment c;
	c.push_back(cvo.a);
	UpdateAgentView(c);
	CheckAgentView();
}

int AABT_Solver::chooseValue() {
	for (vector<int>::iterator i = my_initial_domain.begin();
			i != my_initial_domain.end(); i++) {
		bool elaminated = false;
		current_domains_size[ID] = initial_domains_size[ID];
		for (nogood_store::iterator j = my_nogood_store.begin();
				j != my_nogood_store.end(); j++)
			if (j->RHS.value == *i) {
				elaminated = true;
				current_domains_size[ID] -= 1;
				break;
			}
		if (!elaminated) {
			AABT_Assignment a;
			a.id = my_assignment.id;
			a.time_stamp = my_assignment.time_stamp;
			a.value = *i;

			for (CompoundAssignment::iterator k = Agent_View.begin();
					k != Agent_View.end(); k++)
				if (!Consistent(a, *k)) {
					CompoundAssignment ca;
					ca.push_back(*k);
					AABT_Nogood ng;
					ng.LHS = ca;
					ng.RHS = a;
					current_domains_size[ID] -= 1;
					break;
				} else {
					//*	my_assignment.value=*i;
					my_assignment.time_stamp += 1;
					return *i;
				}
		}
	}
//*	my_assignment.value=0;
	return 0;
}

vector<AABT_Explanation> AABT_Solver::UpdateExplanations(
		const vector<AABT_Explanation>& Ei, AABT_Nogood& ng, const AABT_Assignment &a) {
	vector<AABT_Explanation> up_E = Ei;
	setRhs(ng, a);
	bool a_is_in_up_E;
	AABT_Explanation e;
	vector<AABT_Explanation>::iterator a_index;
	for (vector<AABT_Explanation>::iterator i = up_E.begin(); i != up_E.end(); i++) {
		for (CompoundAssignment::iterator j = i->LHS.begin();
				j != i->LHS.begin(); j++)
			if (j->id == a.id) {
				up_E.erase(i);
				break;
			}

		if (i->id == a.id) {
			a_is_in_up_E = true;
			a_index = i;
		}
	}
	if (!a_is_in_up_E) {
		e.RHS = initial_domains_size[a.id];
		up_E.push_back(e);
		a_index = up_E.end();
	}

	AABT_Explanation e2;
	e2.LHS = union_func(e.LHS, ng.LHS);
	e2.RHS = e.RHS - 1;
	current_domains_size[e2.id] -= 1;
	up_E.erase(a_index);
	up_E.push_back(e2);
	return up_E;
}

AABT_Solver::order AABT_Solver::ComputeOrder(const vector<AABT_Explanation>& exp) {

	vector<incoming> graph(n);
	order o(n);
	int max = 100;
	order::iterator p = o.begin();
	int f = 1;
	for (vector<incoming>::iterator t = graph.begin(); t != graph.end(); t++) {
		t->id = f;
		++f;
		t->n = 0;
	}
	for (const auto& i : exp) {
		for (const auto& j : i.LHS) {
			if (std::find(graph[i.id].effective.begin(),
					graph[i.id].effective.end(), j.id)
					== graph[i.id].effective.end()) {
				graph[i.id].effective.push_back(j.id);
				graph[i.id].n += 1;
			}
		}
	}
	while (!graph.empty()) {
		vector<int> roots;
		int d;
		for (const auto& s : graph)
			if (s.n == 0)
				roots.push_back(s.id);

		for (const auto& z : roots)
			if (current_domains_size[z] < max) {
				max = current_domains_size[z];
				*p = z;
				d = z;
			}
		graph.erase(
				find_if(graph.begin(), graph.end(),
						[&](const incoming& in) {return in.id==d;}),
				graph.end());

		vector<int>::iterator r;
		for (const auto& u : graph) {
//			auto r = std::find(u.effective.begin(), u.effective.end(), d);
//			if (r != u.effective.end()) {
//				u.effective.erase(r);
//				u.n--;
//			}
			// FIXME Check this
		}
		p++;
	}
	return o;
}

CVOrderData AABT_Solver::ChooseVariableOrder(
		const vector<AABT_Explanation> &e, AABT_Nogood &ng) {
	CVOrderData cvo;
	cvo.o = my_order;
	cvo.tv = my_temination_value;
//	cvo.a = my_assignment;
//	cvo.E = ;
	vector<AABT_Explanation> up_E;
	order up_O;
	Termination_value up_Tv;
	for (CompoundAssignment::iterator it = ng.LHS.begin(); it != ng.LHS.end();
			it++) {
		up_E = UpdateExplanations(e, ng, *it);
		up_O = ComputeOrder(up_E);
		Termination_value::iterator tvi = up_Tv.begin();
		for (order::iterator oi = up_O.begin(); oi != up_O.end(); oi++)
			for (vector<AABT_Explanation>::iterator vi = up_E.begin();
					vi != up_E.end(); vi++)
				if (vi->id == *oi) {
					*tvi = vi->RHS;
					tvi++;
					break;
				}
		bool is_smaller = true;
		for (Termination_value::iterator i = up_Tv.begin(), j =
				my_temination_value.begin();
				i != up_Tv.end(), j != my_temination_value.end(); i++, j++)
			if (*i >= *j) {
				is_smaller = false;
				break;
			}
		if (is_smaller) {
			cvo.a = *it;
			cvo.o = up_O;
			cvo.tv = up_Tv;
			cvo.E = up_E;
		}
	}
	return cvo;
}

AABT_Nogood AABT_Solver::Solve(const nogood_store &ng) {
	AABT_Nogood s_ng;
	AABT_Assignment up_a;
	up_a.id = n + 1;
	for (const auto& it : ng) {
		CompoundAssignment::const_iterator cai = it.LHS.begin();
		if (cai->id < up_a.id) {
			if (up_a.id != n + 1)
				/*	needs an operator overloading
				 * if (std::find(s_ng.LHS.begin(), s_ng.LHS.end(), up_a)
				 != s_ng.LHS.end())*/
				s_ng.LHS.push_back(up_a);
			up_a = *cai;
		}
	}
	s_ng.RHS = up_a;
	return s_ng;
}

bool AABT_Solver::Coherent(const AABT_Nogood &ng, const CompoundAssignment &ca) {
	for (const auto& i : ng.LHS)
		for (const auto& j : ca)
			if (i.id == j.id && i.value != j.value)
				return false;
	return true;
}

bool AABT_Solver::Coherent(const AABT_Nogood &ng) {
	for (const auto& i : ng.LHS)
		for (const auto& j : Agent_View)
			if (i.id == j.id && i.value != j.value)
				return false;
	return true;
}

bool AABT_Solver::Coherent(const AABT_Explanation &exp) {
	for (const auto& i : exp.LHS)
		for (const auto& j : Agent_View)
			if (i.id == j.id && i.value != j.value)
				return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Nogood &ng) {
	vector<int>::iterator my_index;
	my_index = std::find(my_order.begin(), my_order.end(), ID);
	for (const auto& ci : ng.LHS)
		if (std::find(my_index + 1, my_order.end(), ci.id) != my_order.end())
			return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Explanation &e) {
	vector<int>::iterator my_index;
	my_index = std::find(my_order.begin(), my_order.end(), ID);
	for (const auto& ci : e.LHS)
		if (std::find(my_index + 1, my_order.end(), ci.id) != my_order.end())
			return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Nogood & ng,
		const AABT_Solver::order &o) {
	vector<int>::const_iterator my_index;
	my_index = std::find(o.begin(), o.end(), ID);

	for (const auto& ci : ng.LHS)
		if (std::find(my_index + 1, o.end(), ci.id) != o.end())
			return false;
	return true;
}

AABT_Solver::CompoundAssignment AABT_Solver::union_func(const AABT_Assignment &a1,
		const CompoundAssignment &ca) {
	CompoundAssignment up_ca = ca;
	for (const auto& j : ca) {
		if (j.id == a1.id)
			break;
		up_ca.push_back(j);
	}
	return up_ca;
}

AABT_Solver::CompoundAssignment AABT_Solver::union_func(
		const CompoundAssignment &ca1, const CompoundAssignment &ca2) {
	CompoundAssignment up_ca = ca1;
	for (const auto& i : ca2)
		for (const auto& j : ca1) {
			if (j.id == i.id)
				break;
			up_ca.push_back(i);
		}

	return up_ca;
}

void AABT_Solver::setRhs(AABT_Nogood &ng, const AABT_Assignment &a) {
//	for (const auto& ni : ng.LHS)
//		if (ni.id == a.id)
//			ng = ng.LHS.erase(ni); // DONE: replace with lambda

	// FIXME Find out what's wrong with this code
	//	auto& x = std::remove_if(ng.LHS.begin(), ng.LHS.end(),
	//			[&](const AABT_Assignment& aa){return (aa.id == a.id);});
	//	ng.LHS.erase(x, ng.LHS.end());
	ng.LHS.push_back(ng.RHS);
	ng.RHS = a;
}

bool AABT_Solver::Consistent(const AABT_Assignment &a,
		const CompoundAssignment &ca) {
	for (CompoundAssignment::iterator it = Agent_View.begin();
			it != Agent_View.end(); it++)
		if (a.value == it->value || (it->value - a.value == it->id - a.id))
			return false;
	return true;

}
bool AABT_Solver::Consistent(const AABT_Assignment &a, const AABT_Assignment &ca) {

	if (a.value == ca.value || (ca.value - a.value == ca.value - a.id))
		return false;
	return true;

}

AABT_Message AABT_Solver::getMsg() {
}
