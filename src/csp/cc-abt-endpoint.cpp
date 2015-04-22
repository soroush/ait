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

#include "cc-abt-solver.hpp"
#include "abt-socket.hpp"
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace AIT;
//FIXME remove incorrect using after moving Socket to a new namespace
using namespace AIT::CSP;
using namespace AIT::protocols::csp;
using namespace AIT::protocols::csp::ccabt;
using namespace zmq;
using namespace std;


CC_ABT_Solver::EndPoint::EndPoint(const P_EndPoint& ep,
        zmq::context_t& context) :
        socket_(new Socket(context, ZMQ_PUSH)) {
    this->CopyFrom(ep);
    int linger = 0;
    this->socket_->get_zmq_socket().setsockopt(ZMQ_LINGER,&linger, sizeof(linger));
}

CC_ABT_Solver::EndPoint::~EndPoint() {
}

Socket* CC_ABT_Solver::EndPoint::socket() const {
    return (this->socket_);
}
