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

#ifndef AABT_ORDER_H_
#define AABT_ORDER_H_

#include <forward_list>
#include "aabt-explanation.h"

namespace AIT {
namespace CSP {

struct CVOrderData {
	CVOrderData();
	CVOrderData(const CVOrderData& other);
	~CVOrderData();
	CVOrderData& operator=(const CVOrderData& c1);
	operator protocols::csp::aabt::P_CVOrderData() const;
	void readFromProtocol(const protocols::csp::aabt::P_CVOrderData&);

	AABT_Assignment a;
	std::vector<int> o;
	std::vector<int> tv;
	std::forward_list<AABT_Explanation> E;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* AABT_ORDER_H_ */
