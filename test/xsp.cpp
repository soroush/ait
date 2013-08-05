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

#include "../src/csp/parser/xcsp-pskel.hxx"
#include "../src/csp/parser/xdomains-parser.h"
#include "../src/csp/parser/xdomain-parser.h"
#include "../src/csp/domain.h"
#include "../src/global.h"

using namespace AIT::CSP;
using namespace std;

int main(int argc, char *argv[]) {
	_INFO("Testing XCSP 2.1 Parser");
	instance_t_pskel instanceParser;
	std::forward_list<Domain> domains;
	XDomainsParser domainsParser(domains);
	XDomainParser domainParser;
	domainsParser.domain_parser(domainParser);
	instanceParser.domains_parser(domainsParser);

	return 0;
}
