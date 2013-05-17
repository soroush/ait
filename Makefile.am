AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libait-@AIT_API_VERSION@.la
libait_@AIT_API_VERSION@_la_SOURCES = \
					src/precompiledTypes.cpp
libait_@AIT_API_VERSION@_la_LDFLAGS = -version-info $(AIT_SO_VERSION)
ait_includedir = $(includedir)/ait
ait_include_HEADERS =  \
			src/abt-solver.h \
			src/common_async.hpp \
			src/common_csp.hpp
ait_libincludedir = $(libdir)/ait-$(AIT_API_VERSION)/include
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ait-$(AIT_API_VERSION).pc
dist_noinst_SCRIPTS = autogen.sh
