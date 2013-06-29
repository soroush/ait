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

#include "compound-assignment.h"
#include <algorithm>

using namespace std;
using namespace AIT;
using namespace protocols::csp;

AIT::CompoundAssignment::CompoundAssignment() {
}

AIT::CompoundAssignment::CompoundAssignment(const CompoundAssignment& other) :
		items(other.items) {
}

AIT::CompoundAssignment::~CompoundAssignment() {
	this->items.clear();
}

bool AIT::CompoundAssignment::operator ==(
		const CompoundAssignment& other) const {
	return (this->items == other.items);
}

bool AIT::CompoundAssignment::operator !=(
		const CompoundAssignment& other) const {
	return (this->items != other.items);
}

CompoundAssignment& AIT::CompoundAssignment::operator =(
		const CompoundAssignment& other) {
	this->items = other.items;
	return *this;
}

AIT::CompoundAssignment::operator protocols::csp::P_CompoundAssignment() const {
	P_CompoundAssignment ca;
	for (const auto& a : items) {
		P_Assignment* newAssignment = ca.add_assignments();
		newAssignment->set_id(a.id);
		newAssignment->set_value(a.value);
	}
	return ca;
	// FIXME Copy constructor called. Reimplement with move if possible.
}

void CompoundAssignment::readFromProtocol(const P_CompoundAssignment& ca) {
	for(const auto& a : ca.assignments()){
		this->items.insert(Assignment(a.id(),a.value()));
	}
}

//CompoundAssignment::CompoundAssignment() {
//
//}
//
//CompoundAssignment::~CompoundAssignment() {
//}
//
//void CompoundAssignment::add(const P_Assignment& item) {
//	// FIXME: Why for(auto& a : mutable_assignments()) does not work?
//	for (auto& a : *mutable_assignments()) {
//		if (a.id() == item.id()) {
//			a.set_value(item.value());
//			return;
//		}
//	}
//	// if not found:
//	P_Assignment* newAssign = add_assignments();
//	newAssign->CopyFrom(item);
//}
//
//void CompoundAssignment::add(const P_CompoundAssignment& item) {
//	for (const auto& a : item.assignments()) {
//		add(a);
//	}
//}
//
//void CompoundAssignment::add(const int& id, const int& value) {
//	for (auto& a : *mutable_assignments()) {
//		if (id == a.id()) {
//			a.set_value(value);
//			return;
//		}
//	}
//	// if not found an element with specified id, add it to elements:
//	P_Assignment* a = add_assignments();
//	a->set_id(id);
//	a->set_value(value);
//}
