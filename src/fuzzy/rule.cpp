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

#include "rule.hpp"

using namespace AIT::FUZZY;
using namespace std;

Rule::Rule(const string& name) :
        m_name { name } {
}

void Rule::addInput(const MembershipFunction* const function) {
    this->m_functions.push_back(function);
}

void Rule::addOperation(const Operator* const operator_) {
    this->m_operators.push_back(operator_);
}

ImplicatedMembershipFunction* Rule::evaluate(
        const vector<float>& inputs) const {

}
