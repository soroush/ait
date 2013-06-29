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

#ifndef AABT_EXPLANATION_H_
#define AABT_EXPLANATION_H_

#include <vector>

#include "aabt-assignment.h"
#include "aabt.pb.h"
#include "common-protocols.pb.h"
#include "aabt.pb.h"

namespace AIT {
namespace CSP {

struct AABT_Explanation {
	AABT_Explanation();
	AABT_Explanation(const AABT_Explanation& other);
	~AABT_Explanation();
	AABT_Explanation& operator=(const AABT_Explanation& e1);
	operator protocols::csp::aabt::P_Explanation() const;
	void readFromProtocol(const protocols::csp::aabt::P_Explanation&);

	int id;
	std::vector<AABT_Assignment> LHS;
	int RHS;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* AABT_EXPLANATION_H_ */
