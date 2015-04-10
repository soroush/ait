AC_PREREQ([2.69])
AC_INIT([libait], [2.0.0], [https://github.com/soroush/ait])
AC_CONFIG_SRCDIR([src/global.hpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Check for C++11 compiler
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

# Checks for libraries.
AX_XSDCXX
AX_LIB_EXPAT(2.0.0)
PKG_CHECK_MODULES([ZMQ], [libzmq >= 2.2])
PKG_CHECK_MODULES([PROTOCOL_BUFFERS], [protobuf >= 2.4])
BOOST_REQUIRE([1.54.0],[AC_MSG_WARN([Unable to find Boost, all logging backends may be disabled...])])
BOOST_LOG([mt],[AC_MSG_WARN([Boost Logging library not found. All logging backends has been disabled.])])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([memset pow socket])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/csp/Makefile
                 src/csp/parser/Makefile
                 src/fuzzy/Makefile
                 test/Makefile
                 libait.pc])

AC_SUBST([AIT_CSP_SO_VERSION], [1:0:0])
AC_SUBST([AIT_CSP_API_VERSION], [1.0.0])

AC_SUBST([AIT_FUZZY_SO_VERSION], [1:0:0])
AC_SUBST([AIT_FUZZY_API_VERSION], [1.0.0])

# Configure automake
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_SILENT_RULES([yes])

LT_INIT([static shared pic-only])

AC_OUTPUT
