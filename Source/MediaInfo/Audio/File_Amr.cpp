// File_Amr - Info for AMR files
// Copyright (C) 2007-2008 Jerome Martinez, Zen@MediaArea.net
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
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Amr::Read_Buffer_Continue()
{
    if (Codec.empty()) //Test of header only if it is a file --> The codec field is empty
    {
        //Integrity
        if (Buffer_Size<=16)
            return;

        //Header
        if (!(CC5(Buffer)==CC5("#!AMR")))
        {
            Finished();
            return;
        }
    }

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "AMR");

    Stream_Prepare(Stream_Audio);
    Fill(Stream_Audio, 0, Audio_Format, "AMR");
    Fill(Stream_Audio, 0, Audio_Codec, "AMR");

    //No need of more
    Finished();
}

//---------------------------------------------------------------------------
void File_Amr::Read_Buffer_Finalize()
{
    if (Codec.empty())
        return; //This is only if this is not a file

    //Filling
    Ztring Profile;
    if (0)
        ;
    else if (Codec==_T("samr"))             {Profile=_T("Narrow band");}
    else if (Codec==_T("sawb"))             {Profile=_T("Wide band");}
    else if (Codec==_T("A104"))             {Profile=_T("Wide band");}

    Fill(Stream_Audio, 0, Audio_Format_Profile, Profile);
}

} //NameSpace

#endif //MEDIAINFO_AMR_YES

