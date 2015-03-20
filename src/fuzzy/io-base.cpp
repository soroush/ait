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

#include "io-base.hpp"
#include <algorithm>
using namespace std;
using namespace AIT::FUZZY;

IOBase::IOBase(const string& name) :
        m_name { name } {
}

void IOBase::addFunction(const MembershipFunction* const function) {
    this->m_functions.push_back(function);
}

void IOBase::removeFunction(const MembershipFunction* const function) {
    this->m_functions.erase(
            remove(this->m_functions.begin(),
                this->m_functions.end(),
                function),
            this->m_functions.end());
}

IOBase::~IOBase() {
}

const MembershipFunction* IOBase::function(
        const std::string& name) {
    auto result = find_if(
            this->m_functions.begin(),
            this->m_functions.end(),
            [name](const MembershipFunction* x)->bool{return x->name()==name;}
            );
    if(result != this->m_functions.end()){
        return *result;
    }
}
