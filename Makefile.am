AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libait.la
AM_CPPFLAGS = -pipe
libait_la_LIBADD = $(ZMQ_LIBS) $(PROTOCOL_BUFFERS_LIBS)
libait_la_SOURCES = \
			src/abt.pb.cc \
			src/abt-monitor.cpp \
			src/abt-socket.cpp \
			src/abt-solver.cpp \
			src/common-protocols.pb.cc 
libait_la_LDFLAGS = -version-info $(AIT_SO_VERSION)
ait_includedir = $(includedir)/ait
ait_include_HEADERS =  \
			src/abt.pb.h \
			src/abt-monitor.h \
			src/abt-socket.h \
			src/abt-solver.h \
			src/common_async.h \
			src/common-protocols.pb.h \
			src/common_csp.h \
			src/global.h
ait_libincludedir = $(libdir)/ait/include
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libait.pc
dist_noinst_SCRIPTS = autogen.sh
