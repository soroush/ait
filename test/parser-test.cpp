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

#include "../src/csp/predicate.h"
#include "../src/global.h"

using namespace AIT::CSP;
using namespace std;

int main(int argc, char *argv[]) {
	_INFO("Testing Parser");
	string parameters = "int A int B int X int Y int Z";
	string functional = "and(ne(neg(A),B),le(add(X,Y),mul(sub(X,Y),abs(Z))))";
	Predicate p(parameters,functional,Predicate::Type::Functional);
	cout << p.evaluate({{1,2,3,4,5}}) << endl;
	cout << p.evaluate({{10,20,3,4,5}}) << endl;
	cout << p.evaluate({{10,20,1,1,-1}}) << endl;
	cout << p.evaluate({{10,20,1,-2,-8}}) << endl;
	cout << p.evaluate({{-8,-2,1,20,10}}) << endl;
//	Predicate q("int A int B","gt(A,B)",Predicate::Type::Functional);
//	cout << q.evaluate({{1,2}}) << endl;
//	cout << q.evaluate({{2,1}}) << endl;
	return 0;
}
