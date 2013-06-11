/*
 * compound-assignment.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#include "compound-assignment.h"

using namespace AIT;
using namespace protocols::csp;

CompoundAssignment::CompoundAssignment() {

}

CompoundAssignment::~CompoundAssignment() {
}

void CompoundAssignment::add(const P_Assignment& item) {
	// FIXME: Why for(auto& a : mutable_assignments()) does not work?
	for (auto& a : *mutable_assignments()) {
		if (a.id() == item.id()) {
			a.set_value(item.value());
			return;
		}
	}
	// if not found:
	P_Assignment* newAssign = add_assignments();
	newAssign->CopyFrom(item);
}

void CompoundAssignment::add(const P_CompoundAssignment& item) {
	for (const auto& a : item.assignments()) {
		add(a);
	}
}

void CompoundAssignment::add(const int& id, const int& value) {
	for (auto& a : *mutable_assignments()) {
		if (id == a.id()) {
			a.set_value(value);
			return;
		}
	}
	// if not found an element with specified id, add it to elements:
	P_Assignment* a = add_assignments();
	a->set_id(id);
	a->set_value(value);
}
