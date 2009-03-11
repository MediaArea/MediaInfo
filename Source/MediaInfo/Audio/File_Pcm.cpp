// File_Pcm - Info for PCM files
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
#if defined(MEDIAINFO_PCM_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Pcm.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
int32u Pcm_VOB_Frequency[]=
{
    48000,
    32000,
        0,
        0,
};

//---------------------------------------------------------------------------
const char* Pcm_VOB_ChannelsPositions(int8u NumberOfChannelsMinusOne)
{
    switch (NumberOfChannelsMinusOne)
    {
        case  0 : return "Front: C";                                    //1 channel
        case  1 : return "Front: L R";                                  //2 channels
        case  2 : return "Front: L R, LFE";                             //3 channels (not sure)
        case  3 : return "Front: L R, Rear: L R";                       //4 channels
        case  4 : return "Front: L R, Rear: L R, LFE";                  //5 channels (not sure)
        case  5 : return "Front: L R C, Rear: L R, LFE";                //6 channels
        case  6 : return "Front: L R C, Middle: L R, Rear: L R";        //7 channels
        case  7 : return "Front: L R C, Middle: L R, Rear: L R, LFE";   //8 channels
        default : return "";
    }
}

//---------------------------------------------------------------------------
const char* Pcm_VOB_ChannelsPositions2(int8u NumberOfChannelsMinusOne)
{
    switch (NumberOfChannelsMinusOne)
    {
        case  0 : return "2/0";                                         //1 channel
        case  1 : return "2/0";                                         //2 channels
        case  2 : return "3/0.1";                                       //3 channels (not sure)
        case  3 : return "3/0.1";                                       //4 channels
        case  4 : return "3/2.1";                                       //5 channels (not sure)
        case  5 : return "3/2.1";                                       //6 channels
        case  6 : return "3.2/2.1";                                     //7 channels
        case  7 : return "3.2/2.1";                                     //8 channels
        default : return "";
    }
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Pcm::Read_Buffer_Continue()
{
    if (Codec!=_T("VOB")
     && Codec!=_T("EVOB")) //No need of data
    {
        //Filling
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "PCM");
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, Audio_Format, "PCM");
        Fill(Stream_Audio, 0, Audio_Codec, "PCM");

        //Finished
        Accept("PCM");
        Finish("PCM");
    }
}

