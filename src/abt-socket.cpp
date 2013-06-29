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

#include "abt-socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

using namespace AIT;
using namespace zmq;

Socket::Socket(context_t &context_, int type_) :
		socket_t(context_, type_) {
}

Socket::Socket(zmq::socket_t&& rhs) :socket_t(std::move(rhs)) {
}

Socket::~Socket() {
}

size_t Socket::sendMessage(
		const protocols::csp::abt::P_CommunicationProtocol packet) {
	size_t length = packet.ByteSize();
	message_t message(length);
	packet.SerializeToArray(message.data(), length);
	return socket_t::send(message);
}

size_t Socket::sendMessage(const protocols::csp::abt::P_Message packet) {
	size_t length = packet.ByteSize();
	message_t message(length);
	packet.SerializeToArray(message.data(), length);
	return socket_t::send(message);
}

size_t AIT::Socket::recvMessage(
		protocols::csp::abt::P_CommunicationProtocol& packet) {
	message_t message;
	bool returnValue = recv(&message);
	packet.ParseFromArray(message.data(), message.size());
	return returnValue;
}

size_t AIT::Socket::recvMessage(protocols::csp::abt::P_Message& packet) {
	message_t message;
	bool returnValue = recv(&message);
	packet.ParseFromArray(message.data(), message.size());
	return returnValue;
}

size_t Socket::sendMessage(const protocols::csp::aabt::P_Message message) {
	// TODO implement
}

size_t Socket::recvMessage(protocols::csp::aabt::P_Message& message) {
	// TODO implement
}

std::string AIT::Socket::getIP() {
	std::string address = "";
	FILE * fp = popen("ifconfig", "r");
	if (fp) {
		char *p = NULL, *e;
		size_t n;
		while ((getline(&p, &n, fp) > 0) && p) {
			if (p = strstr(p, "inet addr:")) {
				p += 10;
				if (e = strchr(p, ' ')) {
					*e = '\0';
					return std::string(p);
					address = std::string(p);

				}
			}
		}
	}
	pclose(fp);
	return address;
}

