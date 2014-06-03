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

#include "xproblem-type-parser.h"

using namespace AIT;
using namespace CSP;
using namespace std;

XProblemTypeParser::XProblemTypeParser() {
}

XProblemTypeParser::~XProblemTypeParser() {
}

CSP_Problem::Presentation::Type XProblemTypeParser::post_problemType() {
	string type = this->post_string();
	if (type == "CSP")
		return CSP_Problem::Presentation::Type::CSP;
	if (type == "QCSP")
		return CSP_Problem::Presentation::Type::QCSP;
	if (type == "WCSP")
		return CSP_Problem::Presentation::Type::WCSP;
	_ERROR("Unknown problem type: `%s'", type.c_str());
	return CSP_Problem::Presentation::Type::Unknown;
}

