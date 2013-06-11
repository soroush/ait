/*
 * abt-nogood.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#include "abt-nogood.h"
using namespace AIT;
using namespace protocols::csp;
using namespace protocols::csp::abt;

ABT_Nogood::ABT_Nogood() {
}

ABT_Nogood::~ABT_Nogood() {
}

void ABT_Nogood::addLHS(const P_Assignment& item) {
	// FIXME: Why for(auto& a : mutable_assignments()) does not work?
	for (auto& a : *mutable_lhs()->mutable_assignments()) {
		if (a.id() == item.id()) {
			a.set_value(item.value());
			return;
		}
	}
	// if not found:
	P_Assignment* newAssign = mutable_lhs()->add_assignments();
	newAssign->CopyFrom(item);
}

void ABT_Nogood::addLHS(const P_CompoundAssignment& item) {
	for (const auto& a : item.assignments()) {
		addLHS(a);
	}
}

void ABT_Nogood::addLHS(const int& id, const int& value) {
	for (auto& a : *mutable_lhs()->mutable_assignments()) {
		if (id == a.id()) {
			a.set_value(value);
			return;
		}
	}
	// if not found an element with specified id, add it to elements:
	P_Assignment* a = mutable_lhs()->add_assignments();
	a->set_id(id);
	a->set_value(value);
}


