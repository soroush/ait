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

#include <algorithm>
#include "aabt-message.h"

using namespace AIT::CSP;
using namespace AIT::protocols::csp::aabt;
using namespace std;

AABT_Message::AABT_Message() {
}

AABT_Message::AABT_Message(const AABT_Message& other) :
		msg_type(other.msg_type), sender_id(other.sender_id), vi(other.vi), ei(
				other.ei), oi(other.oi), tvi(other.tvi), ng(other.ng) {
}

AABT_Message::AABT_Message(P_Message& p_message) {
	readFromProtocol(p_message);
}

AABT_Message::~AABT_Message() {
}

AABT_Message& AABT_Message::operator =(const AABT_Message& other) {
	msg_type = other.msg_type;
	sender_id = other.sender_id;
	vi = other.vi;
	ei = other.ei;
	ng = other.ng;
	oi = other.oi;
	tvi = other.tvi;
	return *this;
}

void AABT_Message::readFromProtocol(const P_Message& p_message) {
	switch (p_message.type()) {
	case P_MessageType::T_OK:
		this->msg_type = AABT_MessageType::OK;
		this->vi.readFromProtocol(p_message.vi());
		this->ei.readFromProtocol(p_message.ei());
		this->oi.clear();
		copy(p_message.oi().begin(), p_message.oi().end(), this->oi.begin());
		this->tvi.clear();
		copy(p_message.tvi().begin(), p_message.tvi().end(), this->tvi.begin());
		break;

	case P_MessageType::T_NOGOOD:
		this->msg_type = AABT_MessageType::NOGOOD;
		this->oi.clear();
		copy(p_message.oi().begin(), p_message.oi().end(), this->oi.begin());
		this->tvi.clear();
		copy(p_message.tvi().begin(), p_message.tvi().end(), this->tvi.begin());
		this->ng.readFromProtocol(p_message.nogood());
		break;

	case P_MessageType::T_ORDER:
		this->msg_type = AABT_MessageType::ORDER;
		this->oi.clear();
		copy(p_message.oi().begin(), p_message.oi().end(), this->oi.begin());
		this->tvi.clear();
		copy(p_message.tvi().begin(), p_message.tvi().end(), this->tvi.begin());
		break;
	case P_MessageType::T_STOP:
		this->msg_type = AABT_MessageType::STOP;
		break;
	}
	this->sender_id = p_message.sender();
}

AABT_Message::operator P_Message() const {
	P_Message m;
	switch (this->msg_type) {
		case AABT_MessageType::OK:
			m.set_type(P_MessageType::T_OK);
			m.mutable_vi()->CopyFrom(this->vi);
			m.mutable_ei()->CopyFrom(this->ei);
			for(const auto& oii : this->oi){
				m.mutable_oi()->Add(oii);
			}
			for(const auto& tvii : this->tvi){
				m.mutable_tvi()->Add(tvii);
			}
			break;
		case AABT_MessageType::ORDER:
			m.set_type(P_MessageType::T_ORDER);
			for(const auto& oii : this->oi){
				m.mutable_oi()->Add(oii);
			}
			for(const auto& tvii : this->tvi){
				m.mutable_tvi()->Add(tvii);
			}
			break;
		case AABT_MessageType::NOGOOD:
			m.set_type(P_MessageType::T_NOGOOD);
			for(const auto& oii : this->oi){
				m.mutable_oi()->Add(oii);
			}
			for(const auto& tvii : this->tvi){
				m.mutable_tvi()->Add(tvii);
			}
			m.mutable_nogood()->CopyFrom(this->ng);
			break;
		case AABT_MessageType::STOP:
			m.set_type(P_MessageType::T_STOP);
			break;
	}
	m.set_sender(this->sender_id);
	return m;
}
