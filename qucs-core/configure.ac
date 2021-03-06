#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.64)
AC_INIT([qucs-core],[0.0.18],[qucs-bugs@lists.sourceforge.net])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
LT_PREREQ([2.2.2])
dnl use AM_PROG_AR if present
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT([disable-static dlopen win32-dll])
AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_SRCDIR([src/circuit.cpp])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

dnl AM_INIT_AUTOMAKE does several things, including defining VERSION and PACKAGE
AM_INIT_AUTOMAKE([no-define])
AM_MAINTAINER_MODE

AC_PREFIX_DEFAULT([/usr/local])
test "x$prefix" = xNONE && prefix="/usr/local"

dnl Checks for programs.
AC_PROG_CXX(clang++ g++)
AX_CXX_COMPILE_STDCXX_11
AC_PROG_CC
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_CC_C_O

dnl Check for parser and lexer generators.
AC_PROG_YACC
AC_PROG_LEX

if test "x$LEX" != "x:"; then
  if test $USE_MAINTAINER_MODE = yes; then
    AC_MSG_CHECKING([for $LEX version])
    [lexver=`eval $LEX --version 2>/dev/null | head -1 | sed 's/[^0-9\.]//g'`]
    if test -n "$lexver"; then
      case "$lexver" in
        [2.5.9 | [2-9].[5-9].[1-9][0-9])]
          AC_MSG_RESULT([$lexver >= 2.5.9])
        ;;
          [*)]
          AC_MSG_ERROR([$lexver < 2.5.9])
        ;;
      esac
    else
      AC_MSG_RESULT([not identified])
    fi
    unset lexver
  fi
  AC_MSG_CHECKING([whether $LEX accepts --nounistd])
  cat <<EOF > lextest.l
%%
token     action;
EOF
  lexout="`$LEX --nounistd -olextest.c lextest.l 2>&1`"
  rm -f lextest.l lextest.c
  if test "x$lexout" != "x"; then
    AC_MSG_RESULT([no])
  else
    AC_MSG_RESULT([yes])
    LEX="$LEX --nounistd"
  fi
  unset lexout
fi

dnl Check for debugging.
AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug],
                 [include debug output @<:@default=no@:>@]),
  [case "$enableval" in
   yes) enable_debug="yes" ;;
   no)  enable_debug="no"  ;;
   *)   enable_debug="no"  ;;
   esac],
  [enable_debug="no"])
if test $USE_MAINTAINER_MODE = yes; then
  enable_debug="yes"
fi
if test "$enable_debug" = yes; then
  AC_DEFINE(DEBUG, 1, [Define if debug output should be supported.])
else
  AC_DEFINE(NDEBUG, 1, [Define if debug code should be suppressed.])
  if test "x$GCC" = xyes; then
    CFLAGS="`echo $CFLAGS | sed -e 's/\-g //g'`"
    CXXFLAGS="`echo $CXXFLAGS | sed -e 's/\-g //g'`"
# disable stripping (debian bug #437876)
#    case $host_os in
#      *darwin*) LDFLAGS="$LDFLAGS" ;;
#      *)        LDFLAGS="$LDFLAGS -s" ;;
#    esac
  fi
fi

dnl append -O0 to CXXFLAGS if in debug mode and using gcc
dnl to assist with debugging by preventing optimisation.
dnl gcc will use the final invocation of -OX and ignore earlier
dnl values
if test "$enable_debug" = yes; then
 if test "x$GCC" = xyes; then
    CXXFLAGS="$CXXFLAGS -O0"
    AC_MSG_NOTICE([Appending gcc optimisation flag -O0 due to --enable-debug setting.])
 fi
fi
unset enable_debug


dnl MacOSX build and runtime environment options
dnl   borrowed from http://cgit.freedesktop.org/libreoffice/core/tree/configure.ac

dnl Default-enabled search for SDK
AC_ARG_ENABLE([sdk],
  AS_HELP_STRING([--enable-sdk],
                 [search for Mac OSX SDK @<:@default=yes@:>@. \
                  Disable this option to set custom compiler flags CXX, CXXFLAGS]),
  [case "$enableval" in
   yes) enable_sdk="yes" ;;
   no)  enable_sdk="no"  ;;
   *)   enable_sdk="yes"  ;;
   esac],
  [enable_sdk="yes"])

