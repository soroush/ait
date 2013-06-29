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

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "aabt-solver.h"
#include "aabt-message.h"
#include "aabt-order.h"
#include "global.h"
#include "abt.pb.h"

using namespace std;
using namespace AIT::CSP;
using namespace AIT::protocols::csp;

struct incoming {
	int n;
	int id;
	std::vector<int> effective;
};

void AABT_Solver::Agile_ABT() {
	for (int i = 0; i < n; ++i)
		teminationValue.push_back(1000);
	end = false;
	assignment.time_stamp = 0;
	assignment.value = 0;

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
	CheckOrder(order, msg.tvi);

	UpdateAgentView(union_func(msg.vi, msg.ei.LHS));
	if (Coherent(msg.ei) && Compatible(msg.ei))
		E.push_front(msg.ei);
	CheckAgentView();
}

void AABT_Solver::ProcessOrder(const AABT_Message& msg) {
	CheckOrder(msg.oi, msg.tvi);
	CheckAgentView();
}

void AABT_Solver::ResolveConflict(const AABT_Message& msg) {
	CheckOrder(msg.oi, msg.tvi);
	UpdateAgentView(union_func(msg.vi, msg.ng.LHS));

	if (Coherent(msg.ng, union_func(assignment, agentView))) {
		if (Compatible(msg.ng, order)) {

			nogoodStore.push_front(msg.ng);
			assignment.value = 0;
			CheckAgentView();
		} else if (msg.ng.RHS.value == assignment.value) {
			// TODO: Remove extra code, add sendMessage ok to msg.sender
			AABT_Message m;
			m.ei = explanation;
			m.vi = assignment;
			m.oi = order;
			m.tvi = teminationValue;
		}
	}
}

void AABT_Solver::CheckOrder(const vector<int>& o, const vector<int>& tv) {
	bool is_stronger = true;
	// FIXME Replace code with lexicographical compare
	for (vector<int>::const_iterator i = tv.begin(), j =
			teminationValue.begin();
			i != tv.end() && j != teminationValue.end(); i++, j++)
		if (*i >= *j)
			is_stronger = false;
	if (is_stronger) {
		teminationValue = tv;
		order = o;
	}

	nogoodStore.remove_if(
			[&](const AABT_Nogood& ngd)->bool {return !Coherent(ngd,agentView);});

	E.remove_if(
			[&](const AABT_Explanation& exp)->bool {return !Compatible(exp);});

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
	if (!Consistent(assignment, agentView)) {
		assignment.value = chooseValue();
		if (assignment.value > 0) {

			AABT_Message m;
			m.msg_type = AABT_MessageType::OK;
			m.vi = assignment;
			m.ei = explanation;
			m.oi = order;
			m.tvi = teminationValue;
			// FIXME: What do this code do?
//			for (order::iterator i = order.begin();; i++) {
//				if (*i == assignment.id)
//					break;
//				//TODO sendmsg ok to agent i
//			}

		} else
			BackTrack();
	}
	// TODO complete the code
//	else //if my_order was modified
	{
		AABT_Message m;
		m.msg_type = AABT_MessageType::OK;
		m.vi = assignment;
		m.ei = explanation;
		m.oi = order;
		m.tvi = teminationValue;
		// FIXME Change the weird looking code
//		for (order::iterator i = order.begin();; i++) {
//			if (*i == assignment.id)
//				break;
//			//TODO sendmsg ok to agent i
//		}
	}
}

void AABT_Solver::UpdateAgentView(const AABT_Solver::CompoundAssignment& a) {
	for (const auto& i : a) {
		for (auto& j : agentView) {
			if (i.id == j.id) {
				if (i.time_stamp > j.time_stamp)
					j = i;
			}
		}
	}
	nogoodStore.remove_if(
			[&](const AABT_Nogood& ngd) {return !Coherent(ngd, this->agentView);});
	E.remove_if([&](const AABT_Explanation& exp) {return !Coherent(exp);});
}

