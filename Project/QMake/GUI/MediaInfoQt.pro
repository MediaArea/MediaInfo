# -------------------------------------------------
# Project created by QtCreator 2010-07-23T13:03:11
# -------------------------------------------------

QT += core gui widgets network xml svg

win32 {
    DEFINES+=EDGE_WEBVIEW2_YES
} else {
    qtHaveModule(webenginewidgets) {
        QT += webenginewidgets
    } else {
        DEFINES+=MEDIAINFO_HTML_NO
    }
}

win32|macx {
    TARGET = "MediaInfo"
} else {
    TARGET = "mediainfo-gui"

    # Installation prefix
    isEmpty(PREFIX): PREFIX = /usr

    target.path = $$PREFIX/bin
    INSTALLS += target

    # Desktop integration
    desktop.path = $$PREFIX/share/applications
    desktop.files = ../../GNU/GUI/mediainfo-gui.desktop
    INSTALLS += desktop

    metainfo.path = $$PREFIX/share/metainfo
    metainfo.files = ../../GNU/GUI/mediainfo-gui.metainfo.xml
    INSTALLS += metainfo

    # KDE service menus
    kde3servicemenu.path = $$PREFIX/share/apps/konqueror/servicemenus
    kde3servicemenu.commands = $(CHK_DIR_EXISTS) $(INSTALL_ROOT)$$kde3servicemenu.path || $(MKDIR) $(INSTALL_ROOT)$$kde3servicemenu.path && $(INSTALL_FILE) $$_PRO_FILE_PWD_/../../GNU/GUI/mediainfo-gui.kde3.desktop $(INSTALL_ROOT)$$kde3servicemenu.path/mediainfo-gui.desktop
    kde3servicemenu.uninstall = $(DEL_FILE) $(INSTALL_ROOT)$$kde3servicemenu.path/mediainfo-gui.desktop
    INSTALLS += kde3servicemenu

    kde4servicemenu.path = $$PREFIX/share/kde4/services/ServiceMenus
    kde4servicemenu.commands = $(CHK_DIR_EXISTS) $(INSTALL_ROOT)$$kde4servicemenu.path || $(MKDIR) $(INSTALL_ROOT)$$kde4servicemenu.path && $(INSTALL_FILE) $$_PRO_FILE_PWD_/../../GNU/GUI/mediainfo-gui.kde4.desktop $(INSTALL_ROOT)$$kde4servicemenu.path/mediainfo-gui.desktop
    kde4servicemenu.uninstall = $(DEL_FILE) $(INSTALL_ROOT)$$kde4servicemenu.path/mediainfo-gui.desktop
    INSTALLS += kde4servicemenu

    kde5servicemenu.path = $$PREFIX/share/kservices5/ServiceMenus
    kde5servicemenu.commands = $(CHK_DIR_EXISTS) $(INSTALL_ROOT)$$kde5servicemenu.path || $(MKDIR) $(INSTALL_ROOT)$$kde5servicemenu.path && $(INSTALL_FILE) $$_PRO_FILE_PWD_/../../GNU/GUI/mediainfo-gui.kde4.desktop $(INSTALL_ROOT)$$kde5servicemenu.path/mediainfo-gui.desktop
    kde5servicemenu.uninstall = $(DEL_FILE) $(INSTALL_ROOT)$$kde5servicemenu.path/mediainfo-gui.desktop
    INSTALLS += kde5servicemenu

    kde6servicemenu.path = $$PREFIX/share/kio/servicemenus
    kde6servicemenu.commands = $(CHK_DIR_EXISTS) $(INSTALL_ROOT)$$kde6servicemenu.path || $(MKDIR) $(INSTALL_ROOT)$$kde6servicemenu.path && $(INSTALL_FILE) $$_PRO_FILE_PWD_/../../GNU/GUI/mediainfo-gui.kde6.desktop $(INSTALL_ROOT)$$kde6servicemenu.path/mediainfo-gui.desktop
    kde6servicemenu.uninstall = $(DEL_FILE) $(INSTALL_ROOT)$$kde6servicemenu.path/mediainfo-gui.desktop
    INSTALLS += kde6servicemenu

    # Icons
    icon_png.path = $$PREFIX/share/icons/hicolor/256x256/apps
    icon_png.files = ../../../Source/Resource/Image/MediaInfo.png
    INSTALLS += icon_png

    icon_svg.path = $$PREFIX/share/icons/hicolor/scalable/apps
    icon_svg.files = ../../../Source/Resource/Image/MediaInfo.svg
    INSTALLS += icon_svg

    icon_xpm.path = $$PREFIX/share/pixmaps
    icon_xpm.files = ../../../Source/Resource/Image/MediaInfo.xpm
    INSTALLS += icon_xpm
}

