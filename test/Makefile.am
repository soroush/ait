AM_CPPFLAGS = -I$(srcdir)/.. $(OTHER_CPPFLAGS)
LDADD = ../src/libait.la ../src/csp/libait_csp.la ../src/csp/parser/libait_csp_parser.la 
check_PROGRAMS = parser_test 
#NQueens_ABT_Monitor NQueens_ABT_Agent
#NQueens_ABT_Monitor_SOURCES = abt-monitor.cpp
#NQueens_ABT_Agent_SOURCES = n-queen-abt-solver.cpp
parser_test_SOURCES = parser-test.cpp
TESTS = $(check_PROGRAMS)