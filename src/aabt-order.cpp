/*
 * aabt-order.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: soroush
 */

#include <algorithm>
#include "aabt-order.h"

using namespace AIT;
using namespace protocols::csp::aabt;
using namespace std;

CVOrderData::CVOrderData() {
}

CVOrderData::CVOrderData(const CVOrderData& other) :
		a(other.a), o(other.o), tv(other.tv), E(other.E) {
}

CVOrderData::~CVOrderData() {
}

CVOrderData& CVOrderData::operator =(const CVOrderData& other) {
	a = other.a;
	o = other.o;
	tv = other.tv;
	E = other.E;
	return *this;
}

CVOrderData::operator P_CVOrderData() const {
	P_CVOrderData p_o;
	p_o.mutable_assignment()->CopyFrom(this->a);
	for (const auto& x : this->o)
		p_o.mutable_order()->Add(x);
	for (const auto& x : this->tv)
		p_o.mutable_tv()->Add(x);
	for (const auto& x : this->E) {
		P_Explanation *p_e = p_o.mutable_e()->Add();
		p_e->CopyFrom(x);
	}
	return p_o;
}

void CVOrderData::readFromProtocol(const P_CVOrderData& p_data) {
	this->a.readFromProtocol(p_data.assignment());
	this->o.clear();
	copy(p_data.order().begin(), p_data.order().end(), this->o.begin());
	this->tv.clear();
	copy(p_data.tv().begin(), p_data.tv().end(), this->tv.begin());
	auto adder = [&](const P_Explanation& p_e) {
		AABT_Explanation e;
		e.readFromProtocol(p_e);
		this->E.push_back(e);
	};
	for_each(p_data.e().begin(),p_data.e().end(),adder);
}

