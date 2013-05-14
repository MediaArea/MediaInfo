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

@echo off

rem --- Clean up ---
del MediaInfo_Lib_GNU_Prepare.7z
rmdir MediaInfo_Lib_GNU_Prepare /S /Q
mkdir MediaInfo_Lib_GNU_Prepare

rem --- Copying : Projects ---
xcopy ..\Project\GNU\Library\autogen MediaInfo_Lib_GNU_Prepare\Project\GNU\Library\ /S
xcopy ..\Project\GNU\Library\configure.ac MediaInfo_Lib_GNU_Prepare\Project\GNU\Library\ /S
xcopy ..\Project\GNU\Library\Makefile.am MediaInfo_Lib_GNU_Prepare\Project\GNU\Library\ /S
xcopy ..\Project\GNU\Library\libmediainfo.pc.in MediaInfo_Lib_GNU_Prepare\Project\GNU\Library\ /S
xcopy ..\Project\GNU\Library\libmediainfo-config.in MediaInfo_Lib_GNU_Prepare\Project\GNU\Library\ /S

rem --- Copying : Release ---
xcopy Release_Lib_GNU_FromSource.sh MediaInfo_Lib_GNU_Prepare\Release\

rem --- Copying : Sources ---
xcopy ..\Source\ThirdParty\*.h MediaInfo_Lib_GNU_Prepare\Source\ThirdParty\ /S
xcopy ..\Source\ThirdParty\*.c MediaInfo_Lib_GNU_Prepare\Source\ThirdParty\ /S
xcopy ..\Source\ThirdParty\*.cpp MediaInfo_Lib_GNU_Prepare\Source\ThirdParty\ /S
xcopy ..\Source\MediaInfo\*.h MediaInfo_Lib_GNU_Prepare\Source\MediaInfo\ /S
xcopy ..\Source\MediaInfo\*.cpp MediaInfo_Lib_GNU_Prepare\Source\MediaInfo\ /S
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.cpp MediaInfo_Lib_GNU_Prepare\Source\MediaInfoDLL\ /S
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.h MediaInfo_Lib_GNU_Prepare\Source\MediaInfoDLL\ /S
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL_Static.h MediaInfo_Lib_GNU_Prepare\Source\MediaInfoDLL\ /S

rem --- Copying : Information files ---
copy ..\History_DLL.txt MediaInfo_Lib_GNU_Prepare\
copy ..\License.html MediaInfo_Lib_GNU_Prepare\
copy ..\Changes.txt MediaInfo_Lib_GNU_Prepare\
copy ReadMe_DLL_Linux.txt MediaInfo_Lib_GNU_Prepare\Release\
copy ReadMe_DLL_Mac.txt MediaInfo_Lib_GNU_Prepare\Release\


rem --- Compressing Archive ---
if "%2"=="SkipCompression" goto SkipCompression
move MediaInfo_Lib_GNU_Prepare MediaInfoLib
..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -t7z -mx9 MediaInfo_Lib_GNU_Prepare.7z MediaInfoLib\*
move MediaInfoLib MediaInfo_Lib_GNU_Prepare
:SkipCompression

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_Lib_GNU_Prepare /S /Q
:SkipCleanUp
