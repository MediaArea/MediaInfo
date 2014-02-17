@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@echo off

rem --- Clean up ---
del MediaInfo_Lib_Source.tar
del libmediainfo_.tar.bz2
del libmediainfo_-1.tar.gz
del libmediainfo__AllInclusive.7z
rmdir MediaInfo_Lib_Source /S /Q
rmdir ZenLib /S /Q
rmdir zlib /S /Q
mkdir MediaInfo_Lib_Source

@rem --- Copying : debian ---
xcopy ..\debian\* MediaInfo_Lib_Source\debian\ /S

@rem --- Copying : Sources ---
xcopy ..\Source\*.c MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.cpp MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.h MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.rc MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.def MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.pas MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.cs MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.aspx MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.config MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.jsl MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.vb MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.java MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\Resource\*.txt MediaInfo_Lib_Source\Source\Resource\ /S
xcopy ..\Source\*.csv MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.dfm MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.au3 MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.pb MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.py MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\Doc\* MediaInfo_Lib_Source\Source\Doc\ /S
xcopy ..\Source\Install\*.nsi MediaInfo_Lib_Source\Source\Install\

@rem --- Copying : Projects ---
xcopy ..\Project\*.bpg MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.bpf MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.bpr MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.dfm MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.cpp MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.bdsgroup MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.bdsproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.groupproj MediaInfo_Lib_Source\Project\ /S /Y
xcopy ..\Project\*.cbproj MediaInfo_Lib_Source\Project\ /S /Y
xcopy ..\Project\*.dpr MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.pas MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.dof MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.dev MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.sln MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.cs MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.csproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.jsl MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vjsproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vbproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vb MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vc MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vcproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vcxproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vcxproj.filters MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.ico MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.workspace MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.cbp MediaInfo_Lib_Source\Project\ /S /Y
xcopy ..\Project\*.resx MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.rc MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.def MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.idl MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\BCB\*.h MediaInfo_Lib_Source\Project\BCB\ /S
xcopy ..\Project\CMake\*.txt MediaInfo_Lib_Source\Project\CMake\ /S
xcopy ..\Project\MSVC2013\*.h MediaInfo_Lib_Source\Project\MSVC2013\ /S
xcopy ..\Project\MSVC2012\*.h MediaInfo_Lib_Source\Project\MSVC2012\ /S
xcopy ..\Project\MSVC2010\*.h MediaInfo_Lib_Source\Project\MSVC2010\ /S
xcopy ..\Project\MSVC2008\*.h MediaInfo_Lib_Source\Project\MSVC2008\ /S
xcopy ..\Project\MSVC2005\*.h MediaInfo_Lib_Source\Project\MSVC2005\ /S
xcopy ..\Project\MSCS\*.aspx MediaInfo_Lib_Source\Project\MSCS2010\ /S
xcopy ..\Project\MSCS\*.config MediaInfo_Lib_Source\Project\MSCS2010\ /S
xcopy ..\Project\BCB\*.res MediaInfo_Lib_Source\Project\BCB /S
xcopy ..\Project\Delphi\*.res MediaInfo_Lib_Source\Project\Delphi /S
xcopy ..\Project\MSVB\*.rc MediaInfo_Lib_Source\Project\MSVB\ /S
xcopy "..\Project\MSVB\Example\My Project\*.*" "MediaInfo_Lib_Source\Project\MSVB\Example\My Project\" /Y
xcopy "..\Project\MSVB\Example VB6\*.*" "MediaInfo_Lib_Source\Project\MSVB\Example VB6\" /Y
xcopy ..\Project\GNU\* MediaInfo_Lib_Source\Project\GNU\ /S
xcopy ..\Project\Java\Example.JNA\*.bat MediaInfo_Lib_Source\Project\Java\Example.JNA\ /S
xcopy ..\Project\Java\Example.JNA\*.sh MediaInfo_Lib_Source\Project\Java\Example.JNA\ /S
xcopy ..\Project\Java\Example.JNA\*.java MediaInfo_Lib_Source\Project\Java\Example.JNA\ /S
xcopy ..\Project\Java\Example.JNA\*.txt MediaInfo_Lib_Source\Project\Java\Example.JNA\ /S
xcopy ..\Project\Java\Example.JNative\*.bat MediaInfo_Lib_Source\Project\Java\Example.JNative\ /S
xcopy ..\Project\Java\Example.JNative\*.sh MediaInfo_Lib_Source\Project\Java\Example.JNative\ /S
xcopy ..\Project\Java\Example.JNative\*.java MediaInfo_Lib_Source\Project\Java\Example.JNative\ /S
xcopy ..\Project\Java\Example.JNative\*.txt MediaInfo_Lib_Source\Project\Java\Example.JNative\ /S
xcopy ..\Project\NetBeans\Example.JNA\*.xml MediaInfo_Lib_Source\Project\NetBeans\Example.JNA\ /s
xcopy ..\Project\NetBeans\Example.JNA\*.properties MediaInfo_Lib_Source\Project\NetBeans\Example.JNA\ /s
xcopy ..\Project\NetBeans\Example.JNA\*.mf MediaInfo_Lib_Source\Project\NetBeans\Example.JNA\
xcopy ..\Project\NetBeans\Example.JNA\*.txt MediaInfo_Lib_Source\Project\NetBeans\Example.JNA\
xcopy ..\Project\NetBeans\Example.JNA\src\*.java MediaInfo_Lib_Source\Project\NetBeans\Example.JNA\src\
xcopy ..\Project\NetBeans\Example.JNative\*.xml MediaInfo_Lib_Source\Project\NetBeans\Example.JNative\ /s
xcopy ..\Project\NetBeans\Example.JNative\*.properties MediaInfo_Lib_Source\Project\NetBeans\Example.JNative\ /s
xcopy ..\Project\NetBeans\Example.JNative\*.mf MediaInfo_Lib_Source\Project\NetBeans\Example.JNative\
xcopy ..\Project\NetBeans\Example.JNative\*.txt MediaInfo_Lib_Source\Project\NetBeans\Example.JNative\
xcopy ..\Project\NetBeans\Example.JNative\src\*.java MediaInfo_Lib_Source\Project\NetBeans\Example.JNative\src\
xcopy ..\Project\Solaris\* MediaInfo_Lib_Source\Project\Solaris\ /S

