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

#include "xpredicates-parser.hpp"

using namespace AIT::CSP;
using namespace std;

XPredicatesParser::XPredicatesParser() {
}

XPredicatesParser::~XPredicatesParser() {
}

void XPredicatesParser::predicate(std::unique_ptr<Predicate> predicate) {
    this->m_predicates.push_back(std::move(predicate));
}

void XPredicatesParser::nbPredicates(unsigned long long size) {
    this->m_predicates.reserve(size);
}

vector<std::unique_ptr<Predicate>>&& XPredicatesParser::post_predicates_t() {
    return std::move(this->m_predicates);
}
