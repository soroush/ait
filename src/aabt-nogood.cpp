/*
 * aabt-nogood.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#include <algorithm>
#include "aabt-nogood.h"

using namespace AIT;
using namespace std;
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
