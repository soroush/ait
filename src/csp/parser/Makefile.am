# AIT Library (Artificial Intelligence Toolkit), A C++ library of AI tools.
#
# Copyright (c) 2012,2013 Soroush Rabiei <soroush-r@users.sf.net>,
# Roya Ghasemzadeh <ghasemzadeh.roya1@gmail.com>
#
# AIT is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# See the file COPYING included with this distribution for more
# information.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -pipe
noinst_LTLIBRARIES = libait_csp_parser.la
ait_includedir = $(includedir)/ait/csp/parser
libait_csp_parser_la_SOURCES = \
		exp_func.cpp \
		exp_num.cpp \
		exp_ref.cpp \
		expression.cpp \
		domain_scanner.cpp \
		domain_parse.cpp \
		constraint-parameters_scanner.cpp \
		constraint-parameters_parse.cpp \
		constraint-scope_scanner.cpp \
		constraint-scope_parse.cpp \
		parameters_scanner.cpp \
		parameters_parse.cpp \
		predicate-f_scanner.cpp \
		predicate-f_parse.cpp \
		xcsp-pskel.cxx \
		xinstance-parser.cpp \
		xpresentation-parser.cpp \
		xproblem-type-parser.cpp \
		xproblem-format-parser.cpp \
		xdomain-parser.cpp \
		xdomains-parser.cpp \
		xvariable-parser.cpp \
		xvariables-parser.cpp \
		xexpression-parser.cpp \
		xpredicate-parser.cpp \
		xpredicates-parser.cpp \
		xrelation-parser.cpp \
		xsemantics-type.cpp \
		xrelations-parser.cpp \
		xconstraint-parser.cpp \
		xconstraints-parser.cpp
ait_include_HEADERS =  \
		exp_func.hpp \
		exp_num.hpp \
		exp_ref.hpp \
		expression.hpp \
		domain_scanner.hpp \
		domain_scannerbase.h \
		domain_parser.hpp \
		domain_parser.ih \
		domain_parserbase.h \
		parameters_scanner.hpp \
		parameters_scannerbase.h \
		parameters_parser.hpp \
		parameters_parser.ih \
		parameters_parserbase.h \
		constraint-parameters_scanner.hpp \
		constraint-parameters_scannerbase.h \
		constraint-parameters_parser.hpp \
		constraint-parameters_parser.ih \
		constraint-parameters_parserbase.h \
		constraint-scope_scanner.hpp \
		constraint-scope_scannerbase.h \
		constraint-scope_parser.hpp \
		constraint-scope_parser.ih \
		constraint-scope_parserbase.h \
		predicate-f_scanner.hpp \
		predicate-f_scannerbase.h \
		predicate-f_parser.hpp \
		predicate-f_parser.ih \
		predicate-f_parserbase.h \
		xcsp-pskel.hxx \
		xinstance-parser.hpp \
		xpresentation-parser.hpp \
		xproblem-type-parser.hpp \
		xproblem-format-parser.hpp \
		xdomain-parser.hpp \
		xdomains-parser.hpp \
		xvariable-parser.hpp \
		xvariables-parser.hpp \
		xexpression-parser.hpp \
		xpredicate-parser.hpp \
		xpredicates-parser.hpp \
		xrelation-parser.hpp \
		xsemantics-type.hpp \
		xrelations-parser.hpp \
		xconstraint-parser.hpp \
		xconstraints-parser.hpp
ait_libincludedir = $(libdir)/ait/include
EXTRA_DIST = predicate-f_parser.ih \
		predicate-f_scanner.ih \
		parameters_parser.ih \
		parameters_scanner.ih \
		constraint-scope_parser.ih \
		constraint-scope_scanner.ih \
		constraint-parameters_parser.ih \
		constraint-parameters_scanner.ih \
		domain_parser.ih \
		domain_scanner.ih
