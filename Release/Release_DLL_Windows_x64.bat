@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@echo off

rem --- Clean up ---
del MediaInfoDLL_Windows_x64.7z
del MediaInfoDLL_Windows_x64.zip
rmdir MediaInfoDLL_Windows_x64\ /S /Q
mkdir MediaInfoDLL_Windows_x64\


rem --- Copying : Documentation ---
mkdir Doc
cd ..\Source\Doc
..\..\..\Shared\Binary\Windows_i386\Doxygen\doxygen
cd ..\..\Release
mkdir MediaInfoDLL_Windows_x64\Developers\Doc\
copy ..\Doc\*.* MediaInfoDLL_Windows_x64\Developers\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfoDLL_Windows_x64\Developers\ /S
mkdir MediaInfoDLL_Windows_x64\Developers\List_Of_Parameters
copy ..\Source\Resource\Text\Stream\*.csv MediaInfoDLL_Windows_x64\Developers\List_Of_Parameters

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
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL3.py MediaInfoDLL_Windows_x64\Developers\Source\MediaInfoDLL\

@rem --- Copying : Projects ---
xcopy ..\Project\BCB\Example\*.bpf MediaInfoDLL_Windows_x64\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.bpr MediaInfoDLL_Windows_x64\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.res* MediaInfoDLL_Windows_x64\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.dfm MediaInfoDLL_Windows_x64\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.h MediaInfoDLL_Windows_x64\Developers\Project\BCB\Example\
xcopy ..\Project\BCB\Example\*.cpp MediaInfoDLL_Windows_x64\Developers\Project\BCB\Example\
xcopy ..\Project\CodeBlocks\Example\*.cbp MediaInfoDLL_Windows_x64\Developers\Project\CodeBlocks\Example\
xcopy ..\Project\Delphi\Example\*.dpr MediaInfoDLL_Windows_x64\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.dfm MediaInfoDLL_Windows_x64\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.res MediaInfoDLL_Windows_x64\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.pas MediaInfoDLL_Windows_x64\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.bdsproj MediaInfoDLL_Windows_x64\Developers\Project\Delphi\Example\
xcopy ..\Project\Delphi\Example\*.bdsgroup MediaInfoDLL_Windows_x64\Developers\Project\Delphi\Example\
xcopy ..\Project\DevCpp\Example\*.dev MediaInfoDLL_Windows_x64\Developers\Project\DevCpp\Example\
xcopy ..\Project\MSCS2008\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\
xcopy ..\Project\MSCS2008\Example\*.cs MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\Example\
xcopy ..\Project\MSCS2008\Example\*.csproj MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\Example\
xcopy ..\Project\MSCS2008\Example\*.res* MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\Example\
xcopy ..\Project\MSCS2008\Example\*.ico MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\Example\
xcopy ..\Project\MSCS2008\asp_net_web_application\*.cs MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\asp_net_web_application\ /S
xcopy ..\Project\MSCS2008\asp_net_web_application\*.csproj MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\asp_net_web_application\
xcopy ..\Project\MSCS2008\asp_net_web_application\*.aspx MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\asp_net_web_application\
xcopy ..\Project\MSCS2008\asp_net_web_application\*.config MediaInfoDLL_Windows_x64\Developers\Project\MSCS2008\asp_net_web_application\
xcopy ..\Project\MSCS2010\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\
xcopy ..\Project\MSCS2010\Example\*.cs MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\Example\
xcopy ..\Project\MSCS2010\Example\*.csproj MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\Example\
xcopy ..\Project\MSCS2010\Example\*.res* MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\Example\
xcopy ..\Project\MSCS2010\Example\*.ico MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\Example\
xcopy ..\Project\MSCS2010\asp_net_web_application\*.cs MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\asp_net_web_application\ /S
xcopy ..\Project\MSCS2010\asp_net_web_application\*.csproj MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\asp_net_web_application\
xcopy ..\Project\MSCS2010\asp_net_web_application\*.aspx MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\asp_net_web_application\
xcopy ..\Project\MSCS2010\asp_net_web_application\*.config MediaInfoDLL_Windows_x64\Developers\Project\MSCS2010\asp_net_web_application\
xcopy ..\Project\MSJS\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSJS\
xcopy ..\Project\MSJS\Example\*.jsl MediaInfoDLL_Windows_x64\Developers\Project\MSJS\Example\
xcopy ..\Project\MSJS\Example\*.vjsproj MediaInfoDLL_Windows_x64\Developers\Project\MSJS\Example\
xcopy ..\Project\MSJS\Example\*.res* MediaInfoDLL_Windows_x64\Developers\Project\MSJS\Example\
xcopy ..\Project\MSVB\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSVB\
xcopy ..\Project\MSVB\Example\*.vb MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.vbproj MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example\
xcopy ..\Project\MSVB\Example\*.res* MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example\
xcopy "..\Project\MSVB\Example\My Project\*.*" "MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example\My Project\"
xcopy "..\Project\MSVB\Example VB6\*.*" "MediaInfoDLL_Windows_x64\Developers\Project\MSVB\Example VB6\"
xcopy ..\Project\MSVC2008\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSVC2008\
xcopy ..\Project\MSVC2008\Example\HowToUse_Dll.vcproj MediaInfoDLL_Windows_x64\Developers\Project\MSVC2008\Example\
xcopy ..\Project\MSVC2010\*.sln MediaInfoDLL_Windows_x64\Developers\Project\MSVC2010\
xcopy ..\Project\MSVC2010\Example\HowToUse_Dll.vcxproj MediaInfoDLL_Windows_x64\Developers\Project\MSVC2010\Example\
xcopy ..\Project\MSVC2010\Example\HowToUse_Dll.vcxproj.filters MediaInfoDLL_Windows_x64\Developers\Project\MSVC2010\Example\
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
xcopy ..\Project\MSVC2010\Example\x64\Release\MediaInfo.lib MediaInfoDLL_Windows_x64\Developers\Project\MSVC2010\Example\x64\Release\
xcopy ..\Project\MSVC2010\Example\x64\Debug\MediaInfo.lib MediaInfoDLL_Windows_x64\Developers\Project\MSVC2010\Example\x64\Debug\

rem --- Copying : Examples ---
xcopy ..\Source\Example\HowToUse_Dll*.* MediaInfoDLL_Windows_x64\Developers\Source\Example\
xcopy Example.ogg MediaInfoDLL_Windows_x64\Developers\

rem --- Copying : Information files ---
copy ..\License.html MediaInfoDLL_Windows_x64\Developers\
copy ..\History_DLL.txt MediaInfoDLL_Windows_x64\Developers\History.txt
copy ..\Changes.txt MediaInfoDLL_Windows_x64\Developers\
copy ReadMe_DLL_Windows.txt MediaInfoDLL_Windows_x64\ReadMe.txt

rem --- Copying : DLL ---
xcopy ..\Project\MSVC2013\DLL\x64\Release\MediaInfo.dll MediaInfoDLL_Windows_x64\
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
