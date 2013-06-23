#include <iostream>
#include <vector>
#include  <algorithm>
#include "aabt_solver.h"
using namespace std;

int aabt_solver::n;
vector<int> aabt_solver::current_domains_size;
vector<int> aabt_solver::initial_domains_size;
struct incoming {
	int n;
	int id;
	std::vector<int> effective;
};

void aabt_solver::Agile_ABT() {
	for (int i = 0; i < n; ++i)
		my_temination_value.push_back(1000);
	end = false;
	my_assignment.time_stamp = 0;
	my_assignment.value = 0;

	CheckAgentView();
	while (!end) {
		message msg = getMsg();
		switch (msg.msg_type) {
		case OK:
			ProcessInfo(msg);
			break;
		case NOGOOD:
			ResolveConflict(msg);
			break;
		case ORDER:
			ProcessOrder(msg);
			break;
		case STOP:
			end = true;
			break;
		default:
			break;
		}

	}

}

void aabt_solver::ProcessInfo(const message& msg) {
	CheckOrder(my_order, msg.tvi);

	UpdateAgentView(union_func(msg.vi, msg.ei.LHS));
	if (Coherent(msg.ei) && Coherent(msg.ei))
		E.push_back(msg.ei);
	CheckAgentView();
}

void aabt_solver::ProcessOrder(const message&  msg) {
	CheckOrder(msg.oi, msg.tvi);
	CheckAgentView();
}

void aabt_solver::ResolveConflict(message msg) {
	CheckOrder(msg.oi, msg.tvi);
	UpdateAgentView(union_func(msg.vi, msg.ng.LHS));

	if (Coherent(msg.ng, union_func(my_assignment, Agent_View))) {
		if (Compatible(msg.ng, my_order)) {

			my_nogood_store.push_back(msg.ng);
			my_assignment.value = 0;
			CheckAgentView();
		} else if (msg.ng.RHS.value == my_assignment.value) {
			message m;
			m.ei = my_exp;
			msg.vi = my_assignment;
			msg.oi = my_order;
			msg.tvi = my_temination_value;
			//send msg ok to msg.sender
		}
	}
}

void aabt_solver::CheckOrder(aabt_solver::order o,
		aabt_solver::Termination_value tv) {
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

	for (nogood_store::iterator j = my_nogood_store.begin();
			j != my_nogood_store.end(); j++)
		if (!Coherent(*j, Agent_View))
			my_nogood_store.erase(j);

	for (vector<explanation>::iterator z = E.begin(); z != E.end(); z++)
		if (!Coherent(*z))
			E.erase(z);
}

