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

#ifndef DOMAIN_PARSER_H_
#define DOMAIN_PARSER_H_

#include "domain_parserbase.h"
#include "domain_scanner.hpp"
#include <sstream>
#include <vector>

#undef DomainParser
class DomainParser: public DomainParserBase {

public:
    DomainParser(const std::string& input, std::vector<int>& values);
    int parse();

private:
    std::istringstream str;
    DomainLexer d_scanner;
    std::vector<int>& values;

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

inline DomainParser::DomainParser(const std::string& input,
        std::vector<int>& values_) :
        str(input), d_scanner(str, std::cout), values(values_) {
}

inline void DomainParser::error(char const *msg) {
    std::cerr << msg << '\n';
}

inline int DomainParser::lex() {
    return d_scanner.lex();
}

inline void DomainParser::print() {
    print__();
}

inline void DomainParser::exceptionHandler__(std::exception const &exc) {
    // TODO
    throw;
}
#endif // DOMAIN_PARSER_H_
