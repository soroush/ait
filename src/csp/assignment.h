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

#ifndef ASSIGNMENT_H_
#define ASSIGNMENT_H_

// FIXME: Remove inclusion of build-time configuration file in public API and
//        packages.
//#include "../../config.h"
#include "common_async.h"
//#ifdef PRECOMPILED_PROTOCOLS
#include "common-protocols_precompiled.pb.h"
//#elif defined(GENERATED_PROTOCOLS)
//#include "common-protocols.pb.h"
//#endif
#include "../global.h"

namespace AIT {
namespace CSP {

struct LIBRARY_API Assignment {
	Assignment();
	Assignment(const Assignment& other);
	Assignment(const AgentID&, const int&);
	~Assignment();
	int id;
	int value;
	bool operator ==(const Assignment& other) const;
	bool operator !=(const Assignment& other) const;
	bool operator >(const Assignment& other) const;
	bool operator <(const Assignment& other) const;// for std::set
	Assignment& operator =(const Assignment& other);
	operator protocols::csp::P_Assignment();
	void readFromProtocol(const protocols::csp::P_Assignment&);
};

} /* namespace CSP */
} /* namespace AIT */

#endif /* ASSIGNMENT_H_ */
