# Copyright (c) 2007-2012 MediaArea.net SARL <info@mediaarea.net>
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.

# norootforbuild

%define mediainfo_version		0.7.58
%define libmediainfo_version	0.7.58
%define libzen_version			0.4.25

Name:			mediainfo
Version:		%mediainfo_version
Release:		1
Summary:		Convenient unified display of the most relevant technical and tag data for video and audio files (CLI)

Group:			Productivity/Multimedia/Other
License:		LGPLv3+
URL:			http://mediainfo.sourceforge.net/
Packager:		MediaArea.net SARL <info@mediaarea.net>
Source0:		mediainfo_%{version}-1.tar.gz

BuildRequires:	dos2unix
BuildRequires: 	gcc-c++
BuildRequires:	libmediainfo-devel
BuildRequires:	libzen-devel >= %libzen_version
BuildRequires:	pkgconfig
%if %{undefined rhel_version} || 0%{?rhel_version} < 600
%if 0%{?mandriva_version}
%ifarch x86_64
BuildRequires:	lib64wxgtku2.8-devel
%else
BuildRequires:	libwxgtku2.8-devel
%endif
%else
%if 0%{?suse_version} && 0%{?suse_version} >= 1140
BuildRequires:	wxWidgets-devel
%else
BuildRequires:	wxGTK-devel
%endif
%endif
BuildRequires: 	zlib-devel
%if 0%{?suse_version}
BuildRequires:	update-desktop-files
%endif
%endif
Requires:	    libmediainfo0 >= %libmediainfo_version
Requires:	    libzen0 >= %libzen_version

BuildRoot:		%{_tmppath}/%{name}-%{version}-%{release}-root

%description
MediaInfo is a convenient unified display of the most relevant technical
and tag data for video and audio files.

The MediaInfo data display includes:
 - Container: format, profile, commercial name of the format, duration,
   overall bit rate, writing application and library, title, author,
   director, album, track number, date, duration...
 - Video: format, codec id, aspect, frame rate, bit rate, color space,
   chroma subsampling, bit depth, scan type, scan order...
 - Audio: format, codec id, sample rate, channels, bit depth, language,
   bit rate...
 - Text: format, codec id, language of subtitle...
 - Chapters: count of chapters, list of chapters...

MediaInfo analyticals include:
 - Container: MPEG-4, QuickTime, Matroska, AVI, MPEG-PS (including DVD),
   MPEG-TS (including Blu-ray), MXF, GXF, LXF, WMV, FLV, Real...
 - Tags: Id3v1, Id3v2, Vorbis comments, APE tags...
 - Video: MPEG-1/2 Video, H.263, MPEG-4 Visual (including DivX, XviD),
   H.264/AVC, Dirac...
 - Audio: MPEG Audio (including MP3), AC3, DTS, AAC, Dolby E, AES3, FLAC...
 - Subtitles: CEA-608, CEA-708, DTVCC, SCTE-20, SCTE-128, ATSC/53, CDP, 
   DVB Subtitle, Teletext, SRT, SSA, ASS, SAMI...

This package includes the command line interface.


%if %{undefined rhel_version} || 0%{?rhel_version} < 600

%package gui
Summary:     Convenient unified display of the most relevant technical and tag data for video and audio files (GUI)
Group:       Productivity/Multimedia/Other
Requires:    libzen0 >= %libzen_version
Requires:    libmediainfo0 >= %{version}
%if %{undefined rhel_version} || 0%{?rhel_version} < 600
%if 0%{?mandriva_version}
%ifarch x86_64
Requires:    lib64wxgtku2.8
%else
Requires:    libwxgtku2.8
%endif
%else
%if 0%{?suse_version} && 0%{?suse_version} >= 1140
Requires:    wxWidgets
%else
Requires:    wxGTK
%endif
%endif
%endif

%description gui
MediaInfo is a convenient unified display of the most relevant technical
and tag data for video and audio files.

The MediaInfo data display includes:
 - Container: format, profile, commercial name of the format, duration,
   overall bit rate, writing application and library, title, author,
   director, album, track number, date, duration...
 - Video: format, codec id, aspect, frame rate, bit rate, color space,
   chroma subsampling, bit depth, scan type, scan order...
 - Audio: format, codec id, sample rate, channels, bit depth, language,
   bit rate...
 - Text: format, codec id, language of subtitle...
 - Chapters: count of chapters, list of chapters...

MediaInfo analyticals include:
 - Container: MPEG-4, QuickTime, Matroska, AVI, MPEG-PS (including DVD),
   MPEG-TS (including Blu-ray), MXF, GXF, LXF, WMV, FLV, Real...
 - Tags: Id3v1, Id3v2, Vorbis comments, APE tags...
 - Video: MPEG-1/2 Video, H.263, MPEG-4 Visual (including DivX, XviD),
   H.264/AVC, Dirac...
 - Audio: MPEG Audio (including MP3), AC3, DTS, AAC, Dolby E, AES3, FLAC...
 - Subtitles: CEA-608, CEA-708, DTVCC, SCTE-20, SCTE-128, ATSC/53, CDP, 
   DVB Subtitle, Teletext, SRT, SSA, ASS, SAMI...

