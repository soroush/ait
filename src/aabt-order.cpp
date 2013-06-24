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

CVOrderData::CVOrderData(const CVOrderData& other) :
		a(other.a), o(other.o), tv(other.tv), E(other.E) {
}

CVOrderData::~CVOrderData() {
}

CVOrderData& CVOrderData::operator =(const CVOrderData& other) {
	a = other.a;
	o.clear();
	E.clear();
	tv.clear();
	copy(other.o.begin(), other.o.end(), this->o.begin());
	copy(other.E.begin(), other.E.end(), this->E.begin());
	copy(other.tv.begin(), other.tv.end(), this->tv.begin());
	return *this;
}
