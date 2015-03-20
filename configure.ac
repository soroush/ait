AC_INIT([libait], [1.0.0], [soroush-r@users.sf.net], [libait], [https://github.com/soroush/ait])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])
AM_SILENT_RULES
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
AM_PROG_AR
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
LT_INIT([shared static win32-dll pic-only])

# Check for dependencies
AX_XSDCXX
AX_LIB_EXPAT(2.0.0)
PKG_CHECK_MODULES([ZMQ], [libzmq >= 4.0.0])
PKG_CHECK_MODULES([PROTOCOL_BUFFERS], [protobuf >= 2.4.1])
BOOST_REQUIRE([1.54])
BOOST_LOG([])

# Enable custom protobuff directory 
AC_ARG_WITH([protobuf-libdir],
    [AS_HELP_STRING([--with-protobuf-libdir=LIB_DIR],
    [location of the protocol buffers libraries, defaults to /usr/lib])],
    [PROTOBUF_LIBDIR="$withval"],
    [PROTOBUF_LIBDIR='/usr/lib'])
AC_SUBST([PROTOBUF_LIBDIR])
LDFLAGS="$LDFLAGS -L$PROTOBUF_LIBDIR"

# Enable custom xsdcxx directory 
AC_ARG_WITH([xsd-include],
    [AS_HELP_STRING([--with-xsdcxx-include=LIB_DIR],
    [location of the CodeSynthesis XSD C++ headers, defaullts to /usr/lib])],
    [XSDCXX_INCLUDE="$withval"],
    [XSDCXX_INCLUDE='/usr/lib'])
AC_SUBST([XSDCXX_INCLUDE])
CPPFLAGS="$CPPFLAGS -I$XSDCXX_INCLUDE"

# Choose between either re-generate protocol sources using protoc or compile pre-generated sources
AC_ARG_ENABLE([precompiled-protocols],
		[AC_HELP_STRING([--enable-precompiled-protocols=@<:@yes/no@:>@])],
		[enable_precompiled_protocols=yes],
		[enable_precompiled_protocols=no])

# Custom protoc command
AC_ARG_WITH([protoc],
    [AS_HELP_STRING([--with-protoc=PATH],
    [location of the protoc protocol buffer compiler binary, defaults to protoc])],
    [PROTOC="$withval"],
    [PROTOC='protoc'])
AC_SUBST([PROTOC])

# Check if protocol buffers compiler is available. If not, disable 
# --enable-precompiled-protocols

# Automake conditions
AM_CONDITIONAL([USE_PRECOMPILED_PROTOCOLS], [test $enable_precompiled_protocols = yes])
AM_CONDITIONAL([USE_GENERATED_PROTOCOLS], [test $enable_precompiled_protocols = no])

AS_IF([test "x$enable_precompiled_protocols" = "xyes"],
      [AC_DEFINE(PRECOMPILED_PROTOCOLS,1,[Define if uisng pre-compiled sources of protocols])],
      [AC_DEFINE(GENERATED_PROTOCOLS,1,[Define if generating new sources of protocols])])

AC_SUBST([AIT_CSP_SO_VERSION], [1:0:0])
AC_SUBST([AIT_CSP_API_VERSION], [1.0.0])

AC_SUBST([AIT_FUZZY_SO_VERSION], [1:0:0])
AC_SUBST([AIT_FUZZY_API_VERSION], [1.0.0])

AC_CONFIG_FILES([Makefile src/Makefile
                    src/csp/Makefile src/csp/parser/Makefile
                    src/fuzzy/Makefile
                    test/Makefile
                    libait.pc:libait.pc.in])
AC_OUTPUT
