AC_INIT([libait], [0.1.0], [soroush-r@users.sf.net], [libait], [http://sourceforge.net/projects/ait++/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11()
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
PKG_CHECK_MODULES([ZMQ], [libzmq >= 3.2.3])
PKG_CHECK_MODULES([PROTOCOL_BUFFERS], [protobuf >= 2.5.0])
LT_INIT([shared static])
AC_SUBST([AIT_SO_VERSION], [0:1:0])
AC_SUBST([AIT_API_VERSION], [0.1.0])
AC_CONFIG_FILES([Makefile src/Makefile test/Makefile
                 libait.pc:libait.pc.in])
AC_OUTPUT
