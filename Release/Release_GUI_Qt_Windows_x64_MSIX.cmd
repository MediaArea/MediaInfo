:: config
::  example:
::   set QT_PATH=D:\Qt
::   set QT_VER1=6.8.0
::   set QT_VER2=6_8_0
::   set VCREDIST_VER=14.42.34433
::   set FFMPEG_EXE=%~dp0\..\..\MediaInfo-FFmpeg-Plugin\ffmpeg.exe
::   set CERT_PATH=D:\sign_cert.pfx
::   set CERT_PASS=K1wdqYSDk0locw6HSSjT
set QT_PATH=%QT_PATH%
set QT_VER1=%QT_VER1%
set QT_VER2=%QT_VER2%
set VCREDIST_VER=%VCREDIST_VER%
set FFMPEG_EXE=%FFMPEG_EXE%
set CERT_PATH=%CERT_PATH%
set CERT_PASS=%CERT_PASS%

:: set paths
set PATH_TEMP=%PATH%
set PATH=%QT_PATH%\%QT_VER1%\msvc2022_64\bin\;%QT_PATH%\Tools\QtCreator\bin\jom\;%PATH%
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: build Qt GUI
rmdir /s /q %~dp0\..\Project\QMake\GUI\build\Desktop_Qt_%QT_VER2%_MSVC2022_64bit-Release
mkdir %~dp0\..\Project\QMake\GUI\build\Desktop_Qt_%QT_VER2%_MSVC2022_64bit-Release
pushd %~dp0\..\Project\QMake\GUI\build\Desktop_Qt_%QT_VER2%_MSVC2022_64bit-Release
qmake.exe ..\..\MediaInfoQt.pro -spec win32-msvc "CONFIG+=qtquickcompiler" && jom.exe qmake_all
jom.exe
jom.exe clean
popd

:: deploy Qt GUI
rmdir /s /q %~dp0\MediaInfo_Qt_Windows_x64
mkdir %~dp0\MediaInfo_Qt_Windows_x64
copy %~dp0\..\Project\QMake\GUI\build\Desktop_Qt_%QT_VER2%_MSVC2022_64bit-Release\x64\MediaInfo.exe %~dp0\MediaInfo_Qt_Windows_x64\
windeployqt %~dp0\MediaInfo_Qt_Windows_x64\MediaInfo.exe

:: clean-up
rmdir /s /q %~dp0\..\Project\QMake\GUI\build\Desktop_Qt_%QT_VER2%_MSVC2022_64bit-Release

:: add Graph and FFmpeg plugins
xcopy %~dp0\..\..\MediaInfo-Graph-Plugin-Binaries\x64 %~dp0\MediaInfo_Qt_Windows_x64\ /i /e /r /y
xcopy %~dp0\..\..\MediaInfo-Graph-Plugin\Plugin %~dp0\MediaInfo_Qt_Windows_x64\Plugin\ /i /e /r /y
copy %FFMPEG_EXE% %~dp0\MediaInfo_Qt_Windows_x64\

:: manage dependencies
del /s %~dp0\MediaInfo_Qt_Windows_x64\vc_redist.x64.exe
copy "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\%VCREDIST_VER%\x64\Microsoft.VC143.CRT\concrt140.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\%VCREDIST_VER%\x64\Microsoft.VC143.CRT\msvcp140.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\%VCREDIST_VER%\x64\Microsoft.VC143.CRT\msvcp140_1.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\%VCREDIST_VER%\x64\Microsoft.VC143.CRT\msvcp140_2.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\%VCREDIST_VER%\x64\Microsoft.VC143.CRT\msvcp140_atomic_wait.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\%VCREDIST_VER%\x64\Microsoft.VC143.CRT\msvcp140_codecvt_ids.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\%VCREDIST_VER%\x64\Microsoft.VC143.CRT\vcruntime140.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\%VCREDIST_VER%\x64\Microsoft.VC143.CRT\vcruntime140_1.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "%~dp0\..\..\MediaArea-Utils-Binaries\Windows\libcurl\x64\Release\LIBCURL.DLL" "%~dp0\MediaInfo_Qt_Windows_x64\"
copy "%~dp0\..\..\MediaArea-Utils-Binaries\Windows\libcurl\curl-ca-bundle.crt" "%~dp0\MediaInfo_Qt_Windows_x64\"

:: build and add IExplorerCommand shell extension
MSBuild /t:Clean;Build /restore "/p:RestorePackagesConfig=true;Configuration=Release Qt;Platform=x64" %~dp0\..\Project\MSVC2022\MediaInfo_WindowsShellExtension\MediaInfo_WindowsShellExtension.vcxproj
copy "%~dp0\..\Project\MSVC2022\MediaInfo_WindowsShellExtension\x64\Release Qt\MediaInfo_WindowsShellExtension.dll" "%~dp0\MediaInfo_Qt_Windows_x64\"

:: add MSIX manifest and assets
xcopy %~dp0\..\Source\WindowsQtPackage %~dp0\MediaInfo_Qt_Windows_x64\ /i /e /r /y

:: package and sign MSIX
MakeAppx pack /d %~dp0\MediaInfo_Qt_Windows_x64\ /p %~dp0\MediaInfo_Qt_Windows_x64.msix /o
signtool.exe sign /fd SHA256 /a /f %CERT_PATH% /p %CERT_PASS% /tr http://ts.ssl.com /td sha256 %~dp0\MediaInfo_Qt_Windows_x64.msix

:: reset paths
set PATH=%PATH_TEMP%
set PATH_TEMP=
