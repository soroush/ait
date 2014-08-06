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

#include "xdomains-parser.hpp"
#include "xdomain-parser.hpp"
#include "../csp-problem.hpp"
#include "../domain.hpp"

#include <iostream>
#include <utility>

using namespace AIT::CSP;
using namespace std;

XDomainsParser::XDomainsParser(){
}

void XDomainsParser::domain(unique_ptr<Domain> domain) {
    this->m_domains.push_back(std::move(domain));
}

void XDomainsParser::nbDomains(unsigned long long number) {
    //this->m_domains.reserve(number);
}

vector<unique_ptr<Domain>>&& XDomainsParser::post_domains_t() {
    return std::move(this->m_domains);
}
