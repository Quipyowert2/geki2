dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AC_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN(AM_MISSING_PROG,
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

dnl Configure Paths for KXL
dnl AM_PATH_KXL
dnl Test for libKXL, and define KXL_CFLAGS and KXL_LIBS as appropriate.
dnl enables arguments --with-kxl-prefix= -with-kxl-inc-prefix=
dnl
AC_DEFUN(AM_PATH_KXL,
[dnl
dnl Get the cfags and libraries for KXL
dnl
AC_ARG_WITH(kxl-prefix, [  --with-kxl-prefix=PFX   Prefix where KXL library is installed(optional)],
	[kxl_prefix="$withval"], [kxl_prefix=""])
AC_ARG_WITH(kxl-inc-prefix, [  --with-kxl-inc-prefix=PFX Prefix where include libraries are (optional)],
	[kxl_inc_prefix="$withval"], [kxl_inc_prefix=""])

dnl Add any special include directories
AC_MSG_CHECKING(for KXL CFLAGS)
if test "$kxl_inc_prefix" != "" ; then
	KXL_CFLAGS="$KXL_CFLAGS -I$kxl_inc_prefix"
	CFLAGS="-I$kxl_inc_prefix"
fi
AC_MSG_RESULT($KXL_CFLAGS)

dnl add any special lib dirs
AC_MSG_CHECKING(for KXL LDFLAGS)
if test "$kxl_prefix" != "" ; then
	KXL_LIBS="$KXL_LIBS -L$kxl_prefix"
	LIBS="-L$kxl_prefix"
fi

dnl add the KXL library
KXL_LIBS="$KXL_LIBS -lKXL"
LDFLAGS="$KXL_LIBS"
AC_MSG_RESULT($KXL_LIBS)

AC_PATH_XTRA
AC_CHECK_LIB([KXL], [KXL_CreateWindow],,
        [AC_MSG_ERROR(No linkable libKXL was found.)], [$X_LIBS -lX11]
)

dnl That should be it.  Now just export out symbols:
AC_SUBST(KXL_CFLAGS)
AC_SUBST(KXL_LIBS)
])

