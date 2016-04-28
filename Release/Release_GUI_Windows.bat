@rem Copyright (c) MediaArea.net SARL. All Rights Reserved.
@rem
@rem Use of this source code is governed by a BSD-style license that can
@rem be found in the License.html file in the root of the source tree.
@rem

@rem echo off

@rem --- Clean up ---
del   MediaInfo_GUI_Windows.exe

rem --- Installer ---
..\..\..\MediaArea-Utils-Binaries\Windows\NSIS\makensis ..\Source\Install\MediaInfo_GUI_Windows.nsi