dnl Specify version of SDK
AC_ARG_WITH(macosx-sdk,
    AS_HELP_STRING([--with-macosx-sdk],
        [Use a specific SDK for building.])
    [
                          Usage:     --with-macosx-sdk=<version>

                          e. g.: --with-macosx-sdk=10.6

                          there are 3 options to control the MacOSX build:
                          --with-macosx-sdk (refered as 'sdk' below)
                          --with-macosx-version-min-required (refered as 'min' below)
                          --with-macosx-version-max-allowed (refered as 'max' below)

                          the connection between these value and the default they take is as follow:
                          ( ? means not specified on the command line, s means the SDK version found,
                          constraint: x <= y <= z)

                          ==========================================
                           command line      || config result
                          ==========================================
                          min  | max  | sdk  || min  | max  | sdk  |
                          ?    | ?    | ?    || 10.6 | 10.s | 10.s |
                          ?    | ?    | 10.x || 10.6 | 10.x | 10.x |
                          ?    | 10.x | ?    || 10.6 | 10.s | 10.s |
                          ?    | 10.x | 10.y || 10.6 | 10.x | 10.y |
                          10.x | ?    | ?    || 10.x | 10.s | 10.s |
                          10.x | ?    | 10.y || 10.x | 10.y | 10.y |
                          10.x | 10.y | ?    || 10.x | 10.y | 10.y |
                          10.x | 10.y | 10.z || 10.x | 10.y | 10.z |


                          see: http://developer.apple.com/library/mac/#technotes/tn2064/_index.html
                          for a detailled technical explanation of these variables

                          Note: MACOSX_DEPLOYMENT_TARGET will be set to the value of 'min'.

                          Note that even if in theory using a --with-macosx-version-max-allowed
                          (i.e. the MAC_OS_X_VERSION_MAX_ALLOWED macro) less than the SDK version
                          should work, in practice Apple does not seem to test that, and at least
                          compiling with -DMAC_OS_X_VERSION_MAX_ALLOWED=1060 against the 10.7 SDK
                          fails in a couple of places. Just because of oversights in ifdefs in the SDK
                          headers, but still.
    ],
,)

AC_ARG_WITH(macosx-version-min-required,
    AS_HELP_STRING([--with-macosx-version-min-required],
        [set the minimum OS version needed to run the built Qucs-core])
    [
                          Usage:     --with-macosx-version-min-required=<version>

                          e. g.: --with-macosx-version-min-required=10.6
                          see --with-macosx-sdk for more info
    ],
,)

AC_ARG_WITH(macosx-version-max-allowed,
    AS_HELP_STRING([--with-macosx-version-max-allowed],
        [set the maximum allowed OS version the Qucs-core compilation can use APIs from])
    [
                          Usage:     --with-macosx-version-max-allowed=<version>

                          e. g.: --with-macosx-version-max-allowed=10.6
                          see --with-macosx-sdk for more info
    ],
,)




