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

#ifndef XINSTANCE_PARSER_H_
#define XINSTANCE_PARSER_H_

#include "xcsp-pskel.hxx"

namespace AIT {
namespace CSP {

class CSP_Problem;

class XInstanceParser: public instance_t_pskel {
public:
    XInstanceParser(CSP_Problem* const);
    ~XInstanceParser();
    void presentation(std::unique_ptr<CSP_Problem::Presentation>);
    void domains(std::vector<std::unique_ptr<Domain>>&&);
    void variables(std::vector<std::unique_ptr<Variable>>&&);
    void predicates(std::vector<std::unique_ptr<Predicate>>&&);
    void constraints(std::vector<std::unique_ptr<Constraint>>&&);
    std::unique_ptr<CSP_Problem> post_instance_t(){}
private:
    CSP_Problem* const m_problem;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* XINSTANCE_PARSER_H_ */