void AABT_Solver::BackTrack() {
	AABT_Nogood ng = Solve(nogoodStore);
	if (ng.LHS.empty()) {
		end = true;
		AABT_Message m;
		m.msg_type = AABT_MessageType::STOP; //send message stop to system
	}
	CVOrderData cvo = ChooseVariableOrder(E, ng);
	bool is_stronger = true;
	// FIXME: Replace this code with std::lexicographical_compare
	for (vector<int>::iterator i = cvo.tv.begin(), j = teminationValue.begin();
			i != cvo.tv.end() and j != teminationValue.end(); i++, j++)
		if (i > j)
			is_stronger = false;
	if (is_stronger) {
		teminationValue = cvo.tv;
		order = cvo.o;
		E = cvo.E;
		setRhs(ng, cvo.a);
		AABT_Message m1;
		m1.msg_type = AABT_MessageType::NOGOOD;
		m1.ng = ng;
		m1.oi = order;
		m1.tvi = teminationValue;

		//send ngd message to cvo.assignment.id
		/*for (vector<AABT_Explanation>::iterator i = E.begin();
		 i != E.end(); i++)
		 if (i->id == cvo.a.id)
		 E.erase(i);*/

		E.remove_if(
				[&](const AABT_Explanation& exp) {return (exp.id==cvo.a.id);});

		AABT_Message m;
		m.msg_type = AABT_MessageType::ORDER;
		m.oi = order;
		m.tvi = teminationValue;
		//TODO broadcast message order
	}

	else {
		setRhs(ng, cvo.a);
		AABT_Message m;
		m.msg_type = AABT_MessageType::NOGOOD;
		m.oi = order;
		m.tvi = teminationValue;
		//sendmsg nogood to Ak;
	}
	cvo.a.value = 0;
	CompoundAssignment c;
	c.push_back(cvo.a);
	UpdateAgentView(c);
	CheckAgentView();
}

int AABT_Solver::chooseValue() {
	for (vector<int>::iterator i = initialDomain.begin();
			i != initialDomain.end(); i++) {
		bool elaminated = false;
		currentDomainsSize[id] = initialDomainsSize[id];
		for (nogood_store::iterator j = nogoodStore.begin();
				j != nogoodStore.end(); j++)
			if (j->RHS.value == *i) {
				elaminated = true;
				currentDomainsSize[id] -= 1;
				break;
			}
		if (!elaminated) {
			AABT_Assignment a;
			a.id = assignment.id;
			a.time_stamp = assignment.time_stamp;
			a.value = *i;

			for (CompoundAssignment::iterator k = agentView.begin();
					k != agentView.end(); k++)
				if (!Consistent(a, *k)) {
					CompoundAssignment ca;
					ca.push_back(*k);
					AABT_Nogood ng;
					ng.LHS = ca;
					ng.RHS = a;
					currentDomainsSize[id] -= 1;
					break;
				} else {
					//*	my_assignment.value=*i;
					assignment.time_stamp += 1;
					return *i;
				}
		}
	}
//*	my_assignment.value=0;
	return 0;
}

forward_list<AABT_Explanation> AABT_Solver::UpdateExplanations(
		const forward_list<AABT_Explanation>& Ei, AABT_Nogood& ng,
		const AABT_Assignment &a) {
	forward_list<AABT_Explanation> up_E = Ei;
	setRhs(ng, a);
	bool a_is_in_up_E;
	AABT_Explanation e;
	forward_list<AABT_Explanation>::iterator a_index;
	for (auto i = up_E.begin(); i != up_E.end(); i++) {
		for (CompoundAssignment::iterator j = i->LHS.begin();
				j != i->LHS.begin(); j++)
			if (j->id == a.id) {
				// up_E.erase(i); FIXME replace code with more elegant C++
				break;
			}

		if (i->id == a.id) {
			a_is_in_up_E = true;
			a_index = i;
		}
	}
	if (!a_is_in_up_E) {
		e.RHS = initialDomainsSize[a.id];
		up_E.push_front(e);
		a_index = up_E.end();
	}

	AABT_Explanation e2;
	e2.LHS = union_func(e.LHS, ng.LHS);
	e2.RHS = e.RHS - 1;
	currentDomainsSize[e2.id] -= 1;
	// FIXME Standardize...
	//up_E.erase(a_index);
	//up_E.push_back(e2);
	return up_E;
}

