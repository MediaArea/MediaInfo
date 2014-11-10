@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@rem echo off
@rem --- General ---
cd ..
del *.~* *.obj *.o *.tds *.dcu *.ddp *.opt *.ncb *.suo *.ilk *.idb *.pdb *.pch *.plg *.aps *.user *.win *.layout *.local *.depend *.identcache *.tgs *.tgw *.sdf /Q /S
del *.~* *.obj *.o *.tds *.dcu *.ddp *.opt *.ncb *.suo *.ilk *.idb *.pdb *.pch *.plg *.aps *.user *.win *.layout *.local *.depend  *.identcache *.tgs *.tgw *.sdf /AH /Q /S
del *.ogg *.ogm *.avi *.mkv /Q /S
del *.ogg *.ogm *.avi *.mkv /AH /Q /S
cd Release

@rem --- In Project files ---
cd ..\Project
del *.exe *.dll *.a *.lib *.exp *.class *.zip *.7z /Q /S
del *.exe *.dll *.a *.lib *.exp *.class *.zip *.7z /AH /Q /S
cd ..\Release

@rem --- MS Visual Studio specific ---
cd ..\Project
rmdir MSVC\CLI\Release /Q /S
rmdir MSVC\CLI\Release_Ansi /Q /S
rmdir MSVC\ShellExtension\Release /Q /S
cd ..\Release

@rem Borland Developper Studio specific ---
cd ..\Source
rmdir CLI\__history /Q /S
rmdir Common\__history /Q /S
rmdir GUI\__history /Q /S
rmdir GUI\VCL\__history /Q /S
rmdir GUI\WxWidgets\__history /Q /S
rmdir Install\__history /Q /S
rmdir PreRelease\__history /Q /S
rmdir PreRelease\VCL\__history /Q /S
rmdir PreRelease\WxWidgets\__history /Q /S
cd ..\Release
cd ..\Project
rmdir BCB\__history /Q /S
rmdir BCB\CLI\__history /Q /S
rmdir BCB\CLI\Debug_Build /Q /S
rmdir BCB\CLI\Release_Build /Q /S
rmdir BCB\GUI\__history /Q /S
rmdir BCB\GUI\Debug_Build /Q /S
rmdir BCB\GUI\Release_Build /Q /S
rmdir BCB\WxWidgets\__history /Q /S
rmdir BCB\WxWidgets\Debug_Build /Q /S
rmdir BCB\WxWidgets\Release_Build /Q /S
rmdir BCB\PreRelease\__history /Q /S
rmdir BCB\PreRelease\Debug_Build /Q /S
rmdir BCB\PreRelease\Release_Build /Q /S
rmdir MSVC2005\CLI\Win32 /Q /S
rmdir MSVC2005\CLI\x64 /Q /S
rmdir MSVC2005\GUI\Win32 /Q /S
rmdir MSVC2005\GUI\x64 /Q /S
rmdir MSVC2005\MediaInfo_Qt\Win32 /Q /S
rmdir MSVC2005\MediaInfo_Qt\x64 /Q /S
rmdir MSVC2005\MediaInfo_Qt\_Automated /Q /S
rmdir MSVC2005\ipch /Q /S
rmdir MSVC2008\CLI\Win32 /Q /S
rmdir MSVC2008\CLI\x64 /Q /S
rmdir MSVC2008\GUI\Win32 /Q /S
rmdir MSVC2008\GUI\x64 /Q /S
rmdir MSVC2008\MediaInfo_Qt\Win32 /Q /S
rmdir MSVC2008\MediaInfo_Qt\x64 /Q /S
rmdir MSVC2008\MediaInfo_Qt\_Automated /Q /S
rmdir MSVC2008\ipch /Q /S
rmdir MSVC2010\CLI\Win32 /Q /S
rmdir MSVC2010\CLI\x64 /Q /S
rmdir MSVC2010\GUI\Win32 /Q /S
rmdir MSVC2010\GUI\x64 /Q /S
rmdir MSVC2010\MediaInfo_Qt\Win32 /Q /S
rmdir MSVC2010\MediaInfo_Qt\x64 /Q /S
rmdir MSVC2010\MediaInfo_Qt\_Automated /Q /S
rmdir MSVC2010\ipch /Q /S
rmdir MSVC2012\CLI\Win32 /Q /S
rmdir MSVC2012\CLI\x64 /Q /S
rmdir MSVC2012\GUI\Win32 /Q /S
rmdir MSVC2012\GUI\x64 /Q /S
rmdir MSVC2012\MediaInfo_Qt\Win32 /Q /S
rmdir MSVC2012\MediaInfo_Qt\x64 /Q /S
rmdir MSVC2012\MediaInfo_Qt\_Automated /Q /S
rmdir MSVC2012\ipch /Q /S
cd ..\Release

@rem Code::Blocks specific ---
cd ..\Project
rmdir CodeBlocks\CLI\.objs /Q /S
rmdir CodeBlocks\GUI\.objs /Q /S
rmdir CodeBlocks\PreRelease\.objs /Q /S
cd ..\Release

@rem GCC specific ---
cd ..\Project
rmdir GCC\CLI\.deps /Q /S
rmdir GCC\CLI\.objs /Q /S
rmdir GCC\CLI_WithoutLib\.deps /Q /S
rmdir GCC\CLI_WithoutLib\.objs /Q /S
rmdir GCC\GUI\.deps /Q /S
rmdir GCC\GUI\.objs /Q /S
cd ..\Release

@rem --- Release specific ---
del *.zip *.gz *.bz2 *.lib *.dll *.exp *.a *.so *.7z /Q /S
del MediaInfo*.* Debug*.* /Q /S
rmdir BCB\GUI\Plugins /Q /S