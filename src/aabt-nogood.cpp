/*
 * aabt-nogood.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#include <algorithm>
#include "aabt-nogood.h"

using namespace AIT;
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
	LHS.clear();
	copy(other.LHS.begin(), other.LHS.end(), this->LHS.begin());
	RHS = other.RHS;
	return *this;
}

AABT_Nogood::operator protocols::csp::aabt::P_NoGood() const {
}

void AABT_Nogood::readFromProtocol(
		const protocols::csp::aabt::P_NoGood& message) {
}

