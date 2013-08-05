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


%class-name		         ParametersParser
%baseclass-header        parameters_parserbase.h
%class-header            parameters_parser.h
%implementation-header   parameters_parser.ih
%parsefun-source         parameters_parse.cpp
%scanner                 parameters_scanner.h
%scanner-token-function  d_scanner.lex()

%union{
	std::string*	s;
}

%token INT ID

//%type <s> id

%%

parameters:	type id parameters |
			type id ;
type:		int;
int:		INT ;
id:			ID	{ 
					this->parameters.push_back(0); 
					this->names[d_scanner.matched().c_str()]=this->parameters.size()-1; 
				};
