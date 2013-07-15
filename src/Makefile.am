SUBDIRS = csp .
AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libait.la
libait_la_SOURCES =
libait_la_LIBADD = $(ZMQ_LIBS) $(PROTOCOL_BUFFERS_LIBS) csp/libait_csp.la
ait_includedir = $(includedir)/ait
ait_include_HEADERS =  \
			global.h
