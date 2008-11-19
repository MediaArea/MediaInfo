dnl #########################################################################
dnl ### Init
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Test if we are at the good place
dnl
AC_INIT(../../../Source/CLI/CLI_Main.cpp)

dnl -------------------------------------------------------------------------
dnl sets build, host, target variables and the same with _alias
dnl
AC_CANONICAL_BUILD

dnl -------------------------------------------------------------------------
dnl Name and version
dnl
AM_INIT_AUTOMAKE(MediaInfo,0)

dnl -------------------------------------------------------------------------
dnl Autotools init
dnl
CFLAGS="$CFLAGS"
CXXFLAGS="$CXXFLAGS"
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL

dnl #########################################################################
dnl ### Options
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Arguments - Enable
dnl
AC_ARG_ENABLE(debug,        AC_HELP_STRING([--enable-debug],        [Enable full debug]),                       , enable_debug=no)
AC_ARG_ENABLE(gprof,        AC_HELP_STRING([--enable-gprof],        [Enable gprof log]),                        , enable_gprof=no)
AC_ARG_ENABLE(staticlibs,   AC_HELP_STRING([--enable-staticlibs],   [Use static libmediainfo and libzen]),      , enable_staticlibs=no)

dnl -------------------------------------------------------------------------
dnl Arguments - With
dnl
AC_ARG_WITH(dll,            AC_HELP_STRING([--with-dll],            [Do not link to libmediainfo, dynamic loading]), , with_dll=no)

dnl -------------------------------------------------------------------------
dnl External options
dnl

dnl #########################################################################
dnl ### Options from elsewhere
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
if test -e ../../../../ZenLib/Project/GNU/Library/libzen-config; then
	enable_unicode="$(../../../../ZenLib/Project/GNU/Library/libzen-config Unicode)"
else
	if pkg-config --exists libzen; then
		enable_unicode="$(pkg-config --variable=Unicode libzen)"
	else
		AC_MSG_ERROR([libzen configuration is not found])
	fi
fi

dnl #########################################################################
dnl ### Parsing options
dnl #########################################################################


dnl #########################################################################
dnl ### MediaInfo flags
dnl #########################################################################


dnl #########################################################################
dnl ### C/C++ compiler options used to compile
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
if test "$enable_unicode" = "yes"; then
	AC_DEFINE(UNICODE)
	MediaInfo_CXXFLAGS="$MediaInfo_CXXFLAGS -DUNICODE"
	MediaInfo_Unicode="yes"
else
	MediaInfo_Unicode="no"
fi

dnl -------------------------------------------------------------------------
dnl Use DLL
dnl
if test "$with_dll" = "yes"; then
	AC_DEFINE(MEDIAINFO_DLL)
	LIBS="$LIBS -ldl"
fi

dnl -------------------------------------------------------------------------
dnl GNU compiler
dnl
if test "$GXX" = yes ; then
	if test "$enable_debug" = "yes"; then
		CFLAGS="$CFLAGS -O0 -g -Wall -Wundef"
		CXXFLAGS="$CXXFLAGS -O0 -g -Wall -W -Wundef -Wcast-qual -Wno-ctor-dtor-privacy"
	else
		CFLAGS="$CFLAGS -O2"
		CXXFLAGS="$CXXFLAGS -O2"
	fi
	if test "$enable_gprof" = "yes"; then
		CFLAGS="$CFLAGS -pg -g"
		CXXFLAGS="$CXXFLAGS -pg -g"
	fi
fi

dnl #########################################################################
dnl ### Needed libs
dnl #########################################################################

dnl 	LIBS="$LIBS -lz -lpthread $sharedstatedir/libmediainfo.a $sharedstatedir/libzen.a "

dnl -------------------------------------------------------------------------
dnl MediaInfoLib
dnl
if test "$with_dll" != "yes"; then
	with_dll="no"
	if test -e ../../../../MediaInfoLib/Project/GNU/Library/libmediainfo.la; then
		with_mediainfolib="builtin"
		CXXFLAGS="$CXXFLAGS $(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config CXXFLAGS)"
		LIBS="$LIBS $(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config LIBS)"
	else
		if pkg-config --exists libmediainfo; then
			CXXFLAGS="$CXXFLAGS $(pkg-config --cflags libmediainfo)"
			if test "$enable_staticlibs" = "yes"; then
				with_mediainfolib="system (static)"
				LIBS="$LIBS $(pkg-config --variable=Libs_Static libmediainfo)"
			else
				with_mediainfolib="system"
				LIBS="$LIBS $(pkg-config --libs libmediainfo)"
			fi
		else
			AC_MSG_ERROR([libmediainfo configuration is not found])
		fi
	fi
else
	with_dll="yes"
	with_mediainfolib="system"
fi

dnl -------------------------------------------------------------------------
dnl ZenLib
dnl
if test -e ../../../../ZenLib/Project/GNU/Library/libzen.la; then
	with_zenlib="builtin"
	CXXFLAGS="$CXXFLAGS $(../../../../ZenLib/Project/GNU/Library/libzen-config CXXFLAGS)"
	LIBS="$LIBS $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)"
else
	if pkg-config --exists libzen; then
		CXXFLAGS="$CXXFLAGS $(pkg-config --cflags libzen)"
		if test "$enable_staticlibs" = "yes"; then
			with_zenlib="system (static)"
			LIBS="$LIBS $(pkg-config --variable=Libs_Static libzen)"
		else
			with_zenlib="system"
			LIBS="$LIBS $(pkg-config --libs libzen)"
		fi
	else
		AC_MSG_ERROR([libzen configuration is not found])
	fi
fi

dnl #########################################################################
dnl ### Output
dnl #########################################################################

AC_OUTPUT(Makefile)

dnl #########################################################################
dnl ### Report how we have been configured
dnl #########################################################################

echo ""
echo "Configured MediaInfo for '$build'"
echo ""
echo "  Unicode?                                                $enable_unicode"
echo "  Using libzen?                                           $with_zenlib"
echo "  Using libmediainfo?                                     $with_mediainfolib"
echo ""
echo "  libmediainfo dynamicly loaded?                          $with_dll"
echo ""
echo "  CXXFLAGS: $CXXFLAGS"
echo "  LIBS: $LIBS"