case $host_os in
  *linux* | *cygwin* )

  dnl Set Clang
  dnl STRICT_ANSI: http://stackoverflow.com/questions/13525774/clang-and-float128-bug-error
  if test "$CXX -dM -E - < /dev/null | grep __clang__" ; then
    CFLAGS="$CFLAGS -pipe"
    CXXFLAGS="$CXXFLAGS -pipe -fno-exceptions -ldl -D__STRICT_ANSI__"
    use_CLANG="yes"
  fi

  dnl Set GCC
  if test "x$use_CLANG" != "xyes" -a "x$GCC" = "xyes" ; then
    CFLAGS="$CFLAGS -pipe"
    CXXFLAGS="$CXXFLAGS -pipe -fno-exceptions -fno-check-new -ldl -rdynamic"
  fi

  if test $USE_MAINTAINER_MODE = yes; then
    CFLAGS="$CFLAGS -W -Wall -Wmissing-prototypes"
    CXXFLAGS="$CXXFLAGS -W -Wall"
  fi
  ;;

  *darwin* )

  dnl ===================================================================
  dnl Test if search for SDK was disabled
  AS_IF([test "x$enable_sdk" = "xyes"], [

  dnl Check OS X SDK and compiler
  dnl borrowed from http://cgit.freedesktop.org/libreoffice/core/tree/configure.ac
  AC_MSG_CHECKING([what Mac OS X SDK to use])

  bitness=-m64

  if test -z "$with_macosx_sdk"; then
      if test -d /Developer/SDKs/MacOSX10.6.sdk; then
          with_macosx_sdk=10.6
      elif test -d /Developer-old/SDKs/MacOSX10.6.sdk; then
          with_macosx_sdk=10.6
      elif test -d /Xcode3/SDKs/MacOSX10.6.sdk; then
          with_macosx_sdk=10.6
      elif test -d /Developer/SDKs/MacOSX10.7.sdk; then
          with_macosx_sdk=10.7
      elif test -d "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk"; then
          with_macosx_sdk=10.6
      elif test -d "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk"; then
          with_macosx_sdk=10.7
      elif test -d "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk"; then
          with_macosx_sdk=10.8
      elif test -d "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk"; then
          with_macosx_sdk=10.9
      elif test -x /usr/bin/xcode-select; then
          xcodepath="`xcode-select -print-path`"
          if test -d "$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk"; then
              with_macosx_sdk=10.6
          elif test -d "$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk"; then
              with_macosx_sdk=10.7
          elif test -d "$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk"; then
              with_macosx_sdk=10.8
          elif test -d "$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk"; then
              with_macosx_sdk=10.9
          fi
      fi
      if test -z "$with_macosx_sdk"; then
          AC_MSG_ERROR([Could not figure out the location of a Mac OS X SDK and its version])
      fi
  fi

  case $with_macosx_sdk in
  10.5)
      MACOSX_SDK_VERSION=1050
      ;;
  10.6)
      MACOSX_SDK_VERSION=1060
      ;;
  10.7)
      MACOSX_SDK_VERSION=1070
      ;;
  10.8)
      MACOSX_SDK_VERSION=1080
      ;;
  10.9)
      MACOSX_SDK_VERSION=1090
      ;;
  10.10)
      MACOSX_SDK_VERSION=101000
      ;;
  *)
      AC_MSG_ERROR([with-macosx-sdk $with_macosx_sdk is not a supported value, supported values are 10.5--10])
      ;;
  esac

  # Next find it (again, if we deduced its version above by finding
  # it... but we need to look for it once more in case
  # --with-macosx-sdk was given so that the above search did not
  # happen).
  if test -z "$MACOSX_SDK_PATH"; then
      case $with_macosx_sdk in
      10.5)
          if test -x /usr/bin/xcode-select; then
              xcodepath="`xcode-select -print-path`"
              if test -d "$xcodepath/SDKs/MacOSX$with_macosx_sdk.sdk"; then
                  MACOSX_SDK_PATH="$xcodepath/SDKs/MacOSX$with_macosx_sdk.sdk"
              fi
          elif test -d /Developer/SDKs/MacOSX10.5.sdk; then
              MACOSX_SDK_PATH=/Developer/SDKs/MacOSX10.5.sdk
          elif test -d /Developer-old/SDKs/MacOSX10.5.sdk; then
              MACOSX_SDK_PATH=/Developer-old/SDKs/MacOSX10.5.sdk
          elif test -d /Xcode3/SDKs/MacOSX10.5.sdk; then
              MACOSX_SDK_PATH=/Xcode3/SDKs/MacOSX10.5.sdk
          fi
          ;;
      10.6)
          if test -d /Developer/SDKs/MacOSX10.6.sdk; then
              MACOSX_SDK_PATH=/Developer/SDKs/MacOSX10.6.sdk
          elif test -d /Developer-old/SDKs/MacOSX10.6.sdk; then
              MACOSX_SDK_PATH=/Developer-old/SDKs/MacOSX10.6.sdk
          elif test -d /Xcode3/SDKs/MacOSX10.6.sdk; then
              MACOSX_SDK_PATH=/Xcode3/SDKs/MacOSX10.6.sdk
          elif test -d "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk"; then
              MACOSX_SDK_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk
          elif test -x /usr/bin/xcode-select; then
              xcodepath="`xcode-select -print-path`"
              if test -d "$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk"; then
                  MACOSX_SDK_PATH="$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk"
              fi
          fi
          ;;
      10.7)
          if test -d /Developer/SDKs/MacOSX$with_macosx_sdk.sdk; then
              MACOSX_SDK_PATH=/Developer/SDKs/MacOSX$with_macosx_sdk.sdk
          elif test -d "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk"; then
              MACOSX_SDK_PATH=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk
          elif test -x /usr/bin/xcode-select; then
              xcodepath="`xcode-select -print-path`"
              if test -d "$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk"; then
                  MACOSX_SDK_PATH="$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk"
              fi
          fi
          ;;
      10.8|10.9|10.10)
          xcodepath="`xcode-select -print-path`"
          if test -d "$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk"; then
              MACOSX_SDK_PATH="$xcodepath/Platforms/MacOSX.platform/Developer/SDKs/MacOSX$with_macosx_sdk.sdk"
          fi
          ;;
      esac
      if test -z "$MACOSX_SDK_PATH"; then
          AC_MSG_ERROR([Could not figure out the location of Mac OS X $with_macosx_sdk SDK])
      fi
  fi
  AC_MSG_RESULT([SDK $with_macosx_sdk at $MACOSX_SDK_PATH])

  if test "$with_macosx_version_min_required" = ""; then
      case $with_macosx_sdk in
      10.5)
          with_macosx_version_min_required="10.5";;
      *)
          with_macosx_version_min_required="10.6";;
      esac
  fi

  if test "$with_macosx_version_max_allowed" = ""; then
      with_macosx_version_max_allowed="$with_macosx_sdk"
  fi

  # export this so that "xcrun" invocations later return matching values
  DEVELOPER_DIR="${MACOSX_SDK_PATH%/SDKs*}"
  DEVELOPER_DIR="${DEVELOPER_DIR%/Platforms*}"
  export DEVELOPER_DIR
  FRAMEWORKSHOME="$MACOSX_SDK_PATH/System/Library/Frameworks"
  MACOSX_DEPLOYMENT_TARGET="$with_macosx_version_min_required"

  case "$with_macosx_version_min_required" in
  10.5)
      MAC_OS_X_VERSION_MIN_REQUIRED="1050"
      ;;
  10.6)
      MAC_OS_X_VERSION_MIN_REQUIRED="1060"
      ;;
  10.7)
      MAC_OS_X_VERSION_MIN_REQUIRED="1070"
      ;;
  10.8)
      MAC_OS_X_VERSION_MIN_REQUIRED="1080"
      ;;
  10.9)
      MAC_OS_X_VERSION_MIN_REQUIRED="1090"
      ;;
  10.10)
      MAC_OS_X_VERSION_MIN_REQUIRED="101000"
      ;;
  *)
      AC_MSG_ERROR([with-macosx-version-min-required $with_macosx_version_min_required is not a supported value, supported values are 10.5--10])
      ;;
  esac

  if test "$BITNESS_OVERRIDE" = ""; then
      case "$with_macosx_version_min_required" in
      10.5)
          case "$with_macosx_sdk" in
          10.5)
              ;;
          *)
              AC_MSG_WARN([Building with a SDK > 10.5 possibly breaks 10.5 compatibility.])
              add_warning "Building with a SDK > 10.5 possibly breaks 10.5 compatibility."
              ;;
          esac
          ;;
      esac
  fi

  # If no CC and CXX environment vars, try to guess where the compiler is
  #LIBTOOL=libtool
  INSTALL_NAME_TOOL=install_name_tool
  if test -z "$save_CC"; then
      AC_MSG_CHECKING([what compiler to use])
      case $with_macosx_sdk in
      10.5)
          CC="${gccprefix}gcc-4.2 $arch -mmacosx-version-min=$with_macosx_version_min_required -isysroot $MACOSX_SDK_PATH"
          CXX="${gccprefix}g++-4.2 $arch -mmacosx-version-min=$with_macosx_version_min_required -isysroot $MACOSX_SDK_PATH"
          INSTALL_NAME_TOOL=`xcrun -find install_name_tool`
          ;;
      10.6)
          # did someone copy her 10.6 sdk into xcode 4 (needed on Mountain Lion)?
          if test "$(echo $MACOSX_SDK_PATH | cut -c1-23)" = "/Applications/Xcode.app"; then
              CC="`xcrun -find gcc` $bitness -mmacosx-version-min=$with_macosx_version_min_required -isysroot $MACOSX_SDK_PATH"
              CXX="`xcrun -find g++` $bitness -mmacosx-version-min=$with_macosx_version_min_required -isysroot $MACOSX_SDK_PATH"
          else
              CC="gcc-4.2 $bitness -mmacosx-version-min=$with_macosx_version_min_required -isysroot $MACOSX_SDK_PATH"
              CXX="g++-4.2 $bitness -mmacosx-version-min=$with_macosx_version_min_required -isysroot $MACOSX_SDK_PATH"
          fi
          INSTALL_NAME_TOOL=`xcrun -find install_name_tool`
          #LIBTOOL=libtool
          ;;
      10.7|10.8|10.9|10.10)
          if test "$with_macosx_version_min_required" != 10.6; then
              # Use libc++ instead of libstdc++ when possible
              stdlib=-stdlib=libc++
          fi
          #if test "$ENABLE_LTO" = TRUE; then
          #    lto=-flto
          #fi
          CC="`xcrun -find clang` $bitness $lto -mmacosx-version-min=$with_macosx_version_min_required -isysroot $MACOSX_SDK_PATH"
          CXX="`xcrun -find clang++` $bitness $lto $stdlib -mmacosx-version-min=$with_macosx_version_min_required -isysroot $MACOSX_SDK_PATH"
          INSTALL_NAME_TOOL=`xcrun -find install_name_tool`
          AR=`xcrun -find ar`
          NM=`xcrun -find nm`
          STRIP=`xcrun -find strip`
          #LIBTOOL=`xcrun -find libtool`
          RANLIB=`xcrun -find ranlib`
          ;;
      esac
      AC_MSG_RESULT([$CC and $CXX])
  fi

  case "$with_macosx_version_max_allowed" in
  10.5)
      MAC_OS_X_VERSION_MAX_ALLOWED="1050"
      ;;
  10.6)
      MAC_OS_X_VERSION_MAX_ALLOWED="1060"
      ;;
  10.7)
      MAC_OS_X_VERSION_MAX_ALLOWED="1070"
      ;;
  10.8)
      MAC_OS_X_VERSION_MAX_ALLOWED="1080"
      ;;
  10.9)
      MAC_OS_X_VERSION_MAX_ALLOWED="1090"
      ;;
  10.10)
      MAC_OS_X_VERSION_MAX_ALLOWED="101000"
      ;;
  *)
      AC_MSG_ERROR([with-macosx-version-max-allowed $with_macosx_version_max_allowed is not a supported value, supported values are 10.5--10])
      ;;
  esac

  AC_MSG_CHECKING([that macosx-version-min-required is coherent with macosx-version-max-allowed])
  if test $MAC_OS_X_VERSION_MIN_REQUIRED -gt $MAC_OS_X_VERSION_MAX_ALLOWED; then
      AC_MSG_ERROR([the version minimumn required must be inferior or equal to the version maximum allowed])
  else
      AC_MSG_RESULT([ok])
  fi

  AC_MSG_CHECKING([that macosx-version-max-allowed is coherent with macos-with-sdk])
  if test $MAC_OS_X_VERSION_MAX_ALLOWED -gt $MACOSX_SDK_VERSION; then
      AC_MSG_ERROR([the version maximum allowed cannot be greater thatn the sdk level])
  else
      AC_MSG_RESULT([ok])
  fi
  AC_MSG_NOTICE([MAC_OS_X_VERSION_MIN_REQUIRED=$MAC_OS_X_VERSION_MIN_REQUIRED])
  AC_MSG_NOTICE([MAC_OS_X_VERSION_MAX_ALLOWED=$MAC_OS_X_VERSION_MAX_ALLOWED])


  if test $USE_MAINTAINER_MODE = yes; then
    CFLAGS="$CFLAGS -W -Wall -Wmissing-prototypes"
    CXXFLAGS="$CXXFLAGS -W -Wall"
  fi

  dnl End test for enable_sdk
  ])

  ;;

  *mingw* )
  if test "x$GCC" = xyes; then
    CFLAGS="$CFLAGS -pipe"
    CXXFLAGS="$CXXFLAGS -pipe -fno-exceptions -fno-check-new"
    if test $USE_MAINTAINER_MODE = yes; then
      CFLAGS="$CFLAGS -W -Wall -Wmissing-prototypes"
      CXXFLAGS="$CXXFLAGS -W -Wall"
    fi
  fi
  ;;
