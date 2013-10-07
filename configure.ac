AC_INIT([libait], [0.1.0], [soroush-r@users.sf.net], [libait], [http://sourceforge.net/projects/ait++/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])
AM_SILENT_RULES
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11()
AM_PROG_AR
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
AX_XSDCXX
PKG_CHECK_MODULES([ZMQ], [libzmq >= 2.2.0])
PKG_CHECK_MODULES([PROTOCOL_BUFFERS], [protobuf >= 2.4.1])
AC_ARG_ENABLE([precompiled-protocols],
  AS_HELP_STRING([--enable-precompiled-protocols=@<:@yes/no@:>@],
                 [enable the use of precompiled sources of Protocols Buffer @<:@default=yes@:>@]),
	[case "${enableval}" in
	yes) enable_precompiled_protocols=true ;;
	no)  enable_precompiled_protocols=false ;;
	*) AC_MSG_ERROR(bad value ${enableval} for --enable-precompiled-protocols. Please say `yes' or `no'.) ;;
	esac]
)
AM_CONDITIONAL([USE_PRECOMPILED_PROTOCOLS], [test $enable_precompiled_protocols = true])
AM_CONDITIONAL([USE_GENERATED_PROTOCOLS], [test $enable_precompiled_protocols = false])
AS_IF([test $enable_precompiled_protocols = true],
      [AC_DEFINE(PRECOMPILED_PROTOCOLS,1,[Define if uisng pre-compiled sources of protocols])],
      [AC_DEFINE(GENERATED_PROTOCOLS,1,[Define if generating new sources of protocols])])
AC_ARG_WITH([protobuf-libdir],
    [AS_HELP_STRING([--with-protobuf-libdir=LIB_DIR],
    [location of the protocol buffers libraries, defaults to /usr/lib])],
    [PROTOBUF_LIBDIR="$withval"],
    [PROTOBUF_LIBDIR='/usr/lib'])
AC_SUBST([PROTOBUF_LIBDIR])
LDFLAGS="$LDFLAGS -L$PROTOBUF_LIBDIR"
AC_CHECK_LIB([protobuf], [main], [], [AC_MSG_ERROR([cannot find protobuf library])])
AC_ARG_WITH([protoc],
    [AS_HELP_STRING([--with-protoc=PATH],
    [location of the protoc protocol buffer compiler binary, defaults to protoc])],
    [PROTOC="$withval"],
    [PROTOC='protoc'])
AC_SUBST([PROTOC])
LT_INIT([shared static win32-dll pic-only])
AC_SUBST([AIT_SO_VERSION], [0:1:0])
AC_SUBST([AIT_API_VERSION], [0.1.0])
AC_CONFIG_FILES([Makefile src/Makefile src/csp/Makefile src/csp/parser/Makefile 
					test/Makefile
                 libait.pc:libait.pc.in])
AC_OUTPUT
