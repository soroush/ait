/*
 * assignment.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#include "assignment.h"
using namespace AIT;
using namespace protocols::csp;

Assignment::Assignment() :
		id(0), value(0) {
}

Assignment::Assignment(const Assignment& other) :
		id(other.id), value(other.value) {
}

Assignment::~Assignment() {
}

bool Assignment::operator ==(const Assignment& other) const {
	return this->id == other.id and this->value == other.value;
}

bool Assignment::operator !=(const Assignment& other) const {
	return !(*this == other);
}

bool Assignment::operator >(const Assignment& other) const {
	return this->id > other.id;
}

bool Assignment::operator <(const Assignment& other) const {
	return this->id < other.id;
}

Assignment::operator protocols::csp::P_Assignment() {
	P_Assignment pa;
	pa.set_id(this->id);
	pa.set_value(this->value);
	return pa;
}

Assignment& Assignment::operator =(const Assignment& other) {
	this->id = other.id;
	this->value = other.value;
	return *this;
}

Assignment::Assignment(const AgentID& id_, const int& value_) :
		id(id_), value(value_) {
}

void Assignment::readFromProtocol(const P_Assignment& message) {
	this->id = message.id();
	if (message.has_value())
		this->value = message.value();
	else
		this->value = 0;
}

