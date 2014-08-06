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

#include "csp-solver.hpp"
#include "parser/xinstance-parser.hpp"
#include "parser/xpresentation-parser.hpp"
#include "parser/xproblem-type-parser.hpp"
#include "parser/xproblem-format-parser.hpp"
#include "parser/xdomains-parser.hpp"
#include "parser/xdomain-parser.hpp"
#include "parser/xvariables-parser.hpp"
#include "parser/xvariable-parser.hpp"
#include "parser/xpredicates-parser.hpp"
#include "parser/xpredicate-parser.hpp"
#include "parser/xexpression-parser.hpp"
#include "parser/xconstraints-parser.hpp"
#include "parser/xconstraint-parser.hpp"
#include "parser/xrelations-parser.hpp"
#include "parser/xrelation-parser.hpp"

using namespace AIT;
using namespace AIT::CSP;
using namespace std;

CSP_Solver::CSP_Solver(): instance(unique_ptr<CSP_Problem>(new CSP_Problem)) {
}

CSP_Solver::~CSP_Solver() {
}

void CSP_Solver::parseFromFile(const string& path) {
	// Initialize parsers
	// Common parsers:
    xml_schema::string_pimpl stringParser;
    xml_schema::positive_integer_pimpl positiveIntegerParser;
//
	XInstanceParser instanceParser{this->instance.get()};
	XPresentationParser presentation;
	XProblemTypeParser problemType;
	XProblemFormatParser problemFormat;
	presentation.parsers(stringParser,positiveIntegerParser,stringParser,stringParser,stringParser,problemType,problemFormat);
	XDomainsParser domains;
	XDomainParser domain;
	domain.parsers(stringParser,positiveIntegerParser);
	domains.parsers(domain,positiveIntegerParser);
	XVariablesParser variables;
	XVariableParser variable{this->instance.get()};
	variable.parsers(stringParser,stringParser);
	variables.parsers(variable,positiveIntegerParser);
	XPredicatesParser predicates;
	XPredicateParser predicate;
	XExpressionParser expression;
	expression.parsers(stringParser,stringParser,stringParser,stringParser);
	predicate.parsers(stringParser,expression,stringParser);
	predicates.parsers(predicate,positiveIntegerParser);
	XConstraintsParser constraints;
	XConstraintParser constraint{this->instance.get()};
	constraint.parsers(stringParser,stringParser,stringParser,stringParser,positiveIntegerParser);
	constraints.parsers(constraint,positiveIntegerParser);
	XRelationsParser relations;
	XRelationParser relation;
	relations.relation_parser(relation);
	instanceParser.parsers(presentation,domains,variables,relations,predicates,constraints);
//	// Prepare document and parse
	xml_schema::document doc(instanceParser,"instance");
	doc.parse(path);
}

const CSP_Problem* CSP_Solver::getInstance() const {
    return this->instance.get();
}
