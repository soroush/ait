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
#include "aabt-nogood.h"

using namespace std;
using namespace AIT::CSP;
using namespace AIT::protocols::csp;
using namespace AIT::protocols::csp::aabt;

AABT_Nogood::AABT_Nogood() {
}

AABT_Nogood::~AABT_Nogood() {
}

AABT_Nogood::AABT_Nogood(const AABT_Nogood& other) :
		LHS(other.LHS), RHS(other.RHS) {
}

AABT_Nogood& AABT_Nogood::operator =(const AABT_Nogood& other) {
	LHS = other.LHS;
	RHS = other.RHS;
	return *this;
}

void AABT_Nogood::readFromProtocol(const P_NoGood& message) {
	this->LHS.clear();
	for (const auto& a : message.lhs().assignments()){
		AABT_Assignment aa;
		aa.readFromProtocol(a);
		this->LHS.push_back(aa);
	}
	this->RHS.readFromProtocol(message.rhs());
}


AABT_Nogood::operator P_NoGood() const {
	P_NoGood n;
	for(const auto& a: this->LHS){
		P_Assignment* p_assignment = n.mutable_lhs()->add_assignments();
		p_assignment->CopyFrom(a);
	}
	n.mutable_rhs()->CopyFrom(this->RHS);
	return n;
}
