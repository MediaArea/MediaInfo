@rem echo off

@rem --- Clean up ---
del   MediaInfo_CLI_Windows_x64.zip
rmdir MediaInfo_CLI_Windows_x64 /S /Q
mkdir MediaInfo_CLI_Windows_x64

rem --- Copying : Exe ---
xcopy MSVCx64\CLI\MediaInfo.exe MediaInfo_CLI_Windows_x64\ /S
xcopy ..\..\MediaInfoLib\Release\MSVCx64\Dll\MediaInfo.dll MediaInfo_CLI_Windows_x64\ /S

rem --- Copying : Plugins ---
xcopy ..\Source\Ressource\Plugin\Custom\* MediaInfo_CLI_Windows_x64\Plugin\Custom\ /S

rem --- Copying : Information files ---
copy ..\Licence.html MediaInfo_CLI_Windows_x64\
copy ..\History_CLI.txt MediaInfo_CLI_Windows_x64\History.txt
copy Readme_CLI_Windows.txt MediaInfo_CLI_Windows_x64\ReadMe.txt


rem --- Compressing Archive ---
cd MediaInfo_CLI_Windows_x64\
..\..\..\Shared\Binary\Win32\7-Zip\7z a -r -tzip ..\MediaInfo_CLI_Windows_x64.zip *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_CLI_Windows_x64\ /S /Q
:SkipCleanUp