@rem --- Copying : Release ---
xcopy *.txt MediaInfo_Lib_Source\Release\
xcopy *.bat MediaInfo_Lib_Source\Release\
xcopy *.ogg MediaInfo_Lib_Source\Release\
xcopy *.sh  MediaInfo_Lib_Source\Release\
xcopy *.sub MediaInfo_Lib_Source\Release\
xcopy BCB\*.txt MediaInfo_Lib_Source\Release\BCB\ /S
xcopy MSVC2010\*.txt MediaInfo_Lib_Source\Release\MSVC2010\ /S
xcopy GCC_MinGW32\*.txt MediaInfo_Lib_Source\Release\GCC_MinGW32\ /S
xcopy GCC_Linux_i386\*.txt MediaInfo_Lib_Source\Release\GCC_Linux_i386\ /S

@rem --- Copying : Contrib ---
xcopy ..\Contrib\* MediaInfo_Lib_Source\Contrib\ /S

rem --- Copying : Information files ---
copy ..\*.txt MediaInfo_Lib_Source\
copy ..\*.html MediaInfo_Lib_Source\

rem --- Copying : CVS files ---
copy ..\*.cvsignore MediaInfo_Lib_Source\

rem --- Copying : ZenLib files ---
cd ..\..\ZenLib\Release
call Release_Source.bat SkipCleanUp SkipCompression
cd ..\..\MediaInfoLib\Release
move ..\..\ZenLib\Release\ZenLib_Source .\ZenLib

rem --- Copying : zlib files ---
xcopy ..\..\Shared\Source\zlib .\zlib\ /S
xcopy ..\..\Shared\Project\zlib\Template .\zlib\ /S


rem --- Compressing Archive ---
if "%2"=="SkipCompression" goto SkipCompression
move MediaInfo_Lib_Source MediaInfoLib
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -ttar -mx9 MediaInfo_Lib_Source.tar MediaInfoLib\*
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -tbzip2 -mx9 libmediainfo_.tar.bz2 MediaInfo_Lib_Source.tar
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -tgzip -mx9 libmediainfo_-1.tar.gz MediaInfo_Lib_Source.tar
del MediaInfo_Lib_Source.tar
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -t7z -mx9 libmediainfo__AllInclusive.7z MediaInfoLib\* ZenLib\* zlib\*
move MediaInfoLib MediaInfo_Lib_Source
:SkipCompression

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_Lib_Source /S /Q
:SkipCleanUp
rmdir ZenLib /S /Q
rmdir zlib /S /Q
