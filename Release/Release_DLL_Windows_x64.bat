@echo off

rem --- Clean up ---
del MediaInfoDLL_Windows_x64.7z
del MediaInfoDLL_Windows_x64.zip
rmdir MediaInfoDLL_Windows_x64\ /S /Q
mkdir MediaInfoDLL_Windows_x64\


rem --- Copying : Documentation ---
mkdir Doc
cd ..\Source\Doc
..\..\..\Shared\Binary\Doxygen
cd ..\..\Release
mkdir MediaInfoDLL_Windows_x64\Developpers\Doc\
copy Doc\*.* MediaInfoDLL_Windows_x64\Developpers\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfoDLL_Windows_x64\Developpers\ /S

rem --- Copying : Include ---
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.h MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL_Static.h MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.def MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.pas MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.cs MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.jsl MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.vb MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.java MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.py MediaInfoDLL_Windows_x64\Developpers\Source\MediaInfoDLL\

@rem --- Copying : Projects ---
xcopy ..\Project\MSCS\*.sln MediaInfoDLL_Windows_x64\Developpers\Project\MSCS\
xcopy ..\Project\MSCS\Example\*.cs MediaInfoDLL_Windows_x64\Developpers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.csproj MediaInfoDLL_Windows_x64\Developpers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.res* MediaInfoDLL_Windows_x64\Developpers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.ico MediaInfoDLL_Windows_x64\Developpers\Project\MSCS\Example\
xcopy ..\Project\MSVB\*.sln MediaInfoDLL_Windows_x64\Developpers\Project\MSVB\
xcopy ..\Project\MSVB\Example\*.vb MediaInfoDLL_Windows_x64\Developpers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.vbproj MediaInfoDLL_Windows_x64\Developpers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.res* MediaInfoDLL_Windows_x64\Developpers\Project\MSVB\Example\
xcopy "..\Project\MSVB\Example\My Project\*.*" "MediaInfoDLL_Windows_x64\Developpers\Project\MSVB\Example\My Project\"
xcopy ..\Project\MSVC\*.sln MediaInfoDLL_Windows_x64\Developpers\Project\MSVC\
xcopy ..\Project\MSVC\Example\HowToUse_Dll.vcproj MediaInfoDLL_Windows_x64\Developpers\Project\MSVC\Example\

rem --- Copying : Libs ---
xcopy ..\Release\MSVCx64\DLL\MediaInfo.lib MediaInfoDLL_Windows_x64\Developpers\Release\MSVC\DLL\
xcopy ..\Release\MSVCx64\DLL\MediaInfod.lib MediaInfoDLL_Windows_x64\Developpers\Release\MSVC\DLL\

rem --- Copying : Examples ---
xcopy ..\Source\Example\HowToUse_Dll.* MediaInfoDLL_Windows_x64\Developpers\Source\Example\
xcopy Example.ogg MediaInfoDLL_Windows_x64\Developpers\

rem --- Copying : Information files ---
copy ..\Licence.html MediaInfoDLL_Windows_x64\Developpers\
copy ..\History_DLL.txt MediaInfoDLL_Windows_x64\Developpers\History.txt
copy ReadMe_DLL_Windows.txt MediaInfoDLL_Windows_x64\ReadMe.txt

rem --- Copying : DLL ---
xcopy MSVCx64\Dll\MediaInfo.dll MediaInfoDLL_Windows_x64\
xcopy ..\..\MediaInfo\Release\MSVCx64\Dll\MediaInfo_InfoTip.dll MediaInfoDLL_Windows_x64\
xcopy ..\..\MediaInfo\Project\MSVC\ShellExtension\*.bat MediaInfoDLL_Windows_x64\


rem --- Compressing Archive ---
cd MediaInfoDLL_Windows_x64\
..\..\..\Shared\Binary\Win32\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_DLL_Windows_x64.7z *
..\..\..\Shared\Binary\Win32\7-Zip\7z a -r -tzip ..\MediaInfo_DLL_Windows_x64.zip *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfoDLL_Windows_x64\ /S /Q
:SkipCleanUp
