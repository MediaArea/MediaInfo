@echo off

rem --- Clean up ---
del MediaInfoDLL_Windows_i386.7z
del MediaInfoDLL_Windows_i386.zip
rmdir MediaInfoDLL_Windows_i386\ /S /Q
mkdir MediaInfoDLL_Windows_i386\


rem --- Copying : Documentation ---
mkdir Doc
cd ..\Source\Doc
..\..\..\Shared\Binary\Windows_i386\Doxygen\doxygen
cd ..\..\Release
mkdir MediaInfoDLL_Windows_i386\Developers\Doc\
copy ..\Doc\*.* MediaInfoDLL_Windows_i386\Developers\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfoDLL_Windows_i386\Developers\ /S
mkdir MediaInfoDLL_Windows_i386\Developers\List_Of_Parameters
copy ..\Source\Ressource\Text\Stream\*.csv MediaInfoDLL_Windows_i386\Developers\List_Of_Parameters

rem --- Copying : Include ---
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.h MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL_Static.h MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.def MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.pas MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.cs MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.jsl MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.vb MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.java MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.py MediaInfoDLL_Windows_i386\Developers\Source\MediaInfoDLL\

@rem --- Copying : Projects ---
xcopy ..\Project\BCB\Example\*.bpf MediaInfoDLL_Windows_i386\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.bpr MediaInfoDLL_Windows_i386\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.res* MediaInfoDLL_Windows_i386\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.dfm MediaInfoDLL_Windows_i386\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.h MediaInfoDLL_Windows_i386\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.cpp MediaInfoDLL_Windows_i386\Developers\Project\BCB\Example\
xcopy ..\Project\CodeBlocks\Example\*.cbp MediaInfoDLL_Windows_i386\Developers\Project\CodeBlocks\Example\
xcopy ..\Project\Delphi\Example\*.dpr MediaInfoDLL_Windows_i386\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.dfm MediaInfoDLL_Windows_i386\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.res MediaInfoDLL_Windows_i386\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.pas MediaInfoDLL_Windows_i386\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.bdsproj MediaInfoDLL_Windows_i386\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.bdsgroup MediaInfoDLL_Windows_i386\Developers\Project\Delphi\Example\
xcopy ..\Project\DevCpp\Example\*.dev MediaInfoDLL_Windows_i386\Developers\Project\DevCpp\Example\
xcopy ..\Project\MSCS\*.sln MediaInfoDLL_Windows_i386\Developers\Project\MSCS\
xcopy ..\Project\MSCS\Example\*.cs MediaInfoDLL_Windows_i386\Developers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.csproj MediaInfoDLL_Windows_i386\Developers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.res* MediaInfoDLL_Windows_i386\Developers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.ico MediaInfoDLL_Windows_i386\Developers\Project\MSCS\Example\
xcopy ..\Project\MSJS\*.sln MediaInfoDLL_Windows_i386\Developers\Project\MSJS\
xcopy ..\Project\MSJS\Example\*.jsl MediaInfoDLL_Windows_i386\Developers\Project\MSJS\Example\
xcopy ..\Project\MSJS\Example\*.vjsproj MediaInfoDLL_Windows_i386\Developers\Project\MSJS\Example\
xcopy ..\Project\MSJS\Example\*.res* MediaInfoDLL_Windows_i386\Developers\Project\MSJS\Example\
xcopy ..\Project\MSVB\*.sln MediaInfoDLL_Windows_i386\Developers\Project\MSVB\
xcopy ..\Project\MSVB\Example\*.vb MediaInfoDLL_Windows_i386\Developers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.vbproj MediaInfoDLL_Windows_i386\Developers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.res* MediaInfoDLL_Windows_i386\Developers\Project\MSVB\Example\
xcopy "..\Project\MSVB\Example\My Project\*.*" "MediaInfoDLL_Windows_i386\Developers\Project\MSVB\Example\My Project\"
xcopy "..\Project\MSVB\Example VB6\*.*" "MediaInfoDLL_Windows_i386\Developers\Project\MSVB\Example VB6\"
xcopy ..\Project\MSVC\*.sln MediaInfoDLL_Windows_i386\Developers\Project\MSVC\
xcopy ..\Project\MSVC\Example\HowToUse_Dll.vcproj MediaInfoDLL_Windows_i386\Developers\Project\MSVC\Example\
xcopy ..\Project\GCC\Example\Make* MediaInfoDLL_Windows_i386\Developers\Project\GCC\Example\
xcopy ..\Project\Java\Example\*.java MediaInfoDLL_Windows_i386\Developers\Project\Java\Example\
xcopy ..\Project\Java\Example\*.txt MediaInfoDLL_Windows_i386\Developers\Project\Java\Example\
xcopy ..\Project\Java\Example\*.bat MediaInfoDLL_Windows_i386\Developers\Project\Java\Example\
xcopy ..\Project\Java\Example\*.sh MediaInfoDLL_Windows_i386\Developers\Project\Java\Example\
xcopy ..\Project\NetBeans\Example\*.xml MediaInfoDLL_Windows_i386\Developers\Project\NetBeans\Example\ /s
xcopy ..\Project\NetBeans\Example\*.properties MediaInfoDLL_Windows_i386\Developers\Project\NetBeans\Example\ /s
xcopy ..\Project\NetBeans\Example\*.mf MediaInfoDLL_Windows_i386\Developers\Project\NetBeans\Example\
xcopy ..\Project\NetBeans\Example\*.txt MediaInfoDLL_Windows_i386\Developers\Project\NetBeans\Example\
xcopy ..\Project\NetBeans\Example\src\*.java MediaInfoDLL_Windows_i386\Developers\Project\NetBeans\Example\src\

rem --- Copying : Libs ---
xcopy ..\Release\BCB\DLL\MediaInfo.lib MediaInfoDLL_Windows_i386\Developers\Release\BCB\DLL\
xcopy ..\Release\MSVC\DLL\MediaInfo.lib MediaInfoDLL_Windows_i386\Developers\Release\MSVC\DLL\
xcopy ..\Release\MSVC\DLL\MediaInfod.lib MediaInfoDLL_Windows_i386\Developers\Release\MSVC\DLL\

rem --- Copying : Examples ---
xcopy ..\Source\Example\HowToUse_Dll.* MediaInfoDLL_Windows_i386\Developers\Source\Example\
xcopy Example.ogg MediaInfoDLL_Windows_i386\Developers\

rem --- Copying : Information files ---
copy ..\License.html MediaInfoDLL_Windows_i386\Developers\
copy ..\History_DLL.txt MediaInfoDLL_Windows_i386\Developers\History.txt
copy ReadMe_DLL_Windows.txt MediaInfoDLL_Windows_i386\ReadMe.txt

rem --- Copying : Contrib ---
xcopy ..\Contrib\*.* MediaInfoDLL_Windows_i386\Developers\Contrib\ /S

rem --- Copying : DLL ---
xcopy MSVC2005\Dll\MediaInfo.dll MediaInfoDLL_Windows_i386\
xcopy ..\..\MediaInfo\Release\MSVC\Dll\MediaInfo_InfoTip.dll MediaInfoDLL_Windows_i386\
xcopy ..\..\MediaInfo\Project\MSVC\ShellExtension\*.bat MediaInfoDLL_Windows_i386\


rem --- Compressing Archive ---
cd MediaInfoDLL_Windows_i386\
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_DLL_Windows_i386.7z *
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -tzip ..\MediaInfo_DLL_Windows_i386.zip *
cd ..

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfoDLL_Windows_i386\ /S /Q
:SkipCleanUp
