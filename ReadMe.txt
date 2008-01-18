MediaInfo.lib : static library
MediaInfo.Dll : dynamic library
HowToUse.exe  : static-linked example
HowToUse_Dll  : dynamic-linked example

For DLL, there are two versions of functions : 
- MediaInfoLib_XXX : Unicode version (international, UTF16, 2-byte characters), this become the main version, there are not only English language in the world ;-). So now Chinese / French / German languages can be showed in same time
- MediaInfoLibA_XXX : Deprecated, Ansi version (localized, UTF8, 1-byte characters), as the old DLL : you should link your old code to this functions.

DLL can be compiled on theses plateforms :
- Project/BCB    : Borland C++ Builder 6
- Project/DevC   : DevC++ (planned)
- Project/MSVC   : Visual Studio 7.1 (aka 2003), C++

Examples for using theses DLL are given :
- Project/Delphi : Borland Delphi 7
- Project/MSCS   : Visual Studio 7.1 (aka 2003), C#
- Project/MSJS   : Visual Studio 7.1 (aka 2003), J#
- Project/MSVB   : Visual Studio 7.1 (aka 2003), Basic (and no more VB6 code)

For compiling (or using static-linked library), you will need of :
- Zlib.lib
- Id3Lib.lib
- Zenlib.lib
- Ebml.lib
- Matroska.lib
- WxBase.lib (part of wxWidgets project)
- Flac.lib (Flac and Flac++)
- MACLib.lib
- Mp4ff.lib

Download Shared.zip and read Configuration.txt for more info and ready-to-use files

You can enable or disable format support with Compilation defines :
MEDIAINFO_XXX_NO or MEDIAINFO_XXX_YES (if both are defined, NO is choosen)
XXX is :
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