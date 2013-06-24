/*
 * aabt-order.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#include "aabt-order.h"

using namespace AIT;

CVOrderData::CVOrderData() {
}

CVOrderData::~CVOrderData() {
}

CVOrderData& CVOrderData::operator =(const CVOrderData& other) {
	a = other.a;
	for (const auto& x : other.o)
		o.push_back(x);
	for (const auto& x : other.E)
		E.push_back(x);
	for (const auto& x : other.tv)
		tv.push_back(x);
	return *this;
}
