AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libait.la
AM_CPPFLAGS = -pipe
libait_la_LIBADD = $(ZMQ_LIBS) $(PROTOCOL_BUFFERS_LIBS)
libait_la_SOURCES = \
			src/abt.pb.cc \
			src/assignment.cpp \
			src/compound-assignment.cpp \
			src/abt-message.cpp \
			src/abt-monitor.cpp \
			src/abt-socket.cpp \
			src/abt-nogood.cpp \
			src/abt-endpoint.cpp \
			src/abt-solver.cpp \
			src/aabt-solver.cpp \
			src/aabt.pb.cpp \
			src/aabt-assignment.cpp \
			src/aabt-explanation.cpp \
			src/aabt-message.cpp \
			src/aabt-nogood.cpp \
			src/aabt-order.cpp \
			src/common-protocols.pb.cc 
libait_la_LDFLAGS = -version-info $(AIT_SO_VERSION)
ait_includedir = $(includedir)/ait
ait_include_HEADERS =  \
			src/abt.pb.h \
			src/assignment.h \
			src/compound-assignment.h \
			src/abt-message.h \
			src/abt-monitor.h \
			src/abt-socket.h \
			src/abt-solver.h \
			src/abt-nogood.h \
			src/abt-endpoint.h \
			src/common_async.h \
			src/common-protocols.pb.h \
			src/aabt-solver.h \
			src/aabt.pb.h \
			src/aabt-assignment.h \
			src/aabt-explanation.h \
			src/aabt-message.h \
			src/aabt-nogood.h \
			src/aabt-order.h \
			src/global.h
ait_libincludedir = $(libdir)/ait/include
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libait.pc
dist_noinst_SCRIPTS = autogen.sh
