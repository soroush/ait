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
using namespace AIT::CSP;
using namespace std;

CSP_Problem::~CSP_Problem() {
}

Variable* CSP_Problem::variable(const string& name) const {
//    for(auto v : this->variables){
//        if(v->getName() == name){
//            return v;
//        }
//    }
    auto item = find_if(this->m_variables.begin(), this->m_variables.end(),
            [&](Variable* v) {return v->getName() == name;});
    if (item != this->m_variables.end()) {
        return *item;
    } else {
        // TODO: throw an exception
    }
}

Variable* CSP_Problem::variable(const size_t& index) const {
    return this->m_variables[index];
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
    return 0;
}

Domain* CSP_Problem::domain(const string& name) const {
    auto item = find_if(this->m_domains.begin(), this->m_domains.end(),
            [&](Domain* d) {return d->getName() == name;});
    if (item != this->m_domains.end()) {
        return *item;
    } else {
        // TODO: throw an exception
    }
    return nullptr;
}

RelationBase* CSP_Problem::relation(const string& name) const {
        cout << "RELATIONS............" << endl;
    for(const auto& r:this->m_relations){
        cout << "RELATION: " << r->getName() << endl;
    }
    auto item = find_if(this->m_relations.begin(), this->m_relations.end(),
                [&](RelationBase* r) {return r->getName() == name;});
        if (item != this->m_relations.end()) {
            return *item;
        } else {
            // TODO: throw an exception
        }
        cout << "RETURNING NULL!" << endl;
        return nullptr;
}

void CSP_Problem::addDomain(Domain&& d) {
    this->m_domains.push_back(new Domain(std::move(d)));
}

void CSP_Problem::addVariable(Variable&& v) {
    this->m_variables.push_back(new Variable(std::move(v)));
}

void CSP_Problem::addConstraint(Constraint&& c) {
    this->m_constraints.push_back(new Constraint(std::move(c)));
}

void CSP_Problem::addPredicate(Predicate&& p) {
    this->m_relations.push_back(new Predicate(std::move(p)));
}

const std::vector<Domain*>& CSP_Problem::domains() const {
    return this->m_domains;
}

const vector<RelationBase*>& CSP_Problem::relationBases() const {
    return this->m_relations;
}

const vector<Constraint*>& CSP_Problem::constraints() const {
    return this->m_constraints;
}

const std::vector<Variable*>& CSP_Problem::variables() const {
    return this->m_variables;
}

void CSP_Problem::setType(const Type& type) {
    this->m_type = type;
}

void CSP_Problem::setName(const std::string& name) {
    this->m_name = name;
}

void CSP_Problem::setMaxConstraintArity(const unsigned int& max) {
    this->m_maxConstraintArity = max;
}

void CSP_Problem::setMinViolatedConstraints(
        std::pair<NumberType, unsigned int> number) {
    this->m_minViolatedConstraints = number;
}

void CSP_Problem::setNbSolutions(std::pair<NumberType, unsigned int> number) {
    this->m_nbSolutions = number;
}

void CSP_Problem::setSolution(const std::string& solution) {
    this->m_solution = solution;
}

void CSP_Problem::setFormat(const Format& format) {
    this->m_format = format;
}
