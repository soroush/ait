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

#include <config.h>
#include "../src/fuzzy/io-base.hpp"
#include <iostream>
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
#define BOOST_ALL_DYN_LINK
#define DSO
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/format.hpp>
#endif

using namespace AIT::FUZZY;
using namespace std;
#ifdef HAVE_BOOST_LOG_CORE_CORE_HPP
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;
#endif

int main() {
//    src::severity_logger<severity_level> lg;
//    logging::add_common_attributes();
//    logging::register_simple_formatter_factory<logging::trivial::severity_level,
//            char>("Severity");
//    logging::add_file_log(keywords::auto_flush = true, keywords::format =
//            "[%TimeStamp%][%Severity%]: %Message%");
//
//    IOBase io { "Food" };
//    MembershipFunction f1 { "Good" };
//    MembershipFunction f2 { "Bad" };
//    MembershipFunction f3 { "Average" };
//    io.addFunction(&f1);
//    io.addFunction(&f2);
//    io.addFunction(&f3);
//    for (const auto& name : { "Good", "Bad", "Average" }) {
//        if (io.function(name)->name() != name) {
//            BOOST_LOG_SEV(lg, fatal)<<
//            boost::format("Name of the function is supposed to be %1%,"
//                    "though  io.function(%1%) returned %2%")
//            %name%io.function(name)->name();
//            return -1;
//        } else {
//            BOOST_LOG_SEV(lg, info)<<
//            boost::format("Name of the function is supposed to be %1%,"
//                    "and io.function(%1%) returned %2%.")
//            %name%io.function(name)->name();
//        }
//    }
    return 0;
}
