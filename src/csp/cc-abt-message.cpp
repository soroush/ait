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

using namespace AIT::CSP;
using namespace AIT::protocols::csp;
using namespace AIT::protocols::csp::ccabt;

CC_ABT_Solver::Message::Message() {
}

CC_ABT_Solver::Message::Message(const Message& other) :
        type(other.type), sender(other.sender), assignment(other.assignment), nogood(
                other.nogood), sequence(other.sequence), approved(
                other.approved) {
    this->tv = other.tv;
}

CC_ABT_Solver::Message::~Message() {
    nogood.items.clear();
}

CC_ABT_Solver::Message& CC_ABT_Solver::Message::operator =(
        const Message& other) {
    type = other.type;
    sender = other.sender;
    assignment = other.assignment;
    nogood = other.nogood;
    sequence = other.sequence;
    approved = other.approved;
    tv = other.tv;
    return *this;
}

CC_ABT_Solver::Message::operator P_Message() const {
    P_Message m;
    m.set_type(type);
    m.set_sender(sender);
    switch (type) {
    case P_MessageType::T_ADDLINK:
        break;
    case P_MessageType::T_NOGOOD:
        for (const auto& a : nogood.items) {
            P_Assignment* aa = m.mutable_nogood()->add_assignments();
            aa->set_priority(a.id);
            aa->set_value(a.value);
        }
        m.mutable_tv()->Clear();
        for (const auto& s : this->tv) {
            m.mutable_tv()->Add(s);
        }
        break;
    case P_MessageType::T_OK:
        m.mutable_assignment()->set_priority(assignment.id);
        m.mutable_assignment()->set_value(assignment.value);
        m.set_sequence(this->sequence);
        m.mutable_tv()->Clear();
        for (const auto& s : this->tv) {
            m.mutable_tv()->Add(s);
        }
        break;
    case P_MessageType::T_STOP:
        break;
    case P_MessageType::T_OK_MONITOR:
        m.mutable_assignment()->set_priority(assignment.id);
        m.mutable_assignment()->set_value(assignment.value);
        m.set_sequence(this->sequence);
        m.set_approved(this->approved);
        break;
    }
    return m;
}

void CC_ABT_Solver::Message::readFromProtocol(const P_Message& message) {
    this->type = message.type();
    this->sender = message.sender();
    this->tv.clear();
    switch (message.type()) {
    case P_MessageType::T_ADDLINK:
        break;
    case P_MessageType::T_NOGOOD:
        for (const auto& a : message.nogood().assignments()) {
            this->nogood.items.insert(Assignment(a.priority(), a.value()));
        }
        this->tv.reserve(message.tv_size());
        for (int i=0 ; i<  message.tv_size(); ++i) {
            this->tv.push_back(message.tv(i));
        }
        break;
    case P_MessageType::T_OK:
        this->assignment.id = message.assignment().priority();
        this->assignment.value = message.assignment().value();
        this->sequence = message.sequence();
        this->tv.reserve(message.tv_size());
        for (int i=0 ; i<  message.tv_size(); ++i) {
            this->tv.push_back(message.tv(i));
        }
        break;
    case P_MessageType::T_STOP:
        break;
    case P_MessageType::T_OK_MONITOR:
        this->assignment.id = message.assignment().priority();
        this->assignment.value = message.assignment().value();
        this->sequence = message.sequence();
        this->approved = message.approved();
        break;
    }
}

