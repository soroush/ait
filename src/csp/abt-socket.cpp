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

#include "abt-socket.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>

using namespace AIT;
using namespace zmq;
using namespace std;

Socket::Socket(context_t &context_, int type_) :
    m_socket{context_, type_} {
}

Socket::Socket(zmq::socket_t&& rhs) :
    m_socket{std::move(rhs)} {
}

Socket::~Socket() {
    this->m_socket.close();
}

size_t Socket::sendMessage(
        const protocols::csp::abt::P_CommunicationProtocol packet) {
    size_t length = packet.ByteSize();
    message_t message(length);
    packet.SerializeToArray(message.data(), length);
    return this->m_socket.send(message);
}

size_t Socket::sendMessage(const protocols::csp::abt::P_Message packet) {
    size_t length = packet.ByteSize();
    message_t message(length);
    packet.SerializeToArray(message.data(), length);
    return this->m_socket.send(message);
}

size_t Socket::recvMessage(
        protocols::csp::abt::P_CommunicationProtocol& packet) {
    message_t message;
    bool returnValue = this->m_socket.recv(&message);
    packet.ParseFromArray(message.data(), message.size());
    return returnValue;
}

size_t Socket::recvMessage(protocols::csp::abt::P_Message& packet) {
    message_t message;
    bool returnValue = this->m_socket.recv(&message);
    packet.ParseFromArray(message.data(), message.size());
    return returnValue;
}

socket_t& Socket::get_zmq_socket(){
    return this->m_socket;
}
