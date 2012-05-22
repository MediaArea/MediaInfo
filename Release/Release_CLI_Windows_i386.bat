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
del   MediaInfo_CLI_Windows_i386.zip
rmdir MediaInfo_CLI_Windows_i386 /S /Q
mkdir MediaInfo_CLI_Windows_i386

rem --- Copying : Exe ---
xcopy ..\Project\MSVC2008\CLI\Win32\Release\MediaInfo.exe MediaInfo_CLI_Windows_i386\ /S
xcopy ..\..\MediaInfoLib\Project\MSVC2005\DLL\Win32\Release\MediaInfo.dll MediaInfo_CLI_Windows_i386\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\Custom\* MediaInfo_CLI_Windows_i386\Plugin\Custom\ /S

rem --- Copying : Information files ---
copy ..\License.* MediaInfo_CLI_Windows_i386\
copy ..\History_CLI.txt MediaInfo_CLI_Windows_i386\History.txt
copy Readme_CLI_Windows.txt MediaInfo_CLI_Windows_i386\ReadMe.txt

rem --- Copying : Contrib files ---
mkdir MediaInfo_CLI_Windows_i386\Contrib
copy ..\Contrib\CLI_Help.doc MediaInfo_CLI_Windows_i386\Contrib\

rem --- Compressing Archive ---
cd MediaInfo_CLI_Windows_i386\
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -tzip ..\MediaInfo_CLI_Windows_i386.zip *
cd ..


rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_CLI_Windows_i386\ /S /Q
:SkipCleanUp