esac

dnl Check for new platforms.
osversion="`grep \"$host\" $srcdir/doc/PLATFORMS 2>/dev/null`"
if test "x$osversion" = "x"; then
  AC_MSG_RESULT([
  The $PACKAGE package has never been compiled or run on this platform.
  Please send an email to <qucs-devel@lists.sourceforge.net> including your
  compiler version and whether it worked correctly or not.  '$host'
  will then be added to the PLATFORMS file included in this package.
])
fi
unset osversion

dnl Check for additional programs.
AC_PATH_PROG(GPERF, gperf, :)
if test $USE_MAINTAINER_MODE = yes; then
  if test "$GPERF" = ":"; then
    AC_MSG_ERROR([The $PACKAGE package requires an installed gperf.])
  fi
fi

dnl check for adms
if test $USE_MAINTAINER_MODE = yes; then
  dnl now, to build or not to build the adms subproject. The adms
  dnl subproject is required to generate code during the maintainer
  dnl build process. It is not required for distributions. It is
  dnl only required if MAINTAINER_MODE is true in the Makefile.am's
  dnl
  dnl The default behaviour is "check for adms, if it's not there,
  dnl build and use the bundled copy". Note also that it supports
  dnl --with-mkadms=PATH (user may have a copy installed in an odd
  dnl place), --with-mkadms=internal (needed for distcheck, where
  dnl we want to test everything) and --with-mkadms=external (for
  dnl package maintainers, who don't like bundled subprojects).
  AC_ARG_WITH([mkadms],
    [AS_HELP_STRING([--with-mkadms],
      [Either the full path to adms, or "external", "internal", or "check" @<:@check@:>@])],
    [ADMSXML=$withval],
    [with_mkadms=check])
  AS_IF([test "$with_mkadms" = check -o "$with_mkadms" = external],
    [AC_PATH_PROG([ADMSXML], [mkadms], [no])
     AC_MSG_NOTICE([Using host external admsXml rather than bundled version.])],
    [AC_MSG_NOTICE([Using bundled admsXml.])])
  AS_IF([test "$with_mkadms" = external -a "$ADMSXML" = no],
    [AC_MSG_ERROR([External adms demanded, but not found.])])
  dnl We conditionally set ADMSXML in the top level Makefile.am in
  dnl qucs-core. AM_SUBST_NOTMAKE([ADMSXML]) stops automake from
  dnl generating a line of the form ADMSXML = @ADMSXML@ in Makefile.in.
  dnl We need to do that assignment conditionally.
  AM_SUBST_NOTMAKE([ADMSXML])
else
  dnl not maintainer mode, set with_mkadms to external which will prevent it
  dnl being built
  with_mkadms = external
fi

dnl We set up an automake conditional because we need to do
dnl different stuff for internal/external adms inside Makefile.am
AM_CONDITIONAL([USE_INTERNAL_ADMS],
  [test "$with_mkadms" = internal -o "$ADMSXML" = no])
dnl if we are using internal adms we need to add it to the
dnl subdirectories autoconf will recurse into as it has its
dnl own configure.ac and Makefile.am
AM_COND_IF([USE_INTERNAL_ADMS], [AC_CONFIG_SUBDIRS([deps/adms/])])

dnl Checks for libraries.
AC_CHECK_LIB(m, sin)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdlib.h string.h unistd.h ieeefp.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl Check for type sizes.
AC_CHECK_SIZEOF([short])
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([long])
type32="int"
if test $ac_cv_sizeof_short -eq 4; then
  type32=short
elif test $ac_cv_sizeof_int -eq 4; then
  type32=int
elif test $ac_cv_sizeof_long -eq 4; then
  type32=long
fi
AC_DEFINE_UNQUOTED(nr_int32_t, $type32, [C-type for 32-bit integers.])
AC_SUBST(QUCS_INT32_TYPE, $type32)
unset type32
type16="short"
if test $ac_cv_sizeof_short -eq 2; then
  type16=short
elif test $ac_cv_sizeof_int -eq 2; then
  type16=int
elif test $ac_cv_sizeof_long -eq 2; then
  type16=long
fi
AC_DEFINE_UNQUOTED(nr_int16_t, $type16, [C-type for 16-bit integers.])
AC_SUBST(QUCS_INT16_TYPE, $type16)
unset type16

dnl Check for double type.
AC_ARG_ENABLE([double],
  AS_HELP_STRING([--enable-double=TYPE],
                 [type of double representation @<:@default=double@:>@]),
  [case "$enableval" in
   no | yes | double)
     double_type="double"
     double_size=8
     ;;
   ldouble)
     double_type="long double"
     AC_CHECK_SIZEOF([long double],12)
     double_size=$ac_cv_sizeof_long_double
     ;;
   float)
     double_type="float"
     double_size=4
     ;;
   *)
     double_type="none"
     double_size=0
     ;;
   esac],
  [double_type="double"; double_size=8])
