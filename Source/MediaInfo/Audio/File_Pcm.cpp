// File_Pcm - Info for PCM files
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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_PCM_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Pcm.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Pcm::Header_Begin()
{
    if (!File_Name.empty())
    {
        Finnished();
        return false;
    }

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "PCM");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "LPCM (Big signed)"); //Currently only called from DVD Video
    Fill("Resolution", 24); //Currently only called from DVD Video

    Finnished();
    return false;

    /*
    //Parsing
    Element_Size=Buffer_Size;
    int32u Resolution, SamplingRate, Channels;
    BS_BEGIN();
    SKIP_BS(8,                                                  Unknown);
    GET_BS (2, Resolution,                                      Resolution);
    GET_BS (2, SamplingRate,                                    SamplingRate);
    GET_BS (4, Channels,                                        Channels);
    ELEMENT(0, "Block", Element_Size);
    FLUSH();

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "PCM");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "PCM");
    switch(Resolution)
    {
        case  0 : Fill("Resolution", 16);
        case  1 : Fill("Resolution", 20);
        case  2 : Fill("Resolution", 24);
        default : ;
    }
    switch(Resolution)
    {
        case  0 : Fill("SamplingRate", 48000);
        case  1 : Fill("SamplingRate", 96000);
        default : ;
    }
    Fill("Channel(s)", Channels+1);
    */
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Pcm::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
            break;
        case (Stream_Chapters) :
            break;
        case (Stream_Image) :
            break;
        case (Stream_Menu) :
            break;
        case (Stream_Max) :
            break;
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_PCM_YES
