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

#ifndef PREDICATE_H_
#define PREDICATE_H_

#include <vector>
#include <stack>
#include <map>
#include <string>
#include "parser/expression.h"
#include "../global.h"


namespace AIT {
namespace CSP {

class LIBRARY_API Predicate {
public:
	enum class Type {Functional, Infix, Postfix, MathML};
	Predicate(const std::string& parameters, const std::string& input, const Type&);
	~Predicate();
	bool evaluate(const std::vector<int>&);
	bool evaluate(std::vector<int>&&);
private:
	std::vector<int> parameters;
	std::map<std::string, size_t> names;
	std::vector<Expression*> postfix;
	std::stack<int> evaluation;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* PREDICATE_H_ */
