/*
 * aabt-message.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#include <algorithm>
#include "aabt-message.h"

using namespace AIT;
using namespace std;

AABT_Message::AABT_Message(const AABT_Message& other) :
		vi(other.vi), ei(other.ei), ng(other.ng), tvi(other.tvi), oi(other.oi) {
	msg_type = other.msg_type;
	sender_id = other.sender_id;
}

AABT_Message& AABT_Message::operator =(const AABT_Message& other) {
	msg_type = other.msg_type;
	sender_id = other.sender_id;
	vi = other.vi;
	ei = other.ei;
	ng = other.ng;
	oi.clear();
	tvi.clear();
	std::copy(other.oi.begin(), other.oi.end(), oi.begin());
	std::copy(other.tvi.begin(), other.tvi.end(), tvi.begin());
	return *this;
}
