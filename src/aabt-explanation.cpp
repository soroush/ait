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

AIT::AABT_Explanation::AABT_Explanation(const AABT_Explanation& other):LHS(other.LHS)
{
	RHS=other.RHS;
	id=other.id;
}

AABT_Explanation& AIT::AABT_Explanation::operator =(
		const AABT_Explanation& other) {
	LHS.clear();
	copy(other.LHS.begin(),other.LHS.end(),this->LHS.begin());
	RHS = other.RHS;
	id = other.id;
	return *this;
}

