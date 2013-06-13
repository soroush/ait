/*
 * compound-assignment.h
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#ifndef COMPOUND_ASSIGNMENT_H_
#define COMPOUND_ASSIGNMENT_H_

#include "common-protocols.pb.h"
#include "assignment.h"
#include <set>

namespace AIT {

//typedef std::set<Assignment> CompoundAssignment;

class CompoundAssignment: public AIT::protocols::csp::P_CompoundAssignment {
public:
	CompoundAssignment();
	virtual ~CompoundAssignment();
	void add(const protocols::csp::P_Assignment& item);
	void add(const protocols::csp::P_CompoundAssignment& item);
	void add(const int& id, const int& value);
};

} /* namespace AIT */
#endif /* COMPOUND_ASSIGNMENT_H_ */
