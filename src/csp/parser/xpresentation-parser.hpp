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

#ifndef XPRESENTATION_PARSER_H_
#define XPRESENTATION_PARSER_H_

#include "xcsp-pskel.hxx"
#include <memory>
#define BOOST_ALL_DYN_LINK
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>

namespace AIT {
namespace CSP {

class XPresentationParser: public presentation_t_pskel {
public:
    XPresentationParser();
    ~XPresentationParser();
    void name(const std::string&);
    void maxConstraintArity(unsigned long long);
    void minViolatedConstraints(const std::string&);
    void nbSolutions(const std::string&);
    void solution(const std::string&);
    void type(const AIT::CSP::CSP_Problem::Presentation::Type&);
    void format(const AIT::CSP::CSP_Problem::Presentation::Format&);
    std::unique_ptr<CSP_Problem::Presentation> post_presentation_t();
private:
    std::string m_name;
    unsigned int m_maxConstraintArity;
    std::pair<CSP_Problem::Presentation::NumberType, unsigned int> m_minViolatedConstraints;
    std::pair<CSP_Problem::Presentation::NumberType, unsigned int> m_nbSolutions;
    std::string m_solution;
    CSP_Problem::Presentation::Type m_type;
    CSP_Problem::Presentation::Format m_format;
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

};

} /* namespace CSP */
} /* namespace AIT */
#endif /* XPRESENTATION_PARSER_H_ */
