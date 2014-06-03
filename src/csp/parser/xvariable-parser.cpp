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

#include "xvariable-parser.h"
#include "../csp-problem.h"

using namespace AIT::CSP;
using namespace std;

XVariableParser::XVariableParser(CSP_Problem* const instance) :
        m_instance(instance) {
}

XVariableParser::~XVariableParser() {
}

void XVariableParser::name(const string& name) {
    this->m_name = name;
}

void XVariableParser::domain(const string& domainName) {
    this->m_domain = domainName;
}

std::unique_ptr<Variable> XVariableParser::post_variable_t() {
    Domain* ref = m_instance->domain(this->m_domain);
    return unique_ptr<Variable>(new Variable(ref,this->m_name));
}
