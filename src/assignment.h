/*
 * assignment.h
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

#include "common-protocols.pb.h"

namespace AIT {

struct Assignment {
	Assignment();
	Assignment(const Assignment& other);
	~Assignment();
	int id;
	int value;
	bool operator ==(const Assignment& other) const;
	bool operator !=(const Assignment& other) const;
	bool operator >(const Assignment& other) const;
	bool operator <(const Assignment& other) const;// for std::set
	Assignment& operator =(const Assignment& other);
	operator protocols::csp::P_Assignment();
};

} /* namespace AIT */
#endif /* ASSIGNMENT_H_ */
