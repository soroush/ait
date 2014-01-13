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

#include "csp-problem.h"
using namespace AIT::CSP;
using namespace std;

Variable* CSP_Problem::variable(const string& name) const {
}

Domain* CSP_Problem::domain(const string& name) const {
}

RelationBase* CSP_Problem::relation(const string& name) const {
}

void CSP_Problem::addDomain(Domain&& d) {
//	if (domainNames.find(d.getName()) == domainNames.end()) {
//		domains.push_front(move(d));
//		domainNames[d.getName()] = &;
//	}
}

void CSP_Problem::addVariable(Variable&& v) {
}

const forward_list<Constraint>& CSP_Problem::Constraints() const {
	return this->constraints;
}

