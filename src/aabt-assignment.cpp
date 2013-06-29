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

#include "aabt-assignment.h"

using namespace AIT::CSP;
using namespace AIT::protocols::csp::aabt;

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

void AABT_Assignment::readFromProtocol(const P_Assignment& p_assignment) {
	this->id = p_assignment.id();
	this->time_stamp = p_assignment.time_stamp();
	this->value = p_assignment.value();
}

