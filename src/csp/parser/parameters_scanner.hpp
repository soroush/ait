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

#ifndef PARAMETERS_SCANNER_H_
#define PARAMETERS_SCANNER_H_

#include "parameters_scannerbase.h"

class ParametersLexer: public ParametersLexerBase {
public:
    explicit ParametersLexer(std::istream &in = std::cin, std::ostream &out =
            std::cout);
    ParametersLexer(std::string const &infile, std::string const &outfile);
    int lex();

private:
    int lex__();
    int executeAction__(size_t ruleNr);
    void print();
    void preCode();
    void postCode(PostEnum__ type);
};

inline ParametersLexer::ParametersLexer(std::istream &in, std::ostream &out) :
        ParametersLexerBase(in, out) {
}

inline ParametersLexer::ParametersLexer(std::string const &infile,
        std::string const &outfile) :
        ParametersLexerBase(infile, outfile) {
}

inline int ParametersLexer::lex() {
    return lex__();
}

inline void ParametersLexer::preCode() {
    // optionally replace by your own code
}

inline void ParametersLexer::postCode(PostEnum__ type) {
    // optionally replace by your own code
}

inline void ParametersLexer::print() {
    print__();
}

#endif // PARAMETERS_SCANNER_H_

