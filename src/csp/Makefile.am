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
AM_CXXFLAGS = \
			$(ZMQ_CFLAGS) \
			$(BOOST_CPPFLAGS) \
			$(PROTOCOL_BUFFERS_CFLAGS) \
			$(EXPAT_CFLAGS)
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libait_csp.la
libait_csp_la_LDFLAGS = -version-info $(AIT_CSP_SO_VERSION)
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
			abt-solver.cpp \
			abt.pb.cc \
			common-protocols.pb.cc
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
			abt-solver.h \
			abt.pb.h \
			common-protocols.pb.h
libait_csp_la_LIBADD = \
		$(ZMQ_LIBS) \
        $(PROTOCOL_BUFFERS_LIBS) \
        $(BOOST_LOG_LIBS) \
		$(EXPAT_LDFLAGS) \
		parser/libait_csp_parser.la
# 	-lexpat -lboost_log -lboost_log_setup -lboost_thread -lboost_system -lpthread \
#
ait_libincludedir = $(libdir)/ait/include
EXTRA_DIST = \
	abt.proto
