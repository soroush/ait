/*
 * abt-nogood.h
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#ifndef ABT_NOGOOD_H_
#define ABT_NOGOOD_H_

#include "abt.pb.h"
#include "compound-assignment.h"

namespace AIT {

struct ABT_Nogood {
	ABT_Nogood();
	ABT_Nogood(const ABT_Nogood& other);
	ABT_Nogood(const CompoundAssignment& lhs, const Assignment& rhs);
	~ABT_Nogood();
	CompoundAssignment lhs;
	Assignment rhs;
	bool operator ==(const ABT_Nogood& other) const;
	bool operator !=(const ABT_Nogood& other) const;
	ABT_Nogood& operator =(const ABT_Nogood& other);
	operator protocols::csp::abt::P_Nogood();
};

} /* namespace AIT */
#endif /* ABT_NOGOOD_H_ */
