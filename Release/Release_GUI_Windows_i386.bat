@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@rem echo off

@rem --- Clean up ---
del   MediaInfo_GUI_Windows_i386.exe
del   MediaInfo_GUI_Windows_i386_WithoutInstaller.7z
rmdir MediaInfo_GUI_Windows_i386 /S /Q
mkdir MediaInfo_GUI_Windows_i386


@rem --- Preparing ---
copy BCB\GUI\MediaInfo_GUI.exe BCB\GUI\MediaInfo.exe

@rem --- Copying : Exe ---
copy  ..\Project\BCB\GUI\Win32\Release\MediaInfo_GUI.exe MediaInfo_GUI_Windows_i386\MediaInfo.exe
copy  ..\..\MediaInfoLib\Project\MSVC2015\Win32\Release\MediaInfo.dll MediaInfo_GUI_Windows_i386\MediaInfo_i386.dll
xcopy ..\..\MediaInfoLib\Project\MSVC2015\Win32\Release\MediaInfo_InfoTip.dll MediaInfo_GUI_Windows_i386\ /S

@rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\* MediaInfo_GUI_Windows_i386\Plugin\ /S

@rem --- Copying : Information files ---
copy ..\License.html MediaInfo_GUI_Windows_i386\
copy ..\History_GUI.txt MediaInfo_GUI_Windows_i386\History.txt
copy ReadMe_GUI_Windows.txt MediaInfo_GUI_Windows_i386\ReadMe.txt


rem --- Compressing Archive ---
cd MediaInfo_GUI_Windows_i386\
..\..\..\..\MediaArea-Utils-Binaries\Windows\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_GUI_Windows_i386_WithoutInstaller.7z *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_GUI_Windows_i386\ /S /Q
:SkipCleanUp