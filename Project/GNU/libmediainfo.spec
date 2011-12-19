# Copyright (c) 2007-2008 oc2pus
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.
#
# Please submit bugfixes or comments to toni@links2linux.de

# norootforbuild

%define libmediainfo_version	0.7.52
%define libzen_version			0.4.23

Name:			libmediainfo0
Version:		%libmediainfo_version
Release:		1
Summary:		Supplies technical and tag information about a video or audio file
Group:			System/Libraries
License:		GPL
URL:			http://mediainfo.sourceforge.net/
Packager:		MediaArea.net <info@mediaarea.net>
Source0:		libmediainfo_%{version}-1.tar.gz
BuildRoot:		%{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:	dos2unix
BuildRequires: 	gcc-c++
BuildRequires:	libzen0-devel >= %libzen_version
BuildRequires:	pkgconfig
BuildRequires: 	zlib-devel
BuildRequires:	doxygen
Requires:		libzen0 >= %libzen_version

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

This package contains the shared library for MediaInfo.

%package -n libmediainfo0-devel
Summary:	Include files and mandatory libraries for development
Group:		Development/Libraries/C and C++
Requires:	libmediainfo0 = %{version}
Requires:	libzen0-devel >= %libzen_version

%description -n libmediainfo0-devel
Include files and mandatory libraries for development.

%prep
%setup -q -n MediaInfoLib
cp           Release/ReadMe_DLL_Linux.txt ReadMe.txt
mv           History_DLL.txt History.txt
dos2unix     *.txt *.html Source/Doc/*.html
%__chmod 644 *.txt *.html Source/Doc/*.html

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
	%configure --enable-shared --disable-libcurl --disable-libmms \
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

%files
%defattr(-,root,root,-)
%doc History.txt License.html ReadMe.txt
%{_libdir}/libmediainfo.so.*

%files -n libmediainfo0-devel
%defattr(-,root,root,-)
%doc Changes.txt Documentation.html Doc Source/Example
%dir %{_includedir}/MediaInfo
%{_includedir}/MediaInfo/*
%dir %{_includedir}/MediaInfoDLL
%{_includedir}/MediaInfoDLL/*
%{_libdir}/libmediainfo.a
%{_libdir}/libmediainfo.la
%{_libdir}/libmediainfo.so
%{_libdir}/pkgconfig/*.pc

%changelog
* Tue Jan 01 2009 MediaArea.net <info@mediaarea.net> - 0.7.52-0
- See History.txt for more info and real dates
- Previous packages made by Toni Graffy <toni@links2linux.de>
