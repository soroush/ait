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

#ifndef PREDICATE_F_PARSER_H_
#define PREDICATE_F_PARSER_H_

#include "predicate-f_parserbase.h"
#include "predicate-f_scanner.hpp"
#include "expression.hpp"
#include "../predicate.hpp"
#include <sstream>
#include <vector>
#include <map>

#undef FunctionalParser
class FunctionalParser: public FunctionalParserBase {

public:
    FunctionalParser(const std::string& input, AIT::CSP::Predicate* predicate);
    ~FunctionalParser() {
    }
    int parse();

private:
    std::istringstream str;
    PredicateFunctionalLexer d_scanner;
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

using namespace std;
using namespace AIT::CSP;

inline FunctionalParser::FunctionalParser(const string& input,
        Predicate* predicate_) :
        str(input), d_scanner(str, std::cout), predicate(predicate_) {

}

inline void FunctionalParser::error(char const *msg) {
    std::cerr << msg << '\n';
}

inline int FunctionalParser::lex() {
    return d_scanner.lex();
}

inline void FunctionalParser::print() {
    print__();
}

inline void FunctionalParser::exceptionHandler__(std::exception const &exc) {
    // TODO: re-implement to handle exceptions thrown by actions
    throw;
}

#endif
