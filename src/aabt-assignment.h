/*
 * aabt-assignment.h
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#ifndef AABT_ASSIGNMENT_H_
#define AABT_ASSIGNMENT_H_

#include "aabt.pb.h"

namespace AIT {

struct AABT_Assignment {
	int id;
	int time_stamp;
	int value;


	AABT_Assignment();

	AABT_Assignment(const AABT_Assignment& a);
	~AABT_Assignment();
	AABT_Assignment& operator=(const AABT_Assignment& a);

	operator protocols::csp::aabt::P_Assignment() const;
	void readFromProtocol(const protocols::csp::aabt::P_Assignment&);
};
} /* namespace AIT */
#endif /* AABT_ASSIGNMENT_H_ */
