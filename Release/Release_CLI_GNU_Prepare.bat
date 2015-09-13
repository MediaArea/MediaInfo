@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@echo off

rem --- Clean up ---
del MediaInfo_CLI_GNU_Prepare.7z
rmdir MediaInfo_CLI_GNU_Prepare /S /Q
mkdir MediaInfo_CLI_GNU_Prepare


rem --- Copying : Sources ---
xcopy ..\Source\Common\*.cpp MediaInfo_CLI_GNU_Prepare\Source\Common\ /S
xcopy ..\Source\Common\*.h MediaInfo_CLI_GNU_Prepare\Source\Common\ /S
xcopy ..\Source\CLI\*.cpp MediaInfo_CLI_GNU_Prepare\Source\CLI\ /S
xcopy ..\Source\CLI\*.h MediaInfo_CLI_GNU_Prepare\Source\CLI\ /S

rem --- Copying : Projects ---
xcopy ..\Project\GNU\CLI\autogen.sh MediaInfo_CLI_GNU_Prepare\Project\GNU\CLI\ /S
xcopy ..\Project\GNU\CLI\configure.ac MediaInfo_CLI_GNU_Prepare\Project\GNU\CLI\ /S
xcopy ..\Project\GNU\CLI\Makefile.am MediaInfo_CLI_GNU_Prepare\Project\GNU\CLI\ /S
xcopy ..\Project\GNU\CLI\AddThisToRoot_CLI_compile.sh MediaInfo_CLI_GNU_Prepare\Project\GNU\CLI\ /S

rem --- Copying : Release ---
xcopy Release_CLI_GNU.sub MediaInfo_CLI_GNU_Prepare\Release\
xcopy Release_CLI_*.sh MediaInfo_CLI_GNU_Prepare\Release\

rem --- Copying : Information files ---
copy ..\History_CLI.txt MediaInfo_CLI_GNU_Prepare\
copy ..\License.html MediaInfo_CLI_GNU_Prepare\
copy ReadMe_CLI_Linux.txt MediaInfo_CLI_GNU_Prepare\Release\
copy ReadMe_CLI_Mac.txt MediaInfo_CLI_GNU_Prepare\Release\

rem --- Copying : Contrib files ---
mkdir MediaInfo_CLI_GNU_Prepare\Contrib
copy ..\Contrib\CLI_Help.doc MediaInfo_CLI_GNU_Prepare\Contrib\


rem --- Preparing Archive : MediaInfo ---
move MediaInfo_CLI_GNU_Prepare MediaInfo
mkdir MediaInfo_CLI_GNU_Prepare
move MediaInfo MediaInfo_CLI_GNU_Prepare

rem --- Preparing Archive : MediaInfoLib ---
cd ..\..\MediaInfoLib\Release
call Release_Lib_GNU_Prepare.bat SkipCleanUp SkipCompression
cd ..\..\MediaInfo\Release
move ..\..\MediaInfoLib\Release\MediaInfo_Lib_GNU_Prepare MediaInfo_CLI_GNU_Prepare\MediaInfoLib

rem --- Preparing Archive : ZenLib ---
cd ..\..\ZenLib\Release
call Release_GNU_Prepare.bat SkipCleanUp SkipCompression
cd ..\..\MediaInfo\Release
move ..\..\ZenLib\Release\ZenLib_GNU_Prepare MediaInfo_CLI_GNU_Prepare\ZenLib

rem --- Shared ---
mkdir MediaInfo_CLI_GNU_Prepare\Shared
xcopy ..\..\Shared\Project\_Common\* MediaInfo_CLI_GNU_Prepare\Shared\Project\_Common\ /S
xcopy ..\..\Shared\Project\curl\*.sh MediaInfo_CLI_GNU_Prepare\Shared\Project\curl\ /S
xcopy ..\..\Shared\Project\ZLib\*.sh MediaInfo_CLI_GNU_Prepare\Shared\Project\ZLib\ /S
xcopy ..\..\Shared\Project\ZLib\Template\projects\GNU\* MediaInfo_CLI_GNU_Prepare\Shared\Project\ZLib\Template\projects\GNU\ /S
xcopy ..\..\Shared\Project\WxWidgets\*.sh MediaInfo_CLI_GNU_Prepare\Shared\Project\WxWidgets\ /S


rem --- Compressing Archive ---
if "%2"=="SkipCompression" goto SkipCompression
..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -t7z -mx9 MediaInfo_CLI_GNU_Prepare.7z MediaInfo_CLI_GNU_Prepare\*
:SkipCompression

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_CLI_GNU_Prepare /S /Q
:SkipCleanUp