void aabt_solver::CheckAgentView() {
	if (!Consistent(my_assignment, Agent_View)) {
		my_assignment.value = chooseValue();
		if (my_assignment.value > 0) {

			message m;
			m.msg_type = OK;
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
		message m;
		m.msg_type = OK;
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

void aabt_solver::UpdateAgentView(aabt_solver::CompoundAssignment a) {

	for (CompoundAssignment::iterator i = a.begin(); i != a.end(); i++)

		for (CompoundAssignment::iterator j = Agent_View.begin();
				j != Agent_View.end(); j++)
			if (i->id == j->id)
				if (i->time_stamp > j->time_stamp)
					i = j;
	for (nogood_store::iterator j = my_nogood_store.begin();
			j != my_nogood_store.end(); j++)
		if (!Coherent(*j, Agent_View))
			my_nogood_store.erase(j);

	for (vector<explanation>::iterator z = E.begin(); z != E.end(); z++)
		if (!Coherent(*z))
			E.erase(z);
}

void aabt_solver::BackTrack() {
	nogood ng = Solve(my_nogood_store);
	if (ng.LHS.empty()) {
		end = true;
		message m;
		m.msg_type = STOP; //send message stop to system
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
		message m1;
		m1.msg_type = NOGOOD;
		m1.ng = ng;
		m1.oi = my_order;
		m1.tvi = my_temination_value;
		//send ngd message to cvo.assignment.id
		for (vector<explanation>::iterator i = E.begin(); i != E.end(); i++)
			if (i->id == cvo.a.id)
				E.erase(i);
		message m;
		m.msg_type = ORDER;
		m.oi = my_order;
		m.tvi = my_temination_value;
		//broadcast message order
	}

	else {
		setRhs(ng, cvo.a);
		message m;
		m.msg_type = NOGOOD;
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

int aabt_solver::chooseValue() {
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
			assignment a;
			a.id = my_assignment.id;
			a.time_stamp = my_assignment.time_stamp;
			a.value = *i;

			for (CompoundAssignment::iterator k = Agent_View.begin();
					k != Agent_View.end(); k++)
				if (!Consistent(a, *k)) {
					CompoundAssignment ca;
					ca.push_back(*k);
					nogood ng;
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

vector<aabt_solver::explanation> aabt_solver::UpdateExplanations(
		vector<explanation> Ei, nogood ng, assignment a) {
	vector<explanation> up_E = Ei;
	setRhs(ng, a);
	bool a_is_in_up_E;
	explanation e;
	vector<explanation>::iterator a_index;
	for (vector<explanation>::iterator i = up_E.begin(); i != up_E.end(); i++) {
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

	explanation e2;
	e2.LHS = union_func(e.LHS, ng.LHS);
	e2.RHS = e.RHS - 1;
	current_domains_size[e2.id] -= 1;
	up_E.erase(a_index);
	up_E.push_back(e2);
	return up_E;
}

aabt_solver::order aabt_solver::ComputeOrder(vector<explanation> exp) {

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
	for (vector<explanation>::iterator i = exp.begin(); i != exp.end(); i++) {
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
		for (vector<incoming>::iterator q = graph.begin(); q != graph.end();
				q++)
			if (q->id == *d)
				graph.erase(q);
		vector<int>::iterator r;
		for (vector<incoming>::iterator u = graph.begin(); u != graph.end();
				u++)
			if ((r = std::find(u->effective.begin(), u->effective.end(), *d))
					!= u->effective.end()) {
				u->effective.erase(r);
				u->n--;
			}

		p++;
	}

	return o;
}

aabt_solver::CVOrderData aabt_solver::ChooseVariableOrder(vector<explanation> e,
		nogood ng) {
	CVOrderData cvo;
	cvo.o = my_order;
	cvo.tv = my_temination_value;
//	cvo.a = my_assignment;
//	cvo.E = ;
	vector<explanation> up_E;
	order up_O;
	Termination_value up_Tv;
	for (CompoundAssignment::iterator it = ng.LHS.begin(); it != ng.LHS.end();
			it++) {
		up_E = UpdateExplanations(e, ng, *it);
		up_O = ComputeOrder(up_E);
		Termination_value::iterator tvi = up_Tv.begin();
		for (order::iterator oi = up_O.begin(); oi != up_O.end(); oi++)
			for (vector<explanation>::iterator vi = up_E.begin();
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

aabt_solver::nogood aabt_solver::Solve(nogood_store ng) {
	nogood s_ng;
	assignment up_a;
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

bool aabt_solver::Coherent(nogood ng, CompoundAssignment ca) {
	for (CompoundAssignment::iterator i = ng.LHS.begin(); i != ng.LHS.end();
			i++)
		for (CompoundAssignment::iterator j = ca.begin(); j != ca.end(); j++)
			if (i->id == j->id && i->value != j->value)
				return false;
	return true;
}

bool aabt_solver::Coherent(nogood ng) {
	for (CompoundAssignment::iterator i = ng.LHS.begin(); i != ng.LHS.end();
			i++)
		for (CompoundAssignment::iterator j = Agent_View.begin();
				j != Agent_View.end(); j++)
			if (i->id == j->id && i->value != j->value)
				return false;
	return true;
}

bool aabt_solver::Coherent(explanation exp) {
	for (CompoundAssignment::iterator i = exp.LHS.begin(); i != exp.LHS.end();
			i++)
		for (CompoundAssignment::iterator j = Agent_View.begin();
				j != Agent_View.end(); j++)
			if (i->id == j->id && i->value != j->value)
				return false;
	return true;
}

bool aabt_solver::Compatible(nogood ng) {
	vector<int>::iterator my_index;
	my_index = std::find(my_order.begin(), my_order.end(), ID);
	for (CompoundAssignment::iterator ci = ng.LHS.begin(); ci != ng.LHS.end();
			ci++)
		if (std::find(my_index + 1, my_order.end(), ci->id) != my_order.end())
			return false;
	return true;
}

bool aabt_solver::Compatible(explanation e) {
	vector<int>::iterator my_index;
	my_index = std::find(my_order.begin(), my_order.end(), ID);

	for (CompoundAssignment::iterator ci = e.LHS.begin(); ci != e.LHS.end();
			ci++)
		if (std::find(my_index + 1, my_order.end(), ci->id) != my_order.end())
			return false;
	return true;
}

bool aabt_solver::Compatible(aabt_solver::nogood ng, aabt_solver::order o) {
	vector<int>::iterator my_index;
	my_index = std::find(o.begin(), o.end(), ID);

	for (CompoundAssignment::iterator ci = ng.LHS.begin(); ci != ng.LHS.end();
			ci++)
		if (std::find(my_index + 1, o.end(), ci->id) != o.end())
					return false;
			return true;
}

aabt_solver::CompoundAssignment aabt_solver::union_func(assignment a1,
		CompoundAssignment ca) {
	CompoundAssignment up_ca = ca;
	for (CompoundAssignment::iterator j = ca.begin(); j != ca.end(); j++) {
		if (j->id == a1.id)
			break;
		up_ca.push_back(*j);
	}
	return up_ca;
}

aabt_solver::CompoundAssignment aabt_solver::union_func(CompoundAssignment ca1,
		CompoundAssignment ca2) {
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

void aabt_solver::setRhs(nogood ng, assignment a) {
	for (CompoundAssignment::iterator ni = ng.LHS.begin(); ni != ng.LHS.end();
			ni++)
		if (ni->id == a.id)
			ng.LHS.erase(ni);
	ng.LHS.push_back(ng.RHS);
	ng.RHS = a;
}

//not complete
bool aabt_solver::Consistent(assignment a, CompoundAssignment ca) {
	for (CompoundAssignment::iterator it = Agent_View.begin();
			it != Agent_View.end(); it++)
		if (a.value == it->value || (it->value - a.value == it->id - a.id))
			return false;
	return true;

}
bool aabt_solver::Consistent(assignment a, assignment ca) {

	if (a.value == ca.value || (ca.value - a.value == ca.value - a.id))
		return false;
	return true;

}

aabt_solver::message aabt_solver::getMsg() {
}
