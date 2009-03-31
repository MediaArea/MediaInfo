// File_DvDif - Info for DV-DIF files
// Copyright (C) 2002-2009 Jerome Martinez, Zen@MediaArea.net
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
const char* Dv_Ssyb_Pc0(int8u Pc0)
{
    switch (Pc0)
    {
        case 0x13 : return "Timecode";
        case 0x14 : return "Binary group";
        case 0x50 :
        case 0x60 : return "Source";
        case 0x51 :
        case 0x61 : return "Source control";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char*  Dv_Disp[]=
{
    "4/3",
    "",
    "16/9",
    "Letterbox",
    "",
    "",
    "",
    "16/9",
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
const char*  Dv_StereoMode[]=
{
    "Multi-Stero",
    "Lumped",
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
    Frame_Count_Valid=14;
    AuxToAnalyze=0x00; //No Aux to analyze
    IgnoreAudio=false;

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
    TimeCode_First=(int64u)-1;
    DIFBlockNumbers[0][1]=1; //SubCode
    DIFBlockNumbers[1][1]=1; //SubCode
    DIFBlockNumbers[0][2]=2; //Aux
    DIFBlockNumbers[1][2]=2; //Aux
    DIFBlockNumbers[0][3]=8; //Audio
    DIFBlockNumbers[1][3]=8; //Audio
    DIFBlockNumbers[0][4]=134; //Video
    DIFBlockNumbers[1][4]=134; //Video
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Read_Buffer_Finalize()
{
    if (!IsAccepted)
        Accept("DV DIF");

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

    //Delay
    if (TimeCode_First!=(int64u)-1)
    {
        Fill(Stream_Video, 0, Video_Delay, TimeCode_First);
        Fill(Stream_Audio, 0, Audio_Delay, TimeCode_First);
    }
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Header_Parse()
{
    if (AuxToAnalyze!=0x00)
    {
        Header_Fill_Code(AuxToAnalyze, Ztring::ToZtring(AuxToAnalyze, 16));
        Header_Fill_Size(4);
        return;
    }

    //Parsing
    int8u SCT;
    BS_Begin();
    //0
    Get_S1 (3, SCT,                                             "SCT - Section Type"); Param_Info(Dv_sct[SCT]);
    Skip_SB(                                                    "Reserved");
    Skip_S1(4,                                                  "Arb - Arbitrary bits");
    //1
    Skip_S1(4,                                                  "Dseq - DIF sequence number"); //0-9 for 525/60; 0-11 for 625/50
    Get_S1 (1, FSC,                                             "FSC - Channel number");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    BS_End();
    //2
    Get_B1 (DIFBlockNumber,                                     "DIF block number"); //Video: 0-134, Audio: 0-8

    Header_Fill_Code(SCT, Dv_sct[SCT]);
    Header_Fill_Size(80);
}

//---------------------------------------------------------------------------
void File_DvDif::Data_Parse()
{
    if (AuxToAnalyze!=0x00)
    {
        Element();
        return;
    }

    //Integrity
    int8u Number=DIFBlockNumbers[FSC][(size_t)Element_Code]+1;
    switch (Element_Code)
    {
        case 0 : //Header
                    if (DIFBlockNumber!=0)
                        Trusted_IsNot("Wrong order");
                    break;
        case 1 : //Subcode
                    if (Number!=DIFBlockNumber && !(Number==2 && DIFBlockNumber==0))
                        Trusted_IsNot("Wrong order");
                    DIFBlockNumbers[FSC][1]=DIFBlockNumber;
                    break;
        case 2 : //Aux
                    if (Number!=DIFBlockNumber && !(Number==3 && DIFBlockNumber==0))
                        Trusted_IsNot("Wrong order");
                    DIFBlockNumbers[FSC][2]=DIFBlockNumber;
                    break;
        case 3 : //Audio
                    if (Number!=DIFBlockNumber && !(Number==9 && DIFBlockNumber==0))
                        Trusted_IsNot("Wrong order");
                    DIFBlockNumbers[FSC][3]=DIFBlockNumber;
                    break;
        case 4 : //Video
                    if (Number!=DIFBlockNumber && !(Number==135 && DIFBlockNumber==0))
                        Trusted_IsNot("Wrong order");
                    DIFBlockNumbers[FSC][4]=DIFBlockNumber;
                    break;
        default: ;
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

    //If small file
    if (!IsAccepted && (FrameCount>=1 && File_Offset+Buffer_Offset+Element_Size==File_Size))
        video_control_Fill();
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Header()
{
    BS_Begin();
    //3
    Get_SB (   dsf,                                             "DSF - DIF sequence flag"); //0=NTSC, 1=PAL
    Mark_0();
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");

    //4
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Get_S1 (3, apt,                                             "APT"); //Track application ID, 0=4:2:0, 1=not 4:2:0

    //5
    Get_SB (   tf1,                                             "TF1 - Audio data is not valid");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_S1(3,                                                  "AP1 - Audio application ID");

    //6
    Get_SB (  tf2,                                              "TF2 - Video data is not valid");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_S1(3,                                                  "AP3 - Video application ID");

    //7
    Get_SB (  tf3,                                              "TF3 - Subcode is not valid");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_S1(3,                                                  "AP3 - Subcode application ID");

    //8-79
    BS_End();
    Skip_XX(72,                                                 "Reserved"); //Should be filled with 0xFF

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

        Subcode_First=false;
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

    Subcode_First=true; //First --> Second Subcode
}

//---------------------------------------------------------------------------
void File_DvDif::Subcode_Ssyb(int8u syb_num)
{
    Element_Begin("ssyb");

    BS_Begin();
    //ID0-ID1
    Skip_SB(                                                    "FR - Identification of half of channel"); //1=first half, 0=second
    if (syb_num==0)
        Skip_S1( 3,                                             "AP3 - Subcode application ID");
    else if (Subcode_First && syb_num==5)
        Skip_S1(3,                                              "APT - track application ID");
    else
    {
        Skip_SB(                                                "Reserved");
        Skip_SB(                                                "Reserved");
        Skip_SB(                                                "Reserved");
    }
    Skip_S1(8,                                                  "Arbitrary bits");
    Skip_S1(4,                                                  "Syb - SSYSB number");

    //FFh
    BS_End();
    Skip_B1(                                                    "FFh");

    //An Element
    Element();

    Element_End();
}

//---------------------------------------------------------------------------
void File_DvDif::Aux()
{
    for (int8u i=0; i<15; i++)
        Element();
    Skip_XX(2,                                                  "Unused");

    if (Count_Get(Stream_General))
    {
        Accept("DV DIF"); //Here because we currently must be at the end of an element
        Finish("DV DIF");
    }
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
        case 0xFF : Element_Name(Ztring().From_Number(PackType, 16));
                    Skip_B4(                                    "Unused"); break;
        default: Skip_B4(                                       "Unknown");
    }
    Element_End();
}

//---------------------------------------------------------------------------
void File_DvDif::timecode()
{
    Element_Name("timecode");

    //PArsing
    int8u Temp;
    int64u Time=0;
    int8u Frames=0;
    BS_Begin();
    Skip_SB(                                                    "CF - Color fame");
    if (!dsf_IsValid)
        Skip_SB(                                                "Arbitrary bit or DP");
    else if (dsf)    //625/50
        Skip_SB(                                                "Arbitrary bit");
    else        //525/60
        Skip_SB(                                                "DP - Drop frame"); //525/60
    Get_S1 (2, Temp,                                            "Frames (Tens)");
    Frames+=Temp*10;
    Get_S1 (4, Temp,                                            "Frames (Units)");
    Frames+=Temp;
    if (dsf_IsValid && Temp!=0xF)
        Time+=(int64u)(Frames/(dsf?25.000:29.970));

    if (!dsf_IsValid)
        Skip_SB(                                                "BGF0 or PC");
    else if (dsf)    //625/50
        Skip_SB(                                                "BGF0 - Binary group flag");
    else        //525/60
        Skip_SB(                                                "PC - Biphase mark polarity correction"); //0=even; 1=odd
    Get_S1 (3, Temp,                                            "Seconds (Tens)");
    Time+=Temp*10*1000;
    Get_S1 (4, Temp,                                            "Seconds (Units)");
    Time+=Temp*1000;

    if (!dsf_IsValid)
        Skip_SB(                                                "BGF2 or BGF0");
    else if (dsf)    //625/50
        Skip_SB(                                                "BGF2 - Binary group flag");
    else        //525/60
        Skip_SB(                                                "BGF0 - Binary group flag");
    Get_S1 (3, Temp,                                            "Minutes (Tens)");
    Time+=Temp*10*60*1000;
    Get_S1 (4, Temp,                                            "Minutes (Units)");
    Time+=Temp*60*1000;

    if (!dsf_IsValid)
        Skip_SB(                                                "PC or BGF1");
    else if (dsf)    //625/50
        Skip_SB(                                                "PC - Biphase mark polarity correction"); //0=even; 1=odd
    else        //525/60
        Skip_SB(                                                "BGF1 - Binary group flag");
    Skip_SB(                                                    "BGF2 - Binary group flag");
    Get_S1 (2, Temp,                                            "Hours (Tens)");
    Time+=Temp*10*60*60*1000;
    Get_S1 (4, Temp,                                            "Hours (Units)");
    Time+=Temp*60*60*1000;
    Element_Info(Ztring().Duration_From_Milliseconds(Time));
    BS_End();

    if (TimeCode_First==(int64u)-1 && Time!=167185000) //if all bits are set to 1, this is not a valid timestamp
        TimeCode_First=Time;
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
    //PC1
    Skip_SB(                                                    "LF - Locked mode");
    Skip_SB(                                                    "Reserved");
    Skip_S1(6,                                                  "AF - Samples in this frame");

    //PC2
    Info_S1(1, StereoMode,                                      "SM - Stereo mode"); Param_Info(Dv_StereoMode[StereoMode]);
    Info_S1(2, ChannelsPerBlock,                                "CHN - Channels per block"); Param_Info(Dv_ChannelsPerBlock[ChannelsPerBlock]);
    Info_S1(1, Pair,                                            "PA - Pair"); Param_Info(Dv_Pair[Pair]);
    Skip_S1(4,                                                  "AM - Audio mode");

    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "ML - Multi-language");
    Get_SB (   dsf,                                             "50/60"); Param_Info(dsf?"PAL":"NTSC"); //As dsf
    Get_S1 (5, stype,                                           "STYPE - audio blocks per video frame"); Param_Info(stype==0?"2 channels":(stype==2?"4 channels":"Unknown")); //0=25 Mbps, 2=50 Mbps

    Skip_SB(                                                    "EF - Emphasis off");
    Skip_SB(                                                    "TC - Time constant of emphasis");
    Get_S1 (3, SamplingRate,                                    "SMP - Sampling rate"); Param_Info(Dv_Audio_SamplingRate[SamplingRate]);
    Get_S1 (3, Resolution,                                      "QU - Resolution"); Param_Info(Dv_Audio_Resolution[Resolution]);
    BS_End();

    FILLING_BEGIN();
        dsf_IsValid=true;
        if (!IgnoreAudio && (FrameCount==1 || AuxToAnalyze)) //Only the first time
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
                Fill(Stream_Audio, 1, Audio_BitRate, 2*Dv_Audio_SamplingRate[SamplingRate]*Dv_Audio_Resolution[Resolution]);
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

    //PC1
    Info_S1(2, CopyGenerationManagementSystem,                  "CGMS - Copy generation management system"); Param_Info(Dv_CopyGenerationManagementSystem[CopyGenerationManagementSystem]);
    Info_S1(2, InputType,                                       "ISR - Input type"); Param_Info(Dv_InputType[InputType]);
    Info_S1(2, CompressionTimes,                                "CMP - Compression times"); Param_Info(Dv_CompressionTimes[CompressionTimes]);
    Info_S1(2, Emphasis,                                        "EFC - Emphasis"); Param_Info(Dv_Emphasis[Emphasis]);

    //PC2
    Skip_SB(                                                    "REC S Non-recording start point");
    Skip_SB(                                                    "REC E - Non-recording end point");
    Skip_SB(                                                    "FADE S - Recording mode"); //1=Original
    Skip_SB(                                                    "FADE E - Unknown");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");
    Skip_SB(                                                    "Reserved");

    //PC3
    Skip_SB(                                                    "DRF - Direction"); //1=Forward
    Skip_S1(7,                                                  "SPD - Speed");

    //PC4
    Skip_SB(                                                    "Reserved");
    Skip_S1(7,                                                  "GEN - Category");

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
    //PC1
    Skip_S1(4,                                                  "TVCH (tens of units, 0–9)");
    Skip_S1(4,                                                  "TVCH (units, 0–9)");

    //PC2
    Skip_SB(                                                    "B/W - Black and White"); //0=Black and White, 1=Color
    Skip_SB(                                                    "EN - Color Frames is not valid");
    Skip_S1(2,                                                  "CLF - Color frames id");
    Skip_S1(4,                                                  "TVCH (hundreds of units, 0–9)");

    //PC3
    Skip_S1(2,                                                  "SRC");
    Get_SB (   dsf,                                             "50/60 - System"); Param_Info(dsf?"PAL":"NTSC"); //As dsf
    Get_S1 (4, stype,                                           "STYPE - Signal type of video signal"); //0=not 4:2:2, 4=4:2:2

    //PC4
    BS_End();
    Skip_B1(                                                    "TUN/VISC");

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

    BS_Begin();
    //PC1
    Info_S1(2, CopyGenerationManagementSystem,                  "CGMS - Copy generation management system"); Param_Info(Dv_CopyGenerationManagementSystem[CopyGenerationManagementSystem]);
    Skip_S1(2,                                                  "ISR");
    Skip_S1(2,                                                  "CMP");
    Skip_S2(2,                                                  "SS");

    //PC2
    Skip_SB(                                                    "REC S");
    Skip_SB(                                                    "Reserved");
    Skip_S1(2,                                                  "REC M");
    Skip_SB(                                                    "Reserved");
    Get_S1 (3, aspect,                                          "DISP - Aspect ratio"); Param_Info(Dv_Disp[aspect]);

    //PC3
    Skip_SB(                                                    "FF - Frame/Field"); //1=Frame, 0=Field
    Skip_SB(                                                    "FS - First/second field"); //0=Field 2, 1=Field 1, if FF=0 x is output twice, if FF=1, Field x fisrst, other second
    Skip_SB(                                                    "FC - Frame Change"); //0=Same picture as before
    Get_SB (   Interlaced,                                      "IL - Interlaced"); //1=Interlaced
    Skip_SB(                                                    "SF");
    Skip_SB(                                                    "SC");
    Skip_S1(2,                                                  "BCS");

    //PC4
    Skip_SB(                                                    "Reserved");
    Skip_S1(7,                                                  "GEN - Category");

    BS_End();

    FILLING_BEGIN();
        FrameCount++;
        if (!IsAccepted && (FrameCount>=Frame_Count_Valid || AuxToAnalyze))
            video_control_Fill();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::video_control_Fill()
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

    if (Month>12 || Day>31)
        return Ztring(); //If all bits are set to 1, this is invalid
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

    if (Time!=167185000)
        return Ztring().Duration_From_Milliseconds(Time);
    else
        return Ztring(); //If all bits are set to 1, this is invalid
}

} //NameSpace

#endif //MEDIAINFO_DV_YES

