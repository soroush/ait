/*
 * aabt-explanation.h
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#ifndef AABT_EXPLANATION_H_
#define AABT_EXPLANATION_H_

#include <vector>

#include "aabt-assignment.h"

namespace AIT {

#include "common-protocols.pb.h"
#include "aabt.pb.h"
struct AABT_Explanation {
	int id;
	std::vector<AABT_Assignment> LHS;
	int RHS;
	AABT_Explanation();
	AABT_Explanation(const AABT_Explanation& e1);
	~AABT_Explanation();
	AABT_Explanation& operator=(const AABT_Explanation& e1);
	operator protocols::csp::aabt::P_Explanation() const;
	void readFromProtocol(const protocols::csp::aabt::P_Explanation&);
};

} /* namespace AIT */
#endif /* AABT_EXPLANATION_H_ */
