/*
 * compound-assignment.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
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
