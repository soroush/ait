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

#include "xdomain-parser.hpp"

using namespace AIT::CSP;
using namespace std;

XDomainParser::XDomainParser() :
		m_name(""), m_nbValues(0), m_content("") {
}

XDomainParser::~XDomainParser() {
}

void XDomainParser::nbValues(unsigned long long nb) {
	this->m_nbValues = static_cast<size_t>(nb);
}

void XDomainParser::name(const ::string& name) {
	this->m_name = name;
}

unique_ptr<Domain> XDomainParser::post_domain_t() {
	this->m_content = this->post_string();
	return unique_ptr<Domain>(new Domain(this->m_nbValues, this->m_content, this->m_name));
}
