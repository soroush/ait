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
 
%class-name                PredicateFunctionalLexer
%baseclass-header          "predicate-f_scannerbase.h"
%class-header              "predicate-f_scanner.h"
%implementation-header     "predicate-f_scanner.ih"
%lex-source                "predicate-f_scanner.cpp"
 
 %%
"("                                 return FunctionalParser::LP;
")"                                 return FunctionalParser::RP;
","                                 return FunctionalParser::COMMA;
"neg"                               return FunctionalParser::NEG;
"abs"                               return FunctionalParser::ABS;
"add"                               return FunctionalParser::ADD;
"sub"                               return FunctionalParser::SUB;
"mul"                               return FunctionalParser::MUL;
"mod"                               return FunctionalParser::MOD;
"pow"                               return FunctionalParser::POW;
"min"                               return FunctionalParser::MIN;
"max"                               return FunctionalParser::MAX;
"eq"                                return FunctionalParser::EQ;
"ne"                                return FunctionalParser::NEQ;
"ge"                                return FunctionalParser::GE;
"gt"                                return FunctionalParser::GT;
"le"                                return FunctionalParser::LE;
"lt"                                return FunctionalParser::LT;
"not"                               return FunctionalParser::NOT;
"and"                               return FunctionalParser::AND;
"or"                                return FunctionalParser::OR;
"xor"                               return FunctionalParser::XOR;
"iff"                               return FunctionalParser::IFF;
"-"?[[:digit:]]+                    return FunctionalParser::INTEGER;
[[:alpha:]]+[[:alnum:]]+            return FunctionalParser::ID;
[ \t\n]+                            ;
