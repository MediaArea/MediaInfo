# Copyright (c) 2007-2008 oc2pus
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments to toni@links2linux.de

# norootforbuild
%define is_mandrake	%(test -e /etc/mandrake-release && echo 1 || echo 0)
%define is_suse     %(test -e /etc/SuSE-release     && echo 1 || echo 0)
%define is_fedora	%(test -e /etc/fedora-release   && echo 1 || echo 0)

%define _prefix	/usr

Name:			mediainfo-gui
Version:		0.7.7.8
Release:		1
Summary:		Supplies technical and tag information about a video or audio file
Group:			Productivity/Multimedia/Other
License:		GPL
URL:			http://mediainfo.sourceforge.net/
Source0:		MediaInfo_%{version}_Source.tar.bz2
BuildRoot:		%{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:	dos2unix
BuildRequires: 	gcc-c++
BuildRequires:	libmediainfo-devel
BuildRequires:	libzen-devel
BuildRequires:	pkgconfig
BuildRequires:	wxGTK-devel
BuildRequires: 	zlib-devel

%description
MediaInfo supplies technical and tag information about a video or
audio file.

What information can I get from MediaInfo?
* General: title, author, director, album, track number, date, duration...
* Video: codec, aspect, fps, bitrate...
* Audio: codec, sample rate, channels, language, bitrate...
* Text: language of subtitle
* Chapters: number of chapters, list of chapters

DivX, XviD, H263, H.263, H264, x264, ASP, AVC, iTunes, MPEG-1,
MPEG1, MPEG-2, MPEG2, MPEG-4, MPEG4, MP4, M4A, M4V, QuickTime,
RealVideo, RealAudio, RA, RM, MSMPEG4v1, MSMPEG4v2, MSMPEG4v3,
VOB, DVD, WMA, VMW, ASF, 3GP, 3GPP, 3GP2

What format (container) does MediaInfo support?
* Video: MKV, OGM, AVI, DivX, WMV, QuickTime, Real, MPEG-1,
  MPEG-2, MPEG-4, DVD (VOB) (Codecs: DivX, XviD, MSMPEG4, ASP,
  H.264, AVC...)
* Audio: OGG, MP3, WAV, RA, AC3, DTS, AAC, M4A, AU, AIFF
* Subtitles: SRT, SSA, ASS, SAMI

%prep
%setup -q -n MediaInfo
dos2unix     *.html *.txt Release/*.txt
%__chmod 644 *.html *.txt Release/*.txt

%build
export CFLAGS="$RPM_OPT_FLAGS"
export CXXFLAGS="$RPM_OPT_FLAGS"

# now build GUI
pushd Project/GNU/GUI
	%__chmod +x autogen
	./autogen
	%configure

	%__make %{?jobs:-j%{jobs}}
popd

%install
pushd Project/GNU/GUI
	%__make install-strip DESTDIR=%{buildroot}
popd

# icon
%__install -dm 755 %{buildroot}%{_datadir}/icons/hicolor/128x128/apps
%__install -m 644 Source/Ressource/Image/MediaInfo.png \
	%{buildroot}%{_datadir}/icons/hicolor/128x128/apps/%{name}.png

# menu-entry
%__install -dm 755 %{buildroot}/%{_datadir}/applications
%__install -m 644 Project/GNU/GUI/mediainfo-gui.desktop \
	%{buildroot}/%{_datadir}/applications
%__install -dm 755 %{buildroot}/%{_datadir}/kde4/services/ServiceMenus/
%__install -m 644 Project/GNU/GUI/mediainfo-gui.kde4.desktop \
	%{buildroot}/%{_datadir}/kde4/services/ServiceMenus/

%clean
[ -d "%{buildroot}" -a "%{buildroot}" != "" ] && %__rm -rf "%{buildroot}"

%files
%defattr(-,root,root,-)
%doc Release/ReadMe_GUI_Linux.txt
%doc License.html History_GUI.txt
%{_bindir}/mediainfo-gui
%{_datadir}/applications/*.desktop
%{_datadir}/icons/hicolor/128x128/apps/*.png
%{_datadir}/kde4/services/ServiceMenus/*.desktop

%changelog
* Tue Nov 18 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.8-0.pm.2
- changed package layout, reflecting new upstream tar-balls
- rebuild using libzen(-devel) and libmediainfo(-devel)
- renamed package to mediainfo (obsoletes MediaInfo)
- added subpackage gui (obsoletes MediaInfo-gui)
- using desktop file from tarball (includes french localisation)
* Mon Nov 10 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.8-0.pm.1
- update to 0.7.7.8
* Fri Oct 17 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.7-0.pm.1
- update to 0.7.7.7
* Fri Sep 12 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.6-0.pm.1
- update to 0.7.7.6
* Fri Aug 22 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.5-0.pm.1
- update to 0.7.7.5
* Fri Jul 11 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.4-0.pm.1
- update to 0.7.7.4
* Fri Jun 26 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.3-0.pm.1
- update to 0.7.7.3
* Fri Jun 13 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.2-0.pm.1
- update to 0.7.7.2
* Fri May 30 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.1-0.pm.1
- update to 0.7.7.1
* Fri May 16 2008 Toni Graffy <toni@links2linux.de> - 0.7.7.0-0.pm.1
- update to 0.7.7.0
* Mon Apr 28 2008 Toni Graffy <toni@links2linux.de> - 0.7.6.4-0.pm.1
- update to 0.7.6.4
* Sat Apr 12 2008 Toni Graffy <toni@links2linux.de> - 0.7.6.3-0.pm.1
- update to 0.7.6.3
* Fri Mar 28 2008 Toni Graffy <toni@links2linux.de> - 0.7.6.2-0.pm.1
- update to 0.7.6.2
* Fri Mar 14 2008 Toni Graffy <toni@links2linux.de> - 0.7.6.1-0.pm.1
- update to 0.7.6.1
* Fri Feb 29 2008 Toni Graffy <toni@links2linux.de> - 0.7.6.0-0.pm.1
- update to 0.7.6.0
* Fri Feb 08 2008 Toni Graffy <toni@links2linux.de> - 0.7.5.9-0.pm.1
- update to 0.7.5.9
* Fri Feb 01 2008 Toni Graffy <toni@links2linux.de> - 0.7.5.8-0.pm.1
- update to 0.7.5.8
* Wed Jan 09 2008 Toni Graffy <toni@links2linux.de> - 0.7.5.7-0.pm.1
- update to 0.7.5.7
* Mon Dec 10 2007 Toni Graffy <toni@links2linux.de> - 0.7.5.6-0.pm.1
- update to 0.7.5.6
* Fri Nov 09 2007 Toni Graffy <toni@links2linux.de> - 0.7.5.5-0.pm.1
- update to 0.7.5.5
- libMatroska and libEbml are no more used
- The script now look for /usr/lib/libz.a and /usr/include/zlib.h before
  downloading at the official zlib website.
  --> If you have the development package of zlib, no more downloads!
* Sat Nov 03 2007 Toni Graffy <toni@links2linux.de> - 0.7.5.4-0.pm.1
- update to 0.7.5.4
* Fri Nov 02 2007 Toni Graffy <toni@links2linux.de> - 0.7.5.3-0.pm.1
- initial build 0.7.5.3
