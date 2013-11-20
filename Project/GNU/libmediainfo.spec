%define libmediainfo_version      0.7.65
%define libzen_version            0.4.29

Name:           libmediainfo
Version:        %{libmediainfo_version}
Release:        1
Summary:        Most relevant technical and tag data for video and audio files

Group:          System/Libraries
License:        BSD-2-Clause
URL:            http://MediaArea.net/MediaInfo
Packager:       MediaArea.net SARL <info@mediaarea.net>
Source0:        %{name}_%{version}-1.tar.gz

BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:  gcc-c++
BuildRequires:  libzen-devel >= %{libzen_version}
BuildRequires:  pkgconfig
BuildRequires:  zlib-devel
BuildRequires:  doxygen
BuildRequires:  libtool
BuildRequires:  automake
BuildRequires:  autoconf

%description
MediaInfo is a convenient unified display of the most relevant technical
and tag data for video and audio files.

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

%package -n %{name}0
Summary:        Most relevant technical and tag data for video and audio files -- runtime
Group:          System/Libraries

%description -n %{name}0
MediaInfo is a convenient unified display of the most relevant technical
and tag data for video and audio files.

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

%package        doc
Summary:        Most relevant technical and tag data for video and audio files -- documentation
Group:          Development/Libraries
Requires:       %{name}0 = %{version}

%description    doc
MediaInfo is a convenient unified display of the most relevant technical
and tag data for video and audio files.

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

This package contains the documentation

%package        devel
Summary:        Most relevant technical and tag data for video and audio files -- development
Group:          Development/Libraries
Requires:       %{name}0%{?_isa} = %{version}
Requires:       libzen-devel%{?_isa} >= %{libzen_version}

%description    devel
MediaInfo is a convenient unified display of the most relevant technical
and tag data for video and audio files.

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

This package contains the include files and mandatory libraries
for development.

%prep
%setup -q -n MediaInfoLib
cp           Release/ReadMe_DLL_Linux.txt ReadMe.txt
mv           History_DLL.txt History.txt
sed -i 's/.$//' *.txt Source/Example/* 

find Source -type f -exec chmod 644 {} ';'
chmod 644 *.txt License.html

pushd Project/GNU/Library
    autoreconf -i
popd

%build
export CFLAGS="%{optflags}"
export CPPFLAGS="%{optflags}"
export CXXFLAGS="%{optflags}"

pushd Source/Doc/
    doxygen Doxyfile
popd
cp Source/Doc/*.html ./

pushd Project/GNU/Library
    %configure --enable-shared --disable-static --enable-visibility

    make %{?_smp_mflags}
popd

%install
pushd Project/GNU/Library/
    make install DESTDIR=%{buildroot}
popd

# MediaInfoDLL headers and MediaInfo-config
install -dm 755 %{buildroot}%{_includedir}/MediaInfo
install -m 644 Source/MediaInfo/MediaInfo.h %{buildroot}%{_includedir}/MediaInfo
install -m 644 Source/MediaInfo/MediaInfoList.h %{buildroot}%{_includedir}/MediaInfo
install -m 644 Source/MediaInfo/MediaInfo_Const.h %{buildroot}%{_includedir}/MediaInfo
install -m 644 Source/MediaInfo/MediaInfo_Events.h %{buildroot}%{_includedir}/MediaInfo
install -dm 755 %{buildroot}%{_includedir}/MediaInfoDLL
install -m 644 Source/MediaInfoDLL/MediaInfoDLL.cs %{buildroot}%{_includedir}/MediaInfoDLL
install -m 644 Source/MediaInfoDLL/MediaInfoDLL.h %{buildroot}%{_includedir}/MediaInfoDLL
install -m 644 Source/MediaInfoDLL/MediaInfoDLL_Static.h %{buildroot}%{_includedir}/MediaInfoDLL
install -m 644 Source/MediaInfoDLL/MediaInfoDLL.JNA.java %{buildroot}%{_includedir}/MediaInfoDLL
install -m 644 Source/MediaInfoDLL/MediaInfoDLL.JNative.java %{buildroot}%{_includedir}/MediaInfoDLL
install -m 644 Source/MediaInfoDLL/MediaInfoDLL.py %{buildroot}%{_includedir}/MediaInfoDLL
install -m 644 Source/MediaInfoDLL/MediaInfoDLL3.py %{buildroot}%{_includedir}/MediaInfoDLL

sed -i -e 's|Version: |Version: %{version}|g' Project/GNU/Library/libmediainfo.pc
install -dm 755 %{buildroot}%{_libdir}/pkgconfig
install -m 644 Project/GNU/Library/libmediainfo.pc %{buildroot}%{_libdir}/pkgconfig

rm -f %{buildroot}%{_libdir}/%{name}.la


%post -n %{name}0 -p /sbin/ldconfig

%postun -n %{name}0 -p /sbin/ldconfig

%files -n %{name}0
%defattr(-,root,root,-)
%doc History.txt License.html ReadMe.txt
%{_libdir}/%{name}.so.*

%files    doc
%defattr(-,root,root,-)
%doc Changes.txt Documentation.html Doc Source/Example

%files    devel
%defattr(-,root,root,-)
%{_includedir}/MediaInfo
%{_includedir}/MediaInfoDLL
%{_libdir}/%{name}.so
%{_libdir}/pkgconfig/*.pc

%changelog
* Tue Jan 01 2012 MediaArea.net SARL <info@mediaarea.net> - 0.7.65-0
- See History.txt for more info and real dates
- Previous packages made by Toni Graffy <toni@links2linux.de>
- Fedora style made by Vasiliy N. Glazov <vascom2@gmail.com>
