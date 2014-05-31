///*
// AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.
//
// Copyright (c) 2012,2013 Soroush Rabiei <soroush-r@users.sf.net>,
// Roya Ghasemzadeh <ghasemzadeh.roya1@gmail.com>
//
// AIT is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// See the file COPYING included with this distribution for more
// information.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
// */
//
//#include "exp_func.h"
//#include <cstdlib>
//#include <cmath>
//#include <iostream>
//
//using namespace AIT::CSP;
//using namespace std;
//
//exp_func::exp_func() {
//}
//
//exp_func::~exp_func() {
//}
//
//void exp_func_neg::evaluate(stack<int>& s) {
//	int v = s.top();
//	s.pop();
//	s.emplace(-1 * v);
//}
//
//void exp_func_abs::evaluate(stack<int>& s) {
//	int v = s.top();
//	s.pop();
//	s.emplace(abs(v));
//	cout << v <<" abs " << endl;
//}
//
//void exp_func_add::evaluate(stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 + v1);
//}
//
//void exp_func_sub::evaluate(stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 - v1);
//	cout << v1 <<" sub " << v2  << endl;
//}
//
//void exp_func_mul::evaluate(stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 * v1);
//}
//
//void exp_func_div::evaluate(stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 / v1);
//}
//
//void exp_func_mod::evaluate(stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 % v1);
//}
//
//void exp_func_pow::evaluate(stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(pow(v2, v1));
//}
//
//void exp_func_min::evaluate(stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(min(v2, v1));
//}
//
//void exp_func_max::evaluate(stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(max(v2, v1));
//}
//
//void exp_func_eq::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 == v1 ? 1 : 0);
//}
//
//void exp_func_neq::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 != v1 ? 1 : 0);
//	cout << v1 <<" neq " << v2  << endl;
//}
//
//void exp_func_ge::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 >= v1 ? 1 : 0);
//}
//
//void exp_func_gt::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 > v1 ? 1 : 0);
//}
//
//void exp_func_le::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 <= v1 ? 1 : 0);
//}
//
//void exp_func_lt::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace(v2 < v1 ? 1 : 0);
//}
//
//void exp_func_not::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	s.emplace(v1 == 0 ? 1 : 0);
//	// TODO: Check for non-1 boolean literals
//	// TODO: Add forgotten "true", "false" to CNF
//}
//
//void exp_func_and::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace((v2 == 1 and v1 == 1) ? 1 : 0);
//	cout << v1 <<" and " << v2  << endl;
//}
//
//void exp_func_or::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace((v2 == 1 or v1 == 1) ? 1 : 0);
//}
//
//void exp_func_xor::evaluate(std::stack<int>& s) {
//	int v1 = s.top();
//	s.pop();
//	int v2 = s.top();
//	s.pop();
//	s.emplace((v2 == v1) ? 0 : 1);
//	// TODO: Restrict v2 and v1 to be `0' or `1'
//}
//
//void exp_func_iff::evaluate(std::stack<int>& s) {
//	//TODO: Implement
//}
