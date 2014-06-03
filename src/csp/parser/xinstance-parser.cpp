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

#include "xinstance-parser.h"
#include <algorithm>

using namespace AIT::CSP;
using namespace std;

XInstanceParser::XInstanceParser(CSP_Problem* const problem):
    m_problem(problem) {
}

XInstanceParser::~XInstanceParser() {
}

void XInstanceParser::presentation(
        unique_ptr<CSP_Problem::Presentation> presentation) {
    m_problem->m_presentation = std::move(presentation);
}

void XInstanceParser::domains(vector<unique_ptr<Domain>>&& domains) {
    m_problem->m_domains = std::move(domains);
}

void XInstanceParser::variables(vector<unique_ptr<Variable>>&& variables) {
    m_problem->m_variables = std::move(variables);
}

void XInstanceParser::predicates(
        vector<unique_ptr<Predicate>>&& predicates) {
    //move(predicates.begin(),predicates.end(),this->m_problem->m_relations.begin());
    for(size_t i=0; i<predicates.size(); ++i){
        m_problem->m_relations.push_back(std::move(predicates[i]));
    }
}

void AIT::CSP::XInstanceParser::constraints(
        std::vector<std::unique_ptr<Constraint>>&& constraints) {
    this->m_problem->m_constraints = std::move(constraints);
}
