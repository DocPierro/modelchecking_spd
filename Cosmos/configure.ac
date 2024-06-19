#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Cosmos],[1.6],[barbot@lacl.fr])
AM_INIT_AUTOMAKE([foreign -Wall subdir-objects])
AM_SILENT_RULES
AC_CONFIG_SRCDIR([src/Simulator/BatchR.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

#CXXFLAGS=-O3 -g
# : ${CXXFLAGS= -O3 -g -isystem src/gtest/include}

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LEX([noyywrap])
AM_PROG_AR
LT_INIT
AC_PROG_YACC

AX_CXX_COMPILE_STDCXX_11
AX_CODE_COVERAGE

AC_PROG_OCAML
if test "$OCAMLC" = "no"; then
    AC_MSG_ERROR([You must install the OCaml compiler])
fi
AC_PROG_FINDLIB
AC_CHECK_OCAML_PKG([zip])
AC_CHECK_OCAML_PKG([xml-light])

AC_CHECK_PROG(have_prog_gnuplot, [gnuplot], [yes],[no])
AC_MSG_CHECKING([gnuplot])
if test "$have_prog_gnuplot" = "yes" ; then
	AC_MSG_RESULT([yes])
	else
	AC_MSG_RESULT([no])
	AC_MSG_WARN([Gnuplot not found some function are disabled])
fi

# Checks for libraries.
AC_CHECK_LIB([expat],[XML_ParseBuffer])

# Checks for header files.
#AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([float.h inttypes.h libintl.h limits.h stdlib.h string.h sys/ioctl.h sys/time.h unistd.h])

BOOST_REQUIRE([1.46])

#AX_PTHREAD

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T

AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CHECK_FUNCS([dup2 floor gettimeofday memset mkdir pow select sqrt])
AX_PTHREAD

AC_CONFIG_FILES([Makefile src/libgrml/Makefile src/ModelGenerator/Eval/Makefile src/ModelGenerator/GspnParser/Makefile src/ModelGenerator/LhaParser/Makefile src/ModelGenerator/Makefile src/Simulator/Makefile src/LightSimulator/Makefile test/Makefile automataGen/Makefile utils/Makefile modelConvert/Makefile])
AC_OUTPUT
