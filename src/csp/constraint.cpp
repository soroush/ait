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

#include "constraint.hpp"
#include "parser/constraint-parameters_parser.hpp"
#include "parser/constraint-scope_parser.hpp"
#include "csp-problem.hpp"

#include <utility>

using namespace AIT::CSP;
using namespace std;

Constraint::Constraint(const std::string& name, const size_t& arity,
        const std::string& scope_, const std::string& reference_,
        const std::string& parameters_, CSP_Problem* const parent) :
        m_name(name) {
     ConstraintScopeParser scopeParser(scope_, parent, this->scope);
     scopeParser.parse();
    ConstraintParametersParser parameterParser(parameters_, parent,
            this->parameters);
    parameterParser.parse();
    this->reference = parent->relation(reference_);
}

Constraint::Constraint(Constraint&& other) :
        scope(std::move(other.scope)), parameters(std::move(other.parameters)), reference(
                std::move(other.reference)), m_name(std::move(other.m_name)) {
}

Constraint& Constraint::operator =(Constraint&& other) {
    this->scope = std::move(other.scope);
    this->parameters = std::move(other.parameters);
    this->reference = std::move(other.reference);
    this->m_name = std::move(other.m_name);
    return *this;
}

Constraint::~Constraint() {
}

std::string Constraint::getName() const {
    return this->m_name;
}

void Constraint::setName(const std::string& name) {
    this->m_name = name;
}

bool Constraint::satisfies() {
    /** FIXME: This is a BUG. a Predicate do not take a vector of pointers to
     * integers, but a vector of integers. Currently I'll make a new vector and
     * deep copy everything into this one, then pass it to the predicate. But
     * this is not a good solution. Remember to change definition of Predicate.
     */
    vector<int> values;
    for (const auto& value : this->parameters)
        values.push_back(value.getValue());
    return reference->evaluate(std::move(values));
}

const vector<Variable*>& Constraint::Scope() const {
    return this->scope;
}

// Value class

Constraint::Value::Value(Variable* variable) :
        variable(variable), value(0), currentType(Type::Variable) {

}

Constraint::Value::Value(const int& value_) :
        variable(nullptr), value(value_), currentType(Type::Constant) {
}

int Constraint::Value::getValue() const {
    switch (this->currentType) {
    case Type::Variable:
        return this->variable->getValue();
    case Type::Constant:
        return this->value;
    default:
        break;
    }
}

Constraint::Value & Constraint::Value::operator =(
        const Constraint::Value& other) {
    this->currentType = other.currentType;
    this->value = other.value;
    this->variable = other.variable;
    return *this;
}
