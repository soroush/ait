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

#ifndef CONSTRAINT_SCOPE_PARSER_H_
#define CONSTRAINT_SCOPE_PARSER_H_

#include <vector>
#include <sstream>
#include "constraint-scope_parserbase.h"
#include "constraint-scope_scanner.hpp"
#include "../variable.hpp"
#include "../constraint.hpp"
#include "../csp-problem.hpp"

#undef ConstraintScopeParser
class ConstraintScopeParser: public ConstraintScopeParserBase {
public:
    ConstraintScopeParser(const std::string& input,
            AIT::CSP::CSP_Problem* const problem,
            std::vector<AIT::CSP::Variable*>& output);
    int parse();

private:
    std::istringstream str;
    ConstraintScopeLexer d_scanner;
    AIT::CSP::CSP_Problem* const problem;
    std::vector<AIT::CSP::Variable*>& scope;

    void error(char const *msg);
    int lex();
    void print();
    void executeAction(int ruleNr);
    void errorRecovery();
    int lookup(bool recovery);
    void nextToken();
    void print__();
    void exceptionHandler__(std::exception const &exc);
};

inline ConstraintScopeParser::ConstraintScopeParser(const std::string& input,
        AIT::CSP::CSP_Problem* const problem_,
        std::vector<AIT::CSP::Variable*>& output) :
        str(input), d_scanner(str, std::cout), problem(problem_), scope(output) {
}

inline void ConstraintScopeParser::error(char const *msg) {
    std::cerr << msg << '\n';
}

inline int ConstraintScopeParser::lex() {
    return d_scanner.lex();
}

inline void ConstraintScopeParser::print() {
    print__();
}

inline void ConstraintScopeParser::exceptionHandler__(
        std::exception const &exc) {
    throw;
}

#endif // CONSTRAINT_SCOPE_PARSER_H_
