dnl #########################################################################
dnl ### Init
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Name and version
dnl
AC_INIT([mediainfo-gui], [23.10])

dnl -------------------------------------------------------------------------
dnl Test if we are at the good place
dnl
AC_CONFIG_SRCDIR([../../../Source/GUI/WxWidgets/GUI_Main.cpp])

dnl -------------------------------------------------------------------------
dnl sets build, host, target variables and the same with _alias
dnl
AC_CANONICAL_BUILD
AM_INIT_AUTOMAKE([foreign -Wall])

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
dnl ### Configure
dnl #########################################################################

Home=`pwd`

dnl WxWidgets_Options="--without-subdirs --enable-gui"
dnl WxWidgets_MoreTests1="--toolkit=gtk2"
dnl WxWidgets_MoreTests2="--toolkit=mac"
dnl WxWidgets_MoreTests3="--toolkit=cocoa"

dnl #########################################################################
dnl ### Options
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Arguments - Enable
dnl
AC_ARG_ENABLE(debug,            AC_HELP_STRING([--enable-debug],            [Enable full debug]),                         , enable_debug=no)
AC_ARG_ENABLE(gprof,            AC_HELP_STRING([--enable-gprof],            [Enable gprof log]),                          , enable_gprof=no)
AC_ARG_ENABLE(universal_binary, AC_HELP_STRING([--enable-universal_binary], [Create Mac PPC/i386 Universal binary]),      , enable_universal_binary=no)
AC_ARG_ENABLE(arch-ppc,         AC_HELP_STRING([--enable-arch-ppc],         [Create Mac PPC architecture]),               , enable_arch_ppc=no)
AC_ARG_ENABLE(arch-ppc64,       AC_HELP_STRING([--enable-arch-ppc64],       [Create Mac PPC64 architecture]),             , enable_arch_ppc64=no)
AC_ARG_ENABLE(arch-i386,        AC_HELP_STRING([--enable-arch-i386],        [Create Mac i386 architecture]),              , enable_arch_i386=no)
AC_ARG_ENABLE(arch-x86_64,      AC_HELP_STRING([--enable-arch-x86_64],      [Create Mac x86_64 architecture]),            , enable_arch_x86_64=no)
AC_ARG_ENABLE(arch-arm64,       AC_HELP_STRING([--enable-arch-arm64],       [Create Mac arm64 architecture]),             , enable_arch_arm64=no)
AC_ARG_ENABLE(staticlibs,       AC_HELP_STRING([--enable-staticlibs],       [Use static libmediainfo and libzen]),        , enable_staticlibs=no)

dnl -------------------------------------------------------------------------
dnl Arguments - With
dnl
AC_ARG_WITH(macosx-sdk,         AC_HELP_STRING([--with-macosx-sdk],         [Force the Mac SDK]),                         , with_macosx_sdk=no)
AC_ARG_WITH(macosx-version-min, AC_HELP_STRING([--with-macosx-version-min], [Force the Mac Version]),                     , with_macosx_version_min=no)
AC_ARG_WITH(wxwidgets,      AC_HELP_STRING([--with-wxwidgets],      [Use WxWidgets]),                           , with_wxwidgets=yes)
AC_ARG_WITH(wx_static,      AC_HELP_STRING([--with-wx-static],      [Use WxWidgets static libs]),               , with_wx_static=no)
AC_ARG_WITH(wx_gui,         AC_HELP_STRING([--with-wx-gui],         [Use WxWidgets GUI]),                       , with_wx_gui=yes)
#AC_ARG_WITH(wx-path,        AC_HELP_STRING([--with-wx-path=PATH],    [Use local version of wxWidgets]),            , with_wxdir=no)
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

dnl -------------------------------------------------------------------------
dnl WxWidgets (v3)
dnl

