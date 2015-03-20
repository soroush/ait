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

SUBDIRS = .
AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -pipe
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libait_fuzzy.la
libait_fuzzy_la_LDFLAGS = -version-info $(AIT_FUZZY_SO_VERSION)
libait_fuzzy_la_SOURCES = \
	function.cpp \
	membership-function.cpp \
	implicated-mf.cpp \
	triangular-mf.cpp \
	triangular-imf.cpp \
	operator.cpp \
	rule.cpp \
	io-base.cpp
ait_includedir = $(includedir)/ait/fuzzy
ait_include_HEADERS =  \
	function.hpp \
	membership-function.hpp \
	implicated-mf.hpp \
	triangular-mf.hpp \
	triangular-imf.hpp \
	operator.hpp \
	rule.hpp \
	io-base.hpp
ait_libincludedir = $(libdir)/ait/include
