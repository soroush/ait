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

#include <algorithm>
#include "aabt-explanation.h"

using namespace std;
using namespace AIT::CSP;
using namespace AIT::protocols::csp::aabt;

AABT_Explanation::AABT_Explanation() {

}
AABT_Explanation::~AABT_Explanation() {

}

AABT_Explanation::AABT_Explanation(const AABT_Explanation& other) :
		id(other.id), LHS(other.LHS), RHS(other.RHS) {
}

AABT_Explanation& AABT_Explanation::operator =(
		const AABT_Explanation& other) {
	LHS.clear();
	copy(other.LHS.begin(), other.LHS.end(), this->LHS.begin());
	RHS = other.RHS;
	id = other.id;
	return *this;
}

AABT_Explanation::operator P_Explanation() const {
	P_Explanation ex;
	ex.set_id(this->id);
	for (const auto &lhs : this->LHS) {
		P_Assignment* assignment = ex.mutable_lhs()->add_assignments();
		assignment->CopyFrom(lhs);
	}
	ex.set_rhs(this->RHS);
	return ex;
}

void AABT_Explanation::readFromProtocol(const P_Explanation& p_ex) {
	this->id = p_ex.id();
	for (const auto& p_assignment : p_ex.lhs().assignments()) {
		AABT_Assignment assignment;
		assignment.readFromProtocol(p_assignment);
		this->LHS.push_back(assignment);
		// TODO convert to ctor, enable RVO
	}
	this->RHS = p_ex.rhs();
}

