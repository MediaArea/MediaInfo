# Copyright (c) 2007-2008 oc2pus
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments to toni@links2linux.de

# norootforbuild

%define libmediainfo_version	0.7.52
%define libzen_version			0.4.23

Name:			libmediainfo
Version:		%libmediainfo_version
Release:		1
Summary:		Supplies technical and tag information about a video or audio file
Group:			System/Libraries
License:		GPL
URL:			http://mediainfo.sourceforge.net/
Packager:		MediaArea.net SARL <info@mediaarea.net>
Source0:		libmediainfo_%{version}-1.tar.gz
BuildRoot:		%{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:	dos2unix
BuildRequires: 	gcc-c++
BuildRequires:	libzen-devel >= %libzen_version
BuildRequires:	pkgconfig
BuildRequires: 	zlib-devel
BuildRequires: 	glibc-devel
BuildRequires:	doxygen

%description
MediaInfo is a library used for retrieving technical information and other
metadata about audio or video files.

A non-exhaustive list of the information MediaInfo can retrieve from media
files include:
 - General: title, author, director, album, track number, date, duration...
 - Video: codec, aspect, fps, bitrate...
 - Audio: codec, sample rate, channels, language, bitrate...
 - Text: language of subtitle
 - Chapters: number of chapters, list of chapters

MediaInfo supports the following formats:
 - Video: MKV, OGM, AVI, DivX, WMV, QuickTime, Real, MPEG-1, MPEG-2,
          MPEG-4, DVD (VOB)...
 - Video Codecs: DivX, XviD, MSMPEG4, ASP, H.264, AVC...)
 - Audio: OGG, MP3, WAV, RA, AC3, DTS, AAC, M4A, AU, AIFF...
 - Subtitles: SRT, SSA, ASS, SAMI...

%package -n libmediainfo0
Summary:		Library for reading metadata from media files -- shared library
Group:			System/Libraries
Requires:		libzen0 >= %libzen_version
Requires:		glibc

%description -n libmediainfo0
MediaInfo is a library used for retrieving technical information and other
metadata about audio or video files.

A non-exhaustive list of the information MediaInfo can retrieve from media
files include:
 - General: title, author, director, album, track number, date, duration...
 - Video: codec, aspect, fps, bitrate...
 - Audio: codec, sample rate, channels, language, bitrate...
 - Text: language of subtitle
 - Chapters: number of chapters, list of chapters

MediaInfo supports the following formats:
 - Video: MKV, OGM, AVI, DivX, WMV, QuickTime, Real, MPEG-1, MPEG-2,
          MPEG-4, DVD (VOB)...
 - Video Codecs: DivX, XviD, MSMPEG4, ASP, H.264, AVC...)
 - Audio: OGG, MP3, WAV, RA, AC3, DTS, AAC, M4A, AU, AIFF...
 - Subtitles: SRT, SSA, ASS, SAMI...

This package contains the shared library needed for running applications which
use this library.

%package -n libmediainfo-devel
Summary:	Include files and mandatory libraries for development
Group:		Development/Libraries/C and C++
Requires:	libmediainfo0 = %{version}
Requires:	libzen-devel >= %libzen_version
Requires:	glibc-devel

%description -n libmediainfo-devel
MediaInfo is a library used for retrieving technical information and other
metadata about audio or video files.

A non-exhaustive list of the information MediaInfo can retrieve from media
files include:
 - General: title, author, director, album, track number, date, duration...
 - Video: codec, aspect, fps, bitrate...
 - Audio: codec, sample rate, channels, language, bitrate...
 - Text: language of subtitle
 - Chapters: number of chapters, list of chapters

MediaInfo supports the following formats:
 - Video: MKV, OGM, AVI, DivX, WMV, QuickTime, Real, MPEG-1, MPEG-2,
          MPEG-4, DVD (VOB)...
 - Video Codecs: DivX, XviD, MSMPEG4, ASP, H.264, AVC...)
 - Audio: OGG, MP3, WAV, RA, AC3, DTS, AAC, M4A, AU, AIFF...
 - Subtitles: SRT, SSA, ASS, SAMI...

This package contains the headers and other development support files needed
for compiling and linking applications and libraries which use this library.

