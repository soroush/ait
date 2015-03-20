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

SUBDIRS = parser .
AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -pipe
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
noinst_LTLIBRARIES = libait_csp.la
if USE_GENERATED_PROTOCOLS
%.pb.cc %.pb.h: %.proto
	$(PROTOC) --proto_path=$(srcdir) --cpp_out=$(builddir) $^
nodist_libait_csp_la_SOURCES = \
			abt.pb.cc \
			common-protocols.pb.cc
BUILT_SOURCES = \
			abt.pb.h \
			common-protocols.pb.h
MOSTLYCLEANFILES = $(nodist_libait_csp_la_SOURCES) $(BUILT_SOURCES)
endif
libait_csp_la_SOURCES = \
			common_async.cpp \
			assignment.cpp \
			compound-assignment.cpp \
			domain.cpp \
			relation-base.cpp \
			variable.cpp \
			predicate.cpp \
			constraint.cpp \
			csp-problem.cpp \
			csp-solver.cpp \
			abt-message.cpp \
			abt-monitor.cpp \
			abt-socket.cpp \
			abt-nogood.cpp \
			abt-endpoint.cpp \
			abt-solver.cpp
libait_csp_la_LIBADD = parser/libait_csp_parser.la
ait_includedir = $(includedir)/ait/csp
ait_include_HEADERS =  \
			common_async.hpp \
			assignment.hpp \
			compound-assignment.hpp \
			domain.hpp \
			relation-base.hpp \
			variable.hpp \
			predicate.hpp \
			constraint.hpp \
			csp-problem.hpp \
			csp-solver.hpp \
			abt-monitor.hpp \
			abt-monitor.h \
			abt-socket.hpp \
			abt-solver.hpp \
			abt-solver.h
if USE_PRECOMPILED_PROTOCOLS
libait_csp_la_SOURCES += \
			abt_precompiled.pb.cc \
			common-protocols_precompiled.pb.cc
ait_include_HEADERS +=  \
			abt_precompiled.pb.h \
			common-protocols_precompiled.pb.h
endif
ait_libincludedir = $(libdir)/ait/include
EXTRA_DIST = \
	abt.proto
