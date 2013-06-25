/*
 * aabt-explanation.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#include <algorithm>
#include "aabt-explanation.h"

using namespace AIT;
using namespace std;
using namespace protocols::csp::aabt;

AABT_Explanation::AABT_Explanation() {

}
AABT_Explanation::~AABT_Explanation() {

}

AABT_Explanation::AABT_Explanation(const AABT_Explanation& other) :
		id(other.id), LHS(other.LHS), RHS(other.RHS) {
}

AABT_Explanation& AIT::AABT_Explanation::operator =(
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

