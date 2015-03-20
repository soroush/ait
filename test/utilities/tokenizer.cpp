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

#include "tokenizer.hpp"
using namespace std;
using namespace AIT::TEST;

const string Tokenizer::DELIMITERS(" \t\n\r");

Tokenizer::Tokenizer(const string& s) :
        m_string(s), m_offset(0), m_delimiters(DELIMITERS) {
}

Tokenizer::Tokenizer(const string& s, const string& delimiters) :
        m_string(s), m_offset(0), m_delimiters(delimiters) {
}

bool Tokenizer::nextToken() {
    return nextToken(m_delimiters);
}

bool Tokenizer::nextToken(const string& delimiters) {
    size_t i = m_string.find_first_not_of(delimiters, m_offset);
    if (string::npos == i) {
        m_offset = m_string.length();
        return false;
    }

    size_t j = m_string.find_first_of(delimiters, i);
    if (string::npos == j) {
        m_token = m_string.substr(i);
        m_offset = m_string.length();
        return true;
    }

    m_token = m_string.substr(i, j - i);
    m_offset = j;
    return true;
}

const string Tokenizer::getToken() {
    return this->m_token;
}

void Tokenizer::reset() {
    this->m_delimiters = string { };
    this->m_offset = 0;
    this->m_string = string { };
}

const string Tokenizer::getNextToken() {
    if(this->nextToken()){
        return this->getToken();
    }
    else{
        return string{};
    }
}

void Tokenizer::setString(const string& input) {
    this->reset();
    this->m_string = input;
}
