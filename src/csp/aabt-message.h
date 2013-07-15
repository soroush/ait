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

#ifndef AABT_MESSAGE_H_
#define AABT_MESSAGE_H_

#include "aabt-assignment.h"
#include "aabt-explanation.h"
#include "aabt-nogood.h"
#include "../global.h"
#include <vector>

namespace AIT {
namespace CSP {

enum class LIBRARY_API AABT_MessageType {
	OK = 1, NOGOOD = 2, ORDER = 3, STOP = 4
};

struct LIBRARY_API AABT_Message {
	AABT_Message();
	AABT_Message(const AABT_Message& other);
	AABT_Message(protocols::csp::aabt::P_Message&);
	~AABT_Message();
	AABT_Message& operator=(const AABT_Message& m1);
	void readFromProtocol(const protocols::csp::aabt::P_Message&);
	operator protocols::csp::aabt::P_Message() const;

	AABT_MessageType msg_type;
	int sender_id;
	AABT_Assignment vi;
	AABT_Explanation ei;
	std::vector<int> oi;
	std::vector<int> tvi;
	AABT_Nogood ng;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* AABT_MESSAGE_H_ */
