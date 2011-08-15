libmediainfo.so - http://mediainfo.sourceforge.net
Copyright (c) 2002-2011 MediaArea.net SARL, Info@MediaArea.net

This program is freeware (LGLPv3+).
See License.html for more information.


For software developers
-----------------------
You can use it as you want (example: without this text file, without sources),
but a reference to "http://mediainfo.sourceforge.net" in your software would be appreciated.

There are examples for:
- GNU: GNU Autotools based
- CodeBlock: Code::Blocks IDE 1.0-RC2
- MSCS: Mono
- Java: Netbeans (with JNA or JNative binding)
- Python 2
- Python 3
- C/C++
Don't forget to put libmediainfo.so* in your library folder and Example.ogg in your executable folder.
(note: libmediainfo.so* may be in your executable folder too)

Note: versioning method, for people who develop with LoadLibrary method
- if one of 2 first numbers change, there is no guaranties that the DLL is compatible with old one. You should verify with MediaInfo_Option("Version") if you are compatible
- if one of 2 last numbers change, there is a guaranty that the DLL is compatible with old one.
So you should test the version of the DLL, and if one of the 2 first numbers change, not load it.