//---------------------------------------------------------------------------
void File_Pcm::Read_Buffer_Finalize()
{
    //Filling
    Ztring Firm, Endianness, Sign, ITU, Resolution;
    if (0)
        ;
    else if (Codec==_T("EVOB"))             {Firm=_T("");       Endianness=_T("Big");    Sign=_T("Signed");   Resolution=_T("");} //PCM Signed 16 bits Big Endian, Interleavement is for 2 samples*2 channels L0-1/L0-0/R0-1/R0-0/L1-1/L1-0/R1-1/R1-0/L0-2/R0-2/L1-2/R1-2, http://wiki.multimedia.cx/index.php?title=PCM
    else if (Codec==_T("VOB"))              {Firm=_T("");       Endianness=_T("Big");    Sign=_T("Signed");   Resolution=_T("");} //PCM Signed 16 bits Big Endian, Interleavement is for 2 samples*2 channels L0-1/L0-0/R0-1/R0-0/L1-1/L1-0/R1-1/R1-0/L0-2/R0-2/L1-2/R1-2, http://wiki.multimedia.cx/index.php?title=PCM
    else if (Codec==_T("A_PCM/INT/BIG"))    {Firm=_T("");       Endianness=_T("Big");}
    else if (Codec==_T("A_PCM/INT/LITTLE")) {Firm=_T("");       Endianness=_T("Little");}
    else if (Codec==_T("A_PCM/INT/FLOAT"))  {Firm=_T("");       Endianness=_T("Float");}
    else if (Codec==_T("fl32"))             {                   Endianness=_T("Float");                       Resolution=_T("32");}
    else if (Codec==_T("fl64"))             {                   Endianness=_T("Float");                       Resolution=_T("64");}
    else if (Codec==_T("in24"))             {                   Endianness=_T("Big");    Sign=_T("Unsigned"); Resolution=_T("24");}
    else if (Codec==_T("in32"))             {                   Endianness=_T("Big");    Sign=_T("Unsigned"); Resolution=_T("32");}
    else if (Codec==_T("raw "))             {                   Endianness=_T("Little"); Sign=_T("Unsigned");}
    else if (Codec==_T("twos"))             {                   Endianness=_T("Big");    Sign=_T("Signed");}
    else if (Codec==_T("sowt"))             {                   Endianness=_T("Little"); Sign=_T("Signed");}
    else if (Codec==_T("SWF ADPCM"))        {Firm=_T("SWF");}
    else if (Codec==_T("1"))                {                   Endianness=_T("Little"); Sign=_T("Unsigned");}
    else if (Codec==_T("2"))                {Firm=_T("Microsoft");}
    else if (Codec==_T("3"))                {                   Endianness=_T("Float");}
    else if (Codec==_T("10"))               {Firm=_T("OKI");}
    else if (Codec==_T("11"))               {Firm=_T("Intel");}
    else if (Codec==_T("12"))               {Firm=_T("Mediaspace");}
    else if (Codec==_T("13"))               {Firm=_T("Sierra");}
    else if (Codec==_T("14"))               {Firm=_T("Antex");}
    else if (Codec==_T("17"))               {Firm=_T("Dialogic");}
    else if (Codec==_T("18"))               {Firm=_T("Mediavision");}
    else if (Codec==_T("20"))               {Firm=_T("Yamaha");}
    else if (Codec==_T("33"))               {Firm=_T("Antex");}
    else if (Codec==_T("36"))               {Firm=_T("DSP Solution");}
    else if (Codec==_T("38"))               {Firm=_T("Natural MicroSystems");}
    else if (Codec==_T("39"))               {Firm=_T("Crystal Semiconductor");}
    else if (Codec==_T("3B"))               {Firm=_T("Rockwell");}
    else if (Codec==_T("40"))               {Firm=_T("Antex Electronics");}
    else if (Codec==_T("42"))               {Firm=_T("IBM");}
    else if (Codec==_T("45"))               {Firm=_T("Microsoft"); ITU=_T("G.726");}
    else if (Codec==_T("64"))               {Firm=_T("Apicom"); ITU=_T("G.726");}
    else if (Codec==_T("65"))               {Firm=_T("Apicom"); ITU=_T("G.722");}
    else if (Codec==_T("85"))               {Firm=_T("DataFusion Systems"); ITU=_T("G.726");}
    else if (Codec==_T("8B"))               {Firm=_T("Infocom"); ITU=_T("G.721");}
    else if (Codec==_T("97"))               {Firm=_T("ZyXEL");}
    else if (Codec==_T("100"))              {Firm=_T("Rhetorex");}
    else if (Codec==_T("125"))              {Firm=_T("Sanyo");}
    else if (Codec==_T("140"))              {Firm=_T("Dictaphone"); ITU=_T("G.726");}
    else if (Codec==_T("170"))              {Firm=_T("Unisys");}
    else if (Codec==_T("175"))              {Firm=_T("SyCom"); ITU=_T("G.726");}
    else if (Codec==_T("178"))              {Firm=_T("Knownledge");}
    else if (Codec==_T("200"))              {Firm=_T("Creative");}
    else if (Codec==_T("210"))              {Firm=_T("Uher");}
    else if (Codec==_T("285"))              {Firm=_T("Norcom Voice Systems");}
    else if (Codec==_T("1001"))             {Firm=_T("Olivetti");}
    else if (Codec==_T("1C03"))             {Firm=_T("Lucent"); ITU=_T("G.723");}
    else if (Codec==_T("1C0C"))             {Firm=_T("Lucent"); ITU=_T("G.723");}
    else if (Codec==_T("4243"))             {ITU=_T("G.726");}
    else if (Codec==_T("A105"))             {ITU=_T("G.726");}
    else if (Codec==_T("A107"))             {ITU=_T("G.726");}

    Fill(Stream_Audio, 0, Audio_Codec_String, "PCM");
    Fill(Stream_Audio, 0, Audio_Codec_Family, "PCM");
    if (!Firm.empty())
    {
        Fill(Stream_Audio, 0, Audio_Format_Settings, Firm);
        Fill(Stream_Audio, 0, Audio_Format_Settings_Firm, Firm);
        Fill(Stream_Audio, 0, Audio_Codec_Settings, Firm);
        Fill(Stream_Audio, 0, Audio_Codec_Settings_Firm, Firm);
    }
    if (!Endianness.empty())
    {
        Fill(Stream_Audio, 0, Audio_Format_Settings, Endianness);
        Fill(Stream_Audio, 0, Audio_Format_Settings_Endianness, Endianness);
        Fill(Stream_Audio, 0, Audio_Codec_Settings, Endianness);
        Fill(Stream_Audio, 0, Audio_Codec_Settings_Endianness, Endianness);
    }
    if (!Sign.empty())
    {
        Fill(Stream_Audio, 0, Audio_Format_Settings, Sign);
        Fill(Stream_Audio, 0, Audio_Format_Settings_Sign, Sign);
        Fill(Stream_Audio, 0, Audio_Codec_Settings, Sign);
        Fill(Stream_Audio, 0, Audio_Codec_Settings_Sign, Sign);
    }
    if (!ITU.empty())
    {
        Fill(Stream_Audio, 0, Audio_Format_Settings, ITU);
        Fill(Stream_Audio, 0, Audio_Format_Settings_ITU, ITU);
        Fill(Stream_Audio, 0, Audio_Codec_Settings, ITU);
        Fill(Stream_Audio, 0, Audio_Codec_Settings_ITU, ITU);
    }
    Fill(Stream_Audio, 0, Audio_Resolution, Resolution);
    Fill(Stream_Audio, 0, Audio_BitRate_Mode, "CBR");
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Pcm::Header_Parse()
{
    //Filling
    Header_Fill_Code(0, "Block");
    Header_Fill_Size(Element_Size);
}

//---------------------------------------------------------------------------
void File_Pcm::Data_Parse()
{
    //Parsing
    if (Codec==_T("VOB") || Codec==_T("EVOB"))
        VOB();
    else
        Skip_XX(Element_Size,                                   "Data"); //It is impossible to detect... Default is no detection, only filling

    Accept("PCM");
    Finish("PCM");
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Pcm::VOB()
{
    //Parsing
    int8u Frequency, NumberOfChannelsMinusOne;
    Skip_B1(                                                    "Frame number");
    Skip_B2(                                                    "Unknown");
    BS_Begin();
    Skip_S1(4,                                                  "Unknown");
    Skip_S1(4,                                                  "Current frame");
    Skip_S1(2,                                                  "Unknown");
    Get_S1 (2, Frequency,                                       "Frequency");
    Skip_SB(                                                    "Unknown");
    Get_S1 (3, NumberOfChannelsMinusOne,                        "Number of channels (minus 1)");
    BS_End();
    Skip_B1(                                                    "Start code");
    Skip_XX(Element_Size-Element_Offset,                        "Data");

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "PCM");
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, Audio_Format, "PCM");
        Fill(Stream_Audio, 0, Audio_Codec, "PCM");
        Fill(Stream_Audio, 0, Audio_SamplingRate, Pcm_VOB_Frequency[Frequency]);
        Fill(Stream_Audio, 0, Audio_Channel_s_, NumberOfChannelsMinusOne+1);
        Fill(Stream_Audio, 0, Audio_ChannelPositions, Pcm_VOB_ChannelsPositions(NumberOfChannelsMinusOne));
        Fill(Stream_Audio, 0, Audio_ChannelPositions_String2, Pcm_VOB_ChannelsPositions2(NumberOfChannelsMinusOne));
        Fill(Stream_Audio, 0, Audio_BitRate, Pcm_VOB_Frequency[Frequency]*(NumberOfChannelsMinusOne+1)*16);
    FILLING_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_PCM_YES