vector<int> AABT_Solver::ComputeOrder(
		const forward_list<AABT_Explanation>& exp) {

	vector<incoming> graph(n);
	vector<int> o(n);
	int max = 100;
	vector<int>::iterator p = o.begin();
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
			if (currentDomainsSize[z] < max) {
				max = currentDomainsSize[z];
				*p = z;
				d = z;
			}
		graph.erase(
				remove_if(graph.begin(), graph.end(),
						[&](const incoming& in) {return in.id==d;}),
				graph.end());

		vector<int>::iterator r;
		for (auto& u : graph) {
			auto r = std::remove(u.effective.begin(), u.effective.end(), d);
			if (r != u.effective.end()) {
				u.effective.erase(r, u.effective.end());
				u.n--;
			}
			// FIXME Check this
		}
		p++;
	}
	return o;
}

CVOrderData AABT_Solver::ChooseVariableOrder(
		const forward_list<AABT_Explanation> &e, AABT_Nogood &ng) {
	CVOrderData cvo;
	cvo.o = order;
	cvo.tv = teminationValue;
//	cvo.a = my_assignment;
//	cvo.E = ;
	forward_list<AABT_Explanation> up_E;
	vector<int> up_O;
	vector<int> up_Tv;
	for (CompoundAssignment::iterator it = ng.LHS.begin(); it != ng.LHS.end();
			it++) {
		up_E = UpdateExplanations(e, ng, *it);
		up_O = ComputeOrder(up_E);
		vector<int>::iterator tvi = up_Tv.begin();
		for (vector<int>::iterator oi = up_O.begin(); oi != up_O.end(); oi++)
			for (forward_list<AABT_Explanation>::iterator vi = up_E.begin();
					vi != up_E.end(); vi++)
				if (vi->id == *oi) {
					*tvi = vi->RHS;
					tvi++;
					break;
				}
		bool is_smaller = true;
		for (vector<int>::iterator i = up_Tv.begin(), j =
				teminationValue.begin();
				i != up_Tv.end(), j != teminationValue.end(); i++, j++)
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

bool AABT_Solver::Coherent(const AABT_Nogood &ng,
		const CompoundAssignment &ca) {
	for (const auto& i : ng.LHS)
		for (const auto& j : ca)
			if (i.id == j.id && i.value != j.value)
				return false;
	return true;
}

bool AABT_Solver::Coherent(const AABT_Nogood &ng) {
	for (const auto& i : ng.LHS)
		for (const auto& j : agentView)
			if (i.id == j.id && i.value != j.value)
				return false;
	return true;
}

bool AABT_Solver::Coherent(const AABT_Explanation &exp) {
	for (const auto& i : exp.LHS)
		for (const auto& j : agentView)
			if (i.id == j.id && i.value != j.value)
				return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Nogood &ng) {
	vector<int>::iterator my_index;
	my_index = std::find(order.begin(), order.end(), id);
	for (const auto& ci : ng.LHS)
		if (std::find(my_index + 1, order.end(), ci.id) != order.end())
			return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Explanation &e) {
	vector<int>::iterator my_index;
	my_index = std::find(order.begin(), order.end(), id);
	for (const auto& ci : e.LHS)
		if (std::find(my_index + 1, order.end(), ci.id) != order.end())
			return false;
	return true;
}

bool AABT_Solver::Compatible(const AABT_Nogood & ng, const vector<int> &o) {
	vector<int>::const_iterator my_index;
	my_index = std::find(o.begin(), o.end(), id);

	for (const auto& ci : ng.LHS)
		if (std::find(my_index + 1, o.end(), ci.id) != o.end())
			return false;
	return true;
}

AABT_Solver::CompoundAssignment AABT_Solver::union_func(
		const AABT_Assignment &a1, const CompoundAssignment &ca) {
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
	ng.LHS.erase(
			std::remove_if(ng.LHS.begin(), ng.LHS.end(),
					[&](const AABT_Assignment& exp)->bool {return (exp.id==a.id);}),
			ng.LHS.end());

	ng.LHS.push_back(ng.RHS);
	ng.RHS = a;
}

bool AABT_Solver::Consistent(const AABT_Assignment &a,
		const CompoundAssignment &ca) {
	for (CompoundAssignment::iterator it = agentView.begin();
			it != agentView.end(); it++)
		if (a.value == it->value || (it->value - a.value == it->id - a.id))
			return false;
	return true;

}
bool AABT_Solver::Consistent(const AABT_Assignment &a,
		const AABT_Assignment &ca) {

	if (a.value == ca.value || (ca.value - a.value == ca.value - a.id))
		return false;
	return true;

}

void AABT_Solver::getAgentList() {
	//TODO integrate this one
	_INFO( "Sending Request List to monitor agent ...");
	abt::P_CommunicationProtocol requestList;
	requestList.set_type(abt::CP_MessageType::T_REQUEST_LIST);
	requestList.set_id(this->id);
	this->serverRquest.sendMessage(requestList);

	abt::P_CommunicationProtocol requestListAck;
	this->serverRquest.recvMessage(requestListAck);
	if (requestListAck.type() == abt::CP_MessageType::T_REQUEST_ACK) {
		_INFO( "Monitor has accepted to send list of agents.");
	} else {
		_ERROR( "Unable to get agent list from monitor.\n"
		"\tTerminating silently. Goodbye.");
	}

	_INFO( "Waiting for all agents to came online...");
	abt::P_CommunicationProtocol listPacket;
	this->serverBroadcast.recvMessage(listPacket);
	if (listPacket.type() == abt::CP_MessageType::T_LIST) {
		for (int i = 0; i < listPacket.others_size(); ++i) {
			this->everybody.push_front(
					ABT_Solver::EndPoint(listPacket.others(i), context));
			_INFO("New agent introduced by server:\n"
			"\tID:     %d\n"
			"\tHost:   %s\n"
			"\tPort:   %d",
					listPacket.others(i).id(), listPacket.others(i).host().c_str(), listPacket.others(i).port());
		}
		for (auto i = everybody.begin(); i != everybody.end(); ++i) {
			if (i->id() < this->id)
				preceding.push_front(i);
			else if (i->id() > this->id)
				succeeding.push_front(i);
		}
		for (auto &agent : everybody) {
			stringstream address;
			address << "tcp://";
			address << agent.host() << ":" << agent.port();
			agent.socket()->connect(address.str().c_str());
		}
	}
}

void AABT_Solver::sendMessage(const AgentID& agent,
		const AABT_Message& message) {
}

aabt::P_Message AABT_Solver::getMessage() {
	sem_wait(&messageCount);
	pthread_mutex_lock(&this->messageRW);
	protocols::csp::aabt::P_Message x = this->messageQueue.front();
	this->messageQueue.pop();
	pthread_mutex_unlock(&this->messageRW);
	return x;
}

void* AABT_Solver::_messageReader(void* param) {
	// Let's listen to other agents:
	AABT_Solver* solver = (reinterpret_cast<AABT_Solver*>(param));
	solver->listener = new Socket(solver->context, ZMQ_PULL);
	stringstream addressName;
	solver->address = "127.0.0.1";
	addressName << "tcp://" << solver->address << ":*";
	try {
		solver->listener->bind(addressName.str().data());
	} catch (zmq::error_t &e) {
		cerr << e.what() << endl;
	}
	size_t len = 255;
	char endpoint[len];
	solver->listener->getsockopt(ZMQ_LAST_ENDPOINT, &endpoint, &len);
	char * token, *last;
	token = strtok(endpoint, ":");
	while (token != NULL) {
		last = token;
		token = strtok(NULL, ":");
	}
	solver->port = static_cast<unsigned int>(atoi(last));
	_INFO("Socket successfully created. Now listening on %s:%d",
			solver->address.data(), solver->port);
	sem_post(&solver->agentReadyLock);
	while (true) {
		protocols::csp::aabt::P_Message message;
		solver->listener->recvMessage(message);
		pthread_mutex_lock(&(solver->messageRW));
		solver->messageQueue.push(message);
		pthread_mutex_unlock(&(solver->messageRW));
		sem_post(&solver->messageCount);
	}
	return 0;
}

