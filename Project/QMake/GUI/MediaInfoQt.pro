# -------------------------------------------------
# Project created by QtCreator 2010-07-23T13:03:11
# -------------------------------------------------
TARGET = mediainfo-gui
TEMPLATE = app

# QT += webkit
QT += xml
DEFINES += \
    _UNICODE
LIBS += -ldl \
    ../../../../MediaInfoLib/Project/GNU/Library/.libs/libmediainfo.a \
    ../../../../ZenLib/Project/GNU/Library/.libs/libzen.a \
    -lz
SOURCES += ../../../Source/GUI/Qt/main.cpp \
    ../../../Source/GUI/Qt/mainwindow.cpp \
    ../../../Source/Common/Core.cpp \
    ../../../Source/GUI/Qt/custom.cpp \
    ../../../Source/GUI/Qt/editcustom.cpp \
    ../../../Source/GUI/Qt/easyviewwidget.cpp \
    ../../../Source/GUI/Qt/preferences.cpp \
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
    ../../../Source/GUI/Qt/preferences.h \
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
    ../../../Source/GUI/Qt/preferences.ui \
    ../../../Source/GUI/Qt/export.ui \
    ../../../Source/GUI/Qt/about.ui \
    ../../../Source/GUI/Qt/sheetview.ui \
    ../../../Source/GUI/Qt/editcustom.ui \
    ../../../Source/GUI/Qt/editsheet.ui \
    ../../../Source/GUI/Qt/editconfigtreetext.ui
RESOURCES += ../../../Source/Resource/Resources.qrc
MOC_DIR = _Automated
UI_HEADERS_DIR = _Automated
UI_SOURCES_DIR = _Automated
INCLUDEPATH = ../../../Source \
    ../../../../MediaInfoLib/Source \
    ../../../../ZenLib/Source \
QMAKE_UIC += -tr \
    Tr
target.path = /usr/bin
INSTALLS += target
