@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@rem echo off

rem --- Search binaries ---
set BPATH=
if exist "%~dp0\..\..\..\MediaArea-Utils-Binaries" set "BPATH=%~dp0\..\..\..\MediaArea-Utils-Binaries"
if exist "%~dp0\..\..\MediaArea-Utils-Binaries" set "BPATH=%~dp0\..\..\MediaArea-Utils-Binaries"
if "%BPATH%"=="" (
    echo "ERROR: binaries path not found"
    exit /b 1
)

@rem --- Clean up ---
del   MediaInfo_CLI_Windows_ARM64.zip
rmdir MediaInfo_CLI_Windows_ARM64 /S /Q
mkdir MediaInfo_CLI_Windows_ARM64

rem --- Copying : Exe ---
xcopy ..\Project\MSVC2022\ARM64\Release\MediaInfo.exe MediaInfo_CLI_Windows_ARM64\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\Custom\* MediaInfo_CLI_Windows_ARM64\Plugin\Custom\ /S

rem --- Copying : libCURL --
copy %BPATH%\Windows\libcurl\ARM64\Release\LIBCURL.DLL MediaInfo_CLI_Windows_ARM64\

rem --- Copying : Information files ---
copy ..\License.* MediaInfo_CLI_Windows_ARM64\
copy ..\History_CLI.txt MediaInfo_CLI_Windows_ARM64\History.txt
copy Readme_CLI_Windows.txt MediaInfo_CLI_Windows_ARM64\ReadMe.txt

rem --- Copying : Contrib files ---
mkdir MediaInfo_CLI_Windows_ARM64\Contrib
copy ..\Contrib\CLI_Help.doc MediaInfo_CLI_Windows_ARM64\Contrib\

rem --- Compressing Archive ---
cd MediaInfo_CLI_Windows_ARM64\
%BPATH%\Windows\7-Zip\7z a -r -tzip ..\MediaInfo_CLI_Windows_ARM64.zip *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_CLI_Windows_ARM64\ /S /Q
:SkipCleanUp
