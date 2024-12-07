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
del   MediaInfo_GUI_Windows.exe
del   MediaInfo_GUI_Windows_x64.exe
del   MediaInfo_GUI_Windows_i386.exe

rem --- Installer ---
pushd %BPATH%\Windows\NSIS
makensis.exe "%~dp0\..\Source\Install\MediaInfo_GUI_Windows.nsi"
if not "%SNAPSHOT%"=="" (
    makensis.exe /DSNAPSHOT=%SNAPSHOT% "%~dp0\..\Source\Install\MediaInfo_GUI_Windows_Stub.nsi"
) else (
    makensis.exe "%~dp0\..\Source\Install\MediaInfo_GUI_Windows_Stub.nsi"
)
makensis.exe "%~dp0\..\Source\Install\MediaInfo_GUI_Windows_x64.nsi"
makensis.exe "%~dp0\..\Source\Install\MediaInfo_GUI_Windows_ARM64.nsi"
makensis.exe "%~dp0\..\Source\Install\MediaInfo_GUI_Windows_i386.nsi"
popd
