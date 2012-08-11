dnl Copyright (c) 2005 Atheme Development Group
dnl Rights to this code are documented in doc/LICENSE.
dnl
dnl This file is the autoconf stub.
dnl
dnl $Id: configure.ac 8407 2007-06-03 22:35:28Z jilles $

dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT(opensn0w, 2.0.0, support@beige-box.com)

AC_CONFIG_AUX_DIR(autoconf)

AC_CONFIG_HEADER(include/config.h)
AC_PREFIX_DEFAULT(~/opensn0w)

AC_GNU_SOURCE
AC_SYS_LARGEFILE

dnl Automake compatibility. --nenolod
AC_SUBST([PACKAGE], [AC_PACKAGE_TARNAME])
AC_SUBST([VERSION], [AC_PACKAGE_VERSION])

AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package])


if test "$prefix" = "NONE"; then
	prefix="${ac_default_prefix}"
fi

dnl the following is wrong! -- jilles
dnl AC_DEFINE_UNQUOTED([PREFIX], "${prefix}", [Installation prefix.])

dnl OS X is stupid
srcroot=$srcdir
if test "x${srcroot}" = "x." ; then
  srcroot=""
else
  srcroot="${srcroot}/"
fi
AC_SUBST([srcroot])
abs_srcroot="`cd "$srcdir"; pwd`/"
AC_SUBST([abs_srcroot])

AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_CANONICAL_BUILD
case "$target" in
*-*-mingw32)
    AC_DEFINE_UNQUOTED(MINGW,1,[This is a MinGW system])
    AC_DEFINE_UNQUOTED(WINDOWS,1,[This is a Windows system])
    CFLAGS="-I/mingw/include $CFLAGS -D_WIN32_WINNT=0x500 -DWINVER=0x500"
    LIBS="$LIBS -lsetupapi -lpsapi -lcomctl32 -lgdi32 -lcomdlg32 -lmsimg32 -lgdiplus"
        ;;
*-*-linux*)
    AC_DEFINE_UNQUOTED(LINUX,1,[This is a Linux system])
        ;;
*-*-darwin*)
    AC_DEFINE_UNQUOTED(DARWIN,1,[This is a Darwin system])
    LIBS="$LIBS $abs_srcroot/prebuilt/libusb-1.0.a -framework IOKit -framework CoreFoundation"
        ;;
esac

dnl Checks for programs.
AC_ISC_POSIX
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_INSTALL
AC_EXEEXT
AC_PATH_PROG(RM, rm)
AC_PATH_PROG(MV, mv)
AC_PATH_PROG(CP, cp)
AC_PATH_PROG(LN, ln)
AC_PATH_PROG(TAR, tar)
AC_PATH_PROG(AR, ar)
AC_PATH_PROG(RANLIB, ranlib)
AC_PATH_PROG(XARGS, xargs)
AC_PATH_PROG(FIND, find)
AC_PATH_PROG(UNAME, uname)
AC_C_INLINE
AC_C_BIGENDIAN
AC_PROG_MAKE_SET

AC_MSG_CHECKING(how to generate dependency info)
if test "$SunWorkShop" = "yes"; then
  AC_MSG_RESULT(Sun WorkShop/Forte using $CC -xM)
  MKDEP="$CC -xM"
fi
if test "$HPUX" = "yes"; then
  AC_MSG_RESULT(HP-UX C Compiler using makedepend)
  MKDEP="makedepend"
fi
if test "x$MKDEP" = "x"; then
  AC_MSG_RESULT(assuming $CC -MM)
  MKDEP="$CC -MM"
fi

AC_SUBST(MKDEP)

dnl Checks for header files.
AC_CHECK_HEADERS(link.h,,,[-])
AC_CHECK_HEADERS([dlfcn.h])

dnl Buildsys module stuff
BUILDSYS_INIT
BUILDSYS_SHARED_LIB
BUILDSYS_PROG_IMPLIB
LIBS="$LIBS $DYNAMIC_LD_LIBS"

