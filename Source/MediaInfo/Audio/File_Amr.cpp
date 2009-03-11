// File_Amr - Info for AMR files
// Copyright (C) 2007-2009 Jerome Martinez, Zen@MediaArea.net
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

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_AMR_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Amr.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Amr::FileHeader_Begin()
{
    if (!Codec.empty()) //Test of header only if it is a file --> The codec field is empty
        return true;

    //Testing
    if (Buffer_Size<5)
        return false; //Must wait for more data
    if (CC5(Buffer)!=0x2321414D52LL) //"#!AMR"
    {
        Reject("AMR");
        return false;
    }

    //All should be OK...
    return true;
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Amr::FileHeader_Parse()
{
    //Parsing
    if (Codec.empty())
        Skip_C5(                                                "Signature");

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "AMR");
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, Audio_Format, "AMR");
        Fill(Stream_Audio, 0, Audio_Codec, "AMR");
        if (!Codec.empty())
        {
            Ztring Profile;
            if (0)
                ;
            else if (Codec==_T("samr"))             {Profile=_T("Narrow band");}
            else if (Codec==_T("sawb"))             {Profile=_T("Wide band");}
            else if (Codec==_T("A104"))             {Profile=_T("Wide band");}
            Fill(Stream_Audio, 0, Audio_Format_Profile, Profile);
        }

        //No need of more
        Accept("AMR");
        Finish("AMR");
    FILLING_END();
}

} //NameSpace

#endif //MEDIAINFO_AMR_YES

