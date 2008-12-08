@rem echo off

@rem --- Clean up ---
del   MediaInfo_GUI_Win32.exe
del   MediaInfo_GUI_Win32_WithoutInstaller.7z
rmdir MediaInfo_GUI_Win32 /S /Q
mkdir MediaInfo_GUI_Win32


@rem --- Preparing ---
copy BCB\GUI\MediaInfo_GUI.exe BCB\GUI\MediaInfo.exe

@rem --- Copying : Exe ---
xcopy BCB\GUI\MediaInfo.exe MediaInfo_GUI_Win32\ /S
xcopy ..\..\MediaInfoLib\Release\MSVC2005\Dll\MediaInfo.dll MediaInfo_GUI_Win32\ /S
xcopy MSVC\DLL\MediaInfo_InfoTip.dll MediaInfo_GUI_Win32\ /S

@rem --- Copying : Plugins ---
xcopy ..\Source\Ressource\Plugin\* MediaInfo_GUI_Win32\Plugin\ /S

@rem --- Copying : Information files ---
copy ..\License.html MediaInfo_GUI_Win32\
copy ..\History_GUI.txt MediaInfo_GUI_Win32\History.txt
copy ReadMe_GUI_Windows.txt MediaInfo_GUI_Win32\ReadMe.txt


rem --- Compressing Archive ---
cd MediaInfo_GUI_Win32\
..\..\..\Shared\Binary\Win32\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_GUI_Win32_WithoutInstaller.7z *
cd ..

rem --- Installer ---
copy BCB\GUI\MediaInfo_GUI.exe BCB\GUI\MediaInfo.exe
..\..\Shared\Binary\Win32\NSIS\makensis ..\Source\Install\MediaInfo.nsi

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_GUI_Win32\ /S /Q
:SkipCleanUp