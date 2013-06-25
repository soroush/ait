/*
 * aabt-assignment.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#include "aabt-assignment.h"

using namespace AIT;
using namespace protocols::csp::aabt;

AABT_Assignment::AABT_Assignment() {
}

AABT_Assignment::AABT_Assignment(const AABT_Assignment& a) :
		id(a.id), time_stamp(a.time_stamp), value(a.value) {
}
AABT_Assignment::~AABT_Assignment() {
}

AABT_Assignment& AABT_Assignment::operator=(const AABT_Assignment&a) {
	this->id = a.id;
	this->time_stamp = a.time_stamp;
	this->value = a.value;
	return *this;
}

AABT_Assignment::operator P_Assignment() const {
	P_Assignment assignment;
	assignment.set_id(this->id);
	assignment.set_time_stamp(this->time_stamp);
	assignment.set_value(this->value);
	return assignment;
}

void AIT::AABT_Assignment::readFromProtocol(const P_Assignment& p_assignment) {
	this->id = p_assignment.id();
	this->time_stamp = p_assignment.time_stamp();
	this->value = p_assignment.value();
}

