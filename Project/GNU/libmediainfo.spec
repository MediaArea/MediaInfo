# Copyright (c) 2007-2012 MediaArea.net SARL <info@mediaarea.net>
# This file and all modifications and additions to the pristine
# package are under the same license as the package itself.

# norootforbuild

%define libmediainfo_version        0.7.61
%define libzen_version              0.4.25

Name:           libmediainfo
Version:        %libmediainfo_version
Release:        1
Summary:        Most relevant technical and tag data for video and audio files (Library)

Group:          System/Libraries
License:        LGPLv3+
URL:            http://mediainfo.sourceforge.net/
Packager:       MediaArea.net SARL <info@mediaarea.net>
Source0:        libmediainfo_%{version}-1.tar.gz

BuildRequires:  dos2unix
BuildRequires:  gcc-c++
BuildRequires:  libzen-devel >= %libzen_version
BuildRequires:  pkgconfig
BuildRequires:  zlib-devel
BuildRequires:  glibc-devel
BuildRequires:  doxygen
BuildRequires:  libtool
BuildRequires:  automake
BuildRequires:  autoconf

BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root

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

%package -n libmediainfo0
Summary:        Most relevant technical and tag data for video and audio files (Library)
Group:          System/Libraries
Requires:       libzen0 >= %libzen_version
Requires:       glibc

%description -n libmediainfo0
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

This package contains the shared library for MediaInfo.

%package -n libmediainfo-devel
Summary:        Most relevant technical and tag data for video and audio files (Library devel)
Group:          Development/Libraries/C and C++
Requires:       libmediainfo0%{?_isa} = %{version}
Requires:       libzen-devel%{?_isa} >= %libzen_version
Requires:       glibc-devel
Obsoletes:      libmediainfo0-devel

%description -n libmediainfo-devel
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

This package contains the development support files needed for compiling
and linking applications and libraries which use MediaInfo shared library.

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

    %__make %{?_smp_mflags}
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

%__sed -i -e 's|Version: |Version: %{version}|g' Project/GNU/Library/libmediainfo.pc
%__install -dm 755 %{buildroot}%{_libdir}/pkgconfig
%__install -m 644 Project/GNU/Library/libmediainfo.pc %{buildroot}%{_libdir}/pkgconfig

rm -f %{buildroot}%{_libdir}/%{name}.*a

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
%{_libdir}/libmediainfo.so
%{_libdir}/pkgconfig/*.pc

%changelog
* Tue Jan 01 2012 MediaArea.net SARL <info@mediaarea.net> - 0.7.61-0
- See History.txt for more info and real dates
- Previous packages made by Toni Graffy <toni@links2linux.de>
