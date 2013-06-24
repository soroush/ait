/*
 * aabt-assignment.h
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#ifndef AABT_ASSIGNMENT_H_
#define AABT_ASSIGNMENT_H_

namespace AIT {

struct AABT_Assignment {
	int id;
	int time_stamp;
	int value;
	AABT_Assignment() {
		id = 0;
		time_stamp = 0;
		value = 0;
	}

	AABT_Assignment(const AABT_Assignment& a) {
		id = a.id;
		time_stamp = a.time_stamp;
		value = a.value;
	}
	~AABT_Assignment() {
	}
	AABT_Assignment& operator=(const AABT_Assignment& a) {
		id = a.id;
		time_stamp = a.time_stamp;
		value = a.value;
		return *this;
	}
};
} /* namespace AIT */
#endif /* AABT_ASSIGNMENT_H_ */
