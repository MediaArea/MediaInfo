@rem echo off

@rem --- Clean up ---
del   MediaInfo_GUI_Windows_x64.exe
del   MediaInfo_GUI_Windows_x64_WithoutInstaller.7z
rmdir MediaInfo_GUI_Windows_x64 /S /Q
mkdir MediaInfo_GUI_Windows_x64


@rem --- Copying : Exe ---
copy BCB\GUI\MediaInfo_GUI_x64.exe MediaInfo_GUI_Windows_x64\MediaInfo.exe
copy ..\..\MediaInfoLib\Release\MSVCx64\DLL\MediaInfo.dll MediaInfo_GUI_Windows_x64\
copy ..\..\MediaInfoLib\Release\MSVC2005\DLL\MediaInfo.dll MediaInfo_GUI_Windows_x64\MediaInfo_i386.dll
copy MSVCx64\DLL\MediaInfo_InfoTip.dll MediaInfo_GUI_Windows_x64\

@rem --- Copying : Plugins ---
xcopy ..\Source\Ressource\Plugin\* MediaInfo_GUI_Windows_x64\Plugin\ /S

@rem --- Copying : Information files ---
copy ..\License.html MediaInfo_GUI_Windows_x64\
copy ..\History_GUI.txt MediaInfo_GUI_Windows_x64\History.txt
copy ReadMe_GUI_Windows.txt MediaInfo_GUI_Windows_x64\ReadMe.txt


rem --- Compressing Archive ---
cd MediaInfo_GUI_Windows_x64\
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_GUI_Windows_x64_WithoutInstaller.7z *
cd ..

rem --- Installer ---
copy BCB\GUI\MediaInfo_GUI_x64.exe BCB\GUI\MediaInfo.exe
..\..\Shared\Binary\Windows_i386\NSIS\makensis ..\Source\Install\MediaInfo_GUI_Windows_x64.nsi

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_GUI_Windows_x64\ /S /Q
:SkipCleanUp