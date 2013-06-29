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
		this->E.push_front(e);
	};
	for_each(p_data.e().begin(),p_data.e().end(),adder);
}

