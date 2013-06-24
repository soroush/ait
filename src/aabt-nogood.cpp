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

using namespace AIT;
using namespace std;

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

operator protocols::csp::aabt::P_NoGood() const {
P_NoGood pngd;
}
void readFromProtocol(const protocols::csp::aabt::P_NoGood&) {

}
