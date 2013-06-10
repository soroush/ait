/*
 * common_csp.cpp
 *
 *  Created on: Jun 7, 2013
 *      Author: soroush
 */

#include "common_csp.h"

using namespace AIT;
using namespace protocols::csp;

//void AIT::CompoundAssignment::add(const P_Assignment& item) {
//	for (int i = 0; i < assignments_size(); ++i) {
//		if (item.id() == assignments(i).id()) {
//			mutable_assignments(i)->set_value(item.value());
//			return;
//		}
//	}
//}
//
//void AIT::CompoundAssignment::add(const P_CompoundAssignment& item) {
//	for (int i = 0; i < item.assignments_size(); ++i) {
//		add(item.assignments(i));
//	}
//}
//
//void AIT::CompoundAssignment::add(const CompoundAssignment& item) {
//	add(static_cast<P_CompoundAssignment>(item));
//}
//
//void AIT::CompoundAssignment::add(const int& id, const int& value) {
//	for (int i = 0; i < assignments_size(); ++i) {
//		if (id == assignments(i).id()) {
//			mutable_assignments(i)->set_value(value);
//			return;
//		}
//	}
//}

