libmediainfo.so - http://MediaArea.net/MediaInfo
Copyright (c) MediaArea.net SARL, Info@MediaArea.net

This program is freeware (BSD-style license).
See License.html for more information.


For software developers
-----------------------
Don't forget to put libmediainfo.so* in your library folder and Example.ogg in your executable folder.
(note: libmediainfo.so* may be in your executable folder too)

Note: versioning method, for people who develop with LoadLibrary method
- if one of 2 first numbers change, there is no guaranties that the DLL is compatible with old one. You should verify with MediaInfo_Option("Version") if you are compatible
- if one of 2 last numbers change, there is a guaranty that the DLL is compatible with old one.
So you should test the version of the DLL, and if one of the 2 first numbers change, not load it.
