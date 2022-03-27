# -------------------------------------------------
# Project created by QtCreator 2010-07-23T13:03:11
# -------------------------------------------------

QT += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
}

win32 {
    contains(UWP, yes|1) {

        WINRT_ASSETS_PATH=../../../Source/Resource/Image/Assets
        WINRT_MANIFEST=AppxManifest.xml
        INCLUDEPATH+=$$[QT_INSTALL_PREFIX]/include/QtCore/$$[QT_VERSION]

        contains(QT_ARCH, ARM) {
            exists(../../../../MediaInfoLib/Project/MSVC2017/Arm/Release/MediaInfo-Static_UWP.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += ../../../../MediaInfoLib/Project/MSVC2017/Arm/Release/MediaInfo-Static_UWP.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2017/Arm/Release/ZenLib_UWP.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += ../../../../ZenLib/Project/MSVC2017/Arm/Release/ZenLib_UWP.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc15/Arm/Release/zlibuwp.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += ../../../../zlib/contrib/vstudio/Arm/x86/Release/zlibuwp.lib
            } else {
                error("zlib not found on system")
            }
        }

        contains(QT_ARCH, i386) {
            exists(../../../../MediaInfoLib/Project/MSVC2017/Win32/Release/MediaInfo-Static_UWP.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += ../../../../MediaInfoLib/Project/MSVC2017/Win32/Release/MediaInfo-Static_UWP.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2017/Win32/Release/ZenLib_UWP.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += ../../../../ZenLib/Project/MSVC2017/Win32/Release/ZenLib_UWP.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc15/Release/zlibuwp/zlibuwp.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += ../../../../zlib/contrib/vstudio/vc15/Release/zlibuwp/zlibuwp.lib
            } else {
                error("zlib not found on system")
            }
        }

        contains(QT_ARCH, x86_64) {
            exists(../../../../MediaInfoLib/Project/MSVC2017/x64/Release/MediaInfo-Static_UWP.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += ../../../../MediaInfoLib/Project/MSVC2017/x64/Release/MediaInfo-Static_UWP.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2017/x64/Release/ZenLib_UWP.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += ../../../../ZenLib/Project/MSVC2017/x64/Release/ZenLib_UWP.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc15/x64/Release/zlibuwp/zlibuwp.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += ../../../../zlib/contrib/vstudio/vc15/x64/Release/zlibuwp/zlibuwp.lib
            } else {
                error("zlib not found on system")
            }
        }
    } else {
        contains(QT_ARCH, i386) {
            exists(../../../../MediaInfoLib/Project/MSVC2017/Win32/Release/MediaInfo-Static.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += ../../../../MediaInfoLib/Project/MSVC2017/Win32/Release/MediaInfo-Static.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2017/Win32/Release/ZenLib.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += ../../../../ZenLib/Project/MSVC2017/Win32/Release/ZenLib.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc15/x86/ZlibStatReleaseWithoutAsm/zlibstat.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += ../../../../zlib/contrib/vstudio/vc15/x86/ZlibStatReleaseWithoutAsm/zlibstat.lib
            } else {
                error("zlib not found on system")
            }
        }

        contains(QT_ARCH, x86_64) {
            exists(../../../../MediaInfoLib/Project/MSVC2017/x64/Release/MediaInfo-Static.lib) {
                INCLUDEPATH += ../../../../MediaInfoLib/Source
                LIBS += ../../../../MediaInfoLib/Project/MSVC2017/x64/Release/MediaInfo-Static.lib
            } else {
                error("libmediainfo not found on system")
            }

            exists(../../../../ZenLib/Project/MSVC2017/x64/Release/ZenLib.lib) {
                INCLUDEPATH += ../../../../ZenLib/Source
                LIBS += ../../../../ZenLib/Project/MSVC2017/x64/Release/ZenLib.lib
            } else {
                error("libzen not found on system")
            }

            exists(../../../../zlib/contrib/vstudio/vc15/x64/ZlibStatReleaseWithoutAsm/zlibstat.lib) {
                INCLUDEPATH += ../../../../zlib
                LIBS += ../../../../zlib/contrib/vstudio/vc15/x64/ZlibStatReleaseWithoutAsm/zlibstat.lib
            } else {
                error("zlib not found on system")
            }
        }
    }
}

INCLUDEPATH += _Automated \
               ../../../Source

DEFINES += _UNICODE

unix:LIBS += -ldl \
              -lz

SOURCES += ../../../Source/GUI/Qt/main.cpp \
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

HEADERS += ../../../Source/GUI/Qt/mainwindow.h \
    ../../../Source/Common/Core.h \
    ../../../Source/GUI/Qt/easyviewwidget.h \
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

FORMS += ../../../Source/GUI/Qt/mainwindow.ui \
    ../../../Source/GUI/Qt/prefs.ui \
    ../../../Source/GUI/Qt/export.ui \
    ../../../Source/GUI/Qt/about.ui \
    ../../../Source/GUI/Qt/sheetview.ui \
    ../../../Source/GUI/Qt/editcustom.ui \
    ../../../Source/GUI/Qt/editsheet.ui \
    ../../../Source/GUI/Qt/editconfigtreetext.ui

RESOURCES += ../../../Source/Resource/Resources.qrc

OBJECTS_DIR      =  _Automated
MOC_DIR          =  _Automated
UI_HEADERS_DIR   =  _Automated
UI_SOURCES_DIR   =  _Automated
UI_DIR           =  _Automated
RCC_DIR          =  _Automated
