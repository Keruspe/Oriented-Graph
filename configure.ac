AC_PREREQ([2.68])
AC_INIT([libgraph],[0.1],[Marc-Antoine@Perennou.com])

AC_CONFIG_HEADERS([config.hh])
AC_CONFIG_SRCDIR([graph/])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([.aux])

AM_INIT_AUTOMAKE([1.11 subdir-objects check-news foreign no-dist-gzip dist-xz tar-ustar -Wall])
AM_SILENT_RULES([yes])

AC_PROG_INSTALL
AC_PROG_CXX

AC_DISABLE_STATIC
LT_INIT

AC_CONFIG_FILES([
    Makefile
])
AC_OUTPUT
AC_MSG_RESULT([
        $PACKAGE_NAME $VERSION
])
