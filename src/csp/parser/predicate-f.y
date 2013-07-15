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

%union{
double		d;
int		i;
bool		b;
std::string*	s;
}

//%token <d> 	DOUBLE
%token <i> 	INTEGER
%token <s> 	ID

%token LP RP COMMA NEG ABS ADD SUB MUL MOD POW MIN MAX 
%token EQ NEQ GE GT LE LT NOT AND OR XOR IFF

%type <i> int
//%type <d> double
%type <s> id

%%

functional: int_expr;

int_expr:	NEG LP int_expr RP { this->postfix.push_back(new exp_func_neg()); } | 
		ABS LP int_expr RP { this->postfix.push_back(new exp_func_abs());} |
		ADD LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_add()); } |
		SUB LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_sub()); } |
		MUL LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_mul()); } |
		MOD LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_mod()); } |
		POW LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_pow()); } |
		MIN LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_min()); } |
		MAX LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_max()); } | 

		EQ LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_eq()); } | 
		NEQ LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_neq()); } | 
		GE LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_ge()); } | 
		GT LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_gt()); } | 
		LE LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_le()); } | 
		LT LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_lt()); } | 

		NOT LP int_expr RP { this->postfix.push_back(new exp_func_not()); } | 
		AND LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_and()); } | 
		OR LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_or()); } | 
		XOR LP int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_xor()); } | 
		IFF LP int_expr COMMA int_expr COMMA int_expr RP { this->postfix.push_back(new exp_func_iff()); } |
		
		id {
		     this->postfix.push_back(new exp_ref(this->variables,this->names.at(*$1))); 
		   } | 
		int { this->postfix.push_back(new exp_num($1)); };
int:	INTEGER {$$ = atoi(d_scanner.matched().c_str());} ;
//double:	DOUBLE {$$ = atof(d_scanner.matched().c_str());} |
//        int {$$ = static_cast<double>($1);} ;
id:	ID	{ $$ = new std::string(d_scanner.matched().c_str()); };