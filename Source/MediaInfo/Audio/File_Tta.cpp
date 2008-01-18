// File_Tta - Info for TTA  files
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
// Source : http://tta.sourceforge.net/codec.format
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
#if defined(MEDIAINFO_TTA_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Tta.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Tta::File_Tta()
:File__Analyze(), File__Tags_Helper()
{
    //File__Tags_Helper
    Base=this;
}

void File_Tta::Read_Buffer_Continue()
{
    //Tags
    if (!File__Tags_Helper::Read_Buffer_Continue())
        return;
}

//---------------------------------------------------------------------------
void File_Tta::Read_Buffer_Finalize()
{
    //Tags
    File__Tags_Helper::Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Tta::Header_Begin()
{
    if (!File__Tags_Helper::Header_Begin())
        return false;

    //Synchro
    if (Buffer_Offset+4>Buffer_Size)
        return false;
    if (File_Offset==0 && CC4(Buffer+Buffer_Offset)!=CC4("TTA1"))
    {
        Finnished();
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
void File_Tta::Header_Parse()
{
    //Parsing
    int32u SampleRate, Samples, CRC32;
    int16u AudioFormat, Channels, BitsPerSample;
    Skip_C4(                                                    "Signature");
    Get_L2 (AudioFormat,                                        "AudioFormat");
    Get_L2 (Channels,                                           "NumChannels");
    Get_L2 (BitsPerSample,                                      "BitsPerSample");
    Get_L4 (SampleRate,                                         "SampleRate");
    Get_L4 (Samples,                                            "DataLength");
    Get_L4 (CRC32,                                              "CRC32");

    //Filling data
    if (SampleRate==0)
        return;
    int64u PlayTime=((int64u)Samples)*1000/SampleRate;
    if (PlayTime==0)
        return;
    int64u UncompressedSize=Samples*Channels*(BitsPerSample/8);
    if (UncompressedSize==0)
        return;
    float32 CompressionRatio=((float32)File_Size)/UncompressedSize;
    int32u BitRate=(int32u)(Samples*Channels*BitsPerSample*1000/PlayTime*CompressionRatio);

    Stream_Prepare(Stream_General);
    Fill("Format", "TTA");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "TTA ");
    Fill("Resolution", BitsPerSample);
    Fill("Channel(s)", Channels);
    Fill("SamplingRate", SampleRate);
    Fill("PlayTime", PlayTime);
    Fill("CompressionRatio", CompressionRatio);
    Fill("BitRate", BitRate);

    //Filling
    Header_Fill_Size(22);
    Header_Fill_Code(0, "Header");
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Tta::Data_Parse()
{
    //Next element
    File__Tags_Helper::Data_GoTo(File_Size, "TTA");
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Tta::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("OverallBitRate", "R");
            Fill_HowTo("PlayTime", "R");
            Fill_HowTo("Encoded_Library", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("SamplingCount", "R");
            Fill_HowTo("Resolution", "R");
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

#endif //MEDIAINFO_TTA_YES
