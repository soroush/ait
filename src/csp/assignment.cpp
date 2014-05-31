/*
 AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.

 Copyright (c) 2012,2013 Soroush Rabiei <soroush-r@users.sf.net>,
 Roya Ghasemzadeh <ghasemzadeh.roya1@gmail.com>

 AIT is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 See the file COPYING included with this distribution for more
 information.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "assignment.h"

using namespace AIT::CSP;
using namespace AIT::protocols::csp;

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

Assignment::operator P_Assignment() {
	P_Assignment pa;
	pa.set_priority(this->id);
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
	this->id = message.priority();
	if (message.has_value())
		this->value = message.value();
	else
		this->value = 0;
}

