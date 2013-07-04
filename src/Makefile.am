ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
lib_LTLIBRARIES = libait.la
AM_CPPFLAGS = -pipe
libait_la_SOURCES = \
 			abt.pb.cc \
 			assignment.cpp \
 			compound-assignment.cpp \
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
libait_la_LIBADD = $(ZMQ_LIBS) $(PROTOCOL_BUFFERS_LIBS)
libait_la_LDFLAGS = -version-info $(AIT_SO_VERSION)
ait_includedir = $(includedir)/ait
ait_include_HEADERS =  \
			abt.pb.h \
			assignment.h \
			compound-assignment.h \
			constraint.h \
			csp-solver.h \
			abt-monitor.h \
			abt-solver.h \
			common_async.h \
			common-protocols.pb.h \
			aabt-solver.h \
			aabt.pb.h \
			aabt-assignment.h \
			aabt-explanation.h \
			aabt-message.h \
			aabt-nogood.h \
			aabt-order.h \
			global.h
ait_libincludedir = $(libdir)/ait/include
