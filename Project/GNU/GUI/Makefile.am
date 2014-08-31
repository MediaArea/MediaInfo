AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = mediainfo-gui
mediainfo_gui_SOURCES = \
    ../../../Source/GUI/Common/GUI_Main_Common_Core.cpp \
    ../../../Source/GUI/Common/GUI_Main_Easy_Box_Core.cpp \
    ../../../Source/GUI/Common/GUI_Main_Easy_Core.cpp \
    ../../../Source/GUI/WxWidgets/App.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main.rc \
    ../../../Source/GUI/WxWidgets/GUI_Main_Easy.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main_Easy_Box.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main_FileDrop.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main_HTML.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main_Menu.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main_Sheet.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main_Text.cpp \
    ../../../Source/GUI/WxWidgets/GUI_Main_Tree.cpp \
    ../../../Source/CLI/CommandLine_Parser.cpp \
    ../../../Source/CLI/Help.cpp \
    ../../../Source/Common/Core.cpp

#mediainfo_gui_LDFLAGS     = -no-undefined -version-info 0:0:0

AM_CPPFLAGS = -I../../../Source -I../../../../MediaInfoLib/Source -I../../../../ZenLib/Source