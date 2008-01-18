@echo off

rem --- Clean up ---
del MediaInfoLib_MSVC.7z
rmdir MediaInfoLib_MSVC /S /Q
mkdir MediaInfoLib_MSVC


rem --- Copying : Include ---
xcopy ..\Source\MediaInfo\MediaInfo.h MediaInfoLib_MSVC\Include\MediaInfo\MediaInfo.h
xcopy ..\Source\MediaInfo\MediaInfoList.h MediaInfoLib_MSVC\Include\MediaInfo\MediaInfoList.h

rem --- Copying : Documentation ---
mkdir Doc
cd ..\Source\Doc
..\..\..\Shared\Binary\Doxygen Doxygen
cd ..\..\Release
mkdir MediaInfoLib_MSVC\Doc\
xcopy Doc\*.* MediaInfoLib_MSVC\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfoLib_MSVC\ /S

rem --- Copying : Source ---
xcopy ..\Source\Example\HowToUse.cpp MediaInfoLib_MSVC\Source\Example\

@rem --- Copying : Project ---
xcopy ..\Project\MSVC\Example\HowToUse.bpr MediaInfoLib_MSVC\Project\MSVC\Example\
xcopy ..\Project\MSVC\Example\HowToUse.bpf MediaInfoLib_MSVC\Project\MSVC\Example\
xcopy ..\Project\MSVC\Example\HowToUse.cpp MediaInfoLib_MSVC\Project\MSVC\Example\
xcopy ..\Project\MSVC\Example\HowToUse.res MediaInfoLib_MSVC\Project\MSVC\Example\
xcopy ..\Project\MSVC\Example\HowToUse_.cpp MediaInfoLib_MSVC\Project\MSVC\Example\
xcopy ..\Project\MSVC\Example\HowToUse_.dfm MediaInfoLib_MSVC\Project\MSVC\Example\
xcopy ..\Project\MSVC\Example\HowToUse_.h MediaInfoLib_MSVC\Project\MSVC\Example\

@rem --- Copying : Library ---
xcopy MSVC\Library\MediaInfoLib.Lib MediaInfoLib_MSVC\

rem --- Copying : Information files ---
copy ..\*.txt MediaInfoLib_MSVC\
copy ..\*.html MediaInfoLib_MSVC\


rem --- Compressing Archive ---
cd MediaInfoLib_MSVC
..\..\..\Shared\Binary\7z a -r -t7z -mx9 ..\MediaInfoLib_MSVC.7z *.*
cd ..

rem --- Clean up ---
rmdir MediaInfoLib_MSVC /S /Q
