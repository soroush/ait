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

#include "abt-solver.h"

using namespace AIT;
using namespace protocols::csp;
using namespace protocols::csp::abt;

ABT_Solver::Nogood::Nogood() {
}

ABT_Solver::Nogood::Nogood(const Nogood& other) :
		lhs(other.lhs), rhs(other.rhs) {
}

ABT_Solver::Nogood::~Nogood() {
}

bool ABT_Solver::Nogood::operator ==(const Nogood& other) const {
	return (this->lhs == other.lhs and this->rhs == other.rhs);
}

bool ABT_Solver::Nogood::operator !=(const Nogood& other) const {
	return (this->lhs != other.lhs or this->lhs == other.lhs);
}

ABT_Solver::Nogood& ABT_Solver::Nogood::operator =(const Nogood& other) {
	this->lhs = other.lhs;
	this->rhs = other.rhs;
	return *this;
}

ABT_Solver::Nogood::Nogood(const CompoundAssignment& lhs_,
		const Assignment& rhs_) :
		lhs(lhs_), rhs(rhs_) {
}

ABT_Solver::Nogood::operator protocols::csp::abt::P_Nogood() {
	P_Nogood pngd;
	pngd.mutable_lhs()->CopyFrom(this->lhs);
	pngd.mutable_rhs()->CopyFrom(this->rhs);
	return pngd;
}

//ABT_Nogood::ABT_Nogood() {
//}
//
//ABT_Nogood::~ABT_Nogood() {
//}
//
//void ABT_Nogood::addLHS(const P_Assignment& item) {
//	// FIXME: Why for(auto& a : mutable_assignments()) does not work?
//	for (auto& a : *mutable_lhs()->mutable_assignments()) {
//		if (a.id() == item.id()) {
//			a.set_value(item.value());
//			return;
//		}
//	}
//	// if not found:
//	P_Assignment* newAssign = mutable_lhs()->add_assignments();
//	newAssign->CopyFrom(item);
//}
//
//void ABT_Nogood::addLHS(const P_CompoundAssignment& item) {
//	for (const auto& a : item.assignments()) {
//		addLHS(a);
//	}
//}
//
//void ABT_Nogood::addLHS(const int& id, const int& value) {
//	for (auto& a : *mutable_lhs()->mutable_assignments()) {
//		if (id == a.id()) {
//			a.set_value(value);
//			return;
//		}
//	}
//	// if not found an element with specified id, add it to elements:
//	P_Assignment* a = mutable_lhs()->add_assignments();
//	a->set_id(id);
//	a->set_value(value);
//}