dnl Download and compile
WxBuiltInCompile()
{
    if test -e ../../../../Shared/Project/WxWidgets/Compile.sh; then
        cd ../../../../Shared/Project/WxWidgets
        chmod +x Compile.sh
        ./Compile.sh --enable-unicode=$enable_unicode --enable-static=yes --enable-gui=$with_wx_gui --build=$build
        cd $Home
        if test -e ../../../../Shared/Source/WxWidgets/wx-config; then
            if test "x$(../../../../Shared/Source/WxWidgets/wx-config --unicode=$enable_unicode --version)" = "x"; then
                echo
                echo Problem while compiling WxWidgets
                echo
                with_wxwidgets="no"
            else
                echo
                echo Compiled WxWidgets found!
                echo
                with_wxwidgets="builtin"
            fi
        else
            echo
            echo Problem while compiling WxWidgets
            echo
            with_wxwidgets="no"
        fi
    else
        echo
        echo File Shared/Project/WxWidgets/Compile.sh not found
        echo
        with_wxwidgets="no"
    fi
}

dnl Search for a good build, else built it
WxBuiltInSearchGoodBuild()
{
    #if ! test -z "$with_wxdir"; then
    #    if test -e $with_wxdir/configure; then
    #        test -d ../../Source || mkdir -p ../../Source
    #        cp -r $with_wxdir ../../../../Shared/Source/WxWidgets
    #    else
    #        echo
    #        echo The specified path to local WxWidgets sources is wrong.
    #        echo
    #        exit 1
    #    fi
    #fi
    if test -e ../../../../Shared/Source/WxWidgets/wx-config; then
        dnl There is already a compiled version
        if test "x$(../../../../Shared/Source/WxWidgets/wx-config --unicode=$enable_unicode --static=$with_wx_static --version)" = "x"; then
            if test "x$(../../../../Shared/Source/WxWidgets/wx-config --unicode=$enable_unicode --version)" = "x"; then
                echo
                echo WxWidgets not well compiled, try to compile again
                echo
                WxBuiltInCompile
            else
                echo
                echo Already compiled WxWidgets found!
                echo
                with_wxwidgets="builtin"
            fi
        else
            echo
            echo Already compiled WxWidgets found!
            echo
            with_wxwidgets="builtin"
        fi
    else
        echo
        echo WxWidgets not yet compiled, try to compile
        echo
        WxBuiltInCompile
    fi
}

dnl The test
if test "$with_wxwidgets" = "yes"; then
    dnl Searching system libs
    WxVersion="2.6.0"
    case "${build}" in
        *apple* )
            dnl version 2.9 for Mac
            WxVersion="2.9.0"
        ;;
    esac
    AM_PATH_WXCONFIG("$WxVersion", WxSystemLibs=1, WxSystemLibs=0, [base], --unicode=$enable_unicode --static=$with_wx_static)
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
            wxWidgets version is $WxVersion or above, and the compilation
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
       ZenLib_Wx_Config="../../../../Shared/Source/WxWidgets/wx-config"
    fi
fi

dnl #########################################################################
dnl ### MediaInfo specific flags
dnl #########################################################################

dnl -------------------------------------------------------------------------
dnl Use WxWidgets
dnl
if test "$with_wx" != "no"; then
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
 MediaInfo_CFLAGS="$MediaInfo_CXXFLAGS -DUNICODE"
 MediaInfo_CXXFLAGS="$MediaInfo_CXXFLAGS -DUNICODE"
 MediaInfo_Unicode="yes"
else
 MediaInfo_Unicode="no"
fi

dnl -------------------------------------------------------------------------
dnl Use DLL
dnl
if test "$with_dll" = "yes"; then
 AC_DEFINE(MEDIAINFO_DLL_RUNTIME)
 LIBS="$LIBS -ldl"
fi

dnl -------------------------------------------------------------------------
dnl GNU compiler
dnl
dnl C/C++ compiler options used to compile
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

dnl -------------------------------------------------------------------------
dnl MediaInfoLib
dnl
if test "$with_dll" != "yes"; then
    with_dll="no"
    if test -e ../../../../MediaInfoLib/Project/GNU/Library/libmediainfo.la; then
        CXXFLAGS="$CXXFLAGS $(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config CXXFLAGS)"
        if test "$enable_staticlibs" = "yes"; then
            with_mediainfolib="builtin (static)"
            LIBS="$LIBS $(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config LIBS_Static)"
        else
            with_mediainfolib="builtin"
            LIBS="$LIBS $(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config LIBS)"
        fi
    else
        if pkg-config --exists libmediainfo; then
            CXXFLAGS="$CXXFLAGS $(pkg-config --cflags libmediainfo)"
            if test "$enable_staticlibs" = "yes"; then
                with_mediainfolib="system (static)"
                LIBS="$LIBS $(pkg-config --variable=Libs_Static libmediainfo)"
                LIBS="$LIBS $(pkg-config --static --libs libmediainfo)"
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
    CXXFLAGS="$CXXFLAGS $(../../../../ZenLib/Project/GNU/Library/libzen-config CXXFLAGS)"
    MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS $(../../../../ZenLib/Project/GNU/Library/libzen-config CXXFLAGS)"
    if test "$enable_staticlibs" = "yes"; then
        with_zenlib="builtin (static)"
        LIBS="$LIBS $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS_Static)"
    else
        with_zenlib="builtin"
        LIBS="$LIBS $(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)"
    fi
