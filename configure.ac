#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
#AC_INIT(msort, 1.0, mayank20399@gmail.com)
#AM_INIT_AUTOMAKE(msort, 1.0)

AC_INIT([msort], [1.0])
AC_CONFIG_SRCDIR([src/msort.c])
AM_INIT_AUTOMAKE

#AC_INIT([src/foo.c])
#AM_INIT_AUTOMAKE([mumble], [1.5])

#AC_INIT([mumble], [1.5])
#AC_CONFIG_SRCDIR([src/foo.c])
#AM_INIT_AUTOMAKE

# AC_CONFIG_SRCDIR([src/extsort.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])

# Checks for header files.
AC_CHECK_HEADERS([locale.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([pow setlocale])

# AC_CONFIG_FILES([man/Makefile
#                 src/Makefile])
AC_OUTPUT(Makefile man/Makefile src/Makefile)
