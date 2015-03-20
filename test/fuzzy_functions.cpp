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

#include "../src/fuzzy/triangular-mf.hpp"
#include "utilities/tokenizer.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#define BOOST_ALL_DYN_LINK
#define DSO
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/format.hpp>

using namespace AIT::FUZZY;
using namespace AIT::TEST;
using namespace std;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;

src::severity_logger<severity_level> lg;

float checkTriangularMF(const TriangularMF& mf, const float& input,
        const float& output) {
    float calculatedValue = mf(input);
    float errorValue = abs(output - calculatedValue);
    if (errorValue < 0.001f) {
        BOOST_LOG_SEV(lg, trace)<<
        boost::format("OK\ttrimf(%+011.10f) = %+011.2f ~= %+011.10f, ERROR=%+011.10f")
        % input % calculatedValue % output % errorValue;
    } else {
        BOOST_LOG_SEV(lg, error)<<
        boost::format("ERROR\ttrimf(%+011.10f) = +%011.2f != %+011.10f, ERROR=+%011.10f")
        % input % calculatedValue % output % errorValue;
    }
    return errorValue;
}

int main() {

    int n = 5; // number of files
    // for each file open it and do this:
    logging::add_common_attributes();
    logging::register_simple_formatter_factory<logging::trivial::severity_level,
            char>("Severity");
    logging::add_console_log(std::cout, boost::log::keywords::format = "[%TimeStamp%][%Severity%]: %Message%");
    ifstream inputFile { "data/trimf_0.000000_0.250000_0.500000.txt" };
    inputFile.seekg(std::ios::beg);
    // Ignore first line, that's for humans
    inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // Get second line, check algorithm:
    string function;
    string line;
    getline(inputFile, line);
    Tokenizer tok { line, " " };
    if (tok.nextToken()) {
        function = tok.getToken();
        if (function == "trimf") {
            float p1 = stof(tok.getNextToken());
            float p2 = stof(tok.getNextToken());
            float p3 = stof(tok.getNextToken());
            TriangularMF trimf { "test function", p1, p2, p3 };
            // Again, ignore the line, written for dumb humans
            inputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            tok.setString(line);
            getline(inputFile, line);
            float input, output;
            float divergence = 0;
            while (inputFile >> input >> output) {
                divergence += checkTriangularMF(trimf, input, output);
            }
        }
    }
    return 0;
}
