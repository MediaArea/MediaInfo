@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@echo off

rem --- Search binaries ---
set BPATH=
if exist "%~dp0\..\..\..\MediaArea-Utils-Binaries" set BPATH="%~dp0\..\..\..\MediaArea-Utils-Binaries"
if exist "%~dp0\..\..\MediaArea-Utils-Binaries" set BPATH="%~dp0\..\..\MediaArea-Utils-Binaries"
if "%BPATH%"=="" (
    echo "ERROR: binaries path not found"
    exit /b 1
)

rem --- Clean up ---
del MediaInfo_GUI_GNU_Prepare.7z
rmdir MediaInfo_GUI_GNU_Prepare /S /Q
mkdir MediaInfo_GUI_GNU_Prepare


rem --- Copying : Sources ---
xcopy ..\Source\Common\*.cpp MediaInfo_GUI_GNU_Prepare\Source\Common\ /S
xcopy ..\Source\Common\*.h MediaInfo_GUI_GNU_Prepare\Source\Common\ /S
xcopy ..\Source\CLI\*.cpp MediaInfo_GUI_GNU_Prepare\Source\CLI\ /S
xcopy ..\Source\CLI\*.h MediaInfo_GUI_GNU_Prepare\Source\CLI\ /S
xcopy ..\Source\GUI\Common\*.cpp MediaInfo_GUI_GNU_Prepare\Source\GUI\Common\ /S
xcopy ..\Source\GUI\Common\*.h MediaInfo_GUI_GNU_Prepare\Source\GUI\Common\ /S
xcopy ..\Source\GUI\WxWidgets\*.cpp MediaInfo_GUI_GNU_Prepare\Source\GUI\WxWidgets\ /S
xcopy ..\Source\GUI\WxWidgets\*.h MediaInfo_GUI_GNU_Prepare\Source\GUI\WxWidgets\ /S
xcopy ..\Source\*.png MediaInfo_GUI_GNU_Prepare\Source\ /S
xcopy ..\Source\*.xpm MediaInfo_GUI_GNU_Prepare\Source\ /S
xcopy ..\Source\*.icns MediaInfo_GUI_GNU_Prepare\Source\ /S

rem --- Copying : Projects ---
xcopy ..\debian\* MediaInfo_GUI_GNU_Prepare\debian\
xcopy ..\Project\GNU\mediainfo.* MediaInfo_GUI_GNU_Prepare\Project\GNU\
xcopy ..\Project\GNU\GUI\autogen.sh MediaInfo_GUI_GNU_Prepare\Project\GNU\GUI\ /S
xcopy ..\Project\GNU\GUI\configure.ac MediaInfo_GUI_GNU_Prepare\Project\GNU\GUI\ /S
xcopy ..\Project\GNU\GUI\Makefile.am MediaInfo_GUI_GNU_Prepare\Project\GNU\GUI\ /S
xcopy ..\Project\GNU\GUI\wxwin.m4 MediaInfo_GUI_GNU_Prepare\Project\GNU\GUI\ /S
xcopy ..\Project\GNU\GUI\AddThisToRoot_GUI_compile.sh MediaInfo_GUI_GNU_Prepare\Project\GNU\GUI\ /S
xcopy ..\Project\GNU\GUI\mediainfo-gui.* MediaInfo_GUI_GNU_Prepare\Project\GNU\GUI\ /S

rem --- Copying : Release ---
xcopy Release_GUI_GNU.sub MediaInfo_GUI_GNU_Prepare\Release\
xcopy Release_GUI_*.sh MediaInfo_GUI_GNU_Prepare\Release\

rem --- Copying : Information files ---
copy ..\History_GUI.txt MediaInfo_GUI_GNU_Prepare\
copy ..\License.html MediaInfo_GUI_GNU_Prepare\
copy ReadMe_GUI_Linux.txt MediaInfo_GUI_GNU_Prepare\Release\
copy ReadMe_GUI_Mac.txt MediaInfo_GUI_GNU_Prepare\Release\


rem --- Preparing Archive : MediaInfo ---
move MediaInfo_GUI_GNU_Prepare MediaInfo
mkdir MediaInfo_GUI_GNU_Prepare
move MediaInfo MediaInfo_GUI_GNU_Prepare

rem --- Preparing Archive : MediaInfoLib ---
cd ..\..\MediaInfoLib\Release
call Release_Lib_GNU_Prepare.bat SkipCleanUp SkipCompression
cd ..\..\MediaInfo\Release
move ..\..\MediaInfoLib\Release\MediaInfo_Lib_GNU_Prepare MediaInfo_GUI_GNU_Prepare\MediaInfoLib

rem --- Preparing Archive : ZenLib ---
cd ..\..\ZenLib\Release
call Release_GNU_Prepare.bat SkipCleanUp SkipCompression
cd ..\..\MediaInfo\Release
move ..\..\ZenLib\Release\ZenLib_GNU_Prepare MediaInfo_GUI_GNU_Prepare\ZenLib

rem --- Shared ---
mkdir MediaInfo_GUI_GNU_Prepare\Shared
xcopy ..\..\Shared\Project\_Common\* MediaInfo_GUI_GNU_Prepare\Shared\Project\_Common\ /S
xcopy ..\..\Shared\Project\ZLib\*.sh MediaInfo_GUI_GNU_Prepare\Shared\Project\ZLib\ /S
xcopy ..\..\Shared\Project\WxWidgets\*.sh MediaInfo_GUI_GNU_Prepare\Shared\Project\WxWidgets\ /S


rem --- Compressing Archive ---
if "%2"=="SkipCompression" goto SkipCompression
%BPATH%\Windows\7-Zip\7z a -r -t7z -mx9 MediaInfo_GUI_GNU_Prepare.7z MediaInfo_GUI_GNU_Prepare\*
:SkipCompression

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_GUI_GNU_Prepare /S /Q
:SkipCleanUp
