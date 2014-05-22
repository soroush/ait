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

#include "n-queen-abt-solver.h"
#include "../src/CSP/compound-assignment.h"
#include <iostream>

using namespace AIT::CSP;
using namespace std;

NQueen_ABT_Solver::NQueen_ABT_Solver(const std::string& hostname,
		const unsigned short &responder, const unsigned short &publisher,
		const AIT::CSP::AgentID& agentID, const size_t& queenCount) :
		ABT_Solver(hostname, responder, publisher, agentID, queenCount) {
}

bool NQueen_ABT_Solver::consistent(const int& v,
		const AIT::CSP::CompoundAssignment& agentView_) {
	//_INFO("Checking consistency of (%d,%d)", this->id, v);
	if (v == 0)
		return false;
	for (const auto& a : agentView_.items) {
		if (v == a.value || (abs(v - a.value) == abs(this->id - a.id))) {
			_INFO("(%d,%d) <=> (%d,%d)", this->id, v, a.id, a.value);
			return false;
		}
	}
	return true;
}

int NQueen_ABT_Solver::findCulprit(const int& v) {
	AgentID lastCulpirt = 0;
	for (const auto& a : agentView.items) {
		int delta_a = (a.id - id); // FIXME Think about :|
		int delta_v = (a.value - v);
		if (delta_a < 0)
			delta_a *= -1;
		if (delta_v < 0)
			delta_v *= -1;
		if (a.value == v or delta_a == delta_v) { // culprit
			lastCulpirt = (a.id > lastCulpirt) ? a.id : lastCulpirt; // and last
		}
	}
	return lastCulpirt;
}

int NQueen_ABT_Solver::findLastCulprit() {
	AgentID lastID = 0;
	for (const auto& ngd : noGoodStore) {
		for (const auto& a : ngd.lhs.items) {
			if (a.id > lastID)
				lastID = a.id;
		}
	}
	return lastID;
}

NQueen_ABT_Solver::~NQueen_ABT_Solver() {
	_INFO("N-Queen dtor called");
}

int NQueen_ABT_Solver::findCulpritsValue(const int& culpirtsID) {
	for (const auto& a : agentView.items) {
		if (a.id == culpirtsID)
			return a.value;
	}
	return 0;
}

void NQueen_ABT_Solver::prepareProblem() {
	for (int i = 0; i < agentCount; ++i) {
		this->domain.push_back(i + 1);
	}
}

int main(int argc, char *argv[]) {
	_INFO("Running 8 queens agent in background...");
	NQueen_ABT_Solver s(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]),
			atoi(argv[5]));
	s.ABT();
	return 0;
}
