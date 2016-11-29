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

appdir = $(datarootdir)/applications
appdatadir = $(datarootdir)/appdata

dist_app_DATA = mediainfo-gui.desktop
dist_appdata_DATA = mediainfo-gui.appdata.xml

install-data-hook:
	install -dm 755 $(DESTDIR)$(datarootdir)/apps/konqueror/servicemenus && \
	install -m 644 mediainfo-gui.kde3.desktop \
	               $(DESTDIR)$(datarootdir)/apps/konqueror/servicemenus/mediainfo-gui.desktop && \
	install -dm 755 $(DESTDIR)$(datarootdir)/kde4/services/ServiceMenus && \
	install -m 644 mediainfo-gui.kde4.desktop \
	               $(DESTDIR)$(datarootdir)/kde4/services/ServiceMenus/mediainfo-gui.desktop && \
	install -dm 755 $(DESTDIR)$(datarootdir)/icons/hicolor/256x256/apps && \
	install -m 644 ../../../Source/Resource/Image/MediaInfo.png \
	               $(DESTDIR)$(datarootdir)/icons/hicolor/256x256/apps/mediainfo.png && \
	install -dm 755 $(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps && \
	install -m 644 ../../../Source/Resource/Image/MediaInfo.svg \
	               $(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps/mediainfo.svg && \
	install -dm 755 $(DESTDIR)$(datarootdir)/pixmaps && \
	install -m 644 ../../../Source/Resource/Image/MediaInfo.xpm \
	               $(DESTDIR)$(datarootdir)/pixmaps/mediainfo.xpm

uninstall-local:
	( cd '$(DESTDIR)$(datarootdir)/apps/konqueror/servicemenus' && rm -f mediainfo-gui.desktop ) && \
	( cd '$(DESTDIR)$(datarootdir)/kde4/services/ServiceMenus' && rm -f mediainfo-gui.desktop ) && \
	( cd '$(DESTDIR)$(datarootdir)/icons/hicolor/256x256/apps' && rm -f mediainfo.png ) && \
	( cd '$(DESTDIR)$(datarootdir)/icons/hicolor/scalable/apps' && rm -f mediainfo.svg ) && \
	( cd '$(DESTDIR)$(datarootdir)/pixmaps' && rm -f mediainfo.xpm )
