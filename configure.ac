#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([telepathy-bot], [0.1], [oadbd@forkoff.org])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR([src/telepathy-bot.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE


TELEPATHY_GLIB_REQUIRED=0.18.0
GOBJECT_REQUIRED=2.32.0
GLIB_REQUIRED=2.32.0

# Checks for programs.
AC_PROG_CC

LT_PREREQ([2.2])
LT_INIT

PKG_CHECK_MODULES(TELEPATHY_BOT, 
[
	telepathy-glib >= $TELEPATHY_GLIB_REQUIRED
    gobject-2.0 >= $GOBJECT_REQUIRED
	glib-2.0 >= $GLIB_REQUIRED
])
# Checks for libraries.

#AC_CONFIG_FILES([Makefile])
AC_OUTPUT(Makefile src/Makefile)
