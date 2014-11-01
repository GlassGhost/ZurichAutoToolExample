## Process this file to produce a configure script:
##   aclocal -I build-aux/m4 && autoheader && autoconf && automake
##

## Preamble - used to set up meta paths, meta-information, etc.
#
# require a minimum version of AutoConf
AC_PREREQ([2.65])

# software name, version, contact address
AC_INIT([hw],[1.0.0],[foo.bar@example.com])

# if this file does not exist, `configure` was invoked in the wrong directory
AC_CONFIG_SRCDIR([hw/hw.c])

# directories (relative to top-level) to look into for AutoConf/AutoMake files
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux])
# enable AutoMake
AM_INIT_AUTOMAKE([1.10])
# all defined C macros (HAVE_*) will be saved to this file
AC_CONFIG_HEADERS([config.h])

# Macros for the compilers. Check
# http://www.gnu.org/savannah-checkouts/gnu/autoconf/manual/autoconf-2.69/html_node/Compilers-and-Preprocessors.html#Compilers-and-Preprocessors
# for a full list
#
# This macro checks if you have a C compiler
AC_PROG_CC
AM_PROG_CC_C_O

# Check if you have a C++ compiler
# AC_PROG_CXX
# AC_PROG_CXX_C_O

# Check if you have an Objective-C compiler
# AC_PROG_OBJC
# AC_PROG_OBJC_C_O

# Check for fortran compilers
# AC_PROG_F77
# AC_PROG_F77_C_)
# AC_PROG_FC
# AC_PROG_FC_C_O

# Check if the `install` program is present
AC_PROG_INSTALL

## Initialize GNU LibTool
#
# http://www.gnu.org/software/libtool/manual/html_node/LT_005fINIT.html
# 
# GNU LibTool provides a portable way to build libraries.  AutoMake
# knows how to use it; you just need to activate it.
LT_INIT([static])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor pow sqrt])

# Substitute all conditionals in these files; this is normally used to
# create `Makefile`s but could also be used for scripts, include
# files, etc.
AC_CONFIG_FILES([Makefile
                 hw/Makefile
                 libmath/Makefile])
AC_OUTPUT
