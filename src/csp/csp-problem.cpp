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
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <sstream>

using namespace AIT::CSP;
using namespace std;

CSP_Problem::~CSP_Problem() {
}

Variable* CSP_Problem::variable(const string& name) const {
    for (const auto& variable_ptr : this->m_variables) {
        if (name == variable_ptr->getName()) {
            return variable_ptr.get();
        }
    }
    stringstream ss;
    ss << "There is no variable in this problem with name: `" << name << "'"
            << endl;
    throw out_of_range { ss.str() };
    return nullptr;
}

Variable* CSP_Problem::variable(const size_t& index) const {
    return this->m_variables[index].get();
}

size_t CSP_Problem::variableIndex(const std::string& name) const {
    size_t order = 1;
    for (const auto& variable : this->m_variables) {
        if (variable->getName() == name) {
            return order;
        } else {
            ++order;
        }
    }
    throw std::out_of_range{"There is no variable with specified name"};
    return -1;
}

Domain* CSP_Problem::domain(const string& name) const {
    for (const auto& domain_ptr : this->m_domains) {
        if (name == domain_ptr->getName()) {
            return domain_ptr.get();
        }
    }
    stringstream ss;
    ss << "There is no domain in this problem with name: `" << name << "'"
            << endl;
    throw out_of_range { ss.str() };
    return nullptr;
}

RelationBase* CSP_Problem::relation(const string& name) const {
    for (const auto& relation_ptr : this->m_relations) {
        if (name == relation_ptr->getName()) {
            return relation_ptr.get();
        }
    }
    stringstream ss;
    ss << "There is no relation in this problem with name: `" << name << "'"
            << endl;
    throw out_of_range { ss.str() };
    return nullptr;
}

void CSP_Problem::addDomain(unique_ptr<Domain> d) {
    this->m_domains.push_back(std::move(d));
}

void CSP_Problem::addVariable(unique_ptr<Variable> v) {
    this->m_variables.push_back(std::move(v));
}

void CSP_Problem::addConstraint(std::unique_ptr<Constraint> c) {
    this->m_constraints.push_back(std::move(c));
}

void CSP_Problem::addPredicate(std::unique_ptr<Predicate> p) {
    this->m_relations.push_back(std::move(p));
}

const vector<unique_ptr<Domain>>& CSP_Problem::domains() const {
    return this->m_domains;
}

const vector<unique_ptr<RelationBase>>& CSP_Problem::relationBases() const {
    return this->m_relations;
}

const vector<unique_ptr<Constraint>>& CSP_Problem::constraints() const {
    return this->m_constraints;
}

const vector<unique_ptr<Variable>>& CSP_Problem::variables() const {
    return this->m_variables;
}

CSP_Problem::Presentation::Presentation(const string& name,
        const unsigned int& maxConstraint,
        const pair<NumberType, unsigned int>& minViolatedConstraints,
        const pair<NumberType, unsigned int>& nbSolutions, const Type& type,
        const Format& format) :
        m_name(name), m_maxConstraintArity(maxConstraint), m_minViolatedConstraints(
                minViolatedConstraints), m_nbSolutions(nbSolutions), m_type(
                type), m_format(format) {
}

CSP_Problem::Presentation::~Presentation() {
}
