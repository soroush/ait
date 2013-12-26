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
 			aabt.pb.cc \
 			common-protocols.pb.cc
BUILT_SOURCES = \
 			abt.pb.h \
 			aabt.pb.h \
 			common-protocols.pb.h
MOSTLYCLEANFILES = $(nodist_libait_csp_la_SOURCES) $(BUILT_SOURCES)
endif
libait_csp_la_SOURCES = \
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
 			aabt-solver.cpp \
 			aabt-assignment.cpp \
 			aabt-explanation.cpp \
 			aabt-message.cpp \
 			aabt-nogood.cpp \
 			aabt-order.cpp
libait_csp_la_LIBADD = parser/libait_csp_parser.la
libait_csp_la_LDFLAGS = -version-info $(AIT_SO_VERSION)
ait_includedir = $(includedir)/ait/csp
ait_include_HEADERS =  \
			assignment.h \
			compound-assignment.h \
			domain.h \
 			relation-base.h \
 			variable.h \
			predicate.h \
			constraint.h \
			csp-problem.h \
			csp-solver.h \
			abt-monitor.h \
			abt-socket.h \
			abt-solver.h \
			common_async.h \
			aabt-solver.h \
			aabt-assignment.h \
			aabt-explanation.h \
			aabt-message.h \
			aabt-nogood.h \
			aabt-order.h
if USE_PRECOMPILED_PROTOCOLS
libait_csp_la_SOURCES += \
 			abt_precompiled.pb.cc \
 			aabt_precompiled.pb.cc \
 			common-protocols_precompiled.pb.cc
ait_include_HEADERS +=  \
			abt_precompiled.pb.h \
 			aabt_precompiled.pb.h \
 			common-protocols_precompiled.pb.h
endif
ait_libincludedir = $(libdir)/ait/include
EXTRA_DIST = aabt.proto \
	aabt_precompiled.proto \
	abt.proto
