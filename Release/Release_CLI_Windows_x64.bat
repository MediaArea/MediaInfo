@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@rem echo off

rem --- Search binaries ---
set BPATH=
if exist "%~dp0\..\..\..\MediaArea-Utils-Binaries" set BPATH="%~dp0\..\..\..\MediaArea-Utils-Binaries"
if exist "%~dp0\..\..\MediaArea-Utils-Binaries" set BPATH="%~dp0\..\..\MediaArea-Utils-Binaries"
if "%BPATH%"=="" (
    echo "ERROR: binaries path not found"
    exit /b 1
)

@rem --- Clean up ---
del   MediaInfo_CLI_Windows_x64.zip
rmdir MediaInfo_CLI_Windows_x64 /S /Q
mkdir MediaInfo_CLI_Windows_x64

rem --- Copying : Exe ---
xcopy ..\Project\MSVC2022\x64\Release\MediaInfo.exe MediaInfo_CLI_Windows_x64\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\Custom\* MediaInfo_CLI_Windows_x64\Plugin\Custom\ /S

rem --- Copying : libCURL --
copy %BPATH%\Windows\libcurl\x64\Release\LIBCURL.DLL MediaInfo_CLI_Windows_x64\

rem --- Copying : Information files ---
copy ..\License.* MediaInfo_CLI_Windows_x64\
copy ..\History_CLI.txt MediaInfo_CLI_Windows_x64\History.txt
copy Readme_CLI_Windows.txt MediaInfo_CLI_Windows_x64\ReadMe.txt


rem --- Compressing Archive ---
cd MediaInfo_CLI_Windows_x64\
%BPATH%\Windows\7-Zip\7z a -r -tzip ..\MediaInfo_CLI_Windows_x64.zip *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_CLI_Windows_x64\ /S /Q
:SkipCleanUp
