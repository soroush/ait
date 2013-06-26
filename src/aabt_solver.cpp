#include <iostream>
#include <vector>
#include  <algorithm>
#include "aabt-assignment.h"
#include "aabt-message.h"
#include "aabt-order.h"
#include "aabt.pb.h"
#include "aabt-nogood.h"
#include "aabt-explanation.h"
#include "aabt-solver.h"
using namespace std;
using namespace AIT;

int AABT_Solver::n;
vector<int> AABT_Solver::current_domains_size;
vector<int> AABT_Solver::initial_domains_size;
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
	if (Coherent(msg.ei) && Compatible(msg.ei))
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
	for (Termination_value::iterator i = tv.begin(), j =
			my_temination_value.begin();
			i != tv.end(), j != my_temination_value.end(); i++, j++)
		if (*i >= *j)
			is_stronger = false;
	if (is_stronger) {
		my_temination_value = tv;
		my_order = o;
	}

	my_nogood_store.erase(
			std::remove_if(my_nogood_store.begin(), my_nogood_store.end(),
					[&](const AABT_Nogood& ngd)->bool {return !Coherent(ngd,Agent_View);}),
			my_nogood_store.end());

	E.erase(
			std::remove_if(E.begin(), E.end(),
					[&](const AABT_Explanation& exp)->bool {return !Compatible(exp);}),
			E.end());

	/*	for (nogood_store::iterator j = my_nogood_store.begin();
	 j != my_nogood_store.end(); j++)
	 if (!Coherent(*j, Agent_View))
	 j=my_nogood_store.erase(j);*/
	/*for (vector<AABT_Explanation>::iterator z = E.begin(); z != E.end();
	 z++)
	 if (!Coherent(*z))
	 E.erase(z);*/
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

