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

struct ABT_Message {
	ABT_Message();
	ABT_Message(const ABT_Message& other);
	~ABT_Message();
	ABT_Message& operator =(const ABT_Message& other);
	operator protocols::csp::abt::P_Message() const;
	void readFromProtocol(const protocols::csp::abt::P_Message&);

	protocols::csp::abt::P_MessageType type;
	AgentID sender;
	Assignment assignment;
	CompoundAssignment nogood;
};

} /* namespace AIT */
#endif /* ABT_MESSAGE_H_ */
