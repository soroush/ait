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

class Assignment: public AIT::protocols::csp::P_Assignment {
public:
	Assignment();
	virtual ~Assignment();
	bool operator ==(const protocols::csp::P_Assignment& other);
};

} /* namespace AIT */
#endif /* ASSIGNMENT_H_ */