if test "$double_type" = none; then
  AC_MSG_ERROR([valid types are double, float and ldouble.])
fi
AC_DEFINE_UNQUOTED(nr_double_t, $double_type,
  [The global type of double representation.])
AC_SUBST(QUCS_DOUBLE_TYPE, $double_type)
AC_DEFINE_UNQUOTED(NR_DOUBLE_SIZE, $double_size,
  [The size of the double representation.])
AC_SUBST(QUCS_DOUBLE_SIZE, $double_size)
unset double_type
unset double_size

echo "\nCheck functions in the global namespace"
dnl http://www.cplusplus.com/reference/cmath/

# Trigonometric functions
AC_CHECK_FUNCS([ \
cos \
sin \
tan \
acos \
asin \
atan \
atan2 \
])

#Hyperbolic functions
AC_CHECK_FUNCS([ \
cosh \
sinh \
tanh \
acosh \
asinh \
atanh \
])

#Exponential and logarithmic functions
AC_CHECK_FUNCS([ \
exp \
log \
log10 \
modf \
])

#Power functions
AC_CHECK_FUNCS([ \
pow \
sqrt \
hypot \
])

#Error and gamma functions
AC_CHECK_FUNCS([ \
erf \
erfc \
])

# Rounding and remainder functions
AC_CHECK_FUNCS([ \
ceil \
floor \
fmod \
trunc \
round \
])

