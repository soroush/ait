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

#include <algorithm>
#include <utility>

using namespace AIT::CSP;
using namespace std;

Predicate::Predicate(const string& parameters, const string& input,
		const Type& type) {
	switch (type) {
	case Type::Functional: {
		ParametersParser pParser(parameters, this->parameters, this->names);
		pParser.parse();
		FunctionalParser fParser(input, this->postfix, this->parameters,
				this->names);
		fParser.parse();
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

Predicate::~Predicate() {
	for (auto& e : this->postfix)
		delete e;
}

bool Predicate::evaluate(const vector<int>& inputs) {
	this->parameters = inputs;
	while (!this->evaluation.empty())
		this->evaluation.pop();
	for (auto& e : this->postfix) {
		e->evaluate(this->evaluation);
	}
	// TODO: Check for validity
	return (this->evaluation.top() == 1);
}

bool Predicate::evaluate(vector<int> && inputs) {
	this->parameters = inputs;
	while (!this->evaluation.empty())
		this->evaluation.pop();
	for (auto& e : this->postfix) {
		e->evaluate(this->evaluation);
	}
	// TODO: Check for validity
	return (this->evaluation.top() == 1);
}

Predicate::Predicate(Predicate&& other) :
		parameters(std::move(other.parameters)), names(std::move(other.names)), postfix(
				std::move(other.postfix)), evaluation(
				std::move(other.evaluation)) {
}

Predicate& Predicate::operator =(Predicate&& other) {
	this->parameters = std::move(other.parameters);
	this->names = std::move(other.names);
	this->postfix = std::move(other.postfix);
	this->evaluation = std::move(other.evaluation);
	return *this;
}

bool Predicate::evaluate(const vector<int*>& inputs) {
	this->parameters.clear();
	this->parameters.reserve(inputs.size());
	for (const auto& x : inputs) {
		this->parameters.push_back(*x);
	}
	while (!this->evaluation.empty())
		this->evaluation.pop();
	for (auto& e : this->postfix) {
		e->evaluate(this->evaluation);
	}
	// TODO: Check for validity
	return (this->evaluation.top() == 1);
}
