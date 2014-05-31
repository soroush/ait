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

#include "xconstraint-parser.h"

using namespace AIT::CSP;
using namespace std;

XConstraintParser::XConstraintParser(CSP_Problem& instance) :
		m_instance(instance) {
}

XConstraintParser::~XConstraintParser() {
}

void XConstraintParser::parameters(const string& parameters) {
    cout << "constraint parameters: " << parameters << endl;
	this->m_parameters = parameters;
}

void XConstraintParser::name(const string& name) {
    cout << "constraint name: " << name << endl;
	this->m_name = name;
}

void XConstraintParser::scope(const string& scope) {
    cout << "constraint scope: " << scope << endl;
	this->m_scope = scope;
}

void XConstraintParser::reference(const string& reference) {
	this->m_reference = reference;
}

void XConstraintParser::arity(unsigned long long arity) {
	this->m_arity = arity;
}

Constraint XConstraintParser::post_constraint_t() {
	return Constraint(this->m_name, this->m_arity, this->m_scope,
			this->m_reference, this->m_parameters, &(this->m_instance));
	//FIXME: Replace pointer with reference
}
