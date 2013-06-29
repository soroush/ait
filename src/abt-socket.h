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

#ifndef SOCKET_H_
#define SOCKET_H_

#include <zmq.hpp>
#include <string>
#include "abt.pb.h"
#include "aabt.pb.h"

namespace AIT {

class Socket: public zmq::socket_t {
public:
	Socket(zmq::context_t &context_, int type_);
	Socket(zmq::socket_t&& rhs);
	virtual ~Socket();
	// Common
	size_t sendMessage(const protocols::csp::abt::P_CommunicationProtocol);
	size_t recvMessage(protocols::csp::abt::P_CommunicationProtocol&);
	// ABT
	size_t sendMessage(const protocols::csp::abt::P_Message);
	size_t recvMessage(protocols::csp::abt::P_Message&);
	// AABT
	size_t sendMessage(const protocols::csp::aabt::P_Message);
	size_t recvMessage(protocols::csp::aabt::P_Message&);
	static std::string getIP();
};

} /* namespace AIT */
#endif /* SOCKET_H_ */
