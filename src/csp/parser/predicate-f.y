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


%class-name		         FunctionalParser
%baseclass-header        "predicate-f_parserbase.h"
%class-header            "predicate-f_parser.h"
%implementation-header   "predicate-f_parser.ih"
%parsefun-source         "predicate-f_parse.cpp"
%scanner                 "predicate-f_scanner.h"
%scanner-token-function  d_scanner.lex()

%token  INTEGER
%token  ID

%token LP RP COMMA NEG ABS ADD SUB MUL MOD POW MIN MAX 
%token EQ NEQ GE GT LE LT NOT AND OR XOR IFF

%%

functional: int_expr;

int_expr:	NEG LP int_expr RP { 
                                    
                                    this->predicate->addPostfixExpression(Expression::Token::NEG); 
                               } | 
		ABS LP int_expr RP { 
		                          
		                          this->predicate->addPostfixExpression(Expression::Token::ABS);} |
		ADD LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::ADD); } |
		SUB LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::SUB); } |
		MUL LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::MUL); } |
		MOD LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::MOD); } |
		POW LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::POW); } |
		MIN LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::MIN); } |
		MAX LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::MAX); } | 

		EQ LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::EQ); } | 
		NEQ LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::NEQ); } | 
		GE LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::GE); } | 
		GT LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::GT); } | 
		LE LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::LE); } | 
		LT LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::LT); } | 

		NOT LP int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::NOT); } | 
		AND LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::AND); } | 
		OR LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::OR); } | 
		XOR LP int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::XOR); } | 
		IFF LP int_expr COMMA int_expr COMMA int_expr RP {  this->predicate->addPostfixExpression(Expression::Token::IFF); } |
		
		ID {
		       
		      this->predicate->addPostfixExpression(Expression::Token::REF, d_scanner.matched()); 
		   } | 
		INTEGER {  this->predicate->addPostfixExpression(Expression::Token::CONST, d_scanner.matched()); };
