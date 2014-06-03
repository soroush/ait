AM_CPPFLAGS = -I$(srcdir)/.. $(OTHER_CPPFLAGS)
LDADD = ../src/libait.la \
		../src/csp/libait_csp.la \
		../src/csp/parser/libait_csp_parser.la \
		-lzmq -lexpat
check_PROGRAMS = abt_monitor 
abt_monitor_SOURCES = abt-monitor.cpp
TESTS = $(check_PROGRAMS)