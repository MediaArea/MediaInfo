// File_DvDif - Info for DV-DIF files
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_DVDIF_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_DvDif.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char*  Dv_sct[]=
{
    "Header",
    "Subcode",
    "Aux",
    "Audio",
    "Video",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const int32u  Dv_Audio_SamplingRate[]=
{
    48000,
    44100,
    32000,
    0,
    0,
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
const int32u  Dv_Audio_Resolution[]=
{
    16,
    12,
    0,
    0,
    0,
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
const char*  Dv_StereoMode[]=
{
    "Multi-Stero",
    "Lumped",
};

//---------------------------------------------------------------------------
const int8u  Dv_ChannelsPerBlock[]=
{
    1,
    2,
    0,
    0,
};

//---------------------------------------------------------------------------
const char*  Dv_Pair[]=
{
    "One pair of channels",
    "Independant channels",
};

//---------------------------------------------------------------------------
const char*  Dv_CopyGenerationManagementSystem[]=
{
    "Unrestricted",
    "Not used",
    "One generation only",
    "No copy",
};

//---------------------------------------------------------------------------
const char*  Dv_InputType[]=
{
    "Analog",
    "Digital",
    "Reserved",
    "No information",
};

//---------------------------------------------------------------------------
const char*  Dv_CompressionTimes[]=
{
    "Once",
    "Twice",
    "Three+",
    "No information",
};

//---------------------------------------------------------------------------
const char*  Dv_Emphasis[]=
{
    "Enphasis off",
    "Enphasis on",
    "Reserved",
    "Reserved",
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_DvDif::File_DvDif()
:File__Analyze()
{
    //In
    Frame_Count_Valid=8;
    AuxToAnalyze=0x00; //No Aux to analyze

    //Temp
    FrameCount=0;
    FrameSize_Theory=0;
    Duration=0;
    Subcode_First=true;
    apt=0xFF; //Impossible
    dsf_IsValid=false;
    tf1=false; //Valid by default, for direct analyze
    tf2=false; //Valid by default, for direct analyze
    tf3=false; //Valid by default, for direct analyze
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Read_Buffer_Finalize()
{
    if (!Recorded_Date_Date.empty())
    {
        if (Recorded_Date_Time.size()>4)
        {
            Recorded_Date_Time.resize(Recorded_Date_Time.size()-4); //Keep out milliseconds
            Recorded_Date_Date+=_T(" ");
            Recorded_Date_Date+=Recorded_Date_Time;
        }
        Fill(Stream_General, 0, General_Recorded_Date, Recorded_Date_Date);
    }
    if (!File_Name.empty() && Duration)
        Fill(Stream_General, 0, General_Duration, Duration);
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Header_Parse()
{
    Trusted=1; //We must not accept errors in the header (mainly because of problems in Multiple parsing)

    if (AuxToAnalyze!=0x00)
    {
        Header_Fill_Code(AuxToAnalyze, Ztring::ToZtring(AuxToAnalyze, 16));
        Header_Fill_Size(4);
        return;
    }

    //Parsing
    int8u SectionType;
    BS_Begin();
    Get_S1 (3, SectionType,                                     "Section Type");
    Skip_S1(5,                                                  "Section Type (more)"); //11111 if Header or Subcode, 10110 if Vaux, Audio or Video
    Skip_S1(4,                                                  "DIF sequence number"); //0-9 for 525/60; 0-11 for 625/50
    Skip_S1(1,                                                  "Channel number");
    Mark_1();
    Mark_1();
    Mark_1();
    BS_End();
    Get_B1 (DIFBlockNumber,                                     "DIF block number"); //Video: 0-134, Audio: 0-8

    Header_Fill_Code(SectionType, Dv_sct[SectionType]);
    Header_Fill_Size(80);

    Trusted=4; //We must not accept errors in the header (mainly because of problems in Multiple parsing)
}

//---------------------------------------------------------------------------
void File_DvDif::Data_Parse()
{
    if (AuxToAnalyze!=0x00)
    {
        Element();
        return;
    }

    Element_Info(DIFBlockNumber);

    switch (Element_Code)
    {
        case 0 : Header(); break;
        case 1 : Subcode(); break;
        case 2 : Aux(); break;
        case 3 : Audio(); break;
        case 4 : Video(); break;
        default: Skip_XX(Element_Size,                          "Unknown");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Header()
{
    BS_Begin();
    Get_SB (   dsf,                                             "dsf"); //0=NTSC, 1=PAL
    Mark_0();
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");

    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Get_S1 (3, apt,                                             "apt"); //Track application ID, 0=4:2:0, 1=not 4:2:0

    Get_SB (   tf1,                                             "tfl - Audio data is not valid");
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Skip_S1(3,                                                  "apl - Audio application ID");

    Get_SB (  tf2,                                              "tf2 - Video data is not valid");
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Skip_S1(3,                                                  "ap2 - Video application ID");

    Get_SB (  tf3,                                              "tf3 - Subcode is not valid");
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Skip_S1(3,                                                  "ap3 - Subcode application ID");

    BS_End();
    Skip_XX(72,                                                 "Unused");

    FILLING_BEGIN();
        dsf_IsValid=true;
        FrameSize_Theory=(dsf?12:10)*150*80; //12 DIF sequences for PAL, 10 for NTSC

        if (tf1 && tf2 && tf3)
        {
            //This is not logic, the header says no audio and no video! We do not trust the header, resetting all
            tf1=false;
            tf2=false;
            tf3=false;
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::Subcode()
{
    if (tf3)
    {
        Skip_XX(Element_Size,                                   "Unused");
        return;
    }

    for (int8u syb_num=0; syb_num<6; syb_num++)
        Subcode_Ssyb(syb_num);
    Skip_XX(29,                                                 "Unused");

    Subcode_First=!Subcode_First; //First --> Second, or Second --> First (only 2 Subcodes)
}

//---------------------------------------------------------------------------
void File_DvDif::Subcode_Ssyb(int8u syb_num)
{
    Element_Begin("ssyb");
    BS_Begin();
    Skip_S1(1,                                                  "FR ID"); //1=first half, 0=second
    if (Subcode_First && syb_num==0)
        Skip_S1( 3,                                             "ap3 - Subcode application ID");
    else if (!Subcode_First && syb_num==5)
    {
        Skip_S1(1,                                              "Unknown");
        Skip_S1(1,                                              "Unknown");
        Skip_S1(1,                                              "Unknown");
    }
    else
        Skip_S1(3,                                              "apt - track application ID");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");

    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(4,                                                  "syb_num");

    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    BS_End();

    Skip_B5(                                                    "Unknown");
    Element_End();
}

//---------------------------------------------------------------------------
void File_DvDif::Aux()
{
    for (int8u i=0; i<15; i++)
        Element();
    Skip_XX(2,                                                  "Unused");

    if (Count_Get(Stream_General))
        Finnished(); //Here because we currently must be at the end of an element
}

//---------------------------------------------------------------------------
void File_DvDif::Audio()
{
    Element_Name("Audio");

    if (tf1)
    {
        Skip_XX(Element_Size,                                   "Unused");
        return;
    }

    Element();

    Skip_XX(Element_Size-Element_Offset,                        "Unknown");
}

//---------------------------------------------------------------------------
void File_DvDif::Video()
{
    Element_Name("Video");

    if (tf2)
    {
        Skip_XX(Element_Size,                                   "Unused");
        return;
    }

    Skip_XX(Element_Size-Element_Offset,                        "Unknown");
}

//---------------------------------------------------------------------------
void File_DvDif::Element()
{
    Element_Begin();
    int8u PackType;
    if (AuxToAnalyze==0x00)
        Get_B1 (PackType,                                       "Pack Type");
    else
        PackType=AuxToAnalyze; //Forced by parser
    Element_Name(Ztring().From_Number(PackType, 16));
    switch(PackType)
    {
        case 0x13 : timecode(); break;
        case 0x50 : audio_source(); break;
        case 0x51 : audio_control(); break;
        case 0x52 : audio_recdate(); break;
        case 0x53 : audio_rectime(); break;
        case 0x60 : video_source(); break;
        case 0x61 : video_control(); break;
        case 0x62 : video_recdate(); break;
        case 0x63 : video_rectime(); break;
        case 0xFF : Skip_B4(                                    "Unused"); break;
        default: Skip_B4(                                       "Unknown");
    }
    Element_End();
}

//---------------------------------------------------------------------------
void File_DvDif::timecode()
{
    Element_Name("timecode");

    BS_Begin();
    Skip_SB(                                                    "Sync");
    Skip_SB(                                                    "Drop frame");
    Skip_S1(2,                                                  "Frames (Tens)");
    Skip_S1(4,                                                  "Frames (Units)");

    Skip_SB(                                                    "Biphase mark polarity correction"); //0=even; 1=odd
    Skip_S1(3,                                                  "Seconds (Tens)");
    Skip_S1(4,                                                  "Seconds (Units)");

    Skip_SB(                                                    "Binary group flag BGF0");
    Skip_S1(3,                                                  "Minutes (Tens)");
    Skip_S1(4,                                                  "Minutes (Units)");

    Skip_SB(                                                    "Binary group flag BGF1");
    Skip_SB(                                                    "Binary group flag BGF2");
    Skip_S1(2,                                                  "Hours (Tens)");
    Skip_S1(4,                                                  "Hours (Units)");
    BS_End();
}

//---------------------------------------------------------------------------
void File_DvDif::audio_source()
{
    if (tf1)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("audio_source");

    int8u stype, SamplingRate, Resolution;
    BS_Begin();
    Skip_SB(                                                    "Locked mode");
    Mark_1();
    Skip_S1(6,                                                  "Samples in this frame - min. samples");

    Info_S1(1, StereoMode,                                      "Stereo mode"); Param_Info(Dv_StereoMode[StereoMode]);
    Info_S1(2, ChannelsPerBlock,                                "Channels per block"); Param_Info(Dv_ChannelsPerBlock[ChannelsPerBlock]);
    Info_S1(1, Pair,                                            "Pair"); Param_Info(Dv_Pair[Pair]);
    Skip_S1(4,                                                  "Audio mode");

    Skip_SB(                                                    "Res?");
    Skip_SB(                                                    "Multi-language?");
    Get_SB (   dsf,                                             "System"); Param_Info(dsf?"50 fields":"60 fields"); //As dsf
    Get_S1 (5, stype,                                           "stype"); Param_Info(stype==0?"2 channels":(stype==2?"4 channels":"Unknown")); //0=25 Mbps, 2=50 Mbps

    Skip_SB(                                                    "Emphasis off");
    Skip_SB(                                                    "Time constant of emphasis");
    Get_S1 (3, SamplingRate,                                    "Sampling rate"); Param_Info(Dv_Audio_SamplingRate[SamplingRate]);
    Get_S1 (3, Resolution,                                      "Resolution"); Param_Info(Dv_Audio_Resolution[Resolution]);
    BS_End();

    FILLING_BEGIN();
        dsf_IsValid=true;
        if (FrameCount==1 || AuxToAnalyze) //Only the first time
        {
            Stream_Prepare(Stream_Audio);
            Fill(Stream_Audio, 0, Audio_Format, "PCM");
            Fill(Stream_Audio, 0, Audio_Codec, "PCM");
            Fill(Stream_Audio, 0, Audio_Channel_s_, 2);
            Fill(Stream_Audio, 0, Audio_SamplingRate, Dv_Audio_SamplingRate[SamplingRate]);
            Fill(Stream_Audio, 0, Audio_Resolution, Dv_Audio_Resolution[Resolution]);
            Fill(Stream_Audio, 0, Audio_BitRate, 2*Dv_Audio_SamplingRate[SamplingRate]*Dv_Audio_Resolution[Resolution]);

            if (stype==2 || (Resolution==1 && SamplingRate==2)) //stype=? or (Resolution=12 bits and SamplingRate=32 KHz)
            {
                Stream_Prepare(Stream_Audio);
                Fill(Stream_Audio, 1, Audio_Format, "PCM");
                Fill(Stream_Audio, 1, Audio_Codec, "PCM");
                Fill(Stream_Audio, 1, Audio_Channel_s_, 2);
                Fill(Stream_Audio, 1, Audio_SamplingRate, Dv_Audio_SamplingRate[SamplingRate]);
                Fill(Stream_Audio, 1, Audio_Resolution, Dv_Audio_Resolution[Resolution]);
                Fill(Stream_Audio, 0, Audio_BitRate, 2*Dv_Audio_SamplingRate[SamplingRate]*Dv_Audio_Resolution[Resolution]);
            }
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::audio_control()
{
    if (tf1)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("audio_control");

    BS_Begin();

    Info_S1(2, CopyGenerationManagementSystem,                  "Copy generation management system (CGMS)"); Param_Info(Dv_CopyGenerationManagementSystem[CopyGenerationManagementSystem]);
    Info_S1(2, InputType,                                       "Input type"); Param_Info(Dv_InputType[InputType]);
    Info_S1(2, CompressionTimes,                                "Compression times"); Param_Info(Dv_CompressionTimes[CompressionTimes]);
    Info_S1(2, Emphasis,                                        "Emphasis"); Param_Info(Dv_Emphasis[Emphasis]);

    Skip_SB(                                                    "Non-recording start point");
    Skip_SB(                                                    "Non-recording end point");
    Skip_S1(3,                                                  "Recording mode"); //1=Original
    Skip_S1(3,                                                  "Unknown");

    Skip_S1(1,                                                  "Direction"); //1=Forward
    Skip_S1(7,                                                  "Speed"); //0x20 for 4:2:0

    Mark_1();
    Skip_S1(7,                                                  "Category");

    BS_End();
}

//---------------------------------------------------------------------------
void File_DvDif::audio_recdate()
{
    if (tf1)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("audio_recdate");

    recdate();
}

//---------------------------------------------------------------------------
void File_DvDif::audio_rectime()
{
    if (tf1)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("audio_rectime");

    rectime();
}

//---------------------------------------------------------------------------
void File_DvDif::video_source()
{
    if (tf2)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("video_source");

    int8u stype;
    BS_Begin();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();

    Skip_SB(                                                    "Color");
    Skip_SB(                                                    "CLF is not valid");
    Skip_S1(2,                                                  "color frames id");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");

    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Get_SB (   dsf,                                             "System"); Param_Info(dsf?"PAL":"NTSC"); //As dsf
    Get_S1 (4, stype,                                           "stype"); //0=not 4:2:2, 4=4:2:2

    BS_End();
    Skip_B1(                                                    "VISC");

    FILLING_BEGIN();
        dsf_IsValid=true;
        if (FrameCount==0 || AuxToAnalyze) //Only the first time
        {
            Stream_Prepare(Stream_Video);
            Fill(Stream_Video, 0, Video_Format, "Digital Video");
            Fill(Stream_Video, 0, Video_Codec, "DV");
            Fill(Stream_Video, 0, Video_Standard, dsf?"PAL":"NTSC");
            Fill(Stream_Video, 0, Video_Width, 720);
            Fill(Stream_Video, 0, Video_Height, dsf?576:480);
            Fill(Stream_Video, 0, Video_FrameRate, dsf?25.000:29.970);
            Fill(Stream_Video, 0, Video_FrameRate_Mode, "CFR");

            if (dsf==false && stype==4) //NTSC and 4:2:2
                Fill(Stream_Video, 0, Video_Colorimetry, "4:2:2");       //NTSC 50 Mbps
            else if (dsf==false) //NTSC and not 4:2:2 (--> 4:1:1)
                Fill(Stream_Video, 0, Video_Colorimetry, "4:1:1");       //NTSC 25 Mbps
            else if (stype==4) //PAL and 4:2:2
                Fill(Stream_Video, 0, Video_Colorimetry, "4:2:2");       //PAL  50 Mbps
            else if (apt==0) //PAL and 4:2:0
                Fill(Stream_Video, 0, Video_Colorimetry, "4:2:0");       //PAL  25 Mbps 4:2:0
            else if (apt==1) //PAL and not 4:2:0 (--> 4:1:1)
                Fill(Stream_Video, 0, Video_Colorimetry, "4:1:1");       //PAL  25 Mbps 4:1:1

            if (FrameSize_Theory)
                Duration=(int64u)(File_Size*1000/(FrameSize_Theory*(dsf?25.000:29.970)));
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::video_control()
{
    if (tf2)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("video_control");

    int8u aspect;
    bool Interlaced;

    BS_Begin();
    Info_S1(2, CopyGenerationManagementSystem,                  "Copy generation management system(CGMS)"); Param_Info(Dv_CopyGenerationManagementSystem[CopyGenerationManagementSystem]);
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");

    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Get_S1 (3, aspect,                                          "aspect"); //0=4/3, 2=16/9, 3=letterbox, 7=16/9

    Skip_SB(                                                    "Frame (if not, Field)"); //1=Frame, 0=Field
    Skip_SB(                                                    "First Field"); //0=Field 2, 1=Field 1
    Skip_SB(                                                    "Frame Change"); //0=Same picture as before
    Get_SB (   Interlaced,                                      "Interlaced"); //1=Interlaced
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");

    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");

    BS_End();

    FILLING_BEGIN();
        FrameCount++;
        if (FrameCount>=Frame_Count_Valid || AuxToAnalyze)
        {
            Stream_Prepare(Stream_General);
            Fill(Stream_General, 0, General_Format, "Digital Video");

            Fill(Stream_Video, 0, Video_ScanType, Interlaced?"Interlaced":"Progressive");
            Fill(Stream_Video, 0, Video_Interlacement, Interlaced?"Interlaced":"PFF");
            switch (aspect)
            {
                case 0 : Fill(Stream_Video, 0, Video_DisplayAspectRatio, 4.0/3.0); break;
                case 2 :
                case 7 : Fill(Stream_Video, 0, Video_DisplayAspectRatio, 16.0/9.0); break;
                default: ;
            }
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::video_recdate()
{
    if (tf2)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("video_recdate");

    Ztring Date=recdate();
    if (Recorded_Date_Date.empty())
        Recorded_Date_Date=Date;
}

//---------------------------------------------------------------------------
void File_DvDif::video_rectime()
{
    if (tf2)
    {
        Skip_XX(4,                                              "Unused");
        return;
    }

    Element_Name("video_rectime");

    Ztring Date=rectime();
    if (Recorded_Date_Time.empty())
        Recorded_Date_Time=Date;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
Ztring File_DvDif::recdate()
{
    BS_Begin();

    int8u Temp;
    int16u Year=0;
    int8u  Month=0, Day=0;
    Skip_S1(8,                                                  "Time zone specific"); //ds, tm, tens of time zone, units of time zone, 0xFF for Unknwon
    Mark_1();
    Mark_1();
    Get_S1 (2, Temp,                                            "Days (Tens)");
    Day+=Temp*10;
    Get_S1 (4, Temp,                                            "Days (Units)");
    Day+=Temp;
    Mark_1();
    Mark_1();
    Mark_1();
    Get_S1 (1, Temp,                                            "Month (Tens)");
    Month+=Temp*10;
    Get_S1 (4, Temp,                                            "Month (Units)");
    Month+=Temp;
    Get_S1 (4, Temp,                                            "Year (Tens)");
    Year+=Temp*10;
    Get_S1 (4, Temp,                                            "Year (Units)");
    Year+=Temp;
    Year+=Year<25?2000:1900;
    Element_Info(Ztring::ToZtring(Year)+_T("-")+Ztring::ToZtring(Month)+_T("-")+Ztring::ToZtring(Day));

    BS_End();

    Ztring MonthString;
    if (Month<10)
        MonthString=_T("0");
    MonthString+=Ztring::ToZtring(Month);
    Ztring DayString;
    if (Day<10)
        DayString=_T("0");
    DayString+=Ztring::ToZtring(Day);
    return Ztring::ToZtring(Year)+_T("-")+MonthString+_T("-")+DayString;
}

//---------------------------------------------------------------------------
Ztring File_DvDif::rectime()
{
    if (!dsf_IsValid)
    {
        Trusted_IsNot("Not in right order");
        return Ztring();
    }

    BS_Begin();

    int8u Temp;
    int64u Time=0;
    int8u Frames=0;
    Skip_SB(                                                    "Unknown");
    Mark_1();
    Get_S1 (2, Temp,                                            "Frames (Tens)");
    Frames+=Temp*10;
    Get_S1 (4, Temp,                                            "Frames (Units)");
    Frames+=Temp;
    if (Temp!=0xF)
        Time+=(int64u)(Frames/(dsf?25.000:29.970));
    Mark_1();
    Get_S1 (3, Temp,                                            "Seconds (Tens)");
    Time+=Temp*10*1000;
    Get_S1 (4, Temp,                                            "Seconds (Units)");
    Time+=Temp*1000;
    Mark_1();
    Get_S1 (3, Temp,                                            "Minutes (Tens)");
    Time+=Temp*10*60*1000;
    Get_S1 (4, Temp,                                            "Minutes (Units)");
    Time+=Temp*60*1000;
    Mark_1();
    Mark_1();
    Get_S1 (2, Temp,                                            "Hours (Tens)");
    Time+=Temp*10*60*60*1000;
    Get_S1 (4, Temp,                                            "Hours (Units)");
    Time+=Temp*60*60*1000;
    Element_Info(Ztring().Duration_From_Milliseconds(Time));

    BS_End();

    return Ztring().Duration_From_Milliseconds(Time);
}

} //NameSpace

#endif //MEDIAINFO_DV_YES

