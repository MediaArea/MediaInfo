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
mkdir MediaInfoDLL_Windows_x64\Developers\Doc\
copy ..\Doc\*.* MediaInfoDLL_Windows_x64\Developers\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfoDLL_Windows_x64\Developers\ /S
mkdir MediaInfoDLL_Windows_x64\Developers\List_Of_Parameters
copy ..\Source\Ressource\Text\Stream\*.csv MediaInfoDLL_Windows_x64\Developers\List_Of_Parameters

rem --- Copying : Include ---
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.h MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL_Static.h MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.def MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.pas MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.cs MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.jsl MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.vb MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.JNA.java MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.JNative.java MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.py MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\

@rem --- Copying : Projects ---
xcopy ..\Project\MSCS\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSCS\
xcopy ..\Project\MSCS\Example\*.cs MediaInfoDLL_Windows_x64\Developers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.csproj MediaInfoDLL_Windows_x64\Developers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.res* MediaInfoDLL_Windows_x64\Developers\Project\MSCS\Example\
xcopy ..\Project\MSCS\Example\*.ico MediaInfoDLL_Windows_x64\Developers\Project\MSCS\Example\
xcopy ..\Project\MSVB\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSVB\
xcopy ..\Project\MSVB\Example\*.vb MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.vbproj MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.res* MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example\
xcopy "..\Project\MSVB\Example\My Project\*.*" "MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example\My Project\"
xcopy ..\Project\MSVC\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSVC\
xcopy ..\Project\MSVC\Example\HowToUse_Dll.vcproj MediaInfoDLL_Windows_x64\Developers\Project\MSVC\Example\
xcopy ..\Project\GCC\Example\Make* MediaInfoDLL_Windows_x64\Developers\Project\GCC\Example\
xcopy ..\Project\Java\Example.JNA\*.java MediaInfoDLL_Windows_x64\Developers\Project\Java\Example.JNA\
xcopy ..\Project\Java\Example.JNA\*.txt MediaInfoDLL_Windows_x64\Developers\Project\Java\Example.JNA\
xcopy ..\Project\Java\Example.JNA\*.bat MediaInfoDLL_Windows_x64\Developers\Project\Java\Example.JNA\
xcopy ..\Project\Java\Example.JNA\*.sh MediaInfoDLL_Windows_x64\Developers\Project\Java\Example.JNA\
xcopy ..\Project\Java\Example.JNative\*.java MediaInfoDLL_Windows_x64\Developers\Project\Java\Example.JNative\
xcopy ..\Project\Java\Example.JNative\*.txt MediaInfoDLL_Windows_x64\Developers\Project\Java\Example.JNative\
xcopy ..\Project\Java\Example.JNative\*.bat MediaInfoDLL_Windows_x64\Developers\Project\Java\Example.JNative\
xcopy ..\Project\Java\Example.JNative\*.sh MediaInfoDLL_Windows_x64\Developers\Project\Java\Example.JNative\
xcopy ..\Project\NetBeans\Example.JNA\*.xml MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNA\ /s
xcopy ..\Project\NetBeans\Example.JNA\*.properties MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNA\ /s
xcopy ..\Project\NetBeans\Example.JNA\*.mf MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNA\
xcopy ..\Project\NetBeans\Example.JNA\*.txt MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNA\
xcopy ..\Project\NetBeans\Example.JNA\src\*.java MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNA\src\
xcopy ..\Project\NetBeans\Example.JNative\*.xml MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNative\ /s
xcopy ..\Project\NetBeans\Example.JNative\*.properties MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNative\ /s
xcopy ..\Project\NetBeans\Example.JNative\*.mf MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNative\
xcopy ..\Project\NetBeans\Example.JNative\*.txt MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNative\
xcopy ..\Project\NetBeans\Example.JNative\src\*.java MediaInfoDLL_Windows_x64\Developers\Project\NetBeans\Example.JNative\src\

rem --- Copying : Libs ---
xcopy ..\Release\MSVCx64\DLL\MediaInfo.lib MediaInfoDLL_Windows_x64\Developers\Release\MSVC\DLL\
xcopy ..\Release\MSVCx64\DLL\MediaInfod.lib MediaInfoDLL_Windows_x64\Developers\Release\MSVC\DLL\

rem --- Copying : Examples ---
xcopy ..\Source\Example\HowToUse_Dll.* MediaInfoDLL_Windows_x64\Developers\Source\Example\
xcopy Example.ogg MediaInfoDLL_Windows_x64\Developers\

rem --- Copying : Information files ---
copy ..\License.html MediaInfoDLL_Windows_x64\Developers\
copy ..\History_DLL.txt MediaInfoDLL_Windows_x64\Developers\History.txt
copy ..\Changes.txt MediaInfoDLL_Windows_x64\Developers\
copy ReadMe_DLL_Windows.txt MediaInfoDLL_Windows_x64\ReadMe.txt

rem --- Copying : DLL ---
xcopy ..\Project\MSVC\DLL\x64\Release\MediaInfo.dll MediaInfoDLL_Windows_x64\
xcopy ..\Project\MSVC\ShellExtension\x64\Release\MediaInfo_InfoTip.dll MediaInfoDLL_Windows_x64\
xcopy ..\Project\MSVC\ShellExtension\*.bat MediaInfoDLL_Windows_x64\


rem --- Compressing Archive ---
cd MediaInfoDLL_Windows_x64\
..\..\..\Shared\Binary\Windows_i386\7-Zip\7z a -r -t7z -mx9 ..\MediaInfo_DLL_Windows_x64_WithoutInstaller.7z *
cd ..

rem --- Installer ---
..\..\Shared\Binary\Windows_i386\NSIS\makensis ..\Source\Install\MediaInfo_DLL_Windows_x64.nsi


rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfoDLL_Windows_x64\ /S /Q
:SkipCleanUp
