@rem echo off

@rem --- Clean up ---
del   MediaInfo_CLI_Win32.zip
rmdir MediaInfo_CLI_Win32 /S /Q
mkdir MediaInfo_CLI_Win32

rem --- Copying : Exe ---
xcopy MSVC\CLI\MediaInfo.exe MediaInfo_CLI_Win32\ /S
xcopy ..\..\MediaInfoLib\Release\MSVC\Dll\MediaInfo.dll MediaInfo_CLI_Win32\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Ressource\Plugin\Custom\* MediaInfo_CLI_Win32\Plugin\Custom\ /S

rem --- Copying : Information files ---
copy ..\Licence.html MediaInfo_CLI_Win32\
copy ..\History_CLI.txt MediaInfo_CLI_Win32\History.txt
copy Readme_CLI_Windows.txt MediaInfo_CLI_Win32\ReadMe.txt


rem --- Compressing Archive ---
cd MediaInfo_CLI_Win32\
..\..\..\Shared\Binary\Win32\7-Zip\7z a -r -tzip ..\MediaInfo_CLI_Win32.zip *
cd ..


rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_CLI_Win32\ /S /Q
:SkipCleanUp
