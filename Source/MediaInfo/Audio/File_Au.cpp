// File_Au - Info for AU  files
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
//
// Source: http://wiki.multimedia.cx/index.php?title=Au/snd
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_AU_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Au.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

const char* Au_sample_format(int32u sample_format)
{
    switch (sample_format)
    {
        case  1 : return "8-bit mu-law";
        case  2 : return "8-bit signed linear";
        case  3 : return "16-bit signed linear";
        case  4 : return "24-bit signed linear";
        case  5 : return "32-bit signed linear";
        case  6 : return "floating-point";
        case  7 : return "double precision float";
        case  8 : return "fragmented sampled data";
        case 10 : return "DSP program";
        case 11 : return "8-bit fixed-point";
        case 12 : return "16-bit fixed-point";
        case 13 : return "24-bit fixed-point";
        case 14 : return "32-bit fixed-point";
        case 17 : return "mu-law squelch";
        case 18 : return "16-bit linear with emphasis";
        case 19 : return "16-bit linear with compression";
        case 20 : return "16-bit linear with emphasis and compression";
        case 21 : return "Music Kit DSP commands";
        case 22 : return "Music Kit DSP samples";
        case 23 : return "G.721 ADPCM";
        case 24 : return "G.722 ADPCM";
        case 25 : return "G.723 ADPCM";
        case 26 : return "5-bit G.723 ADPCM";
        case 27 : return "8-bit a-law";
        default : return "";
    }
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Au::FileHeader_Parse()
{
    //Parsing
    int32u magic;
    Get_C4 (magic,                                              "magic");

    FILLING_BEGIN();
        if (magic!=0x2E736E64) //.snd
        {
            Finnished();
            return;
        }
    FILLING_END()
    else
        return; //Waiting for more data

    //Parsing
    Ztring arbitrary;
    int32u data_start, data_size, sample_format, sample_rate, channels;
    Get_B4 (data_start,                                         "data_start");
    Get_B4 (data_size,                                          "data_size");
    Get_B4 (sample_format,                                      "sample_format");
    Get_B4 (sample_rate,                                        "sample_rate");
    Get_B4 (channels,                                           "channels");
    if (data_start>24)
        Get_Local(data_start-24, arbitrary,                     "arbitrary data");

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        Fill("Format", "AU");
        Stream_Prepare(Stream_Audio);
        Fill("Codec", Au_sample_format(sample_format));
        Fill("Channel(s)", channels);
        Fill("SamplingRate", sample_rate);
        if (File_Size!=(int64u)-1)
            data_size=File_Size-data_start; //Priority for File size
        if (sample_rate && (data_size!=0 || data_size!=0xFFFFFFFF))
            ; Fill("PlayTime", ((int64u)data_size)*1000/sample_rate);
        Fill("Comments", arbitrary);

        Finnished();
    FILLING_END();
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Au::HowTo(stream_t StreamKind)
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

#endif //MEDIAINFO_AU_YES
