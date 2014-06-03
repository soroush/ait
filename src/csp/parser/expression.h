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

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <stack>
#include <string>
#include <stddef.h>

namespace AIT {
namespace CSP {

class Predicate;

class Expression {
public:
    enum class Token{
        // Parameters
        REF,
        // Unary Operators
        NEG,
        ABS,
        // Binary Operators
        ADD,
        SUB,
        MUL,
        MOD,
        POW,
        MIN,
        MAX,
        // Logical Binary Operators
        EQ,
        NEQ,
        GE,
        GT,
        LE,
        LT,
        AND,
        OR,
        XOR,
        IFF,
        // Logical Unary Operators
        NOT,
        // Constants
        CONST
    };
	Expression(const Token& type, const std::string& name,
	        AIT::CSP::Predicate* const predicate);
	virtual ~Expression();
//	Expression& operator=(const Expression& other);
	virtual void evaluate(std::stack<int>&);
private:
	Token m_type;
	size_t m_refIndex;
	int m_const;
	AIT::CSP::Predicate* const m_predicate;
};

} /* namespace CSP */
} /* namespace AIT */
#endif /* EXPRESSION_H_ */
