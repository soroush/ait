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

#include "xpresentation-parser.h"
#include <stdexcept>
#include <vector>
#include <sstream>
#include "../../global.h"

using namespace AIT;
using namespace CSP;
using namespace std;

XPresentationParser::XPresentationParser(CSP_Problem& _instance) :
		instance(_instance) {
}

XPresentationParser::~XPresentationParser() {
}

void XPresentationParser::name(const string& name) {
	instance.name(name);
}

void XPresentationParser::minViolatedConstraints(const string& min) {
	std::pair<CSP_Problem::NumberType, unsigned int> number;
	if (min == "?") {
		number.first = CSP_Problem::NumberType::Unknown;
		instance.minViolatedConstraints(number);
		return;
	} else {
		string buf;
		stringstream ss(min);
		vector<string> tokens;
		while (ss >> buf)
			tokens.push_back(buf);
		// Check sanity
		if (tokens.size() == 3) {
			if (tokens[0] == "at" && tokens[1] == "least") {
				number.second = stoi(tokens[2]);
				number.first = CSP_Problem::NumberType::AtLeast;
				instance.minViolatedConstraints(number);
				return;
			}
		} else if (tokens.size() == 1)
			try {
				number.second = stoi(tokens[0]);
				number.first = CSP_Problem::NumberType::Exactly;
				instance.minViolatedConstraints(number);
			} catch (std::invalid_argument &e) {
				_ERROR("%s", e.what());
			}
	}
	_ERROR("Invalid value for `minViolatedConstraints' : %s", min.c_str());
	number.first = CSP_Problem::NumberType::Unknown;
	instance.minViolatedConstraints(number);
}

void XPresentationParser::nbSolutions(const string& nb) {
	std::pair<CSP_Problem::NumberType, unsigned int> number;
	if (nb == "?") {
		number.first = CSP_Problem::NumberType::Unknown;
		instance.nbSolutions(number);
		return;
	} else {
		string buf;
		stringstream ss(nb);
		vector<string> tokens;
		while (ss >> buf)
			tokens.push_back(buf);
		// Check sanity
		if (tokens.size() == 3) {
			if (tokens[0] == "at" && tokens[1] == "least") {
				number.second = stoi(tokens[2]);
				number.first = CSP_Problem::NumberType::AtLeast;
				instance.nbSolutions(number);
				return;
			}
		} else if (tokens.size() == 1)
			try {
				number.second = stoi(tokens[0]);
				number.first = CSP_Problem::NumberType::Exactly;
				instance.nbSolutions(number);
			} catch (std::invalid_argument &e) {
				_ERROR("%s", e.what());
			}
	}
	_ERROR("Invalid value for `nbSolutions' : %s", nb.c_str());
	number.first = CSP_Problem::NumberType::Unknown;
	instance.nbSolutions(number);
}

void XPresentationParser::solution(const string& _solution) {
	instance.solution(_solution);
}

void XPresentationParser::type(const CSP_Problem::Type& _type) {
	instance.type(_type);
}

void XPresentationParser::format(const string& format) {
	if (format == "XCSP 2.1") {
		instance.format(CSP_Problem::Format::XCSP_21);
	} else {
		_ERROR("Unsupported format of CSP problem: `%s'", format.c_str());
	}
}

void XPresentationParser::maxConstraintArity(unsigned long long arity) {
	instance.maxConstraintArity(static_cast<unsigned int>(arity));
}
