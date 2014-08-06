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

#ifndef PARAMETERS_PARSER_H_
#define PARAMETERS_PARSER_H_

#include "parameters_parserbase.h"
#include "parameters_scanner.hpp"
#include "../csp-problem.hpp"
#include <sstream>
#include <vector>
#include <map>

#undef ParametersParser
class ParametersParser: public ParametersParserBase {

public:
    ParametersParser(const std::string& input, AIT::CSP::Predicate* predicate);
    int parse();

private:
    std::istringstream str;
    ParametersLexer d_scanner;
    AIT::CSP::Predicate* predicate;

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

inline ParametersParser::ParametersParser(const std::string& input,
        AIT::CSP::Predicate* predicate_) :
        str(input), d_scanner(str, std::cout), predicate(predicate_) {
}

inline void ParametersParser::error(char const *msg) {
    std::cerr << msg << '\n';
}

inline int ParametersParser::lex() {
    return d_scanner.lex();
}

inline void ParametersParser::print() {
    print__();
}

inline void ParametersParser::exceptionHandler__(std::exception const &exc) {
    // TODO: re-implement to handle exceptions thrown by actions
    throw;
}

#endif // PARAMETERS_PARSER_H_
