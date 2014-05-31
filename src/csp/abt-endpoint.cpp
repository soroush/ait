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

#include "abt-solver.h"
#include "abt-socket.h"

using namespace AIT;
//FIXME remove incorrect using after moving Socket to a new namespace
using namespace AIT::CSP;
using namespace AIT::protocols::csp;
using namespace AIT::protocols::csp::abt;
using namespace zmq;

std::list<ABT_Solver::EndPoint*> ABT_Solver::EndPoint::m_everyBody;

ABT_Solver::EndPoint::EndPoint(const protocols::csp::abt::P_EndPoint& ep,
        zmq::context_t& context) :
        socket_(new Socket(context, ZMQ_PUSH)) {
    this->CopyFrom(ep);
    m_everyBody.push_back(this);
}

ABT_Solver::EndPoint::~EndPoint() {
    m_everyBody.remove(this);
}

Socket* ABT_Solver::EndPoint::socket() const {
    return (this->socket_);
}

ABT_Solver::EndPoint* ABT_Solver::EndPoint::getByName(const std::string& name) {
    for (const auto& ep : m_everyBody) {
        if (ep->name() == name) {
            return ep;
        }
    }
    return nullptr;
}

ABT_Solver::EndPoint* ABT_Solver::EndPoint::getByPriority(
        const size_t& priority) {
    for (const auto& ep : m_everyBody) {
        if (ep->priority() == priority) {
            return ep;
        }
    }
    return nullptr;
}
