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

appdir = $(datadir)/applications
metainfodir = $(datadir)/metainfo

dist_app_DATA = mediainfo-gui.desktop
dist_metainfo_DATA = mediainfo-gui.metainfo.xml

install-data-hook:
	install -dm 755 $(DESTDIR)$(datadir)/apps/konqueror/servicemenus && \
	install -m 644 mediainfo-gui.kde3.desktop \
	               $(DESTDIR)$(datadir)/apps/konqueror/servicemenus/mediainfo-gui.desktop && \
	install -dm 755 $(DESTDIR)$(datadir)/kde4/services/ServiceMenus && \
	install -m 644 mediainfo-gui.kde4.desktop \
	               $(DESTDIR)$(datadir)/kde4/services/ServiceMenus/mediainfo-gui.desktop && \
	install -dm 755 $(DESTDIR)$(datadir)/kservices5/ServiceMenus && \
	install -m 644 mediainfo-gui.kde4.desktop \
	               $(DESTDIR)$(datadir)/kservices5/ServiceMenus/mediainfo-gui.desktop && \
	install -dm 755 $(DESTDIR)$(datadir)/icons/hicolor/256x256/apps && \
	install -m 644 ../../../Source/Resource/Image/MediaInfo.png \
	               $(DESTDIR)$(datadir)/icons/hicolor/256x256/apps/mediainfo.png && \
	install -dm 755 $(DESTDIR)$(datadir)/icons/hicolor/scalable/apps && \
	install -m 644 ../../../Source/Resource/Image/MediaInfo.svg \
	               $(DESTDIR)$(datadir)/icons/hicolor/scalable/apps/mediainfo.svg && \
	install -dm 755 $(DESTDIR)$(datadir)/pixmaps && \
	install -m 644 ../../../Source/Resource/Image/MediaInfo.xpm \
	               $(DESTDIR)$(datadir)/pixmaps/mediainfo.xpm

uninstall-local:
	( cd '$(DESTDIR)$(datadir)/apps/konqueror/servicemenus' && rm -f mediainfo-gui.desktop ) && \
	( cd '$(DESTDIR)$(datadir)/kde4/services/ServiceMenus' && rm -f mediainfo-gui.desktop ) && \
	( cd '$(DESTDIR)$(datadir)/kservices5/ServiceMenus' && rm -f mediainfo-gui.desktop ) && \
	( cd '$(DESTDIR)$(datadir)/icons/hicolor/256x256/apps' && rm -f mediainfo.png ) && \
	( cd '$(DESTDIR)$(datadir)/icons/hicolor/scalable/apps' && rm -f mediainfo.svg ) && \
	( cd '$(DESTDIR)$(datadir)/pixmaps' && rm -f mediainfo.xpm )
