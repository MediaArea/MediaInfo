@echo off

rem --- Clean up ---
del MediaInfoLib_GCC_MinGW32.7z
rmdir MediaInfoLib_GCC_MinGW32 /S /Q
mkdir MediaInfoLib_GCC_MinGW32


rem --- Copying : Include ---
xcopy ..\Source\MediaInfo\MediaInfo.h MediaInfoLib_GCC_MinGW32\Include\MediaInfo\
xcopy ..\Source\MediaInfo\MediaInfoList.h MediaInfoLib_GCC_MinGW32\Include\MediaInfo\

rem --- Copying : Documentation ---
mkdir Doc
cd ..\Source\Doc
..\..\..\Shared\Binary\Doxygen Doxygen
cd ..\..\Release
mkdir MediaInfoLib_GCC_MinGW32\Doc\
xcopy Doc\*.* MediaInfoLib_GCC_MinGW32\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfoLib_GCC_MinGW32\ /S

rem --- Copying : Source ---
xcopy ..\Source\Example\HowToUse.cpp MediaInfoLib_GCC_MinGW32\Source\Example\

@rem --- Copying : Project ---
xcopy ..\Project\DevCpp\Example\HowToUse_Cmd.dev MediaInfoLib_GCC_MinGW32\Project\DevCpp\Example\
xcopy ..\Project\GCC\Example\* MediaInfoLib_GCC_MinGW32\Project\GCC\Example\

@rem --- Copying : Library ---
xcopy GCC_MinGW32\Library\MediaInfoLib.a MediaInfoLib_GCC_MinGW32\

rem --- Copying : Information files ---
copy ..\*.txt MediaInfoLib_GCC_MinGW32\
copy ..\*.html MediaInfoLib_GCC_MinGW32\


rem --- Compressing Archive ---
cd MediaInfoLib_GCC_MinGW32
..\..\..\Shared\Binary\7z a -r -t7z -mx9 ..\MediaInfoLib_GCC_MinGW32.7z *.*
cd ..

rem --- Clean up ---
rmdir MediaInfoLib_GCC_MinGW32 /S /Q
