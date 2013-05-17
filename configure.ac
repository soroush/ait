## libAIT

AC_INIT([libAIT], [0.1.0], [soroush-r@users.sf.net], [libait],[http://sourceforge.net/projects/ait++/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])
AC_PROG_CPP
AC_TYPE_SIZE_T
AC_HEADER_STDBOOL
CXXFLAGS="$CXXFLAGS -std=c++0x"
AC_COMPILE_IFELSE([AC_LANG_SOURCE(
  [[template <typename T>
    struct check
    {
      static_assert(sizeof(int) <= sizeof(T), "not big enough");
    };

    typedef check<check<bool>> right_angle_brackets;

    int a;
    decltype(a) b;

    typedef check<int> check_type;
    check_type c;
    check_type&& cr = static_cast<check_type&&>(c);]])],,
  AC_MSG_FAILURE(['$CXX $CXXFLAGS' does not accept ISO C++11]))

AC_PROG_CXX
LT_INIT([shared static])

AC_SUBST([AIT_SO_VERSION], [0:1:0])
AC_SUBST([AIT_API_VERSION], [0.1.0])

AC_CONFIG_FILES([Makefile
                 ait-${AIT_API_VERSION}.pc:ait.pc.in])
AC_OUTPUT
