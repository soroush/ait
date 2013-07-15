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

#ifndef EXP_FUNC_H_
#define EXP_FUNC_H_

#include "expression.h"

namespace AIT {
namespace CSP {

class exp_func: public Expression {
public:
	exp_func();
	virtual ~exp_func();
};

class exp_func_neg: public exp_func {
public:
//	exp_func_neg();
//	~exp_func_neg();
	void evaluate(std::stack<int>&);
};

class exp_func_abs: public exp_func {
public:
//	exp_func_abs();
//	~exp_func_abs();
	void evaluate(std::stack<int>&);
};

class exp_func_add: public exp_func {
public:
//	exp_func_add();
//	~exp_func_add();
	void evaluate(std::stack<int>&);
};

class exp_func_sub: public exp_func {
public:
//	exp_func_sub();
//	~exp_func_sub();
	void evaluate(std::stack<int>&);
};

class exp_func_mul: public exp_func {
public:
//	exp_func_mul();
//	~exp_func_mul();
	void evaluate(std::stack<int>&);
};

class exp_func_div: public exp_func {
public:
//	exp_func_div();
//	~exp_func_div();
	void evaluate(std::stack<int>&);
};

class exp_func_mod: public exp_func {
public:
//	exp_func_mod();
//	~exp_func_mod();
	void evaluate(std::stack<int>&);
};

class exp_func_pow: public exp_func {
public:
//	exp_func_pow();
//	~exp_func_pow();
	void evaluate(std::stack<int>&);
};

class exp_func_min: public exp_func {
public:
//	exp_func_min();
//	~exp_func_min();
	void evaluate(std::stack<int>&);
};

class exp_func_max: public exp_func {
public:
//	exp_func_max();
//	~exp_func_max();
	void evaluate(std::stack<int>&);
};

class exp_func_eq: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_neq: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_ge: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_gt: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_le: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_lt: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_not: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_and: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_or: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_xor: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

class exp_func_iff: public exp_func {
public:
	void evaluate(std::stack<int>&);
};

} /* namespace CSP */
} /* namespace AIT */

#endif /* EXP_FUNC_H_ */
