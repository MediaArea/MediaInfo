@rem MediaInfo
@rem Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
@rem
@rem This program is free software: you can redistribute it and/or modify it
@rem under the terms of the GNU Lesser General Public License as published by
@rem the Free Software Foundation, either version 3 of the License, or
@rem (at your option) any later version.
@rem
@rem This program is distributed in the hope that it will be useful,
@rem but WITHOUT ANY WARRANTY; without even the implied warranty of
@rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
@rem GNU Lesser General Public License for more details.
@rem
@rem You should have received a copy of the GNU Lesser General Public License
@rem along with this program. If not, see <http:@remwww.gnu.org/licenses/>.
@rem
@rem ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@rem ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

@rem echo off

@rem --- Clean up ---
del   MediaInfo_GUI_Windows_i386.exe
del   MediaInfo_GUI_Windows_i386_WithoutInstaller.7z
rmdir MediaInfo_GUI_Windows_i386 /S /Q
mkdir MediaInfo_GUI_Windows_i386


@rem --- Preparing ---
copy BCB\GUI\MediaInfo_GUI.exe BCB\GUI\MediaInfo.exe

@rem --- Copying : Exe ---
copy  ..\Project\BCB\GUI\Release_Build\MediaInfo_GUI.exe MediaInfo_GUI_Windows_i386\MediaInfo.exe
xcopy ..\..\MediaInfoLib\Project\MSVC2005\DLL\Win32\Release\MediaInfo.dll MediaInfo_GUI_Windows_i386\ /S
xcopy ..\..\MediaInfoLib\Project\MSVC2010\ShellExtension\Win32\Release\MediaInfo_InfoTip.dll MediaInfo_GUI_Windows_i386\ /S

@rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\* MediaInfo_GUI_Windows_i386\Plugin\ /S

@rem --- Copying : Information files ---
copy ..\License.html MediaInfo_GUI_Windows_i386\
copy ..\History_GUI.txt MediaInfo_GUI_Windows_i386\History.txt
copy ReadMe_GUI_Windows.txt MediaInfo_GUI_Windows_i386\ReadMe.txt


rem --- Compressing Archive ---
cd MediaInfo_GUI_Windows_i386\
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_GUI_Windows_i386_WithoutInstaller.7z *
cd ..

rem --- Installer ---
copy ..\Project\BCB\GUI\Release_Build\MediaInfo_GUI.exe BCB\GUI\MediaInfo.exe
..\..\Shared\Binary\Windows_i386\NSIS\makensis ..\Source\Install\MediaInfo_GUI_Windows_i386.nsi

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_GUI_Windows_i386\ /S /Q
:SkipCleanUp