# Other functions
AC_CHECK_FUNCS([ \
fabs \
])

# Bessel funcions (libc ?)
AC_CHECK_FUNCS([ jn yn ])

# C strings
# \bug strdup not in C++ STL
AC_CHECK_FUNCS([ strdup strerror strchr])

dnl Checks for complex classes and functions.
AC_CXX_NAMESPACES
AC_CXX_HAVE_COMPLEX
AS_IF([test "X$ac_cv_cxx_have_complex" = "X"],[AC_MSG_ERROR([Qucs needs a templated complex class])])


echo "\nCheck complex transcendental overload in namespace std"
dnl http://www.cplusplus.com/reference/complex/

AC_CHECK_CXX_COMPLEX_FUNCS([ \
  cos \
  cosh \
  exp \
  log \
  log2 \
  log10 \
dnl  pow \ skip, see below
  sin \
  sinh \
  sqrt \
  tan \
  tanh \
dnl C++11
  acos \
  acosh \
  asin \
  asinh \
  atan \
  atanh ])

dnl Check complex values in namespace std
AC_CHECK_CXX_COMPLEX_FUNCS([ \
  real \
  imag \
  abs \
  arg \
  norm \
  conj \
dnl  polar \   skip, see below
dnl C++11
  proj ])

dnl funcions with gwo arguments
AC_CHECK_CXX_COMPLEX_POW
AC_CHECK_CXX_COMPLEX_ATAN2
AC_CHECK_CXX_COMPLEX_FMOD
AC_CHECK_CXX_COMPLEX_POLAR
AC_CHECK_CXX_COMPLEX_POLAR_COMPLEX


