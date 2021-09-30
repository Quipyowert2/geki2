dnl Process this file with autoconf to produce a configure script.
AC_INIT([geki2-KXL], [2.0.8])
AC_CONFIG_SRCDIR([src/geki2.h])
AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

dnl Checks for libraries.
dnl Replace `main' with a function in -lKXL:
AC_CHECK_LIB(KXL, main)
AC_PATH_XTRA
AM_PATH_KXL

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

if test "x$prefix" = "xNONE"; then
        DATA_PATH=/usr/local/share/geki2/data
        BMP_PATH=/usr/local/share/geki2/bmp
        WAV_PATH=/usr/local/share/geki2/wav
else
        DATA_PATH=$prefix/share/geki2/data
        BMP_PATH=$prefix/share/geki2/bmp
        WAV_PATH=$prefix/share/geki2/wav
fi
AC_DEFINE_UNQUOTED(DATA_PATH,"$DATA_PATH")
AC_DEFINE_UNQUOTED(BMP_PATH,"$BMP_PATH")
AC_DEFINE_UNQUOTED(WAV_PATH,"$WAV_PATH")
AC_DEFINE_UNQUOTED(TITLE,"$PACKAGE $VERSION")
AC_SUBST(DATA_PATH)
AC_SUBST(BMP_PATH)
AC_SUBST(WAV_PATH)

AC_OUTPUT([data/Makefile wav/Makefile bmp/Makefile src/Makefile
Makefile geki2.spec])