dnl Checks for library functions.
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([strdup inet_pton inet_ntop gettimeofday umask mmap arc4random getrlimit fork getpid execve])
AC_CHECK_FUNC(socket,, AC_CHECK_LIB(socket, socket))
AC_CHECK_FUNC(gethostbyname,, AC_CHECK_LIB(nsl, gethostbyname))
AC_CHECK_FUNC(inflate,, AC_CHECK_LIB(z, inflate))
AC_CHECK_LIB([dl], [dlsym])
AC_CHECK_LIB([elf], [dlsym])
AC_CHECK_LIB([pthread], [pthread_join])
AC_CHECK_LIB([usb-1.0], [libusb_init])
AC_CHECK_LIB([readline], [readline])
AC_CHECK_LIB([curl], [curl_free])
AC_CHECK_LIB([curldll], [curl_free])
AC_CHECK_LIB([crypto], [SHA1_Init])
AC_CHECK_LIB([ssl], [SHA1_Init])


AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_UID_T
AC_C_INLINE
AC_C_BIGENDIAN
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_HEADER_TIME
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_C_VOLATILE
AC_PROG_CC_C99



dnl gettextize's Makefile.in wants this
mkdir_p="mkdir -p"
AC_SUBST(mkdir_p)

DOCDIR='${prefix}/doc'
MODDIR='${exec_prefix}'
SHAREDIR='${prefix}'
LOGDIR='${prefix}/var'
DATADIR='${prefix}/etc'
RUNDIR='${prefix}/var'