void AABT_Solver::UpdateAgentView(const AABT_Solver::CompoundAssignment& a) {

	for (CompoundAssignment::iterator i = a.begin(); i != a.end(); i++)

		for (CompoundAssignment::iterator j = Agent_View.begin();
				j != Agent_View.end(); j++)
			if (i->id == j->id)
				if (i->time_stamp > j->time_stamp)
					j = i;

	my_nogood_store.erase(
			std::remove_if(my_nogood_store.begin(), my_nogood_store.end(),
					[&](const AABT_Explanation& exp)->bool {return !Compatible(exp);}),
			my_nogood_store.end());
	E.erase(
			std::remove_if(E.begin(), E.end(),
					[&](const AABT_Explanation& exp)->bool {return !Compatible(exp);}),
			E.end());

	/*	for (nogood_store::iterator j = my_nogood_store.begin();
	 j != my_nogood_store.end(); j++)
	 if (!Coherent(*j, Agent_View))
	 my_nogood_store.erase(j);*/
	/*for (vector<AABT_Explanation>::iterator z = E.begin(); z != E.end();
	 z++)
	 if (!Coherent(*z))
	 E.erase(z);*/
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
		/*for (vector<AABT_Explanation>::iterator i = E.begin();
		 i != E.end(); i++)
		 if (i->id == cvo.a.id)
		 E.erase(i);*/

		E.erase(
				std::remove_if(E.begin(), E.end(),
						[&](const AABT_Explanation& exp)->bool {return (exp.id=cvo.a.id);}),
				E.end());

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
		current_domains_size[id] = initial_domains_size[id];
		for (nogood_store::iterator j = my_nogood_store.begin();
				j != my_nogood_store.end(); j++)
			if (j->RHS.value == *i) {
				elaminated = true;
				current_domains_size[id] -= 1;
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
					current_domains_size[id] -= 1;
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
		const vector<AABT_Explanation>& Ei, AABT_Nogood& ng,
		const AABT_Assignment& a) {
	vector<AABT_Explanation> up_E = Ei;
	setRhs(ng, a);
	bool a_is_in_up_E;
	AABT_Explanation e;
	vector<AABT_Explanation>::iterator a_index;
	for (vector<AABT_Explanation>::iterator i = up_E.begin(); i != up_E.end();
			i++) {
		for (CompoundAssignment::iterator j = i->LHS.begin();
				j != i->LHS.begin(); j++)
			if (j->id == a.id) {
				i = up_E.erase(i);
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

AABT_Solver::order AABT_Solver::ComputeOrder(
		const vector<AABT_Explanation>& exp) {

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
	for (vector<AABT_Explanation>::iterator i = exp.begin(); i != exp.end();
			i++) {
		for (CompoundAssignment::iterator j = i->LHS.begin(); j != i->LHS.end();
				j++) {
			if (std::find(graph[i->id].effective.begin(),
					graph[i->id].effective.end(), j->id)
					== graph[i->id].effective.end()) {
				graph[i->id].effective.push_back(j->id);
				graph[i->id].n += 1;
			}
		}
	}
	while (!graph.empty()) {
		vector<int> Roots;
		vector<int>::iterator d;
		for (vector<incoming>::iterator s = graph.begin(); s != graph.end();
				s++)
			if (s->n == 0)
				Roots.push_back(s->id);

		for (vector<int>::iterator z = Roots.begin(); z != Roots.end(); z++)
			if (current_domains_size[*z] < max) {
				max = current_domains_size[*z];
				*p = *z;
				d = z;
			}
		/*for (vector<incoming>::iterator q = graph.begin(); q != graph.end();
				q++)
			if (q->id == *d)
				q = graph.erase(q);*/


		graph.erase(
					std::remove_if(graph.begin(), graph.end(),
							[&](const incoming& exp)->bool {return (exp.id=*d);}),
					graph.end());

		vector<int>::iterator r;
		for (vector<incoming>::iterator u = graph.begin(); u != graph.end();
				u++)
			if ((r = std::find(u->effective.begin(), u->effective.end(), *d))
					!= u->effective.end()) {
				r = u->effective.erase(r);
				u->n--;
			}

		p++;
	}

	return o;
}

CVOrderData AABT_Solver::ChooseVariableOrder(const vector<AABT_Explanation>& e,
		AABT_Nogood& ng) {
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

AABT_Nogood AABT_Solver::Solve(const nogood_store& ng) {
	AABT_Nogood s_ng;
	AABT_Assignment up_a;
	up_a.id = n + 1;
	for (nogood_store::iterator it = ng.begin(); it != ng.end(); it++) {
		CompoundAssignment::iterator cai = it->LHS.begin();
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

bool AABT_Solver::Coherent(const AABT_Nogood& ng,
		const CompoundAssignment& ca) {
	for (CompoundAssignment::iterator i = ng.LHS.begin(); i != ng.LHS.end();
			i++)
		for (CompoundAssignment::iterator j = ca.begin(); j != ca.end(); j++)
			if (i->id == j->id && i->value != j->value)
				return false;
	return true;
}

bool AABT_Solver::Coherent(const AABT_Nogood& ng) {
	for (CompoundAssignment::iterator i = ng.LHS.begin(); i != ng.LHS.end();
			i++)
		for (CompoundAssignment::iterator j = Agent_View.begin();
				j != Agent_View.end(); j++)
			if (i->id == j->id && i->value != j->value)
				return false;
	return true;
}

bool AABT_Solver::Coherent(const AABT_Explanation& exp) {
	for (CompoundAssignment::iterator i = exp.LHS.begin(); i != exp.LHS.end();
			i++)
		for (CompoundAssignment::iterator j = Agent_View.begin();
				j != Agent_View.end(); j++)
			if (i->id == j->id && i->value != j->value)
				return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Nogood& ng) {
	vector<int>::iterator my_index;
	my_index = std::find(my_order.begin(), my_order.end(), id);
	for (CompoundAssignment::iterator ci = ng.LHS.begin(); ci != ng.LHS.end();
			ci++)
		if (std::find(my_index + 1, my_order.end(), ci->id) != my_order.end())
			return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Explanation& e) {
	vector<int>::iterator my_index;
	my_index = std::find(my_order.begin(), my_order.end(), id);

	for (CompoundAssignment::iterator ci = e.LHS.begin(); ci != e.LHS.end();
			ci++)
		if (std::find(my_index + 1, my_order.end(), ci->id) != my_order.end())
			return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Nogood& ng,
		const AABT_Solver::order& o) {
	vector<int>::iterator my_index;
	my_index = std::find(o.begin(), o.end(), id);

for (CompoundAssignment::iterator ci = ng.LHS.begin(); ci != ng.LHS.end();
		ci++)

if (std::find(my_index+1, o.end(), ci->id != o.end())
		return false;

		return true;
	}

AABT_Solver::CompoundAssignment AABT_Solver::union_func(
		const AABT_Assignment& a1, const CompoundAssignment& ca) {
	CompoundAssignment up_ca = ca;
	for (CompoundAssignment::iterator j = ca.begin(); j != ca.end(); j++) {
		if (j->id == a1.id)
			break;
		up_ca.push_back(*j);
	}
	return up_ca;
}

AABT_Solver::CompoundAssignment AABT_Solver::union_func(
		const CompoundAssignment& ca1, const CompoundAssignment& ca2) {
	CompoundAssignment up_ca = ca1;
	for (CompoundAssignment::iterator i = ca2.begin(); i != ca2.end(); i++)
		for (CompoundAssignment::iterator j = ca1.begin(); j != ca1.end();
				j++) {
			if (j->id == i->id)
				break;
			up_ca.push_back(*i);
		}

	return up_ca;
}

void AABT_Solver::setRhs(AABT_Nogood& ng, const AABT_Assignment& a) {
	/*for (CompoundAssignment::iterator ni = ng.LHS.begin(); ni != ng.LHS.end();
	 ni++)
	 if (ni->id == a.id)
	 ng.LHS.erase(ni);*/

	ng.LHS.erase(
			std::remove_if(ng.LHS.begin(), ng.LHS.end(),
					[&](const AABT_Assignment& exp)->bool {return (exp.id=a.id);}),
			ng.LHS.end());

	ng.LHS.push_back(ng.RHS);
	ng.RHS = a;
}

//not complete
bool AABT_Solver::Consistent(const AABT_Assignment& a,
		const CompoundAssignment& ca) {
	for (CompoundAssignment::iterator it = Agent_View.begin();
			it != Agent_View.end(); it++)
		if (a.value == it->value || (it->value - a.value == it->id - a.id))
			return false;
	return true;

}
bool AABT_Solver::Consistent(const AABT_Assignment& a,
		const AABT_Assignment& ca) {

	if (a.value == ca.value || (ca.value - a.value == ca.value - a.id))
		return false;
	return true;

}

protocols::csp::aabt::P_Message getMessage() {
}
