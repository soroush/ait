/*
 * csp-common.hpp
 *
 *  Created on: May 17, 2013
 *      Author: soroush
 */

#ifndef COMMON_CSP_HPP_
#define COMMON_CSP_HPP_

#include "common-protocols.pb.h"

namespace AIT {

using namespace AIT::protocols::csp;

typedef P_Assignment Assignment;

class CompoundAssignment: public P_CompoundAssignment {
public:
	void add(const Assignment& item);
};

typedef CompoundAssignment Nogood;
typedef CompoundAssignment PartialSolution;

}

// end of namespace

#endif /* COMMON_CSP_HPP_ */
