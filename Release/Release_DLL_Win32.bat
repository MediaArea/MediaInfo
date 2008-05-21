@echo off

rem --- Clean up ---
del MediaInfoDLL_Win32.7z
del MediaInfoDLL_Win32.zip
rmdir MediaInfoDLL_Win32\ /S /Q
mkdir MediaInfoDLL_Win32\


rem --- Copying : Documentation ---
mkdir Doc
cd ..\Source\Doc
..\..\..\Shared\Binary\Win32\Doxygen\doxygen
cd ..\..\Release
mkdir MediaInfoDLL_Win32\Developpers\Doc\
copy Doc\*.* MediaInfoDLL_Win32\Developpers\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfoDLL_Win32\Developpers\ /S

rem --- Copying : Include ---
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.h MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL_Static.h MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.def MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.pas MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.cs MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.jsl MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.vb MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.java MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.py MediaInfoDLL_Win32\Developpers\Source\MediaInfoDLL\

@rem --- Copying : Projects ---
xcopy ..\Project\BCB\Example\*.bpf MediaInfoDLL_Win32\Developpers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.bpr MediaInfoDLL_Win32\Developpers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.res* MediaInfoDLL_Win32\Developpers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.dfm MediaInfoDLL_Win32\Developpers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.h MediaInfoDLL_Win32\Developpers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.cpp MediaInfoDLL_Win32\Developpers\Project\BCB\Example\
xcopy ..\Project\Delphi\Example\*.dpr MediaInfoDLL_Win32\Developpers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.dfm MediaInfoDLL_Win32\Developpers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.res MediaInfoDLL_Win32\Developpers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.pas MediaInfoDLL_Win32\Developpers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.bdsproj MediaInfoDLL_Win32\Developpers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.bdsgroup MediaInfoDLL_Win32\Developpers\Project\Delphi\Example\
xcopy ..\Project\DevCpp\Example\*.dev MediaInfoDLL_Win32\Developpers\Project\DevCpp\Example\
xcopy ..\Project\MSCS\*.sln MediaInfoDLL_Win32\Developpers\Project\MSCS\
xcopy ..\Project\MSCS\Example\*.cs MediaInfoDLL_Win32\Developpers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.csproj MediaInfoDLL_Win32\Developpers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.res* MediaInfoDLL_Win32\Developpers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.ico MediaInfoDLL_Win32\Developpers\Project\MSCS\Example\
xcopy ..\Project\MSJS\*.sln MediaInfoDLL_Win32\Developpers\Project\MSJS\
xcopy ..\Project\MSJS\Example\*.jsl MediaInfoDLL_Win32\Developpers\Project\MSJS\Example\
xcopy ..\Project\MSJS\Example\*.vjsproj MediaInfoDLL_Win32\Developpers\Project\MSJS\Example\
xcopy ..\Project\MSJS\Example\*.res* MediaInfoDLL_Win32\Developpers\Project\MSJS\Example\
xcopy ..\Project\MSVB\*.sln MediaInfoDLL_Win32\Developpers\Project\MSVB\
xcopy ..\Project\MSVB\Example\*.vb MediaInfoDLL_Win32\Developpers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.vbproj MediaInfoDLL_Win32\Developpers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.res* MediaInfoDLL_Win32\Developpers\Project\MSVB\Example\
xcopy "..\Project\MSVB\Example\My Project\*.*" "MediaInfoDLL_Win32\Developpers\Project\MSVB\Example\My Project\"
xcopy "..\Project\MSVB\Example VB6\*.*" "MediaInfoDLL_Win32\Developpers\Project\MSVB\Example VB6\"
xcopy ..\Project\MSVC\*.sln MediaInfoDLL_Win32\Developpers\Project\MSVC\
xcopy ..\Project\MSVC\Example\HowToUse_Dll.vcproj MediaInfoDLL_Win32\Developpers\Project\MSVC\Example\
xcopy ..\Project\GCC\Example\Make* MediaInfoDLL_Win32\Developpers\Project\GCC\Example\
xcopy ..\Project\Java\Example\*.java MediaInfoDLL_Win32\Developpers\Project\Java\Example\
xcopy ..\Project\Java\Example\*.txt MediaInfoDLL_Win32\Developpers\Project\Java\Example\
xcopy ..\Project\Java\Example\*.bat MediaInfoDLL_Win32\Developpers\Project\Java\Example\
xcopy ..\Project\Java\Example\*.sh MediaInfoDLL_Win32\Developpers\Project\Java\Example\

rem --- Copying : Libs ---
xcopy ..\Release\BCB\DLL\MediaInfo.lib MediaInfoDLL_Win32\Developpers\Release\BCB\DLL\
xcopy ..\Release\MSVC\DLL\MediaInfo.lib MediaInfoDLL_Win32\Developpers\Release\MSVC\DLL\
xcopy ..\Release\MSVC\DLL\MediaInfod.lib MediaInfoDLL_Win32\Developpers\Release\MSVC\DLL\

rem --- Copying : Examples ---
xcopy ..\Source\Example\HowToUse_Dll.* MediaInfoDLL_Win32\Developpers\Source\Example\
xcopy Example.ogg MediaInfoDLL_Win32\Developpers\

rem --- Copying : Information files ---
copy ..\Licence.html MediaInfoDLL_Win32\Developpers\
copy ..\History_DLL.txt MediaInfoDLL_Win32\Developpers\History.txt
copy ReadMe_DLL_Windows.txt MediaInfoDLL_Win32\ReadMe.txt

rem --- Copying : Contrib ---
xcopy ..\Contrib\*.* MediaInfoDLL_Win32\Developpers\Contrib\ /S

rem --- Copying : DLL ---
xcopy MSVC\Dll\MediaInfo.dll MediaInfoDLL_Win32\
xcopy ..\..\MediaInfo\Release\MSVC\Dll\MediaInfo_InfoTip.dll MediaInfoDLL_Win32\
xcopy ..\..\MediaInfo\Project\MSVC\ShellExtension\*.bat MediaInfoDLL_Win32\


rem --- Compressing Archive ---
cd MediaInfoDLL_Win32\
..\..\..\Shared\Binary\Win32\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_DLL_Win32.7z *
..\..\..\Shared\Binary\Win32\7-Zip\7z a -r -tzip ..\MediaInfo_DLL_Win32.zip *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfoDLL_Win32\ /S /Q
:SkipCleanUp
