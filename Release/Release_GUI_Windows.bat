@rem MediaInfo
@rem Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
@rem
@rem This program is free software: you can redistribute it and/or modify it
@rem under the terms of the GNU Lesser General Public License as published by
@rem the Free Software Foundation, either version 2 of the License, or
@rem (at your option) any later version.
@rem
@rem This program is distributed in the hope that it will be useful,
@rem but WITHOUT ANY WARRANTY; without even the implied warranty of
@rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
@rem GNU Lesser General Public License for more details.
@rem
@rem You should have received a copy of the GNU Lesser General Public License
@rem along with this program. If not, see <http://www.gnu.org/licenses/>.
@rem
@rem ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@rem ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

@rem echo off

@rem --- Clean up ---
del   MediaInfo_GUI_Windows.exe

rem --- Installer ---
..\..\Shared\Binary\Windows_i386\NSIS\makensis ..\Source\Install\MediaInfo_GUI_Windows.nsi

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_GUI_Windows\ /S /Q
:SkipCleanUp