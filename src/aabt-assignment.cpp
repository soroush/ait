/*
 * aabt-assignment.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#include "aabt-assignment.h"
using namespace AIT;

AABT_Assignment::AABT_Assignment() {
}

AABT_Assignment::AABT_Assignment(const AABT_Assignment& a) :
		id(a.id), time_stamp(a.time_stamp), value(a.value) {
}
AABT_Assignment::~AABT_Assignment() {
}

AABT_Assignment& AABT_Assignment::operator=(const AABT_Assignment&a) {
	id = a.id;
	time_stamp = a.time_stamp;
	value = a.value;
	return *this;
}
using namespace AIT;

