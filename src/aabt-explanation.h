/*
 * aabt-explanation.h
 *
 *  Created on: Jun 24, 2013
 *      Author: roya
 */

#ifndef AABT_EXPLANATION_H_
#define AABT_EXPLANATION_H_

#include <vector>

#include "aabt-assignment.h"
#include "aabt.pb.h"
#include "common-protocols.pb.h"
#include "aabt.pb.h"

namespace AIT {

struct AABT_Explanation {
	AABT_Explanation();
	AABT_Explanation(const AABT_Explanation& other);
	~AABT_Explanation();
	AABT_Explanation& operator=(const AABT_Explanation& e1);
	operator protocols::csp::aabt::P_Explanation() const;
	void readFromProtocol(const protocols::csp::aabt::P_Explanation&);

	int id;
	std::vector<AABT_Assignment> LHS;
	int RHS;
};

} /* namespace AIT */
#endif /* AABT_EXPLANATION_H_ */
