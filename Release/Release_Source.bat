@echo off

rem --- Clean up ---
del MediaInfo_Source.tar.bz2
del MediaInfo_Source.tar
rmdir MediaInfo_Source /S /Q
mkdir MediaInfo_Source


@rem --- Copying : Sources ---
xcopy ..\Source\*.cpp MediaInfo_Source\Source\ /S
xcopy ..\Source\*.h MediaInfo_Source\Source\ /S
xcopy ..\Source\*.dfm MediaInfo_Source\Source\ /S
xcopy ..\Source\*.xml MediaInfo_Source\Source\ /S
xcopy ..\Source\*.rc MediaInfo_Source\Source\ /S
xcopy ..\Source\*.txt MediaInfo_Source\Source\ /S
xcopy ..\Source\*.csv MediaInfo_Source\Source\ /S
xcopy ..\Source\*.ico MediaInfo_Source\Source\ /S
xcopy ..\Source\*.nsi MediaInfo_Source\Source\ /S
xcopy ..\Source\*.sh  MediaInfo_Source\Source\ /S
xcopy ..\Source\*.png MediaInfo_Source\Source\ /S
xcopy ..\Source\*.xpm MediaInfo_Source\Source\ /S

@rem --- Copying : Projects ---
xcopy ..\Project\*.bpg MediaInfo_Source\Project\ /S
xcopy ..\Project\*.bpf MediaInfo_Source\Project\ /S
xcopy ..\Project\*.bpr MediaInfo_Source\Project\ /S
xcopy ..\Project\*.cpp MediaInfo_Source\Project\ /S
xcopy ..\Project\*.c MediaInfo_Source\Project\ /S
xcopy ..\Project\*.h MediaInfo_Source\Project\ /S
xcopy ..\Project\*.bdsgroup MediaInfo_Source\Project\ /S
xcopy ..\Project\*.bdsproj MediaInfo_Source\Project\ /S
xcopy ..\Project\*.dev MediaInfo_Source\Project\ /S
xcopy ..\Project\*.sln MediaInfo_Source\Project\ /S
xcopy ..\Project\*.vc MediaInfo_Source\Project\ /S
xcopy ..\Project\*.vcproj MediaInfo_Source\Project\ /S
xcopy ..\Project\*.ico MediaInfo_Source\Project\ /S
xcopy ..\Project\*.workspace MediaInfo_Source\Project\ /S
xcopy ..\Project\*.cbp MediaInfo_Source\Project\ /S
xcopy ..\Project\*.def MediaInfo_Source\Project\ /S
xcopy ..\Project\*.idl MediaInfo_Source\Project\ /S
xcopy ..\Project\*.rgs MediaInfo_Source\Project\ /S
xcopy ..\Project\*.bat MediaInfo_Source\Project\ /S
xcopy ..\Project\BCB\MediaInfo*.res MediaInfo_Source\Project\BCB /S
xcopy ..\Project\MSVC\*.rc MediaInfo_Source\Project\MSVC\ /S
xcopy ..\Project\GNU\* MediaInfo_Source\Project\GNU\ /S

@rem --- Copying : Release ---
xcopy *.txt MediaInfo_Source\Release\
xcopy *.bat MediaInfo_Source\Release\
xcopy *.sh  MediaInfo_Source\Release\
xcopy *.sub MediaInfo_Source\Release\
xcopy BCB\*.txt MediaInfo_Source\Release\BCB\ /S
xcopy MSVC\*.txt MediaInfo_Source\Release\MSVC\ /S
xcopy GCC_MinGW32\*.txt MediaInfo_Source\Release\GCC_MinGW32\ /S
xcopy GCC_Linux_i386\*.txt MediaInfo_Source\Release\GCC_Linux_i386\ /S
xcopy Make_GCC.sh MediaInfo_Source\Release\

@rem --- Copying : debian ---
xcopy ..\debian\* MediaInfo_Source\debian\ /S

rem --- Copying : Information files ---
xcopy ..\*.txt MediaInfo_Source\
xcopy ..\*.html MediaInfo_Source\

rem --- Copying : Contrib files ---
xcopy ..\Contrib\*.doc MediaInfo_Source\Contrib\ /S

rem --- Copying : CVS files ---
xcopy ..\*.cvsignore MediaInfo_Source\


rem --- Compressing Archive ---
if "%2"=="SkipCompression" goto SkipCompression
move MediaInfo_Source MediaInfo
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -ttar -mx9 MediaInfo_Source.tar MediaInfo\*
..\..\Shared\Binary\Windows_i386\7-zip\7z a -r -tbzip2 -mx9 mediainfo_.tar.bz2 MediaInfo_Source.tar
del MediaInfo_Source.tar
move MediaInfo MediaInfo_Source
:SkipCompression

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_Source /S /Q
:SkipCleanUp
