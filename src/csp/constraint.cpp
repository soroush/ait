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

#include "constraint.h"
#include "parser/constraint-parameters_parser.h"
#include "csp-problem.h"

#include <utility>

using namespace AIT::CSP;
using namespace std;

Constraint::Constraint(const std::string& name, const size_t& arity,
		const std::string& scope_, const std::string& reference_,
		const std::string& parameters_, CSP_Problem* parent) {
	//FIXME: Uncomment following lines after resolving pointers problem
    // ConstraintParametersParser scopeParser(scope_, parent, this->scope);
	// scopeParser.parse();
	ConstraintParametersParser parameterParser(parameters_, parent,
			this->parameters);
	parameterParser.parse();
	this->reference = parent->relation(reference_);
}

Constraint::Constraint(Constraint&& other) :
		scope(std::move(other.scope)), parameters(std::move(other.parameters)), reference(
				std::move(other.reference)) {
}

Constraint& Constraint::operator =(Constraint&& other) {
	this->scope = std::move(other.scope);
	this->parameters = std::move(other.parameters);
	this->reference = std::move(other.reference);
	return *this;
}

Constraint::~Constraint() {
}

bool Constraint::satisfies() {
    /** FIXME: This is a BUG. a Predicate do not take a vector of pointers to
    * integers, but a vector of integers. Currently I'll make a new vector and
    * deep copy everything into this one, then pass it to the predicate. But
    * this is not a good solution. Remember to change definition of Predicate.
    */
    vector<int> values(this->parameters.size());
    for(const auto& pointer : this->parameters)
       values.push_back(*pointer);
	return reference->evaluate(std::move(values));
}

const vector<Variable*>& Constraint::Scope() const {
	return this->scope;
}


