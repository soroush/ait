/*
 * abt-message.h
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#ifndef ABT_MESSAGE_H_
#define ABT_MESSAGE_H_

#include "compound-assignment.h"
#include "assignment.h"
#include "abt-nogood.h"

namespace AIT {

#include "common-protocols.pb.h"
#include "abt.pb.h"

class ABT_Message {
public:
	ABT_Message();
	virtual ~ABT_Message();
	protocols::csp::abt::P_MessageType type;
	AgentID sender;
	Assignment assignment;
	CompoundAssignment nogood;
};

} /* namespace AIT */
#endif /* ABT_MESSAGE_H_ */
