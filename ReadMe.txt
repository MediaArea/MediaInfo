MediaInfo.lib : static library
MediaInfo.Dll : dynamic library
HowToUse.exe  : static-linked example
HowToUse_Dll  : dynamic-linked example

For DLL, there are two versions of functions :
- MediaInfoLib_XXX : Unicode version (international, UTF16, 2-byte characters), this become the main version, there are not only English language in the world ;-). So now Chinese / French / German languages can be showed in same time
- MediaInfoLibA_XXX : Deprecated, Ansi version (localized, UTF8, 1-byte characters), as the old DLL : you should link your old code to this functions.

MediaInfo library (static and/or shared) can be compiled within these different tools :
- Project/BCB        : Borland C++ Builder 6
- Project/CMake      : cmake, C++
- Project/GNU        : g++, C++ 
- Project/MSVC2005   : Visual Studio 9  (aka 2005), C++
- Project/MSVC2008   : Visual Studio 10 (aka 2008), C++
- Project/MSVC2010   : Visual Studio 11 (aka 2010), C++
- Project/MSVC2012   : Visual Studio 12 (aka 2012), C++
- Project/Solaris    : g++, C++
- Project/CodeBlocks : C++

Examples for using the MediaInfo library are given under those folders :
- Project/Delphi     : Borland Delphi 7
- Project/Java       : Java
- Project/NetBeans   : Java binding
- Project/PureBasic  : Basic
- Project/MSCS2008   : Visual Studio 10 (aka 2008), C#
- Project/MSCS2010   : Visual Studio 11 (aka 2010), C#
- Project/MSJS       : Visual Studio 7.1 (aka 2003), J#
- Project/MSVB       : Visual Studio 7.1 (aka 2003), Basic (and no more VB6 code)
- Project/MSVB2010   : Visual Studio 11 (aka 2010), Basic

MediaInfo library needs only 2 extra libraries to compile properly :
- zlib.lib
- zenlib.lib

You can enable or disable format support with Compilation macro defines :
MEDIAINFO_XXX_NO or MEDIAINFO_XXX_YES (if both are defined, NO is choosen)
XXX may be :
- VIDEO (all videos)
- AUDIO (all audios)
- RIFF : Microsoft Avi, Wav...
- OGG : Ogg, Ogm...
- MPEG : Mpg, Mpeg, Vob...
- MPEG4 : Mp4, M4a, M4v...
- MPEGA : mpa, mp2, mp3...
- MPEGV : mpv, m2v...
- WM : Wma, Wmv...
- QT : Qt, Mov...
- RM : Rm, Rmvb...
- DVDV : Ifo of DVD
- AAC : Aac
- DTS : Dts
- AC3 : Ac3
- MK : Matroska
- APE : Monkey Audio, Ape, Mac...
- FLAC : Flac
- SNDFILE : SndFile related
- etc...