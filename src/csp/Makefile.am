SUBDIRS = parser .
AUTOMAKE_OPTIONS = subdir-objects
AM_CPPFLAGS = -pipe
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
noinst_LTLIBRARIES = libait_csp.la
libait_csp_la_SOURCES = \
 			abt.pb.cc \
 			assignment.cpp \
 			compound-assignment.cpp \
 			predicate.cpp \
 			constraint.cpp \
 			csp-solver.cpp \
 			abt-message.cpp \
 			abt-monitor.cpp \
 			abt-socket.cpp \
 			abt-nogood.cpp \
 			abt-endpoint.cpp \
 			abt-solver.cpp \
 			aabt-solver.cpp \
 			aabt.pb.cc \
 			aabt-assignment.cpp \
 			aabt-explanation.cpp \
 			aabt-message.cpp \
 			aabt-nogood.cpp \
 			aabt-order.cpp \
 			common-protocols.pb.cc 
libait_csp_la_LIBADD = parser/libait_csp_parser.la
libait_csp_la_LDFLAGS = -version-info $(AIT_SO_VERSION)
ait_includedir = $(includedir)/ait/csp
ait_include_HEADERS =  \
			abt.pb.h \
			assignment.h \
			compound-assignment.h \
			predicate.h \
			constraint.h \
			csp-solver.h \
			abt-monitor.h \
			abt-socket.h \
			abt-solver.h \
			common_async.h \
			common-protocols.pb.h \
			aabt-solver.h \
			aabt.pb.h \
			aabt-assignment.h \
			aabt-explanation.h \
			aabt-message.h \
			aabt-nogood.h \
			aabt-order.h 
ait_libincludedir = $(libdir)/ait/include