%prep
%setup -q -n MediaInfoLib
cp           Release/ReadMe_DLL_Linux.txt ReadMe.txt
mv           History_DLL.txt History.txt
dos2unix     *.txt *.html Source/Doc/*.html Source/Example/*.*
%__chmod 644 *.txt *.html Source/Doc/*.html Source/Example/*.*

%build
export CFLAGS="$RPM_OPT_FLAGS"
export CPPFLAGS="$RPM_OPT_FLAGS"
export CXXFLAGS="$RPM_OPT_FLAGS"

pushd Source/Doc/
	doxygen Doxyfile
popd
cp Source/Doc/*.html ./

pushd Project/GNU/Library
	%__chmod +x autogen
	./autogen
	%configure --disable-static --enable-shared --disable-libcurl --disable-libmms \
%if ( 0%{?centos_version} && 0%{?centos_version} < 500 ) || ( 0%{?rhel_version} && 0%{?rhel_version} < 500 )  
%else
        --enable-visibility
%endif

	%__make clean
	%__make %{?jobs:-j%{jobs}}
popd

%install
pushd Project/GNU/Library/
	%__make install-strip DESTDIR=%{buildroot}
popd

# MediaInfoDLL headers and MediaInfo-config
%__install -dm 755 %{buildroot}%{_includedir}/MediaInfo
%__install -m 644 Source/MediaInfo/MediaInfo.h %{buildroot}%{_includedir}/MediaInfo
%__install -m 644 Source/MediaInfo/MediaInfoList.h %{buildroot}%{_includedir}/MediaInfo
%__install -m 644 Source/MediaInfo/MediaInfo_Const.h %{buildroot}%{_includedir}/MediaInfo
%__install -m 644 Source/MediaInfo/MediaInfo_Events.h %{buildroot}%{_includedir}/MediaInfo
%__install -dm 755 %{buildroot}%{_includedir}/MediaInfoDLL
%__install -m 644 Source/MediaInfoDLL/MediaInfoDLL.cs %{buildroot}%{_includedir}/MediaInfoDLL
%__install -m 644 Source/MediaInfoDLL/MediaInfoDLL.h %{buildroot}%{_includedir}/MediaInfoDLL
%__install -m 644 Source/MediaInfoDLL/MediaInfoDLL_Static.h %{buildroot}%{_includedir}/MediaInfoDLL
%__install -m 644 Source/MediaInfoDLL/MediaInfoDLL.JNA.java %{buildroot}%{_includedir}/MediaInfoDLL
%__install -m 644 Source/MediaInfoDLL/MediaInfoDLL.JNative.java %{buildroot}%{_includedir}/MediaInfoDLL
%__install -m 644 Source/MediaInfoDLL/MediaInfoDLL.py %{buildroot}%{_includedir}/MediaInfoDLL
%__install -m 644 Source/MediaInfoDLL/MediaInfoDLL3.py %{buildroot}%{_includedir}/MediaInfoDLL

%__sed -i -e 's|Version: |Version: %{version}|g' \
	Project/GNU/Library/libmediainfo.pc
%__install -dm 755 %{buildroot}%{_libdir}/pkgconfig
%__install -m 644 Project/GNU/Library/libmediainfo.pc \
	%{buildroot}%{_libdir}/pkgconfig

%clean
[ -d "%{buildroot}" -a "%{buildroot}" != "" ] && %__rm -rf "%{buildroot}"

%post -n libmediainfo0 -p /sbin/ldconfig

%postun -n libmediainfo0 -p /sbin/ldconfig

%files -n libmediainfo0
%defattr(-,root,root,-)
%doc History.txt License.html ReadMe.txt
%{_libdir}/libmediainfo.so.*

%files -n libmediainfo-devel
%defattr(-,root,root,-)
%doc Changes.txt Documentation.html Doc Source/Example
%dir %{_includedir}/MediaInfo
%{_includedir}/MediaInfo/*
%dir %{_includedir}/MediaInfoDLL
%{_includedir}/MediaInfoDLL/*
%{_libdir}/libmediainfo.la
%{_libdir}/libmediainfo.so
%{_libdir}/pkgconfig/*.pc

%changelog
* Tue Jan 01 2009 MediaArea.net SARL <info@mediaarea.net> - 0.7.52-0
- See History.txt for more info and real dates
- Previous packages made by Toni Graffy <toni@links2linux.de>
