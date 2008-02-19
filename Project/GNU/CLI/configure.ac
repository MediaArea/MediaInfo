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
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_INSTALL
CFLAGS=""
CXXFLAGS=""

dnl #########################################################################
dnl ### Options
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Arguments - Enable
dnl
AC_ARG_ENABLE(debug,        AC_HELP_STRING([--enable-debug],        [Enable full debug]),                       , enable_debug=no)

dnl -------------------------------------------------------------------------
dnl Arguments - With
dnl
AC_ARG_WITH(wxwidgets,      AC_HELP_STRING([--with-wxwidgets],      [Do not use WxWidgets]),                    , with_wxwidgets=no)
AC_ARG_WITH(wxstatic,       AC_HELP_STRING([--without-wx-static],   [Do not use WxWidgets static libs]),        , with_wx_static=yes)
AC_ARG_WITH(dll,            AC_HELP_STRING([--with-dll],            [Use DLL instead of static link]),          , with_dll=no)

dnl -------------------------------------------------------------------------
dnl External options
dnl
m4_include(wxwin.m4)
AM_OPTIONS_WXCONFIG

dnl #########################################################################
dnl ### Options from elsewhere
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Unicode
dnl
if test -e ../../../../ZenLib/Project/GNU/Library/ZenLib-config; then
	enable_unicode="$(../../../../ZenLib/Project/GNU/Library/ZenLib-config Unicode)"
else
	AC_MSG_ERROR([Compiled ZenLib is not found])
fi

dnl #########################################################################
dnl ### Parsing options
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl WxWidgets (v1)
dnl

dnl Download and compile
WxBuiltInCompile()
{
	if test -e ../../../../Shared/Project/WxWidgets/Compile.sh; then
		cd ../../../../Shared/Project/WxWidgets
		chmod u+x Compile.sh
		./Compile.sh --enable-unicode=$enable_unicode --enable-static=$with_wx_static --enable-gui=$enable_gui --host=$host
		cd $Path_Original
		if test -e ../../../../Shared/Source/WxWidgets/Compile/wx-config; then
			if test "x$(../../../../Shared/Source/WxWidgets/Compile/wx-config --unicode=$enable_unicode --version)" = "x"; then
				with_wxwidgets="no"
			else
				dnl Compiled builtin lib found!
				with_wxwidgets="builtin"
			fi
		else
			dnl Problem while compiling
			with_wxwidgets="no"
		fi
	else
		with_wxwidgets="no"
	fi
}

dnl Search for a good build, else built it
WxBuiltInSearchGoodBuild()
{
	if test -e ../../../../Shared/Source/WxWidgets/Compile/wx-config; then
		dnl There is already a compiled version
		if test "x$(../../../../Shared/Source/WxWidgets/Compile/wx-config --unicode=$enable_unicode --static=$with_wx_static --version)" = "x"; then
			dnl Not well compiled, try to compile again
			WxBuiltInCompile
		else
			dnl Already compiled builtin lib found!
			with_wxwidgets="builtin"
		fi
	else
		dnl Not yet compiled, try to compile
		WxBuiltInCompile
	fi
}

dnl The test
if test "$with_wxwidgets" = "yes"; then
    dnl Searching system libs
    AM_PATH_WXCONFIG(2.8.0, WxSystemLibs=1, WxSystemLibs=0, [base], --unicode=$enable_unicode --static=$with_wx_static)
    if test "$WxSystemLibs" != 1; then
        dnl Searching builtin libs
		WxBuiltInSearchGoodBuild
	else
        dnl System lib found!
		with_wxwidgets="system"
	fi
	
	if test "$with_wxwidgets" = "no"; then
		AC_MSG_ERROR([
			wxWidgets must be installed on your system.
	 
			Please check that wx-config is in path, the directory
			where wxWidgets libraries are installed (returned by
			'wx-config --libs' or 'wx-config --static --libs' command)
			is in LD_LIBRARY_PATH or equivalent variable and
			wxWidgets version is 2.8.0 or above, and the compilation 
			is compatible with 
			--unicode=$enable_unicode --static=$with_wx_static
			])
	fi
	if test "$with_wxwidgets" = "system"; then
		if test "${with_wx_config+set}" = set; then
			ZenLib_Wx_Config=$with_wx_config;
		else
			ZenLib_Wx_Config="wx-config"
		fi
	fi
	if test "$with_wxwidgets" = "builtin"; then
       ZenLib_Wx_Config="../../../../Shared/Source/WxWidgets/Compile/wx-config"
	fi
fi

dnl #########################################################################
dnl ### MediaInfo flags
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl WxWidgets
dnl
if test "$with_wxwidgets" != "no"; then
	CFLAGS="$CFLAGS $($ZenLib_Wx_Config --unicode=$enable_unicode --cflags)"
	CXXFLAGS="$CXXFLAGS $($ZenLib_Wx_Config --unicode=$enable_unicode --cxxflags)"
	LIBS="$LIBS $($ZenLib_Wx_Config --unicode=$enable_unicode --libs)"
fi

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
fi

dnl #########################################################################
dnl ### Needed libs
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl MediaInfoLib
dnl
if test "$with_dll" != "yes"; then
	if test ! -e ../../../../MediaInfoLib/Project/GNU/Library/libmediainfo.la; then
		AC_MSG_ERROR([MediaInfoLib is not found])
	fi
	CXXFLAGS="$CXXFLAGS $(../../../../MediaInfoLib/Project/GNU/Library/MediaInfoLib-config CXXFLAGS)"
	LIBS="$LIBS $(../../../../MediaInfoLib/Project/GNU/Library/MediaInfoLib-config LIBS)"
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
echo "  Using WxWidgets?                                        $with_wxwidgets"
echo ""
echo "  Using DLL instead of static lib?                        $with_dll"
echo ""
echo "  CXXFLAGS: $CXXFLAGS"
echo "  LIBS: $LIBS"