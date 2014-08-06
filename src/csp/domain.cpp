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

#include "domain.hpp"
#include "parser/domain_parser.hpp"
#include <algorithm>
#include <utility>
#include <iterator>

using namespace AIT::CSP;
using namespace std;

Domain::Domain(const size_t& nbValues, const string& content,
		const string& name) :
		m_name(name) {
	this->m_values.reserve(nbValues);
	DomainParser parser{content, this->m_values};
	parser.parse();
}

Domain::Domain(Domain&& other) :
		m_values(move(other.m_values)), m_name(move(other.m_name)) 
                {
}

Domain& Domain::operator =(Domain&& other) {
	this->m_values = move(other.m_values);
	this->m_name = move(other.m_name);
	return *this;
}

Domain::~Domain() {
}

const string& Domain::getName() const {
	return m_name;
}

void Domain::setName(const string& name) {
	m_name = name;
}

const vector<int>& Domain::getValues() const {
	return m_values;
}

void Domain::setValues(const vector<int>& values) {
	m_values = values;
	std::unique(m_values.begin(),m_values.end());
}
