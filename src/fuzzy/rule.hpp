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

#ifndef RULE_HPP_
#define RULE_HPP_

#include <string>
#include <vector>
#include "operator.hpp"
#include "membership-function.hpp"
#include "implicated-mf.hpp"

namespace AIT {
namespace FUZZY {

class Rule {
public:
    Rule(const std::string& name);
    void addInput(const MembershipFunction* const function);
    void addOperation(const Operator* const operator_);
    ImplicatedMembershipFunction* evaluate(const std::vector<float>& inputs) const;
private:
    std::string m_name;
    std::vector<const MembershipFunction*> m_functions;
    std::vector<const Operator*> m_operators;
};

} /* namespace FUZZY */
} /* namespace AIT */

#endif /* RULE_HPP_ */
