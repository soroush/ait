AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libait.la
AM_CPPFLAGS = -pipe
libait_la_LIBADD = $(ZMQ_LIBS) $(PROTOCOL_BUFFERS_LIBS)
libait_la_SOURCES = \
					src/abt-monitor.cpp \
					src/abt-socket.cpp \
					src/protocols.pb.cc \
					src/precompiledTypes.cpp
libait_la_LDFLAGS = -version-info $(AIT_SO_VERSION)
ait_includedir = $(includedir)/ait
ait_include_HEADERS =  \
			src/global.h \
			src/abt-monitor.h \
			src/abt-socket.h \
			src/abt-solver.hpp \
			src/common_async.hpp \
			src/protocols.pb.h \
			src/common_csp.hpp
ait_libincludedir = $(libdir)/ait/include
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libait.pc
dist_noinst_SCRIPTS = autogen.sh
