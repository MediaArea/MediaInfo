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
del   MediaInfo_All_Windows_i386.zip
rmdir MediaInfo_All_Windows_i386 /S /Q
mkdir MediaInfo_All_Windows_i386


@rem --- Launch all other batches ---
call Release_GUI_Windows_i386 SkipCleanUp
call Release_CLI_Windows_i386 SkipCleanUp
cd ..\..\MediaInfoLib\Release\
call Release_DLL_Windows_i386 SkipCleanUp
cd ..\..\MediaInfo\Release\

@rem --- Modifying ---
move MediaInfo_GUI_Windows_i386\MediaInfo.exe MediaInfo_GUI_Windows_i386\MediaInfo_GUI.exe
move MediaInfo_CLI_Windows_i386\MediaInfo.exe MediaInfo_CLI_Windows_i386\MediaInfo_CLI.exe

@rem --- Copying ---
xcopy MediaInfo_GUI_Windows_i386\*.* MediaInfo_All_Windows_i386\ /S /Y
xcopy MediaInfo_CLI_Windows_i386\*.* MediaInfo_All_Windows_i386\ /S /Y
xcopy ..\..\MediaInfoLib\Release\MediaInfoDLL_Windows_i386\*.* MediaInfo_All_Windows_i386\ /S /Y

rem --- Compressing Archive ---
cd MediaInfo_All_Windows_i386\
..\..\..\Shared\Binary\7z a -r -t7z -mx9 ..\MediaInfo_All_Windows_i386.7z *
cd ..


@rem --- Clean up ---
rmdir MediaInfo_All_Windows_i386\ /S /Q
rmdir MediaInfo_GUI_Windows_i386\ /S /Q
rmdir MediaInfo_CLI_Windows_i386\ /S /Q
rmdir ..\..\MediaInfoLib\Release\MediaInfoDLL_Windows_i386\ /S /Q