@rem echo off

@rem --- Clean up ---
del   MediaInfo_CLI_Win32.zip
rmdir MediaInfo_CLI_Win32 /S /Q
mkdir MediaInfo_CLI_Win32

@rem --- Copying : Exe ---
xcopy MSVC\CLI\MediaInfo.exe MediaInfo_CLI_Win32\ /S
xcopy ..\..\MediaInfoLib\Release\MSVC\Dll\MediaInfo.dll MediaInfo_CLI_Win32\ /S

@rem --- Copying : Plugins ---
xcopy ..\Source\Ressource\Plugin\Custom\* MediaInfo_CLI_Win32\Plugin\Custom\ /S

@rem --- Copying : Licence files ---
xcopy ..\Licence.html MediaInfo_CLI_Win32\

@rem --- Copying : History ---
xcopy ..\History.CLI.txt MediaInfo_CLI_Win32\

@rem --- Copying : Information files ---
xcopy Readme.Windows.txt MediaInfo_CLI_Win32\


rem --- Compressing Archive ---
cd MediaInfo_CLI_Win32\
..\..\..\Shared\Binary\Win32\7-Zip\7z a -r -tzip ..\MediaInfo_CLI_Win32.zip *
cd ..


rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_CLI_Win32\ /S /Q
:SkipCleanUp
