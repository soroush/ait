/*
 * abt-nogood.h
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#ifndef ABT_NOGOOD_H_
#define ABT_NOGOOD_H_

#include "abt.pb.h"

namespace AIT {

class ABT_Nogood: public AIT::protocols::csp::abt::P_Nogood {
public:
	ABT_Nogood();
	virtual ~ABT_Nogood();
	void addLHS(const protocols::csp::P_Assignment& item);
	void addLHS(const protocols::csp::P_CompoundAssignment& item);
	void addLHS(const int& id, const int& value);
};

} /* namespace AIT */
#endif /* ABT_NOGOOD_H_ */
