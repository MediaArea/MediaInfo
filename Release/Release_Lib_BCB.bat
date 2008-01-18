@echo off

rem --- Clean up ---
del MediaInfoLib_BCB.7z
rmdir MediaInfoLib_BCB /S /Q
mkdir MediaInfoLib_BCB


rem --- Copying : Include ---
xcopy ..\Source\MediaInfo\MediaInfo.h MediaInfoLib_BCB\Include\MediaInfo\MediaInfo.h
xcopy ..\Source\MediaInfo\MediaInfoList.h MediaInfoLib_BCB\Include\MediaInfo\MediaInfoList.h

rem --- Copying : Documentation ---
mkdir Doc
cd ..\Source\Doc
..\..\..\Shared\Binary\Doxygen Doxygen
cd ..\..\Release
mkdir MediaInfoLib_BCB\Doc\
xcopy Doc\*.* MediaInfoLib_BCB\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfoLib_BCB\ /S

rem --- Copying : Source ---
xcopy ..\Source\Example\HowToUse.cpp MediaInfoLib_BCB\Source\Example\

@rem --- Copying : Project ---
xcopy ..\Project\BCB\Example\HowToUse.bpr MediaInfoLib_BCB\Project\BCB\Example\
xcopy ..\Project\BCB\Example\HowToUse.bpf MediaInfoLib_BCB\Project\BCB\Example\
xcopy ..\Project\BCB\Example\HowToUse.cpp MediaInfoLib_BCB\Project\BCB\Example\
xcopy ..\Project\BCB\Example\HowToUse.res MediaInfoLib_BCB\Project\BCB\Example\
xcopy ..\Project\BCB\Example\HowToUse_.cpp MediaInfoLib_BCB\Project\BCB\Example\
xcopy ..\Project\BCB\Example\HowToUse_.dfm MediaInfoLib_BCB\Project\BCB\Example\
xcopy ..\Project\BCB\Example\HowToUse_.h MediaInfoLib_BCB\Project\BCB\Example\

@rem --- Copying : Library ---
xcopy BCB\Library\MediaInfoLib.Lib MediaInfoLib_BCB\

rem --- Copying : Information files ---
copy ..\*.txt MediaInfoLib_BCB\
copy ..\*.html MediaInfoLib_BCB\


rem --- Compressing Archive ---
cd MediaInfoLib_BCB
..\..\..\Shared\Binary\7z a -r -t7z -mx9 ..\MediaInfoLib_BCB.7z *.*
cd ..

rem --- Clean up ---
rmdir MediaInfoLib_BCB /S /Q
