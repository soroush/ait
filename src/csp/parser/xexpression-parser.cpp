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

#include "xexpression-parser.h"

using namespace AIT::CSP;
using namespace std;

XExpressionParser::XExpressionParser() {
}

XExpressionParser::~XExpressionParser() {
}

void XExpressionParser::functional(const string& input) {
	this->m_expression = input;
	m_type = Predicate::Type::Functional;
}

void XExpressionParser::math(const string& input) {
	this->m_expression = input;
	m_type = Predicate::Type::MathML;
}

void XExpressionParser::postfix(const string& input) {
	this->m_expression = input;
	m_type = Predicate::Type::Postfix;
}

void XExpressionParser::infix(const string& input) {
	this->m_expression = input;
	m_type = Predicate::Type::Infix;
}

pair<Predicate::Type, string> XExpressionParser::post_expression_t() {
	pair<Predicate::Type, string> result(this->m_type,this->m_expression);
	return result;
}
