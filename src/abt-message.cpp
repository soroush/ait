/*
 * abt-message.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: soroush
 */

#include "abt-message.h"

using namespace AIT;
using namespace protocols::csp;
using namespace protocols::csp::abt;

ABT_Message::ABT_Message() {
}

ABT_Message::ABT_Message(const ABT_Message& other) :
		type(other.type), sender(other.sender), assignment(other.assignment), nogood(
				other.nogood) {
}

ABT_Message::~ABT_Message() {
	nogood.items.clear();
}

ABT_Message& ABT_Message::operator =(const ABT_Message& other) {
	type = other.type;
	sender = other.sender;
	assignment = other.assignment;
	nogood = other.nogood;
	return *this;
}

ABT_Message::operator protocols::csp::abt::P_Message() const {
	P_Message m;
	m.set_type(type);
	m.set_sender(sender);
	switch (type) {
	case P_MessageType::T_ADDLINK:
		break;
	case P_MessageType::T_NOGOOD:
		for (const auto& a : nogood.items) {
			P_Assignment* aa = m.mutable_nogood()->add_assignments();
			aa->set_id(a.id);
			aa->set_value(a.value);
		}
		break;
	case P_MessageType::T_OK:
		m.mutable_assignment()->set_id(assignment.id);
		m.mutable_assignment()->set_value(assignment.value);
		break;
	case P_MessageType::T_STOP:
		break;
	}
	return m;
}

void ABT_Message::readFromProtocol(const P_Message& message) {
	this->type = message.type();
	this->sender = message.sender();
	switch (message.type()) {
	case P_MessageType::T_ADDLINK:
		break;
	case P_MessageType::T_NOGOOD:
		for (const auto& a : message.nogood().assignments()) {
			this->nogood.items.insert(Assignment(a.id(), a.value()));
		}
		break;
	case P_MessageType::T_OK:
		this->assignment.id = message.assignment().id();
		this->assignment.value = message.assignment().value();
		break;
	case P_MessageType::T_STOP:
		break;
	}
}