win32 {
    RC_FILE = mediainfo-gui.rc
    RESOURCES += ../../../Source/Resource/Resources_Windows.qrc
    contains(QT_ARCH, i386) {
        DESTDIR = Win32
    }
    contains(QT_ARCH, x86_64) {
        DESTDIR = x64
    }
    contains(QT_ARCH, arm64) {
        DESTDIR = ARM64
    }
}

!defined(packagesExist, test) {
    defineTest(packagesExist) {
        system(pkg-config $$ARGS): return(true)
        return(false)
    }
}

TEMPLATE = app

CONFIG += qt release c++11

unix {
    exists(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config) {
        INCLUDEPATH += ../../../../MediaInfoLib/Source
        contains(STATIC_LIBS, yes|1) {
            LIBS += $$system(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config LIBS_Static)
            message("custom libmediainfo: yes (static)")
        } else {
            LIBS += $$system(../../../../MediaInfoLib/Project/GNU/Library/libmediainfo-config LIBS)
            message("custom libmediainfo: yes (shared)")
        }
    } else {
        !packagesExist(libmediainfo) {
            error("libmediainfo not found on system")
        }
        LIBS += $$system(pkg-config --libs libmediainfo)
    }

    exists(../../../../ZenLib/Project/GNU/Library/libzen-config) {
        INCLUDEPATH      += ../../../../ZenLib/Source
        contains(STATIC_LIBS, yes|1) {
            LIBS += $$system(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS_Static)
            message("custom libzen       : yes (static)")
        } else {
            LIBS += $$system(../../../../ZenLib/Project/GNU/Library/libzen-config LIBS)
            message("custom libzen       : yes (shared)")
        }
    } else {
        PKGCONFIG += libzen
        message("libzen      : system")
    }

    !exists(../../../Source/Resource/Translations/en.ts) {
        system(../../../Source/GUI/Qt/Qt_Translations_Updater/update_Qt_translations.sh)
    }

    UI_FILES = $$files($$PWD/../../../Source/GUI/Qt/*.ui)
    lessThan(QT_VERSION, 6.7) {
        for(uiFile, UI_FILES) {
            system(sed -i 's/Qt::Orientation::Horizontal/Qt::Horizontal/g' $$uiFile)
            system(sed -i 's/Qt::Orientation::Vertical/Qt::Vertical/g' $$uiFile)
        }
    }
}

win32 {
    QMAKE_CXXFLAGS += /MP /guard:cf
    QMAKE_LFLAGS += /guard:cf /CETCOMPAT
    QMAKE_LFLAGS += /MANIFESTINPUT:$$_PRO_FILE_PWD_\segmentheap.manifest
    QMAKE_LFLAGS += /MANIFESTINPUT:$$_PRO_FILE_PWD_\..\..\..\Source\Resource\longpathaware.manifest

    contains(UWP, yes|1) {

        WINRT_ASSETS_PATH=../../../Source/Resource/Image/Assets
        WINRT_MANIFEST=AppxManifest.xml
        INCLUDEPATH+=$$[QT_INSTALL_PREFIX]/include/QtCore/$$[QT_VERSION]

        contains(QT_ARCH, ARM) {
            exists(../../../../MediaInfoLib/Project/MSVC2022/Arm/Release/MediaInfo-Static_UWP.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += ../../../../MediaInfoLib/Project/MSVC2022/Arm/Release/MediaInfo-Static_UWP.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2022/Arm/Release/ZenLib_UWP.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += ../../../../ZenLib/Project/MSVC2022/Arm/Release/ZenLib_UWP.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc17/Arm/Release/zlibuwp.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += ../../../../zlib/contrib/vstudio/Arm/x86/Release/zlibuwp.lib
            } else {
                error("zlib not found on system")
            }
        }

        contains(QT_ARCH, i386) {
            exists(../../../../MediaInfoLib/Project/MSVC2022/Win32/Release/MediaInfo-Static_UWP.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += ../../../../MediaInfoLib/Project/MSVC2022/Win32/Release/MediaInfo-Static_UWP.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2022/Win32/Release/ZenLib_UWP.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += ../../../../ZenLib/Project/MSVC2022/Win32/Release/ZenLib_UWP.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc17/Release/zlibuwp/zlibuwp.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += ../../../../zlib/contrib/vstudio/vc17/Release/zlibuwp/zlibuwp.lib
            } else {
                error("zlib not found on system")
            }
        }

        contains(QT_ARCH, x86_64) {
            exists(../../../../MediaInfoLib/Project/MSVC2022/x64/Release/MediaInfo-Static_UWP.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += ../../../../MediaInfoLib/Project/MSVC2022/x64/Release/MediaInfo-Static_UWP.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2022/x64/Release/ZenLib_UWP.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += ../../../../ZenLib/Project/MSVC2022/x64/Release/ZenLib_UWP.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc17/x64/Release/zlibuwp/zlibuwp.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += ../../../../zlib/contrib/vstudio/vc17/x64/Release/zlibuwp/zlibuwp.lib
            } else {
                error("zlib not found on system")
            }
        }
    } else {
        !build_pass:system("install_nuget_packages.cmd")

        contains(QT_ARCH, i386) {
            exists(../../../../MediaInfoLib/Project/MSVC2026/Win32/Release/MediaInfo-Static.lib) {
                !build_pass:message("using MSVC2026 MediaInfoLib, ZenLib and zlib")
                MSVCVER = MSVC2026
                MSVCVERN = vc18
            } else {
                !build_pass:message("using MSVC2022 MediaInfoLib, ZenLib and zlib")
                MSVCVER = MSVC2022
                MSVCVERN = vc17
            }

            exists(../../../../MediaInfoLib/Project/$$MSVCVER/Win32/Release/MediaInfo-Static.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += $$PWD/../../../../MediaInfoLib/Project/$$MSVCVER/Win32/Release/MediaInfo-Static.lib
            } else {
                error("MediaInfoLib not found on system")
            }

            exists(../../../../ZenLib/Project/$$MSVCVER/Win32/Release/ZenLib.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += $$PWD/../../../../ZenLib/Project/$$MSVCVER/Win32/Release/ZenLib.lib
            } else {
                exists(../../../../MediaInfoLib/Project/$$MSVCVER/Win32/Release/ZenLib.lib) {
                    !build_pass:message("using ZenLib.lib from MediaInfoLib build")
                    INCLUDEPATH += ../../../../ZenLib/Source
                    LIBS += $$PWD/../../../../MediaInfoLib/Project/$$MSVCVER/Win32/Release/ZenLib.lib
                } else {
                    error("ZenLib not found on system")
                }
            }

            exists(../../../../zlib/contrib/vstudio/$$MSVCVERN/x86/ZlibStatReleaseWithoutAsm/zlibstat.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += $$PWD/../../../../zlib/contrib/vstudio/$$MSVCVERN/x86/ZlibStatReleaseWithoutAsm/zlibstat.lib
            } else {
                error("zlib not found on system")
            }

            exists(packages/Microsoft.Web.WebView2/build/native/x86/WebView2Loader.dll.lib) {
                INCLUDEPATH += $$PWD/packages/Microsoft.Web.WebView2/build/native/include
                LIBS += -L$$PWD/packages/Microsoft.Web.WebView2/build/native/x86 -lWebView2Loader.dll
            } else {
                error("Edge WebView2 lib not found on system")
            }
        }

        contains(QT_ARCH, x86_64) {
            exists(../../../../MediaInfoLib/Project/MSVC2026/x64/Release/MediaInfo-Static.lib) {
                !build_pass:message("using MSVC2026 MediaInfoLib, ZenLib and zlib")
                MSVCVER = MSVC2026
                MSVCVERN = vc18
            } else {
                !build_pass:message("using MSVC2022 MediaInfoLib, ZenLib and zlib")
                MSVCVER = MSVC2022
                MSVCVERN = vc17
            }

            exists(../../../../MediaInfoLib/Project/$$MSVCVER/x64/Release/MediaInfo-Static.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += $$PWD/../../../../MediaInfoLib/Project/$$MSVCVER/x64/Release/MediaInfo-Static.lib
            } else {
                error("MediaInfoLib not found on system")
            }

            exists(../../../../ZenLib/Project/$$MSVCVER/x64/Release/ZenLib.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += $$PWD/../../../../ZenLib/Project/$$MSVCVER/x64/Release/ZenLib.lib
            } else {
                exists(../../../../MediaInfoLib/Project/$$MSVCVER/x64/Release/ZenLib.lib) {
                    !build_pass:message("using ZenLib.lib from MediaInfoLib build")
                    INCLUDEPATH += ../../../../ZenLib/Source
                    LIBS += $$PWD/../../../../MediaInfoLib/Project/$$MSVCVER/x64/Release/ZenLib.lib
                } else {
                    error("ZenLib not found on system")
                }
            }

            exists(../../../../zlib/contrib/vstudio/$$MSVCVERN/x64/ZlibStatReleaseWithoutAsm/zlibstat.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += $$PWD/../../../../zlib/contrib/vstudio/$$MSVCVERN/x64/ZlibStatReleaseWithoutAsm/zlibstat.lib
            } else {
                error("zlib not found on system")
            }

            exists(packages/Microsoft.Web.WebView2/build/native/x64/WebView2Loader.dll.lib) {
                INCLUDEPATH += $$PWD/packages/Microsoft.Web.WebView2/build/native/include
                LIBS += -L$$PWD/packages/Microsoft.Web.WebView2/build/native/x64 -lWebView2Loader.dll
            } else {
                error("Edge WebView2 lib not found on system")
            }

            QMAKE_CXXFLAGS += /guard:ehcont
            QMAKE_LFLAGS += /guard:ehcont
        }

        contains(QT_ARCH, arm64) {
            exists(../../../../MediaInfoLib/Project/MSVC2026/ARM64/Release/MediaInfo-Static.lib) {
                !build_pass:message("using MSVC2026 MediaInfoLib, ZenLib and zlib")
                MSVCVER = MSVC2026
                MSVCVERN = vc18
            } else {
                !build_pass:message("using MSVC2022 MediaInfoLib, ZenLib and zlib")
                MSVCVER = MSVC2022
                MSVCVERN = vc17
            }

            exists(../../../../MediaInfoLib/Project/$$MSVCVER/ARM64/Release/MediaInfo-Static.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += $$PWD/../../../../MediaInfoLib/Project/$$MSVCVER/ARM64/Release/MediaInfo-Static.lib
            } else {
                error("MediaInfoLib not found on system")
            }

            exists(../../../../ZenLib/Project/$$MSVCVER/ARM64/Release/ZenLib.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += $$PWD/../../../../ZenLib/Project/$$MSVCVER/ARM64/Release/ZenLib.lib
            } else {
                exists(../../../../MediaInfoLib/Project/$$MSVCVER/ARM64/Release/ZenLib.lib) {
                    !build_pass:message("using ZenLib.lib from MediaInfoLib build")
                    INCLUDEPATH += ../../../../ZenLib/Source
                    LIBS += $$PWD/../../../../MediaInfoLib/Project/$$MSVCVER/ARM64/Release/ZenLib.lib
                } else {
                    error("ZenLib not found on system")
                }
            }

            exists(../../../../zlib/contrib/vstudio/$$MSVCVERN/arm64/ZlibStatReleaseWithoutAsm/zlibstat.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += $$PWD/../../../../zlib/contrib/vstudio/$$MSVCVERN/arm64/ZlibStatReleaseWithoutAsm/zlibstat.lib
            } else {
                error("zlib not found on system")
            }

            exists(packages/Microsoft.Web.WebView2/build/native/arm64/WebView2Loader.dll.lib) {
                INCLUDEPATH += $$PWD/packages/Microsoft.Web.WebView2/build/native/include
                LIBS += -L$$PWD/packages/Microsoft.Web.WebView2/build/native/arm64 -lWebView2Loader.dll
            } else {
                error("Edge WebView2 lib not found on system")
            }

            QMAKE_CXXFLAGS += /guard:ehcont /guard:signret
            QMAKE_LFLAGS += /guard:ehcont /guard:delayloadsignret
            QMAKE_LFLAGS -= /CETCOMPAT
        }
    }

    !exists(../../../Source/Resource/Translations/en.ts) {
        system($$PWD/../../../Source/GUI/Qt/Qt_Translations_Updater/update_Qt_translations.cmd)
    }
}

INCLUDEPATH += _Automated \
               ../../../Source

DEFINES += _UNICODE

unix:LIBS += -ldl \
              -lz

SOURCES += ../../../Source/GUI/Qt/main.cpp \
    ../../../Source/GUI/Qt/htmlwidget.cpp \
    ../../../Source/GUI/Qt/mainwindow.cpp \
    ../../../Source/Common/Core.cpp \
    ../../../Source/GUI/Qt/custom.cpp \
    ../../../Source/GUI/Qt/editcustom.cpp \
    ../../../Source/GUI/Qt/easyviewwidget.cpp \
    ../../../Source/GUI/Qt/prefs.cpp \
    ../../../Source/GUI/Qt/views.cpp \
    ../../../Source/GUI/Qt/export.cpp \
    ../../../Source/GUI/Qt/about.cpp \
    ../../../Source/GUI/Qt/sheetview.cpp \
    ../../../Source/GUI/Qt/sheet.cpp \
    ../../../Source/GUI/Qt/editsheet.cpp \
    ../../../Source/GUI/Qt/columneditsheet.cpp \
    ../../../Source/GUI/Qt/translate.cpp \
    ../../../Source/GUI/Qt/configtreetext.cpp \
    ../../../Source/GUI/Qt/editconfigtreetext.cpp

win32 {
    SOURCES += ../../../Source/GUI/Qt/edgewebview2widget.cpp
}


HEADERS += ../../../Source/GUI/Qt/mainwindow.h \
    ../../../Source/Common/Core.h \
    ../../../Source/GUI/Qt/easyviewwidget.h \
    ../../../Source/GUI/Qt/htmlwidget.h \
    ../../../Source/GUI/Qt/prefs.h \
    ../../../Source/GUI/Qt/views.h \
    ../../../Source/GUI/Qt/custom.h \
    ../../../Source/GUI/Qt/editcustom.h \
    ../../../Source/GUI/Qt/export.h \
    ../../../Source/GUI/Qt/about.h \
    ../../../Source/GUI/Qt/sheetview.h \
    ../../../Source/GUI/Qt/sheet.h \
    ../../../Source/GUI/Qt/editsheet.h \
    ../../../Source/GUI/Qt/columneditsheet.h \
    ../../../Source/GUI/Qt/translate.h \
    ../../../Source/GUI/Qt/configtreetext.h \
    ../../../Source/GUI/Qt/editconfigtreetext.h

win32 {
    HEADERS += ../../../Source/GUI/Qt/edgewebview2widget.h
}

FORMS += ../../../Source/GUI/Qt/mainwindow.ui \
    ../../../Source/GUI/Qt/prefs.ui \
    ../../../Source/GUI/Qt/export.ui \
    ../../../Source/GUI/Qt/about.ui \
    ../../../Source/GUI/Qt/sheetview.ui \
    ../../../Source/GUI/Qt/editcustom.ui \
    ../../../Source/GUI/Qt/editsheet.ui \
    ../../../Source/GUI/Qt/editconfigtreetext.ui

TRANSLATIONS = ../../../Source/Resource/Translations/ar.ts \
    ../../../Source/Resource/Translations/be.ts \
    ../../../Source/Resource/Translations/bg.ts \
    ../../../Source/Resource/Translations/ca.ts \
    ../../../Source/Resource/Translations/cs.ts \
    ../../../Source/Resource/Translations/da.ts \
    ../../../Source/Resource/Translations/de.ts \
    ../../../Source/Resource/Translations/en.ts \
    ../../../Source/Resource/Translations/es.ts \
    ../../../Source/Resource/Translations/eu.ts \
    ../../../Source/Resource/Translations/fa.ts \
    ../../../Source/Resource/Translations/fr.ts \
    ../../../Source/Resource/Translations/gl.ts \
    ../../../Source/Resource/Translations/gr.ts \
    ../../../Source/Resource/Translations/hr.ts \
    ../../../Source/Resource/Translations/hu.ts \
    ../../../Source/Resource/Translations/hy.ts \
    ../../../Source/Resource/Translations/id.ts \
    ../../../Source/Resource/Translations/it.ts \
    ../../../Source/Resource/Translations/ja.ts \
    ../../../Source/Resource/Translations/ka.ts \
    ../../../Source/Resource/Translations/ko.ts \
    ../../../Source/Resource/Translations/lt.ts \
    ../../../Source/Resource/Translations/nl.ts \
    ../../../Source/Resource/Translations/pl.ts \
    ../../../Source/Resource/Translations/pt-BR.ts \
    ../../../Source/Resource/Translations/pt.ts \
    ../../../Source/Resource/Translations/ro.ts \
    ../../../Source/Resource/Translations/ru.ts \
    ../../../Source/Resource/Translations/sk.ts \
    ../../../Source/Resource/Translations/sq.ts \
    ../../../Source/Resource/Translations/sv.ts \
    ../../../Source/Resource/Translations/th.ts \
    ../../../Source/Resource/Translations/tr.ts \
    ../../../Source/Resource/Translations/uk.ts \
    ../../../Source/Resource/Translations/zh-CN.ts \
    ../../../Source/Resource/Translations/zh-HK.ts \
    ../../../Source/Resource/Translations/zh-TW.ts

RESOURCES += ../../../Source/Resource/Resources.qrc

OBJECTS_DIR      =  _Automated
MOC_DIR          =  _Automated
UI_HEADERS_DIR   =  _Automated
UI_SOURCES_DIR   =  _Automated
UI_DIR           =  _Automated
RCC_DIR          =  _Automated
