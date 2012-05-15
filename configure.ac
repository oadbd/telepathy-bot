#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([telepathy-bot], [0.1], [oadbd@forkoff.org])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/telepathy-bot.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(telepathy-bot, 0.1)


TELEPATHY_GLIB_REQUIRED=0.18.0
GLIB_REQUIRED=2.32.0

# Checks for programs.
AC_PROG_CC

LT_PREREQ([2.2])
LT_INIT

PKG_CHECK_MODULES(TELEPATHY_BOT, 
[
  glib-2.0 >= $GLIB_REQUIRED
	telepathy-glib >= $TELEPATHY_GLIB_REQUIRED
])
# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday])

#AC_CONFIG_FILES([Makefile])
AC_OUTPUT(Makefile src/Makefile)
