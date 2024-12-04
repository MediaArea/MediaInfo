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
del   MediaInfo_GUI_Windows_ARM64.exe
del   MediaInfo_GUI_Windows_ARM64_WithoutInstaller.7z
rmdir MediaInfo_GUI_Windows_ARM64 /S /Q
mkdir MediaInfo_GUI_Windows_ARM64


@rem --- Copying : Exe ---
copy  ..\Project\BCB\GUI\Win64\Release\MediaInfo_GUI.exe MediaInfo_GUI_Windows_ARM64\MediaInfo.exe
xcopy ..\..\MediaInfoLib\Project\MSVC2022\ARM64EC\Release\MediaInfo.dll MediaInfo_GUI_Windows_ARM64\ /S
xcopy ..\..\MediaInfoLib\Project\MSVC2022\ARM64\Release\MediaInfo_InfoTip.dll MediaInfo_GUI_Windows_ARM64\ /S
xcopy "C:\Program Files (x86)\Embarcadero\Studio\22.0\Redist\win64\WebView2Loader.dll" MediaInfo_GUI_Windows_ARM64\

@rem --- Copying : Plugins ---
xcopy ..\Source\Resource\Plugin\* MediaInfo_GUI_Windows_ARM64\Plugin\ /S

@rem --- Copying : libCURL --
copy %BPATH%\Windows\libcurl\x64\Release\LIBCURL.DLL MediaInfo_GUI_Windows_ARM64\

@rem --- Copying : Information files ---
copy ..\License.html MediaInfo_GUI_Windows_ARM64\
copy ..\History_GUI.txt MediaInfo_GUI_Windows_ARM64\History.txt
copy ReadMe_GUI_Windows.txt MediaInfo_GUI_Windows_ARM64\ReadMe.txt


rem --- Compressing Archive ---
cd MediaInfo_GUI_Windows_ARM64\
%BPATH%\Windows\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_GUI_Windows_ARM64_WithoutInstaller.7z *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_GUI_Windows_ARM64\ /S /Q
:SkipCleanUp
