/*
 * assignment.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#include "assignment.h"
using namespace AIT;
using namespace protocols::csp;

Assignment::Assignment() {
}

Assignment::~Assignment() {
}

bool Assignment::operator ==(const protocols::csp::P_Assignment& other) {
	return this->id() == other.id() and this->value() == other.value();
}
