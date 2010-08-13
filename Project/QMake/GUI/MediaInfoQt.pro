# -------------------------------------------------
# Project created by QtCreator 2010-07-23T13:03:11
# -------------------------------------------------
TARGET = MediaInfoQt
TEMPLATE = app

# QT += webkit
DEFINES += MEDIAINFO_DLL_RUNTIME \
    _UNICODE
LIBS += -ldl \
    -lzen
SOURCES += ../../../Source/GUI/Qt/main.cpp \
    ../../../Source/GUI/Qt/mainwindow.cpp \
    ../../../Source/Common/Core.cpp \
    ../../../Source/GUI/Qt/easyviewwidget.cpp \
    ../../../Source/GUI/Qt/preferences.cpp \
    ../../../Source/GUI/Qt/views.cpp \
    ../../../Source/GUI/Qt/export.cpp \
    ../../../Source/GUI/Qt/about.cpp \
    ../../../Source/GUI/Qt/sheetview.cpp \
    ../../../Source/GUI/Qt/sheet.cpp
HEADERS += ../../../Source/GUI/Qt/mainwindow.h \
    ../../../Source/Common/Core.h \
    ../../../Source/GUI/Qt/easyviewwidget.h \
    ../../../Source/GUI/Qt/preferences.h \
    ../../../Source/GUI/Qt/views.h \
    ../../../Source/GUI/Qt/export.h \
    ../../../Source/GUI/Qt/about.h \
    ../../../Source/GUI/Qt/sheetview.h \
    ../../../Source/GUI/Qt/sheet.h
FORMS += ../../../Source/GUI/Qt/mainwindow.ui \
    ../../../Source/GUI/Qt/preferences.ui \
    ../../../Source/GUI/Qt/export.ui \
    ../../../Source/GUI/Qt/about.ui \
    ../../../Source/GUI/Qt/sheetview.ui
RESOURCES += ../../../Source/Ressource/Resources.qrc
MOC_DIR = _Automated
UI_HEADERS_DIR = _Automated
UI_SOURCES_DIR = _Automated
INCLUDEPATH = ../../../Source