dnl Depending on the implementation, funcions may or may not be included on the std namespace
echo "\nCheck for functions in std:: namespace"
 for f in std::acosh std::asinh std::atanh std::hypot std::trunc std::round std::erf std::erfc; do
    AC_MSG_CHECKING([for $f])
    AC_LINK_IFELSE([AC_LANG_SOURCE(
                                  [[#include <cmath>
                                  int main() { return $f(0.7) ? 1 : 0; }
                                  ]])],
      [AC_MSG_RESULT([yes])
       if test $f = "std::acosh"; then
         AC_DEFINE(HAVE_STD_ACOSH, 1,
		     [Define to 1 if you have the `std::acosh` function.])

       elif test $f = "std::asinh"; then
         AC_DEFINE(HAVE_STD_ASINH, 1,
	       [Define to 1 if you have the `std::asinh` function.])

       elif test $f = "std::atanh"; then
         AC_DEFINE(HAVE_STD_ATANH, 1,
	       [Define to 1 if you have the `std::atanh` function.])

       elif test $f = "std::hypot"; then
         AC_DEFINE(HAVE_STD_HYPOT, 1,
	       [Define to 1 if you have the `std::hypot` function.])

       elif test $f = "std::fmod"; then
         AC_DEFINE(HAVE_STD_FMOD, 1,
         [Define to 1 if you have the `std::fmod` function.])

       elif test $f = "std::trund"; then
         AC_DEFINE(HAVE_STD_TRUNC, 1,
         [Define to 1 if you have the `std::trunc` function.])

       elif test $f = "std::round"; then
         AC_DEFINE(HAVE_STD_ROUND, 1,
		     [Define to 1 if you have the `std::round` function.])

       elif test $f = "std::erf"; then
         AC_DEFINE(HAVE_STD_ERF, 1,
	       [Define to 1 if you have the `std::erf` function.])

       elif test $f = "std::erfc"; then
         AC_DEFINE(HAVE_STD_ERFC, 1,
	       [Define to 1 if you have the `std::erfc` function.])
       fi],
      [AC_MSG_RESULT([no])])
  done


