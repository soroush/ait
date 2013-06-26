/*
 * aabt-nogood.h
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#ifndef AABT_NOGOOD_H_
#define AABT_NOGOOD_H_

#include <vector>
#include "aabt-assignment.h"

namespace AIT {

struct AABT_Nogood {
	AABT_Nogood();
	~AABT_Nogood();
	AABT_Nogood(const AABT_Nogood& other);
	AABT_Nogood& operator=(const AABT_Nogood& other);
	void readFromProtocol(const protocols::csp::aabt::P_NoGood&);
	operator protocols::csp::aabt::P_NoGood() const;

	std::vector<AABT_Assignment> LHS;
	AABT_Assignment RHS;
};
} /* namespace AIT */
#endif /* AABT_NOGOOD_H_ */
