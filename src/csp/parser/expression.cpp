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

#include "expression.h"
#include "../csp-problem.h"
#include "../predicate.h"
#include <cmath>

using namespace AIT::CSP;
using namespace std;

Expression::Expression(const Token& type, const std::string& name,
        const std::string& predicate, const CSP_Problem& instance) :
        m_instance(instance), m_type(type), m_predicate(predicate), m_refIndex(-1) {
    if (type == Token::REF) {
        this->m_ref = name;
//        for(size_t i=0; i<m_predicate->getParameters().size(); ++i){
//            if(m_predicate->getParameters()[i].name == name){
//                cout << "FOUND THE REF: " << name << " " << i << endl;
//                this->m_refIndex = i;
//                break;
//            }
//        }
    } else if(type == Token::CONST){
        this->m_const = atoi(name.c_str());
    }
}

Expression::~Expression() {
}

void Expression::evaluate(stack<int>& s) {
    int x, y;
    switch (this->m_type) {
    case Token::REF:{
        Predicate* ref = static_cast<Predicate*>(this->m_instance.relation(this->m_predicate));
        s.push(ref->parameter(this->m_ref));
        break;
    }
    case Token::ABS:
        x = s.top();
        s.pop();
        s.push(abs(x));
        cout << "ABS: " << x << endl;
        break;
    case Token::ADD:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(x+y);
    case Token::SUB:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        cout << "SUB: " << x << " " << y << endl;
        s.push(y-x);
        break;
    case Token::MUL:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y*x);
        break;
    case Token::MOD:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y%x);
        break;
    case Token::POW:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(pow(y,x));
        break;
    case Token::MIN:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y<x?y:x);
    case Token::MAX:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y>x?y:x);
        break;
    case Token::EQ:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y==x?1:0);
        break;
    case Token::NEQ:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        cout << "NEQ: " << x << " " << y << endl;
        s.push(y==x?0:1);
        break;
    case Token::GE:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y>=x?1:0);
        break;
    case Token::GT:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y>x?1:0);
        break;
    case Token::LE:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y<=x?1:0);
        break;
    case Token::LT:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y<x?1:0);
        break;
    case Token::AND:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        cout << "AND: " << x << " " << y << endl;
         s.push(y==1 && x==1);
        break;
    case Token::OR:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(y==1 || x==1);
        break;
    case Token::XOR:
        x = s.top();
        s.pop();
        y = s.top();
        s.pop();
        s.push(!(y==1 && x==1));
        break;
    case Token::IFF:
        //TODO: Implement iff
        break;
    case Token::NOT:
        x = s.top();
        s.pop();
        s.push(x==1?0:1);
        break;
    case Token::CONST:
        s.push(this->m_const);
        break;
    default:
        break;
    }
}

Expression& Expression::operator =(const Expression& other) {
    this->m_const = other.m_const;
    this->m_type = other.m_type;
    this->m_refIndex = other.m_refIndex;
    this->m_predicate = other.m_predicate;
    this->m_ref = other.m_ref;
    return *this;
}
