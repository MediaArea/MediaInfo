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
xcopy ..\..\MediaInfoLib\Release\MSVC\Dll\MediaInfo.dll MediaInfo_GUI_Win32\ /S
xcopy MSVC\DLL\MediaInfo_InfoTip.dll MediaInfo_GUI_Win32\ /S

@rem --- Copying : Plugins ---
xcopy ..\Source\Ressource\Plugin\* MediaInfo_GUI_Win32\Plugin\ /S

@rem --- Copying : Licence files ---
xcopy ..\Licence.html MediaInfo_GUI_Win32\

@rem --- Copying : History ---
xcopy ..\History.GUI.txt MediaInfo_GUI_Win32\

@rem --- Copying : Information files ---
xcopy Readme.Windows.txt MediaInfo_GUI_Win32\


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
