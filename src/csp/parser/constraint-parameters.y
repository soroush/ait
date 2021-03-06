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


%class-name		         ConstraintParametersParser
%baseclass-header        constraint-parameters_parserbase.h
%class-header            constraint-parameters_parser.h
%implementation-header   constraint-parameters_parser.ih
%parsefun-source         constraint-parameters_parse.cpp
%scanner                 constraint-parameters_scanner.h
%scanner-token-function  d_scanner.lex()

%union{
	int i;
	std::string* s;
}

%token INT VAR

%%

parameters:	iv parameters |
			iv ;
iv:			int | var ;
var:		VAR {
					Variable* v = this->problem->variable(d_scanner.matched());
				    this->parameters.emplace_back(v);
				} ;
int:		INT {
					int v = atoi(d_scanner.matched().c_str());
					this->parameters.emplace_back(v);
				} ;