dnl Setup Google Test support
dnl * copy of gtest source into qucs-core/lib/gtest/
dnl * run configure with --enable-gtest
dnl * make
dnl * make check (run all tests including gtest itself)
dnl * cd tests && make check  (run only Qucs unit tests)
dnl
dnl Reason for the nested package
dnl * gtest better be compiled with the same flags as the project
dnl * https://code.google.com/p/googletest/wiki/FAQ#Why_is_it_not_recommended_to_install_a_pre-compiled_copy_of_Goog

AC_ARG_ENABLE([gtest],
  AS_HELP_STRING([--enable-gtest], [Enable compilation of Google Test]))

dnl variable to steer ./Makefile.am to gtest and unit tests
AM_CONDITIONAL([USE_GTEST], [test "$enable_gtest" = yes])

AS_IF([test "x$enable_gtest" = "xyes"], [

  echo "\nEnabling compilation of Google Test library..."

  AC_CHECK_FILE([lib/gtest/configure], [],
      [AC_MSG_ERROR([gtest sources not found, cannot --enable-gtest])])

  dnl Set dir into Makefile.am if found
  GTEST_DIR=lib/gtest
  AC_SUBST([GTEST_DIR])

  dnl configure nested package
  AC_CONFIG_SUBDIRS([lib/gtest/])

])


dnl Definitions at top of <config.h>.
AH_TOP([
/* __BEGIN_DECLS should be used at the beginning of your declarations,
   so that C++ compilers do not mangle their names.  Use __END_DECLS at
   the end of C declarations. */
#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS
# define __END_DECLS
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
])

dnl Add here all your Makefiles. These are created by configure.
AC_CONFIG_FILES([Makefile
                 qucs_typedefs.h
                 doc/Makefile
                 src/Makefile
                 src/math/Makefile
                 src/interface/Makefile
                 src/converter/Makefile
                 src/components/Makefile
                 src/components/microstrip/Makefile
                 src/components/devices/Makefile
                 src/components/digital/Makefile
                 src/components/verilog/Makefile
                 tests/Makefile
                 ])
AC_OUTPUT

dnl Print results.
AC_MSG_RESULT([])
AC_MSG_RESULT([ $PACKAGE version $VERSION configured successfully.])
AC_MSG_RESULT([
Configure Information:
  C Compiler        : $CC
    DEFS            :   $DEFS
    CPPFLAGS        :   $CPPFLAGS
    CFLAGS          :   $CFLAGS

  C++ Compiler      : $CXX
    DEFS            :   $DEFS
    CPPFLAGS        :   $CPPFLAGS
    CXXFLAGS        :   $CXXFLAGS

  Linker            : $LD
    LDFLAGS         :   $LDFLAGS
    LIBS            :   $LIBS
])
