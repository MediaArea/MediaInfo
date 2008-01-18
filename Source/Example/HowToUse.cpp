// HowToUse - Example for MediaInfoLib (commandline version)
// Copyright (C) 2004-2007 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Example for MediaInfoLib
// Command line version
// Note : it uses wprintf instead of wcout, because
//        MinGW doesn't support std::wcout
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "ZenLib/Ztring.h" //Note : I need it for universal atoi, but you have not to use it for be able to use MediaInfoLib
#include "MediaInfo/MediaInfo.h"

using namespace MediaInfoLib;
using namespace ZenLib;

#ifdef _UNICODE
    #define tprintf wprintf
#else
    #define tprintf printf
#endif

int main (int argc, ZenLib::Char *argv[])
{
    //Information about MediaInfo
    MediaInfo MI;
    ZenLib::Ztring To_Display=MI.Option(_T("Info_Version"), _T("0.7.0.0;MediaInfoDLL_Example_MSVC;0.7.0.0")).c_str();

    To_Display += _T("\r\n\r\nInfo_Parameters\r\n");
    To_Display += MI.Option(_T("Info_Parameters")).c_str();

    To_Display += _T("\r\n\r\nInfo_Capacities\r\n");
    To_Display += MI.Option(_T("Info_Capacities")).c_str();

    To_Display += _T("\r\n\r\nInfo_Codecs\r\n");
    To_Display += MI.Option(_T("Info_Codecs")).c_str();

    //An example of how to use the library
    To_Display += _T("\r\n\r\nOpen\r\n");
    MI.Open(_T("Example.ogg"));

    To_Display += _T("\r\n\r\nInform with Complete=false\r\n");
    MI.Option(_T("Complete"));
    To_Display += MI.Inform().c_str();

    To_Display += _T("\r\n\r\nInform with Complete=true\r\n");
    MI.Option(_T("Complete"), _T("1"));
    To_Display += MI.Inform().c_str();

    To_Display += _T("\r\n\r\nCustom Inform\r\n");
    MI.Option(_T("Inform"), _T("General;Example : FileSize=%FileSize%"));
    To_Display += MI.Inform().c_str();

    To_Display += _T("\r\n\r\nGet with Stream=General and Parameter=\"FileSize\"\r\n");
    To_Display += MI.Get(Stream_General, 0, _T("FileSize"), Info_Text, Info_Name).c_str();

    To_Display += _T("\r\n\r\nGetI with Stream=General and Parameter=46\r\n");
    To_Display += MI.Get(Stream_General, 0, 46, Info_Text).c_str();

    To_Display += _T("\r\n\r\nCount_Get with StreamKind=Stream_Audio\r\n");
    To_Display += ZenLib::Ztring::ToZtring(MI.Count_Get(Stream_Audio, -1)); //Warning : this is an integer

    To_Display += _T("\r\n\r\nGet with Stream=General and Parameter=\"AudioCount\"\r\n");
    To_Display += MI.Get(Stream_General, 0, _T("AudioCount"), Info_Text, Info_Name).c_str();

    To_Display += _T("\r\n\r\nGet with Stream=Audio and Parameter=\"StreamCount\"\r\n");
    To_Display += MI.Get(Stream_Audio, 0, _T("StreamCount"), Info_Text, Info_Name).c_str();

    To_Display += _T("\r\n\r\nClose\r\n");
    MI.Close();

    tprintf (_T("%s\r\n"), To_Display.c_str());
    system("PAUSE");
    
    return 0;
}
//---------------------------------------------------------------------------

