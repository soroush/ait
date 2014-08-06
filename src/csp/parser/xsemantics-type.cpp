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

#include "xsemantics-type.hpp"
#include "../relation-base.hpp"
#include <string>
#define BOOST_ALL_DYN_LINK
#define DSO
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/format.hpp>

using namespace AIT::CSP;
using namespace std;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;

XSemanticsType::XSemanticsType() {
}

XSemanticsType::~XSemanticsType() {
}

RelationBase::Semantics XSemanticsType::post_semanticsType() {
	string sem = this->post_string();
	if (sem == "supports") {
		return RelationBase::Semantics::Supports;
	} else if (sem == "conflicts") {
		return RelationBase::Semantics::Conflicts;
	}
    BOOST_LOG_SEV(lg, error) <<
                boost::format("Invalid semantics type: `%1%'.") % sem;
}

