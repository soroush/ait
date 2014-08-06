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

#ifndef CONSTRAINT_SCANNER_H_
#define CONSTRAINT_SCANNER_H_

#include "constraint-scope_scannerbase.h"

class ConstraintScopeLexer: public ConstraintScopeLexerBase {
public:
    explicit ConstraintScopeLexer(std::istream &in = std::cin,
            std::ostream &out = std::cout);
    ConstraintScopeLexer(std::string const &infile, std::string const &outfile);
    int lex();

private:
    int lex__();
    int executeAction__(size_t ruleNr);
    void print();
    void preCode();
    void postCode(PostEnum__ type);
};

inline ConstraintScopeLexer::ConstraintScopeLexer(std::istream &in,
        std::ostream &out) :
        ConstraintScopeLexerBase(in, out) {
}

inline ConstraintScopeLexer::ConstraintScopeLexer(std::string const &infile,
        std::string const &outfile) :
        ConstraintScopeLexerBase(infile, outfile) {
}

inline int ConstraintScopeLexer::lex() {
    return lex__();
}

inline void ConstraintScopeLexer::preCode() {
    // optionally replace by your own code
}

inline void ConstraintScopeLexer::postCode(PostEnum__ type) {
    // optionally replace by your own code
}

inline void ConstraintScopeLexer::print() {
    print__();
}

#endif // CONSTRAINT_SCANNER_H_

