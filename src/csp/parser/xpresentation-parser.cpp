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

#include "xpresentation-parser.hpp"
#include <stdexcept>
#include <vector>
#include <sstream>
#include "../../global.hpp"

using namespace AIT;
using namespace CSP;
using namespace std;

XPresentationParser::XPresentationParser() {
}

XPresentationParser::~XPresentationParser() {
}

void XPresentationParser::name(const string& name) {
    this->m_name = name;
}

void XPresentationParser::minViolatedConstraints(const string& min) {
    std::pair<CSP_Problem::Presentation::NumberType, unsigned int> number;
    if (min == "?") {
        number.first = CSP_Problem::Presentation::NumberType::Unknown;
        this->m_minViolatedConstraints = number;
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
                number.first = CSP_Problem::Presentation::NumberType::AtLeast;
                this->m_minViolatedConstraints = number;
                return;
            }
        } else if (tokens.size() == 1) {
            try {
                number.second = stoi(tokens[0]);
                number.first = CSP_Problem::Presentation::NumberType::Exactly;
                this->m_minViolatedConstraints = number;
            } catch (std::invalid_argument &e) {
                _ERROR("%s", e.what());
            }
            return;
        }
    }
    _ERROR("Invalid value for `minViolatedConstraints' : %s", min.c_str());
    number.first = CSP_Problem::Presentation::NumberType::Unknown;
    this->m_minViolatedConstraints = number;
}

void XPresentationParser::nbSolutions(const string& nb) {
    std::pair<CSP_Problem::Presentation::NumberType, unsigned int> number;
    if (nb == "?") {
        number.first = CSP_Problem::Presentation::NumberType::Unknown;
        this->m_nbSolutions = number;
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
                number.first = CSP_Problem::Presentation::NumberType::AtLeast;
                this->m_nbSolutions = number;
                return;
            }
        } else if (tokens.size() == 1) {
            try {
                number.second = stoi(tokens[0]);
                number.first = CSP_Problem::Presentation::NumberType::Exactly;
                this->m_nbSolutions = number;
            } catch (std::invalid_argument &e) {
                _ERROR("%s", e.what());
            }
            return;
        }
    }
    _ERROR("Invalid value for `nbSolutions' : %s", nb.c_str());
    number.first = CSP_Problem::Presentation::NumberType::Unknown;
    this->m_nbSolutions = number;
}

void XPresentationParser::solution(const string& solution) {
    this->m_solution = solution;
}

void XPresentationParser::type(const CSP_Problem::Presentation::Type& type) {
    this->m_type = type;
}

void XPresentationParser::format(
        const AIT::CSP::CSP_Problem::Presentation::Format& format) {
    this->m_format = format;
}

void XPresentationParser::maxConstraintArity(unsigned long long arity) {
    this->m_maxConstraintArity = static_cast<unsigned int>(arity);
}

unique_ptr<CSP_Problem::Presentation> XPresentationParser::post_presentation_t() {
    return unique_ptr<CSP_Problem::Presentation>(
            new CSP_Problem::Presentation(this->m_name,
                    this->m_maxConstraintArity, this->m_minViolatedConstraints,
                    this->m_nbSolutions, this->m_type, this->m_format));
}