else
    if pkg-config --exists libzen; then
        CXXFLAGS="$CXXFLAGS $(pkg-config --cflags libzen)"
        MediaInfoLib_CXXFLAGS="$MediaInfoLib_CXXFLAGS $(pkg-config --cflags libzen)"
        if test "$enable_staticlibs" = "yes"; then
            with_zenlib="system (static)"
            LIBS="$LIBS $(pkg-config --variable=Libs_Static libzen)"
            LIBS="$LIBS $(pkg-config --static --libs libzen)"
        else
            with_zenlib="system"
            LIBS="$LIBS $(pkg-config --libs libzen)"
        fi
    else
        AC_MSG_ERROR([libzen configuration is not found])
    fi
fi

dnl #########################################################################
dnl ### Compiler specific
dnl #########################################################################

dnl -------------------------------------------------------------------------
case "${build}" in
  powerpc-montavista* )
    CFLAGS="$CFLAGS -D__MONTAVISTA__"
    CXXFLAGS="$CXXFLAGS -D__MONTAVISTA__"
  ;;
esac

dnl -------------------------------------------------------------------------
dnl Mac Architecture and SDK
dnl
if test "$enable_universal_binary" = "yes"; then
    enable_arch_ppc=yes
    enable_arch_i386=yes
fi
if test "$enable_arch_ppc" = "yes"; then
    CXXFLAGS="$CXXFLAGS -arch ppc"
    CFLAGS="$CFLAGS -arch ppc"
    LDFLAGS="$LDFLAGS -arch ppc"
fi
if test "$enable_arch_ppc64" = "yes"; then
    CXXFLAGS="$CXXFLAGS -arch ppc64"
    CFLAGS="$CFLAGS -arch ppc64"
    LDFLAGS="$LDFLAGS -arch ppc64"
fi
if test "$enable_arch_i386" = "yes"; then
    CXXFLAGS="$CXXFLAGS -arch i386"
    CFLAGS="$CFLAGS -arch i386"
    LDFLAGS="$LDFLAGS -arch i386"
fi
if test "$enable_arch_x86_64" = "yes"; then
    CXXFLAGS="$CXXFLAGS -arch x86_64"
    CFLAGS="$CFLAGS -arch x86_64"
    LDFLAGS="$LDFLAGS -arch x86_64"
fi
if test "$enable_arch_arm64" = "yes"; then
	CXXFLAGS="$CXXFLAGS -arch arm64"
	CFLAGS="$CFLAGS -arch arm64"
	LDFLAGS="$LDFLAGS -arch arm64"
fi
if test "x$with_macosx_sdk" != xno ; then
    CXXFLAGS="-isysroot $with_macosx_sdk $CXXFLAGS"
    CFLAGS="-isysroot $with_macosx_sdk $CFLAGS"
    LDFLAGS="-isysroot $with_macosx_sdk $LDFLAGS"
fi
if test "x$with_macosx_version_min" != xno ; then
    CXXFLAGS="-mmacosx-version-min=$with_macosx_version_min $CXXFLAGS"
    CFLAGS="-mmacosx-version-min=$with_macosx_version_min $CFLAGS"
    LDFLAGS="-mmacosx-version-min=$with_macosx_version_min $LDFLAGS"
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
echo "  Using WxWidgets?                                        $with_wxwidgets"
echo ""
echo "  libmediainfo dynamicly loaded?                          $with_dll"
echo ""
echo "  CXXFLAGS: $CXXFLAGS"
echo "  LIBS: $LIBS"
