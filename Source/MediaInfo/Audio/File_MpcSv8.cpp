// File_MpcSv8 - Info for Musepack (SV8) files
// Copyright (C) 2008-2008 Jerome Martinez, Zen@MediaArea.net
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
// Source: http://trac.musepack.net/trac/wiki/SV8Specification
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPCSV8_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_MpcSv8.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Const
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPC_YES)
extern const int16u Mpc_SampleFreq[];
#else //MEDIAINFO_MPC_YES
const int16u Mpc_SampleFreq[]=
{
    44100, //CD
    48000, //DAT, DVC, ADR
    37800, //CD-ROM-XA
    32000, //DSR, DAT-LP, DVC-LP
};
#endif //MEDIAINFO_MPC_YES

//---------------------------------------------------------------------------
namespace Elements
{
    const int32u AP=0x4150;
    const int32u CT=0x4354;
    const int32u EI=0x4549;
    const int32u RG=0x5247;
    const int32u SE=0x5345;
    const int32u SH=0x5348;
    const int32u SO=0x534F;
    const int32u ST=0x5354;
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_MpcSv8::File_MpcSv8()
:File__Analyze(), File__Tags_Helper()
{
    //File__Tags_Helper
    Base=this;
}


//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpcSv8::Read_Buffer_Continue()
{
    //Tags
    if (!File__Tags_Helper::Read_Buffer_Continue())
        return;
}

//---------------------------------------------------------------------------
void File_MpcSv8::Read_Buffer_Finalize()
{
    //Tags
    File__Tags_Helper::Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpcSv8::Header_Begin()
{
    if (!File__Tags_Helper::Header_Begin())
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_MpcSv8::Header_Parse()
{
    //Parsing
    int64u Size;
    int16u Key;
    Get_C2 (Key,                                                "Key");
    Get_VS (Size,                                               "Size");

    //Filling
    Header_Fill_Code(Key, Ztring().From_CC4(Key<<16)); //Quick filling for CC2 with text
    Header_Fill_Size(Key==Elements::AP?Element_Offset:Size); //We don't need the data of audio packet, and we will stop here...
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpcSv8::FileHeader_Parse()
{
    //Parsing
    Element_Begin("SV8 header");
    int32u Magic_Number;
    Get_C4 (Magic_Number,                                       "Magic Number");
    Element_End();

    FILLING_BEGIN();
        //Integrity
        if (Magic_Number!=CC4("MPCK"))
        {
            Finished();
            return;
        }

        //Filling
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "MusePack SV8");
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, Audio_Format, "MusePack SV8");
        Fill(Stream_Audio, 0, Audio_Codec, "SV8");
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_MpcSv8::Data_Parse()
{
    #define ELEMENT_CASE(_NAME) \
        case Elements::_NAME : _NAME(); break;

    //Parsing
    switch (Element_Code)
    {
        ELEMENT_CASE(AP);
        ELEMENT_CASE(CT);
        ELEMENT_CASE(EI);
        ELEMENT_CASE(RG);
        ELEMENT_CASE(SE);
        ELEMENT_CASE(SH);
        ELEMENT_CASE(SO);
        ELEMENT_CASE(ST);
        default : Skip_XX(Element_Size,                         "Data");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpcSv8::AP()
{
    Element_Info("Audio Packet");

    //Jumping
    File__Tags_Helper::Data_GoTo(File_Size-3, "MpcSv8"); //Looking for end key and tags
}

//---------------------------------------------------------------------------
void File_MpcSv8::CT()
{
    Element_Info("Chapter-Tag");

    //Parsing
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_MpcSv8::EI()
{
    Element_Info("Encoder Info");

    //Parsing
    int8u  Quality, Version1, Version2, Version3;
    bool   PNS;
    BS_Begin();
    Get_S1 (7, Quality,                                         "Quality");
    Get_SB (   PNS,                                             "PNS");
    BS_End();
    Get_B1 (Version1,                                           "Major version");
    Get_B1 (Version2,                                           "Minor version");
    Get_B1 (Version3,                                           "Build");
}

//---------------------------------------------------------------------------
void File_MpcSv8::RG()
{
    Element_Info("Replay Gain");

    //Parsing
    int16u TitleGain, AlbumGain;
    Skip_B1(                                                    "Version");
    Get_L2 (TitleGain,                                          "Title gain"); Param_Info(((float32)((int16s)TitleGain))/1000, 2, " dB");
    Skip_L2(                                                    "Title peak");
    Get_L2 (AlbumGain,                                          "Album gain"); Param_Info(((float32)((int16s)TitleGain))/1000, 2, " dB");
    Skip_L2(                                                    "Album peak");
}

//---------------------------------------------------------------------------
void File_MpcSv8::SE()
{
    Element_Info("Stream End");
}

//---------------------------------------------------------------------------
void File_MpcSv8::SH()
{
    Element_Info("Stream Header");

    //Parsing
    int64u SampleCount;
    int8u  Version, SampleFrequency, ChannelCount;
    bool   MidSideStereo;
    Skip_B4(                                                    "CRC32");
    Get_B1 (Version,                                            "Version");
    Get_VS (SampleCount,                                        "Sample count");
    Skip_VS(                                                    "Beginning silence");
    BS_Begin();
    Get_S1 (3, SampleFrequency,                                 "Sample frequency"); Param_Info(Mpc_SampleFreq[SampleFrequency]);
    Skip_S1(5,                                                  "Max used bands");
    Get_S1 (4, ChannelCount,                                    "Channel count");
    Get_SB (   MidSideStereo,                                   "Mid side stereo used");
    Skip_S1(3,                                                  "Audio block frames");
    BS_End();

    //Filling
    FILLING_BEGIN();
        Fill(Stream_Audio, 0, Audio_SamplingRate, Mpc_SampleFreq[SampleFrequency]);
        if (SampleCount)
        {
            Fill(Stream_Audio, 0, Audio_SamplingCount, SampleCount);
            Fill(Stream_Audio, 0, Audio_Duration, SampleCount*1000/Mpc_SampleFreq[SampleFrequency]);
            Fill(Stream_Audio, 0, Audio_BitRate, File_Size*8*Mpc_SampleFreq[SampleFrequency]/SampleCount); //Should be more precise...
        }
        Fill(Stream_Audio, 0, Audio_Resolution, 16); //MPC support only 16 bits
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_MpcSv8::SO()
{
    Element_Info("Seek Table Offset");

    //Parsing
    Skip_VS(                                                    "Offset");
}

//---------------------------------------------------------------------------
void File_MpcSv8::ST()
{
    Element_Info("Seek Table");

    //Parsing
    Skip_XX(Element_Size,                                       "Data");
}

} //NameSpace

#endif //MEDIAINFO_MPCSV8_YES

