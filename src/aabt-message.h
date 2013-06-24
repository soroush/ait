/*
 * aabt-message.h
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#ifndef AABT_MESSAGE_H_
#define AABT_MESSAGE_H_

#include "aabt-assignment.h"
#include "aabt-explanation.h"
#include "aabt-nogood.h"
#include <vector>

namespace AIT {

enum class AABT_MessageType {
	OK = 1, NOGOOD = 2, ORDER = 3, STOP = 4
};

struct AABT_Message {
	AABT_MessageType msg_type;
	int sender_id;
	AABT_Assignment vi;
	AABT_Explanation ei;
	std::vector<int> oi;
	std::vector<int> tvi;
	AABT_Nogood ng;
	AABT_Message();
	AABT_Message(const AABT_Message& m1);
	~AABT_Message();
	AABT_Message& operator=(const AABT_Message& m1);
};
} /* namespace AIT */
#endif /* AABT_MESSAGE_H_ */