ABSPATHS="#"
AC_MSG_CHECKING(if you want to use absolute paths when building)
AC_ARG_ENABLE(absolute-paths,
AC_HELP_STRING([--enable-absolute-paths],[ Use absolute paths when building.]),
[
   case "$enableval" in
   yes)
      case "$srcdir" in
         /*)
         ;;
         *)
            oldsrcdir="$srcdir"
            srcdir="`cd \"$srcdir\";pwd`"
         ;;
      esac
      ABSPATHS=""
      ;;
   no)
      ABSPATHS="#"
      ;;
   esac
])
AC_MSG_RESULT($ABSPATHS)

SRCDIR="$srcdir"
BUILDDIR=`pwd`

FHSPATHS="no"
AC_MSG_CHECKING(if you want FHS-like pathnames)
AC_ARG_ENABLE(fhs-paths,
AC_HELP_STRING([--enable-fhs-paths],[ Use more FHS-like pathnames (for packagers).]),
[
  case "$enableval" in
  yes)
    FHSPATHS="yes"
    DOCDIR='${datadir}/doc/atheme'
    MODDIR='${libdir}/atheme'
    SHAREDIR='${datadir}/atheme'
    LOGDIR='${localstatedir}/log/atheme'
    DATADIR='${localstatedir}/lib/atheme'
    RUNDIR='${localstatedir}/run/atheme'
    ;;
  no)
    FHSPATHS="no"
    ;;
  esac
])
AC_MSG_RESULT($FHSPATHS)
AC_SUBST(DOCDIR)
AC_SUBST(MODDIR)
AC_SUBST(SHAREDIR)
AC_SUBST(DATADIR)
AC_SUBST(LOGDIR)
AC_SUBST(RUNDIR)
AC_SUBST(ABSPATHS)
AC_SUBST(SRCDIR)
AC_SUBST(BUILDDIR)
dnl the following is wrong! -- jilles
dnl AC_DEFINE_UNQUOTED([MODDIR], "${MODDIR}", [Module root directory.])


ATHEME_C_GCC_TRY_FLAGS([-std=gnu99], atheme_cv_c_gcc_std_gnu99)
ATHEME_C_GCC_TRY_FLAGS([-Werror-implicit-function-declaration], atheme_cv_c_gcc_w_error_implicit_function_declaration)

AC_ARG_ENABLE([warnings],
	[AS_HELP_STRING([--enable-warnings],[ Enable compiler warnings])],
	[], [enable_warnings=no])
AS_IF([test "x$enable_warnings" = "xyes"],
[
dnl See what warnings we can get away with
ATHEME_C_GCC_TRY_FLAGS([-Wall], atheme_cv_c_gcc_w_all)
ATHEME_C_GCC_TRY_FLAGS([-Wpointer-arith], atheme_cv_c_gcc_w_pointer_arith)
ATHEME_C_GCC_TRY_FLAGS([-Wimplicit -Wnested-externs], atheme_cv_c_gcc_w_implicit)
ATHEME_C_GCC_TRY_FLAGS([-Wcast-align], atheme_cv_c_gcc_w_cast_align)
ATHEME_C_GCC_TRY_FLAGS([-Wcast-qual], atheme_cv_c_gcc_w_cast_qual)
ATHEME_C_GCC_TRY_FLAGS([-Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations], atheme_cv_c_gcc_prototypes)
ATHEME_C_GCC_TRY_FLAGS([-Wparenthesis], atheme_cv_c_gcc_parenthesis)
ATHEME_C_GCC_TRY_FLAGS([-W -Wno-unused], atheme_cv_c_gcc_w)
ATHEME_C_GCC_TRY_FLAGS([-Wextra], atheme_cv_c_gcc_w_extra)
ATHEME_C_GCC_TRY_FLAGS([-Wshadow], atheme_cv_c_gcc_w_shadow)
ATHEME_C_GCC_TRY_FLAGS([-Wundef], atheme_cv_c_gcc_w_undef)
ATHEME_C_GCC_TRY_FLAGS([-Wpacked], atheme_cv_c_gcc_w_packed)
ATHEME_C_GCC_TRY_FLAGS([-Wnested-externs], atheme_cv_c_gcc_w_nested_externs)
ATHEME_C_GCC_TRY_FLAGS([-Wbad-function-cast], atheme_cv_c_gcc_w_bad_function_cast)
ATHEME_C_GCC_TRY_FLAGS([-Wunused-function -Wunused-label -Wunused-value -Wunused-variable], atheme_cv_c_gcc_w_unused)
ATHEME_C_GCC_TRY_FLAGS([-Wredundant-decls], atheme_cv_c_gcc_w_redundant_decls)
ATHEME_C_GCC_TRY_FLAGS([-Wfloat-equal], atheme_cv_c_gcc_w_float_equal)
dnl Added -fno-builtin-strftime to squelch a few (pointless) warnings from the time_format patch.
ATHEME_C_GCC_TRY_FLAGS([-Wformat=2 -fno-builtin-strftime], atheme_cv_c_gcc_w_format)
ATHEME_C_GCC_TRY_FLAGS([-Wdisabled-optimization], atheme_cv_c_gcc_w_disabled_optimization)
dnl ATHEME_C_GCC_TRY_FLAGS([-pedantic], atheme_cv_c_gcc_pedantic)

dnl Too unreliable to be useful at this time.
dnl ATHEME_C_GCC_TRY_FLAGS([-Wunreachable-code], atheme_cv_c_gcc_w_unreachable_code)

dnl Maintainer flags.
dnl ATHEME_C_GCC_TRY_FLAGS([-Wconversion], atheme_cv_c_gcc_w_conversion)
dnl ATHEME_C_GCC_TRY_FLAGS([-Wwrite-strings], atheme_cv_c_gcc_w_strings)
dnl ATHEME_C_GCC_TRY_FLAGS([-Werror], atheme_cv_c_gcc_w_error)
])

AC_ARG_ENABLE([propolice],
	[AS_HELP_STRING([--disable-propolice],[ Disable propolice protections (for debugging.)])],
	[], [enable_propolice=no])
AS_IF([test "x$enable_propolice" = "xyes"],
[
ATHEME_C_GCC_TRY_FLAGS([-fno-stack-protector-all], atheme_cv_nspa)
ATHEME_C_GCC_TRY_FLAGS([-fno-pie], atheme_cv_npie)
ATHEME_C_GCC_TRY_FLAGS([-nonow], atheme_cv_nonow)
ATHEME_C_GCC_TRY_FLAGS([-norelro], atheme_cv_no_relro)
])

AC_ARG_ENABLE([profile],
	[AS_HELP_STRING([--enable-profile],[ Enable profiling extensions])],
	[], [enable_profile=no])
AS_IF([test "x$enable_profile" = "xyes"],
[
ATHEME_C_GCC_TRY_FLAGS([-pg], atheme_cv_pg)
ac_save_LDFLAGS="${LDFLAGS} -pg"
LDFLAGS=${ac_save_LDFLAGS}
])

ac_save_CPPFLAGS="${CFLAGS} ${orig_CFLAGS} ${CWARNS} -DPREFIX=\\\"\${prefix}\\\" -DLOCALEDIR=\\\"\${LOCALEDIR}\\\" -DMODDIR=\\\"\${MODDIR}\\\" -DSHAREDIR=\\\"${SHAREDIR}\\\" -DSYSCONFDIR=\\\"${sysconfdir}\\\" -DLOGDIR=\\\"${LOGDIR}\\\" -DRUNDIR=\\\"${RUNDIR}\\\" -DDATADIR=\\\"${DATADIR}\\\""
CPPFLAGS=${ac_save_CPPFLAGS}
AC_SUBST(CPPFLAGS)
dnl export CFLAGS

enablerpath="yes"
AC_ARG_ENABLE(rpath,
AC_HELP_STRING([--disable-rpath],[Disable use of -Wl,-rpath= during linking.]),
   [enablerpath="$enableval"], [enablerpath="yes"])
RPATH=""

AC_SUBST(RPATH)

CONTRIB="no"
AC_MSG_CHECKING(if we should compile contrib modules)
AC_ARG_ENABLE(contrib,
AC_HELP_STRING([--enable-contrib], [ Enable contrib modules. ]),
[
 case "$enableval" in
     yes)
        CONTRIB="yes"
        ;;
    no)
        CONTRIB="no"
        ;;
    esac
])
AC_MSG_RESULT($CONTRIB)

if test "$CONTRIB" = "yes"; then
    CONTRIB_ENABLE="contrib"
else
    CONTRIB_ENABLE=""
fi

AC_SUBST(CONTRIB_ENABLE)

OUTPUT_STACKTRACE="no"
AC_ARG_ENABLE(stack,
AC_HELP_STRING([--enable-stack], [ Enable a stack trace on crash. ]),
[
 AC_MSG_CHECKING(for libunwind)
 case "$enableval" in
     yes)
        platform=`uname -m`
        case "$platform" in
            i686)
                platform=x86
                ;;
            amd64)
                platform=x86_64
                ;;
            i386)
                platform=x86
                ;;
        esac

        AC_CHECK_HEADER(libunwind.h, [OUTPUT_STACKTRACE="yes"
                                      AC_DEFINE(ENABLE_STACK, 1, "Enable stack tracing through libunwind.")
                                      STACKTRACE_LIBS="-lunwind -lunwind-$platform"],
                                      [OUTPUT_STACKTRACE="libunwind not found"])
        ;;
    no)
        OUTPUT_STACKTRACE="no"
        ;;
    esac
])
AC_MSG_RESULT($OUTPUT_STACKTRACE)
AC_SUBST(STACKTRACE_LIBS)

BUILDROOT=`pwd`
AC_SUBST(BUILDROOT)

BUILDSYS_TOUCH_DEPS

AC_OUTPUT([
	buildsys.mk
	extra.mk
])

d=`eval echo ${MODDIR}`
d=`eval echo $d`
echo "
Configuration:
	opensn0w version     : ${PACKAGE_VERSION}
	Installation prefix  : ${prefix}
	Module root directory: `eval echo $d`
	Config directory     : ${sysconfdir}
	Contrib modules      : ${CONTRIB}
	Stacktrace           : ${OUTPUT_STACKTRACE}
	CFLAGS               : ${CFLAGS}

Type make to build opensn0w, and make install to install it."
