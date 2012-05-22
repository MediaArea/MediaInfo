@rem MediaInfo
@rem Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
@rem
@rem This program is free software: you can redistribute it and/or modify it
@rem under the terms of the GNU Lesser General Public License as published by
@rem the Free Software Foundation, either version 3 of the License, or
@rem (at your option) any later version.
@rem
@rem This program is distributed in the hope that it will be useful,
@rem but WITHOUT ANY WARRANTY; without even the implied warranty of
@rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
@rem GNU Lesser General Public License for more details.
@rem
@rem You should have received a copy of the GNU Lesser General Public License
@rem along with this program. If not, see <http:@remwww.gnu.org/licenses/>.
@rem
@rem ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@rem ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
rmdir BCB\PreRelease\__history /Q /S
rmdir BCB\PreRelease\Debug_Build /Q /S
rmdir BCB\PreRelease\Release_Build /Q /S
rmdir MSVC\CLI\Win32 /Q /S
rmdir MSVC\CLI\x64 /Q /S
rmdir MSVC\ShellExtension\Win32 /Q /S
rmdir MSVC\ShellExtension\x64 /Q /S
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