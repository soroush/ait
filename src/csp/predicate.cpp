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

#include "predicate.h"
#include "parser/predicate-f_parser.h"
#include "parser/parameters_parser.h"
#include "parser/expression.h"
#include "csp-problem.h"

#include <algorithm>
#include <utility>

#include <iostream>

using namespace AIT::CSP;
using namespace std;

Predicate::Predicate(const string& name, const string& parameters_,
        const string& input, const Type& type) {
    this->m_name = name;
    switch (type) {
    case Type::Functional: {
        ParametersParser pParser(parameters_, this);
        pParser.parse();
        FunctionalParser fParser(input, this);
        fParser.parse();
        //TODO: Implement parsers for other types of representation.
    }
        break;
    case Type::Infix:
        break;
    case Type::Postfix:
        break;
    case Type::MathML:
        break;
    default:
        break;
    }
}

Predicate::Predicate(Predicate&& other) :
        parameters(std::move(other.parameters)), postfix(
                std::move(other.postfix)), evaluation(
                std::move(other.evaluation)) {
    this->m_name = std::move(other.m_name);
}

Predicate::~Predicate() {
}

Predicate & Predicate::operator =(Predicate && other) {
    this->parameters = std::move(other.parameters);
    this->postfix = std::move(other.postfix);
    this->evaluation = std::move(other.evaluation);
    this->m_name = std::move(other.m_name);
    return *this;
}
bool Predicate::evaluate(const vector<int>& inputs) {
//    this->parameters = inputs;
//    // Clear the stack:
//    while (!this->evaluation.empty())
//        this->evaluation.pop();
//
//    for (auto& e : this->postfix) {
//        e->evaluate(this->evaluation);
//    }
//    // TODO: Check for validity
//    return (this->evaluation.top() == 1);
}

bool Predicate::evaluate(vector<int>&& inputs) {
    size_t i = 0;
    for (const auto& v : inputs) {
        this->parameters[i++].value = v;
    }
    while (!this->evaluation.empty())
        this->evaluation.pop();
    for (auto& e : this->postfix) {
        e->evaluate(this->evaluation);
    }
    return (this->evaluation.top() == 1);
}

bool Predicate::evaluate(const vector<int*>& inputs) {
//    this->parameters.clear();
//    this->parameters.reserve(inputs.size());
//    for (const auto& x : inputs) {
//        this->parameters.push_back(*x);
//    }
//
//    while (!this->evaluation.empty())
//        this->evaluation.pop();
//
//    for (auto& e : this->postfix) {
//        e->evaluate(this->evaluation);
//    }
//    // TODO: Check for validity
//    return (this->evaluation.top() == 1);
}

void Predicate::addParameter(const std::string& name, const int& value) {
    this->parameters.emplace_back(Reference { value, name });
}

const std::vector<Predicate::Reference>& Predicate::getParameters() const {
    return this->parameters;
}

void Predicate::addPostfixExpression(const Expression::Token& type,
        const std::string& name) {
    this->postfix.push_back( unique_ptr<Expression>{new Expression(type, name, this)});
}

int Predicate::parameter(const std::string& name) const {
    for (const auto& p : this->parameters) {
        if (p.name == name) {
            return p.value;
        }
    }
// TODO: Throw exception
    return 0;
}