This package contains the graphical user interface.

%endif

%prep
%setup -q -n MediaInfo
dos2unix     *.html *.txt Release/*.txt
%__chmod 644 *.html *.txt Release/*.txt

%build
export CFLAGS="$RPM_OPT_FLAGS"
export CXXFLAGS="$RPM_OPT_FLAGS"

# build CLI
pushd Project/GNU/CLI
    %__chmod +x autogen
    ./autogen
    %configure

    %__make %{?jobs:-j%{jobs}}
popd

%if %{undefined rhel_version} || 0%{?rhel_version} < 600

# now build GUI
pushd Project/GNU/GUI
    %__chmod +x autogen
    ./autogen
    %configure

    %__make %{?jobs:-j%{jobs}}
popd

%endif

%install
pushd Project/GNU/CLI
    %__make install-strip DESTDIR=%{buildroot}
popd

%if %{undefined rhel_version} || 0%{?rhel_version} < 600

pushd Project/GNU/GUI
    %__make install-strip DESTDIR=%{buildroot}
popd

# icon
%__install -dm 755 %{buildroot}%{_datadir}/icons/hicolor/256x256/apps
%__install -m 644 Source/Resource/Image/MediaInfo.png %{buildroot}%{_datadir}/icons/hicolor/256x256/apps/%{name}.png
%__install -dm 755 %{buildroot}%{_datadir}/pixmaps
%__install -m 644 Source/Resource/Image/MediaInfoBig.png %{buildroot}%{_datadir}/pixmaps/%{name}.png

# menu-entry
%__install -dm 755 %{buildroot}/%{_datadir}/applications
%__install -m 644 Project/GNU/GUI/mediainfo-gui.desktop \
    %{buildroot}/%{_datadir}/applications
%if 0%{?suse_version}
  %suse_update_desktop_file -n mediainfo-gui AudioVideo AudioVideoEditing
%endif
%__install -dm 755 %{buildroot}/%{_datadir}/apps/konqueror/servicemenus
%__install -m 644 Project/GNU/GUI/mediainfo-gui.kde3.desktop \
    %{buildroot}/%{_datadir}/apps/konqueror/servicemenus/mediainfo-gui.desktop
%if 0%{?suse_version}
  %suse_update_desktop_file -n %{buildroot}/%{_datadir}/apps/konqueror/servicemenus/mediainfo-gui.desktop AudioVideo AudioVideoEditing
%endif
%__install -dm 755 %{buildroot}/%{_datadir}/kde4/services/ServiceMenus/
%__install -m 644 Project/GNU/GUI/mediainfo-gui.kde4.desktop \
    %{buildroot}/%{_datadir}/kde4/services/ServiceMenus/mediainfo-gui.desktop
%if 0%{?suse_version}
  %suse_update_desktop_file -n %{buildroot}/%{_datadir}/kde4/services/ServiceMenus/mediainfo-gui.desktop AudioVideo AudioVideoEditing
%endif

%endif

%clean
[ -d "%{buildroot}" -a "%{buildroot}" != "" ] && %__rm -rf "%{buildroot}"

%files
%defattr(-,root,root,-)
%doc Release/ReadMe_CLI_Linux.txt
%doc License.html History_CLI.txt
%{_bindir}/mediainfo

%if %{undefined rhel_version} || 0%{?rhel_version} < 600

%files gui
%defattr(-,root,root,-)
%doc Release/ReadMe_GUI_Linux.txt
%doc License.html History_GUI.txt
%{_bindir}/mediainfo-gui
%{_datadir}/applications/*.desktop
%{_datadir}/pixmaps/*.png
%dir %{_datadir}/icons/hicolor
%dir %{_datadir}/icons/hicolor/256x256
%dir %{_datadir}/icons/hicolor/256x256/apps
%{_datadir}/icons/hicolor/256x256/apps/*.png
%dir %{_datadir}/apps
%dir %{_datadir}/apps/konqueror
%dir %{_datadir}/apps/konqueror/servicemenus
%{_datadir}/apps/konqueror/servicemenus/*.desktop
%dir %{_datadir}/kde4
%dir %{_datadir}/kde4/services
%dir %{_datadir}/kde4/services/ServiceMenus
%{_datadir}/kde4/services/ServiceMenus/*.desktop

%endif

%changelog
* Tue Jan 01 2009 MediaArea.net SARL <info@mediaarea.net> - 0.7.58
- See History.txt for more info and real dates
- Previous packages made by Toni Graffy <toni@links2linux.de>
