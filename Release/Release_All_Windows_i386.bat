@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@rem echo off

@rem --- Clean up ---
del   MediaInfo_All_Windows_i386.zip
rmdir MediaInfo_All_Windows_i386 /S /Q
mkdir MediaInfo_All_Windows_i386


@rem --- Launch all other batches ---
call Release_GUI_Windows_i386 SkipCleanUp
call Release_CLI_Windows_i386 SkipCleanUp
cd ..\..\MediaInfoLib\Release\
call Release_DLL_Windows_i386 SkipCleanUp
cd ..\..\MediaInfo\Release\

@rem --- Modifying ---
move MediaInfo_GUI_Windows_i386\MediaInfo.exe MediaInfo_GUI_Windows_i386\MediaInfo_GUI.exe
move MediaInfo_CLI_Windows_i386\MediaInfo.exe MediaInfo_CLI_Windows_i386\MediaInfo_CLI.exe

@rem --- Copying ---
xcopy MediaInfo_GUI_Windows_i386\*.* MediaInfo_All_Windows_i386\ /S /Y
xcopy MediaInfo_CLI_Windows_i386\*.* MediaInfo_All_Windows_i386\ /S /Y
xcopy ..\..\MediaInfoLib\Release\MediaInfoDLL_Windows_i386\*.* MediaInfo_All_Windows_i386\ /S /Y

rem --- Compressing Archive ---
cd MediaInfo_All_Windows_i386\
..\..\..\Shared\Binary\7z a -r -t7z -mx9 ..\MediaInfo_All_Windows_i386.7z *
cd ..


@rem --- Clean up ---
rmdir MediaInfo_All_Windows_i386\ /S /Q
rmdir MediaInfo_GUI_Windows_i386\ /S /Q
rmdir MediaInfo_CLI_Windows_i386\ /S /Q
rmdir ..\..\MediaInfoLib\Release\MediaInfoDLL_Windows_i386\ /S /Q