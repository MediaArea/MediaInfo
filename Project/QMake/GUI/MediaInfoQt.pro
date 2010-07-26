# -------------------------------------------------
# Project created by QtCreator 2010-07-23T13:03:11
# -------------------------------------------------
TARGET = MediaInfoQt
TEMPLATE = app

# QT += webkit
DEFINES += MEDIAINFO_DLL_RUNTIME _UNICODE
LIBS += -ldl -lzen
SOURCES += ../../../Source/GUI/Qt/main.cpp \
    ../../../Source/GUI/Qt/mainwindow.cpp \
    ../../../Source/Common/Core.cpp \
    ../../../Source/GUI/Qt/easyviewwidget.cpp
HEADERS += ../../../Source/GUI/Qt/mainwindow.h \
    ../../../Source/Common/Core.h \
    ../../../Source/GUI/Qt/easyviewwidget.h
FORMS += ../../../Source/GUI/Qt/mainwindow.ui
RESOURCES += ../../../Source/Ressource/Resources.qrc
MOC_DIR = _Automated
UI_HEADERS_DIR = _Automated
UI_SOURCES_DIR = _Automated
INCLUDEPATH = ../../../Source
