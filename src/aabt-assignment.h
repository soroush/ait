/*
 * aabt-assignment.h
 *
 *  Created on: Jun 24, 2013
 *      Author: roya
 */

#ifndef AABT_ASSIGNMENT_H_
#define AABT_ASSIGNMENT_H_

#include "aabt.pb.h"

namespace AIT {

struct AABT_Assignment {
	// TODO implement a static `fromProtocol'
	// or a friend conversion operator
	AABT_Assignment();
	AABT_Assignment(const AABT_Assignment& a);
	~AABT_Assignment();
	AABT_Assignment& operator=(const AABT_Assignment& a);
	operator protocols::csp::aabt::P_Assignment() const;
	void readFromProtocol(const protocols::csp::aabt::P_Assignment&);

	int id;
	int time_stamp;
	int value;
};
} /* namespace AIT */
#endif /* AABT_ASSIGNMENT_H_ */
