# -------------------------------------------------
# Project created by QtCreator 2010-07-23T13:03:11
# -------------------------------------------------

QT += core gui widgets network xml
!win32 {
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
    target.path=/usr/bin
    INSTALLS+=target
}

win32 {
    RC_FILE = mediainfo-gui.rc
    contains(QT_ARCH, i386) {
        DESTDIR = Win32
    }
    contains(QT_ARCH, x86_64) {
        DESTDIR = x64
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
            exists(../../../../MediaInfoLib/Project/MSVC2022/Win32/Release/MediaInfo-Static.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += $$PWD/../../../../MediaInfoLib/Project/MSVC2022/Win32/Release/MediaInfo-Static.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2022/Win32/Release/ZenLib.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += $$PWD/../../../../ZenLib/Project/MSVC2022/Win32/Release/ZenLib.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc17/x86/ZlibStatReleaseWithoutAsm/zlibstat.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += $$PWD/../../../../zlib/contrib/vstudio/vc17/x86/ZlibStatReleaseWithoutAsm/zlibstat.lib
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
            exists(../../../../MediaInfoLib/Project/MSVC2022/x64/Release/MediaInfo-Static.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += $$PWD/../../../../MediaInfoLib/Project/MSVC2022/x64/Release/MediaInfo-Static.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2022/x64/Release/ZenLib.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += $$PWD/../../../../ZenLib/Project/MSVC2022/x64/Release/ZenLib.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc17/x64/ZlibStatReleaseWithoutAsm/zlibstat.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += $$PWD/../../../../zlib/contrib/vstudio/vc17/x64/ZlibStatReleaseWithoutAsm/zlibstat.lib
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
            exists(../../../../MediaInfoLib/Project/MSVC2022/ARM64/Release/MediaInfo-Static.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += $$PWD/../../../../MediaInfoLib/Project/MSVC2022/ARM64/Release/MediaInfo-Static.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2022/ARM64/Release/ZenLib.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += $$PWD/../../../../ZenLib/Project/MSVC2022/ARM64/Release/ZenLib.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc17/arm64/ZlibStatReleaseWithoutAsm/zlibstat.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += $$PWD/../../../../zlib/contrib/vstudio/vc17/arm64/ZlibStatReleaseWithoutAsm/zlibstat.lib
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
    ../../../Source/GUI/Qt/graphplugin.cpp \
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
    SOURCES += ../../../Source/GUI/Qt/webview2widget.cpp
}


HEADERS += ../../../Source/GUI/Qt/mainwindow.h \
    ../../../Source/Common/Core.h \
    ../../../Source/GUI/Qt/easyviewwidget.h \
    ../../../Source/GUI/Qt/graphplugin.h \
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
    HEADERS += ../../../Source/GUI/Qt/webview2widget.h
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
