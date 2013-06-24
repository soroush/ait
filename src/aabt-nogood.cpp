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

AABT_Nogood& AABT_Nogood::operator =(const AABT_Nogood& other) {
	LHS.clear();
	copy(other.LHS.begin(),other.LHS.end(),this->LHS.begin());
	RHS = other.RHS;
	return *this;
}

