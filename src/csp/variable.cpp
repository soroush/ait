/*
 AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.

 Copyright (c) 2013 Soroush Rabiei <soroush-r@users.sf.net>,
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

#include "variable.h"
#include <utility>

using namespace AIT::CSP;
using namespace std;

Variable::Variable() :
		m_domain(&Domain::empty), m_name(""), m_value(0), set(false) {
}

Variable::Variable(Domain* domain, const string& name, const int& value_) :
		m_domain(domain), m_name(name), m_value(value_), set(false) {
}

Variable::Variable(Variable&& other) :
		m_domain(other.m_domain), m_name(move(other.m_name)), m_value(
				move(other.m_value)), set(move(other.set)) {
}

Variable& Variable::operator =(Variable&& other) {
	m_domain = std::move(other.m_domain);
	m_name = std::move(other.m_name);
	m_value = std::move(other.m_value);
	return *this;
}

Variable::~Variable() {
}

int* Variable::value() {
	return &(this->m_value);
}

const Domain* Variable::getDomain() const {
	return this->m_domain;
}

void Variable::setDomain(Domain* domain) {
    this->m_domain = domain;
}

const string& Variable::getName() const {
	return this->m_name;
}

void Variable::setName(const string& name) {
    this->m_name = name;
}

int Variable::getValue() const {
	return this->m_value;
}

void Variable::setValue(int value) {
    this->m_value = value;
	this->set = true;
}

bool Variable::isSet() const {
    return this->set;
}

void Variable::unset() {
    this->set = false;
}
