// File_Dts - Info for DTS files
// Copyright (C) 2004-2008 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_DTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Dts.h"
#include "ZenLib/Utils.h"
#include "ZenLib/BitStream.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char*  DTS_FrameType[]=
{
    "Termination",
    "Normal",
};

const int32u DTS_SamplingRate[]=
{
        0,  8000,  16000,  32000,      0,      0,  11025,  22050,
    44100,     0,      0,  12000,  24000,  48000,  96000, 192000,
};

const int32u DTS_BitRate[]=
{
      32000,   56000,   64000,   96000,  112000,  128000,  192000,  224000,
     256000,  320000,  384000,  448000,  512000,  576000,  640000,  768000,
     960000, 1024000, 1152000, 1280000, 1344000, 1408000, 1411200, 1472000,
    1536000, 1920000, 2048000, 3072000, 3840000,       0,       0,       0,
};

const int8u DTS_Channels[]=
{
    1, 2, 2, 2, 2, 3, 3, 4,
    4, 5, 6, 6, 6, 7, 8, 8,
};

const int8u DTS_Resolution[]=
{16, 20, 24, 24};

const char*  DTS_ChannelPositions[]=
{
    "Mono",
    "Dual mono",
    "L R",
    "L R",
    "L R",
    "L C R",
    "Front: L R, Surround: C",
    "Front: L C R, Surround: C",
    "Front: L R, Surround: L R",
    "Front: L C R, Surround: L R",
    "Front: L R, Middle: L R, Surround: L R",
    "Front: L C R, Rear: L R",
    "Front: L R, Middle: L R, Rear: L R",
    "Front: L C R, Middle: L R, Surround: L R",
    "Front: L R, Middle: L R, Surround: L C C R",
    "Front: L C R, Middle: L R, Surround: L C R",
};

const char*  DTS_ChannelPositions2[]=
{
    "1/0",
    "2/0",
    "2/0",
    "2/0",
    "2/0",
    "3/0",
    "2/1",
    "3/1",
    "2/2",
    "3/2",
    "2.2/2",
    "3/2",
    "2.2/2",
    "3.2/2",
    "2.2/4",
    "3.2/3",
};

const char* DTS_ExtensionAudioDescriptor[]=
{
    "Channel Extension",
    "Reserved",
    "Frequency Extension",
    "Channel Extension + Frequency Extension",
    "",
    "",
    "",
    "",
};

const char* DTS_HD_RefClockCode[]=
{
    "1/32000",
    "1/44100",
    "1/48000",
    "",
};

std::string DTS_HD_SpeakerActivityMask (int16u SpeakerActivityMask)
{
    std::string Text;
    if ((SpeakerActivityMask&0x0003)==0x0003)
        Text+="Front: L C R";
    else
    {
        if (SpeakerActivityMask&0x0001)
            Text+="Front: C";
        if (SpeakerActivityMask&0x0002)
            Text+="Front: L, R";
    }
        
    if ((SpeakerActivityMask&0x0014)==0x0014)
        Text+=", Surround: L C R";
    else
    {
        if (SpeakerActivityMask&0x0004)
            Text+=", Surround: L R";
        if (SpeakerActivityMask&0x0010)
            Text+=", Surround: C";
    }
       
    if ((SpeakerActivityMask&0x00A0)==0x00A0)
        Text+=", High: L C R";
    else
    {
        if (SpeakerActivityMask&0x0020)
            Text+=", High: L R";
        if (SpeakerActivityMask&0x0080)
            Text+=", High: C";
    }

    if ((SpeakerActivityMask&0x0840)==0x0840)
        Text+=", Surround: L C C R";
    else
    {
        if (SpeakerActivityMask&0x0800)
            Text+=", Surround: L R"; //In theory: Surround Side: L R
        if (SpeakerActivityMask&0x0040)
            Text+=", Surround: C C"; //In theory: Surround Rear: L R
    }
        
    if (SpeakerActivityMask&0x0100)
        Text+=", TopCtrSrrd";
    if (SpeakerActivityMask&0x0200)
        Text+=", Ctr: L R";
    if (SpeakerActivityMask&0x0400)
        Text+=", Wide: L R";
    if (SpeakerActivityMask&0x2000)
        Text+=", HiSide: L R";

    if ((SpeakerActivityMask&0xC000)==0x0C00)
        Text+=", HiRear: L C R";
    else
    {
        if (SpeakerActivityMask&0x4000)
            Text+=", HiRear: C";
        if (SpeakerActivityMask&0x8000)
            Text+=", HiRear: L R";
    }
        
    if (SpeakerActivityMask&0x0008)
        Text+=", LFE";
    if (SpeakerActivityMask&0x1000)
        Text+=", LFE2";

    return Text;
}

std::string DTS_HD_SpeakerActivityMask2 (int16u SpeakerActivityMask)
{
    std::string Text;
    if ((SpeakerActivityMask&0x0003)==0x0003)
        Text+="3";
    else
    {
        if (SpeakerActivityMask&0x0001)
            Text+="1";
        if (SpeakerActivityMask&0x0002)
            Text+="2";
    }
        
    if ((SpeakerActivityMask&0x0014)==0x0014)
        Text+="/3";
    else
    {
        if (SpeakerActivityMask&0x0004)
            Text+="/2";
        if (SpeakerActivityMask&0x0010)
            Text+="/1";
    }
       
    if ((SpeakerActivityMask&0x00A0)==0x00A0)
        Text+=".3";
    else
    {
        if (SpeakerActivityMask&0x0020)
            Text+=".2";
        if (SpeakerActivityMask&0x0080)
            Text+=".2";
    }

    if ((SpeakerActivityMask&0x0840)==0x0840)
        Text+="/4";
    else
    {
        if (SpeakerActivityMask&0x0800)
            Text+="/2";
        if (SpeakerActivityMask&0x0040)
            Text+="/2";
    }
        
    if (SpeakerActivityMask&0x0100)
        Text+=".1";
    if (SpeakerActivityMask&0x0200)
        Text+=".2";
    if (SpeakerActivityMask&0x0400)
        Text+=".2";
    if (SpeakerActivityMask&0x2000)
        Text+=".2";

    if ((SpeakerActivityMask&0xC000)==0x0C00)
        Text+=".3";
    else
    {
        if (SpeakerActivityMask&0x4000)
            Text+=".1";
        if (SpeakerActivityMask&0x8000)
            Text+=".2";
    }
        
    if (SpeakerActivityMask&0x0008)
        Text+=".1";
    if (SpeakerActivityMask&0x1000)
        Text+=".1";

    return Text;
}

const char* DTS_HD_TypeDescriptor[]=
{
    "Music",
    "Effects",
    "Dialog",
    "Commentary",
    "Visually Impaired",
    "Hearing  Impaired",
    "Isolated Music Object",
    "Music and Effects",
    "Dialog and Commentary",
    "Effects and Commentary",
    "Isolated Music Object and Commentary",
    "Isolated Music Object and Effects",
    "Karaoke",
    "Music, Effects, Dialog",
    "Complete Audio Presentation",
    "",
};

int32u DTS_HD_MaximumSampleRate[]=
{
      8000,
     16000,
     32000,
     64000,
    128000,
     22050,
     44100,
     88200,
    176400,
    352800,
     12000,
     24000,
     48000,
     96000,
    192000,
    384000,
};
//---------------------------------------------------------------------------

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Dts::File_Dts()
:File__Analyze()
{
    //Configuration
    File_MaximumOffset=64*1024;

    //In
    Frame_Count_Valid=8;

    //Out
    Delay=0;

    //Count
    Frame_Count=0;

    //Temp - Technical info
    Primary_Frame_Byte_Size_minus_1=0;
    HD_size=0;
    SyncCode=0x7FFE;
    Word=true;
    BigEndian=true;
    channel_arrangement_XCh=(int8u)-1;

    //Temp - HD
    HD_SpeakerActivityMask=(int16u)-1;
    HD_BitResolution=(int8u)-1;
    HD_MaximumSampleRate=(int8u)-1;
    HD_TotalNumberChannels=(int8u)-1;
    HD_ExSSFrameDurationCode=(int8u)-1;
    Core_Exists=false;
}

//---------------------------------------------------------------------------
void File_Dts::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonDTS())
        return;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Dts::Header_Begin()
{
    //Must have enough buffer for having header
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && CC4(Buffer+Buffer_Offset)!=SyncCode && CC4(Buffer+Buffer_Offset)!=0x64582025)
    {
        Info("DTS, Synchronisation lost"); //No error, this may be normal
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_Dts::Header_Parse()
{
    //Parsing
    if (CC4(Buffer+Buffer_Offset)==0x64582025) //TODO: 14-bits
    {
        //HD
        int16u header_size;
        int8u  SubStreamIndex, NumAssets, NumAudioPresent;
        bool isBlownUpHeader, StaticFieldsPresent;
        Skip_B4(                                                "Magic");
        Skip_B1(                                                "Unknown");
        BS_Begin();
        Get_S1 (2, SubStreamIndex,                              "Substream index");
        Get_SB (isBlownUpHeader,                                "Is blown up header");
        if (isBlownUpHeader)
        {
            Get_S2 (12, header_size,                            "Header size"); header_size++; Param_Info(header_size, " bytes");
            Get_S3 (20, HD_size,                                "HD block size"); HD_size++; Param_Info(HD_size, " bytes");
        }
        else
        {
            Get_S2 ( 8, header_size,                            "Header size"); header_size++; Param_Info(header_size, " bytes");
            Get_S3 (16, HD_size,                                "HD block size"); HD_size++; Param_Info(HD_size, " bytes");
        }
        TESTELSE_SB_GET(StaticFieldsPresent,                    "Static fields present");
            std::vector<int32u> ActiveExSSMasks;
            Info_S1(2, RefClockCode,                            "Reference clock code"); Param_Info(DTS_HD_RefClockCode[RefClockCode]);
            Get_S1 (3, HD_ExSSFrameDurationCode,                "ExSS frame duration code"); HD_ExSSFrameDurationCode++; Param_Info(HD_ExSSFrameDurationCode);
            TEST_SB_SKIP(                                       "Timestamp flag");
                Skip_S5(36,                                     "Timestamp");
            TEST_SB_END();
            Get_S1 (3, NumAudioPresent,                         "Num audio present"); NumAudioPresent++; Param_Info(NumAudioPresent, " channels");
            Get_S1 (3, NumAssets,                               "Num assets"); NumAssets++; Param_Info(NumAssets, " assets");
            Element_Begin("Active masks");
            for (int8u Pos=0; Pos<NumAudioPresent; Pos++)
            {
                int32u ActiveExSSMask;
                Get_S4 (SubStreamIndex+1, ActiveExSSMask,       "Active ExSS mask");
                ActiveExSSMasks.push_back(ActiveExSSMask);
            }
            Element_End();
            Element_Begin("Active masks 2");
            for (int8u Pos=0; Pos<NumAudioPresent; Pos++)
                for (int8u Pos2=0; Pos2<SubStreamIndex+1; Pos2+=2)
                    if (ActiveExSSMasks[Pos]%2)
                        Skip_S1(8,                              "Active ExSS Mask");
            Element_End();
            TEST_SB_SKIP(                                       "Mix metadata Enabled");
                int8u Bits4MixOutMask, NumMixOutConfigs;
                Skip_S1(2,                                      "Mix metadata adjustment level");
                Get_S1 (2, Bits4MixOutMask,                     "Bits4Mix out mask"); Bits4MixOutMask=4+Bits4MixOutMask*4; Param_Info(Bits4MixOutMask, " bits");
                Get_S1 (2, NumMixOutConfigs,                    "Number of mix out configs"); NumMixOutConfigs++; Param_Info(NumMixOutConfigs, " configs");
                for (int8u Pos=0; Pos<NumMixOutConfigs; Pos++)
                    Skip_S1(Bits4MixOutMask,                    "MixOutChMask");
            TEST_SB_END();
        TESTELSE_SB_ELSE("Static fields present");
            NumAudioPresent=1;
            NumAssets=1;
        TESTELSE_SB_END();
        Asset_Sizes.clear();
        Element_Begin("Sizes");
        for (int8u Pos=0; Pos<NumAssets; Pos++)
        {
            int32u Size;
            if (isBlownUpHeader)
            {
                Get_S3 (20, Size,                               "Size"); Size++; Param_Info(Size, " bytes");
            }
            else
            {
                Get_S3 (16, Size,                               "Size"); Size++; Param_Info(Size, " bytes");
            }
            Asset_Sizes.push_back(Size);
        }
        Element_End();
        for (int8u Pos=0; Pos<NumAssets; Pos++)
        {
            Element_Begin("Asset");
            int16u AssetSize;
            Get_S2 (9, AssetSize,                               "Asset size");
            AssetSize++;  Param_Info(AssetSize, " bytes?");
            Skip_S1(3,                                          "Descriptor data for asset index");
            if (StaticFieldsPresent)
            {
                TEST_SB_SKIP(                                   "Asset type descriptor present");
                    Info_S1( 4, TypeDescriptor,                 "Asset type descriptor"); Param_Info(DTS_HD_TypeDescriptor[TypeDescriptor]);
                TEST_SB_END();
                TEST_SB_SKIP(                                   "Language descriptor present");
                    Info_S3(24, LanguageDescriptor,             "Language descriptor"); Param_Info(Ztring().From_CC3(LanguageDescriptor));
                TEST_SB_END();
                TEST_SB_SKIP(                                   "Info text present");
                    int16u InfoTextByteSize;
                    Get_S2(10, InfoTextByteSize,                "Info text size"); InfoTextByteSize++; Param_Info(InfoTextByteSize, "bytes");
                    for (int16u Pos=0; Pos<InfoTextByteSize; Pos++)
                        Skip_S1(8,                              "Info text");
                TEST_SB_END();
                Get_S1 (5, HD_BitResolution,                    "Bit resolution"); HD_BitResolution++; Param_Info(HD_BitResolution, " bits");
                Get_S1 (4, HD_MaximumSampleRate,                "Maximum sample rate"); Param_Info(DTS_HD_MaximumSampleRate[HD_MaximumSampleRate], " Hz");
                Get_S1 (8, HD_TotalNumberChannels,              "Total number of channels"); HD_TotalNumberChannels++; Param_Info(HD_TotalNumberChannels, " channels");
                TEST_SB_SKIP(                                   "1 to 1 map channels to speakers");
                    int8u SpeakerActivityMaskBits, SpeakerRemapSetsCount;
                    if (HD_TotalNumberChannels>2)
                        Skip_SB(                                "Embedded stereo flag"); //else is 0
                    if (HD_TotalNumberChannels>6)
                        Skip_SB(                                "Embedded 6 Channels flag"); //else is 0
                    TESTELSE_SB_SKIP(                           "Speaker mask enabled");
                        Get_S1 (2, SpeakerActivityMaskBits,     "Speaker activity mask bits"); Param_Info(4+SpeakerActivityMaskBits*4, " bits");
                        SpeakerActivityMaskBits=4+SpeakerActivityMaskBits*4;
                        Get_S2 (SpeakerActivityMaskBits, HD_SpeakerActivityMask, "Speaker activity mask"); Param_Info(DTS_HD_SpeakerActivityMask(HD_SpeakerActivityMask).c_str());
                    TESTELSE_SB_ELSE(                           "Speaker mask enabled");
                        SpeakerActivityMaskBits=0;
                    TESTELSE_SB_END();
                    Get_S1 (3, SpeakerRemapSetsCount,           "Speaker remap sets count");
                    for (int8u Pos=0; Pos<SpeakerRemapSetsCount; Pos++)
                        Skip_S2(SpeakerActivityMaskBits,        "Standard speaker layout mask");
                    for (int8u Pos=0; Pos<SpeakerRemapSetsCount; Pos++)
                    {
                        int8u NumDecCh4Remap;
                        Get_S1(5, NumDecCh4Remap,               "NumDecCh4Remap");
                        //Not finnished!
                    }
                TEST_SB_END();
                Element_End();
            }
        }
        BS_End();
        Skip_XX(header_size-Element_Offset,                   "Unknown");

        //Filling
        Header_Fill_Size(HD_size);
        Header_Fill_Code(1, "HD");
    }
    else
    {
        //Frame
        int8u  EncoderSoftwareRevision;
        bool   crc_present;
        Skip_B4(                                                    "Sync");
        BS_Begin();
        Info_SB(    FrameType,                                      "Frame Type"); Param_Info(DTS_FrameType[FrameType]);
        Skip_S1( 5,                                                 "Deficit Sample Count");
        Get_SB (    crc_present,                                    "CRC Present");
        Skip_S1( 7,                                                 "Number of PCM Sample Blocks");
        Get_S2 (14, Primary_Frame_Byte_Size_minus_1,                "Primary Frame Byte Size minus 1");
        Primary_Frame_Byte_Size_minus_1+=1;
        if (!Word) Primary_Frame_Byte_Size_minus_1=Primary_Frame_Byte_Size_minus_1*8/14*2; Param_Info(Ztring::ToZtring(Primary_Frame_Byte_Size_minus_1)+_T(" bytes")); //Word is on 14 bits!
        Get_S1 ( 6, channel_arrangement,                            "Audio Channel Arrangement"); Param_Info(Ztring::ToZtring(DTS_Channels[channel_arrangement])+_T(" channels"));
        Get_S1 ( 4, sample_frequency,                               "Core Audio Sampling Frequency"); Param_Info(Ztring::ToZtring(DTS_SamplingRate[sample_frequency])+_T(" Hz"));
        Get_S1 ( 5, bit_rate,                                       "Transmission Bit Rate"); Param_Info(Ztring::ToZtring(DTS_BitRate[bit_rate])+_T(" bps"));
        Skip_SB(                                                    "Embedded Down Mix Enabled");
        Skip_SB(                                                    "Embedded Dynamic Range");
        Skip_SB(                                                    "Embedded Time Stamp");
        Skip_SB(                                                    "Auxiliary Data");
        Skip_SB(                                                    "HDCD");
        Get_S1 ( 3, ExtensionAudioDescriptor,                       "Extension Audio Descriptor"); Param_Info(DTS_ExtensionAudioDescriptor[ExtensionAudioDescriptor]);
        Get_SB (    ExtendedCoding,                                 "Extended Coding");
        Skip_SB(                                                    "Audio Sync Word Insertion");
        Get_S1 ( 2, lfe_effects,                                    "Low Frequency Effects");
        Skip_SB(                                                    "Predictor History");
        if (crc_present)
            Skip_S2(16,                                             "Header CRC Check");
        Skip_SB(                                                    "Multirate Interpolator");
        Get_S1 ( 4, EncoderSoftwareRevision,                        "Encoder Software Revision");
        Skip_S1( 2,                                                 "Copy History");
        Get_S1 ( 2, bits_per_sample,                                "Source PCM Resolution"); Param_Info(Ztring::ToZtring(DTS_Resolution[bits_per_sample])+_T(" bits"));
        Get_SB (    ES,                                             "ES");
        Skip_SB(                                                    "Front Sum/Difference");
        Skip_SB(                                                    "Surrounds Sum/Difference");
        Skip_S1( 4,                                                 "Dialog Normalisation Parameter");
        switch (EncoderSoftwareRevision)
        {
            case 6 :
                    Skip_S1( 4,                                     "Dialog Normalisation Parameter");
                    break;
            case 7 :
                    Skip_S1( 4,                                     "Dialog Normalisation Parameter");
                    break;
            default :
                    Skip_S1( 4,                                     "Unspecified");
                    break;
        }
        BS_End();

        //Filling
        Header_Fill_Size(Primary_Frame_Byte_Size_minus_1);
        Header_Fill_Code(0, "Frame");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dts::Data_Parse()
{
    //Counting
    if (File_Offset+Buffer_Offset+Element_Size==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    if (Element_Code==0 || !Core_Exists)
        Frame_Count++;

    //Name
    Element_Info(Ztring::ToZtring(Frame_Count));

    //Parsing
    switch(Element_Code)
    {
        case 0  : Core(); break;
        case 1  : HD(); break;
        default : Skip_XX(Element_Size,                         "Data");
    }
}

//---------------------------------------------------------------------------
void File_Dts::Data_Parse_Fill()
{
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "DTS");
    Fill(Stream_General, 0, General_Format_Profile, Profile);
    Stream_Prepare(Stream_Audio);
    Fill(Stream_Audio, 0, Audio_Format, "DTS");
    if (Profile.empty() && ES)
        Profile="ES";
    Fill(Stream_Audio, 0, Audio_Format_Profile, Profile);
    Fill(Stream_Audio, 0, Audio_Codec, (Profile.find(_T("MA"))==0 || Profile.find(_T("HRA"))==0)?"DTS-HD":"DTS");
    Fill(Stream_Audio, 0, Audio_BitRate_Mode, Profile.find(_T("MA"))==0?"VBR":"CBR");
    Fill(Stream_Audio, 0, Audio_SamplingRate, DTS_SamplingRate[sample_frequency]);
    if (Profile!=_T("MA") && bit_rate<29)
    {
        float64 BitRate;
        if (Profile==_T("Express"))
            BitRate=0; //No core bitrate
        else
            BitRate=(float64)DTS_BitRate[bit_rate];
        if (HD_ExSSFrameDurationCode!=(int8u)-1)
        {
            int32u SamplePerFrames=HD_ExSSFrameDurationCode;
            switch (HD_MaximumSampleRate)
            {
                case  0 : //  8000
                case 10 : // 12000
                                SamplePerFrames*= 128; break;
                case  1 : // 16000
                case  5 : // 22050
                case 11 : // 24000
                                SamplePerFrames*= 256; break;
                case  2 : // 32000
                case  6 : // 44100
                case 12 : // 48000
                                SamplePerFrames*= 512; break;
                case  3 : // 64000
                case  7 : // 88200
                case 13 : // 96000
                                SamplePerFrames*=1024; break;
                case  4 : //128000
                case  8 : //176400
                case 14 : //192000
                                SamplePerFrames*=2048; break;
                case  9 : //352800
                case 15 : //384000
                                SamplePerFrames*=4096; break;
                default     :   SamplePerFrames=    0; break; //Can never happen (4 bits)
            }
            if (SamplePerFrames)
                BitRate+=HD_size*8*DTS_HD_MaximumSampleRate[HD_MaximumSampleRate]/SamplePerFrames;
        }
        //if (Primary_Frame_Byte_Size_minus_1 && Profile==_T("HRA"))
        //    BitRate*=1+((float64)HD_size)/Primary_Frame_Byte_Size_minus_1; //HD block are not in the nominal bitrate
        Fill(Stream_Audio, 0, Audio_BitRate, BitRate, 0);
    }
    else if (bit_rate==29)
        Fill(Stream_Audio, 0, Audio_BitRate, "Open");
    else if (bit_rate==30)
        Fill(Stream_Audio, 0, Audio_BitRate, "Variable");
    else if (bit_rate==31)
        Fill(Stream_Audio, 0, Audio_BitRate, "LossLess");
    if ((ExtendedCoding && (ExtensionAudioDescriptor==0 || ExtensionAudioDescriptor==3 || ExtensionAudioDescriptor==6)))
    {
        switch(channel_arrangement_XCh)
        {
            case 1 :
                    Fill(Stream_Audio, 0, Audio_Channel_s_, 7);
                    Fill(Stream_Audio, 0, Audio_ChannelPositions, Ztring("Front: L C R, Rear: L C R")+(lfe_effects?_T(", LFE"):_T("")));
                    Fill(Stream_Audio, 0, Audio_ChannelPositions_String2, Ztring("3/3")+(lfe_effects?_T(".1"):_T("")));
                    break;
            case 2 :
                    Fill(Stream_Audio, 0, Audio_Channel_s_, 8);
                    Fill(Stream_Audio, 0, Audio_ChannelPositions, Ztring("Front: L C R, Rear: L C C R")+(lfe_effects?_T(", LFE"):_T("")));
                    Fill(Stream_Audio, 0, Audio_ChannelPositions_String2, Ztring("3/4")+(lfe_effects?_T(".1"):_T("")));
                    break;
            default:;
        }
    }
    else
    {
        int8u Channels;
        Ztring ChannelPositions, ChannelPositions2;
        if (channel_arrangement<16)
        {
            Channels=DTS_Channels[channel_arrangement]+(lfe_effects?1:0);
            ChannelPositions.From_Local(DTS_ChannelPositions[channel_arrangement]);
            ChannelPositions2.From_Local(DTS_ChannelPositions2[channel_arrangement]);
        }
        else
        {
            Channels=8;
            ChannelPositions.From_Local("User defined");
        }
        if (lfe_effects)
        {
            ChannelPositions+=_T(", LFE");
            ChannelPositions2+=_T(".1");
        }
        Fill(Stream_Audio, 0, Audio_Channel_s_, Channels);
        Fill(Stream_Audio, 0, Audio_ChannelPositions, ChannelPositions);
        Fill(Stream_Audio, 0, Audio_ChannelPositions_String2, ChannelPositions2);
    }
    Fill(Stream_Audio, 0, Audio_Resolution, DTS_Resolution[bits_per_sample]);

    //Priority on HD data
    if (HD_SpeakerActivityMask!=(int16u)-1)
    {
        Fill(Stream_Audio, 0, Audio_ChannelPositions, DTS_HD_SpeakerActivityMask(HD_SpeakerActivityMask).c_str(), Unlimited, true, true);
        Fill(Stream_Audio, 0, Audio_ChannelPositions_String2, DTS_HD_SpeakerActivityMask2(HD_SpeakerActivityMask).c_str(), Unlimited, true, true);
    }
    if(HD_BitResolution!=(int8u)-1)
        Fill(Stream_Audio, 0, Audio_Resolution, HD_BitResolution, 10, true);
    if(HD_MaximumSampleRate!=(int8u)-1)
        Fill(Stream_Audio, 0, Audio_SamplingRate, DTS_HD_MaximumSampleRate[HD_MaximumSampleRate], 10, true);
    if(HD_TotalNumberChannels!=(int8u)-1)
        Fill(Stream_Audio, 0, Audio_Channel_s_, HD_TotalNumberChannels, 10, true);
    

    Info("DTS detected");
    Finnished();
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dts::Core()
{
    //It exists (not in XSA streams)
    Core_Exists=true;

    //Looking for extensions
    int64u Core_Size=Element_Size, XCh_Sync=Element_Size, XXCh_Sync=Element_Size, X96k_Sync=Element_Size;
    if (ExtendedCoding)
    {
        //XCh
        if (ExtensionAudioDescriptor==0 || ExtensionAudioDescriptor==3)
            for (size_t Pos=0; Pos+4<=Element_Size; Pos++)
                if (CC4(Buffer+Buffer_Offset+Pos)==0x5A5A5A5A)
                    XCh_Sync=Pos;

        //XXCh
        if (ExtensionAudioDescriptor==6)
            for (size_t Pos=0; Pos+4<=Element_Size; Pos++)
                if (CC4(Buffer+Buffer_Offset+Pos)==0x47004A03)
                    XXCh_Sync=Pos;

        //X96k
        if (ExtensionAudioDescriptor==2 || ExtensionAudioDescriptor==3)
            for (size_t Pos=0; Pos+4<=Element_Size; Pos++)
                if (CC4(Buffer+Buffer_Offset+Pos)==0x1D95F262)
                    X96k_Sync=Pos;

        //Finding core size
        if (XCh_Sync<Core_Size)
            Core_Size=XCh_Sync;
        if (XXCh_Sync<Core_Size)
            Core_Size=XXCh_Sync;
        if (X96k_Sync<Core_Size)
            Core_Size=X96k_Sync;
    }

    //Parsing
    Skip_XX(Core_Size,                                          "Core data");
    if (ExtendedCoding && (ExtensionAudioDescriptor==2 || ExtensionAudioDescriptor==3))
    {
        Element_Begin();
        Skip_B4(                                                "Magic");
        Core_X96k(XCh_Sync-Element_Offset);
        Element_End();
    }
    if (ExtendedCoding && (ExtensionAudioDescriptor==0 || ExtensionAudioDescriptor==3))
    {
        Element_Begin();
        Skip_B4(                                                "Magic");
        Core_XCh(Element_Size-Element_Offset);
        Element_End();
    }
    if (ExtendedCoding && ExtensionAudioDescriptor==6)
    {
        Element_Begin();
        Skip_B4(                                                "Magic");
        Core_XXCh(Element_Size-Element_Offset);
        Element_End();
    }

    //Filling
    FILLING_BEGIN();
        if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid && (Profile.empty() || Profile==_T("ES")))
            Data_Parse_Fill();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::Core_XCh(int64u Size)
{
    //Parsing
    Element_Name("XCh (6.1 channels)");
    int16u XChFSIZE;
    int8u  AMODE;
    BS_Begin();
    Get_S2 (10, XChFSIZE,                                       "Primary Frame Byte Size");
    Get_S1 ( 4, AMODE,                                          "Extension Channel Arrangement");
    BS_End();
    if (XChFSIZE==Element_Size-(Element_Offset-6))
        XChFSIZE--; //Compatibility reason (from specs)
    Skip_XX(XChFSIZE+1-6,                                       "XCh data");

    FILLING_BEGIN();
        channel_arrangement_XCh=AMODE;
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::Core_XXCh(int64u Size)
{
    Element_Name("XXCh (6.1 or 7.1 channels)");
    int8u ChannelsAdded;
    BS_Begin();
    Skip_S1 (8,                                                 "?");
    Get_S1  (2, ChannelsAdded,                                  "Channels added?");
    Skip_S1 (6,                                                 "?");
    BS_End();
    Skip_XX(Size-2,                                             "Data");

    FILLING_BEGIN();
        channel_arrangement_XCh=ChannelsAdded;
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::Core_X96k(int64u Size)
{
    //Parsing
    Element_Name("X96k (96 KHz)");
    int16u FSIZE96;
    int8u  REVNO;
    BS_Begin();
    Get_S2 (12, FSIZE96,                                        "96 kHz Extension Frame Byte Data Size");
    Get_S1 ( 4, REVNO,                                          "Revision Number");
    BS_End();
    Skip_XX(Size-2,                                             "X96k data"); //FSIZE96 is until end, not X96k size

    FILLING_BEGIN();
        sample_frequency=14; //96KHz
        Profile="96/24";
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::HD()
{
    //Parsing
    while (Element_Offset<Element_Size)

    {
        //Looking for size
        int64u Next=Element_Offset;
        while (Next+4<=Element_Size)
        {
            Next++;
            int32u CC=CC4(Buffer+Buffer_Offset+(size_t)Next);
            if (CC==0x0A801921
             || CC==0x1D95F262
             || CC==0x41A29547
             || CC==0x47004A03
             || CC==0x5A5A5A5A
             || CC==0x655E315E)
                break;
        }
        if (Next+4>Element_Size)
            Next=Element_Size;

        Element_Begin();
        int32u Magic;
        Get_B4 (Magic,                                          "Magic");
        switch (Magic)
        {
            case 0x0A801921 : HD_XSA(Next-Element_Offset);  break;
            case 0x1D95F262 : HD_X96k(Next-Element_Offset); break;
            case 0x41A29547 : HD_XLL(Next-Element_Offset);  break;
            case 0x47004A03 : HD_XXCh(Next-Element_Offset); break;
            case 0x5A5A5A5A : HD_XCh(Next-Element_Offset);  break;
            case 0x655E315E : HD_XBR(Next-Element_Offset);  break;
            default :
                        //Magic value is unknown
                        if (Profile.empty())
                            Profile="HD";
                        Skip_XX(Next-Element_Offset,            "Data");
        }
        Element_End();
    }

    //Filling
    FILLING_BEGIN();
        if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
            Data_Parse_Fill();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::HD_XCh(int64u Size)
{
    //Parsing
    Element_Name("XCh (6.1 channels)");
    Skip_XX(Size,                                               "Data");

    FILLING_BEGIN();
        channel_arrangement_XCh=1;
        if (Profile.empty())
            Profile="HRA";
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::HD_XXCh(int64u Size)
{
    Element_Name("XXCh (6.1 or 7.1 channels)");
    int8u ChannelsAdded;
    BS_Begin();
    Skip_S1 (8,                                                 "?");
    Get_S1  (2, ChannelsAdded,                                  "Channels added?");
    Skip_S1 (6,                                                 "?");
    BS_End();
    Skip_XX(Size-2,                                             "Data");

    FILLING_BEGIN();
        channel_arrangement_XCh=ChannelsAdded;
        if (Profile.empty())
            Profile="HRA";
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::HD_X96k(int64u Size)
{
    //Parsing
    Element_Name("X96k (96 KHz)");
    Skip_XX(Size,                                               "Data");

    FILLING_BEGIN();
        sample_frequency=14; //96KHz
        Profile="HRA";
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::HD_XLL(int64u Size)
{
    Element_Name("XLL (LossLess)");
    Skip_XX(Size,                                               "Data");

    FILLING_BEGIN();
        Profile="MA";
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::HD_XBR(int64u Size)
{
    Element_Name("XBR (BitRate extension)");
    Skip_XX(Size,                                               "Data");

    FILLING_BEGIN();
        Profile="HRA";
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dts::HD_XSA(int64u Size)
{
    Element_Name("XSA (low bitrate)");
    Skip_XX(Size,                                               "Data");

    FILLING_BEGIN();
        Profile="Express";
    FILLING_END();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Dts::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+6<=Buffer_Size
      && !(BigEndian2int16u   (Buffer+Buffer_Offset+0)==0x7FFE && BigEndian2int16u   (Buffer+Buffer_Offset+2)==0x8001) //16 bits and big    endian bitstream
      && !(LittleEndian2int16u(Buffer+Buffer_Offset+0)==0x7FFE && LittleEndian2int16u(Buffer+Buffer_Offset+2)==0x8001) //16 bits and little endian bitstream
      && !(BigEndian2int16u   (Buffer+Buffer_Offset+0)==0x1FFF && BigEndian2int16u   (Buffer+Buffer_Offset+2)==0xE800 && (BigEndian2int16u   (Buffer+Buffer_Offset+4)&0xFFF0)==0x07F0) //14 bits and big    endian bitstream
      && !(LittleEndian2int16u(Buffer+Buffer_Offset+0)==0x1FFF && LittleEndian2int16u(Buffer+Buffer_Offset+2)==0xE800 && (LittleEndian2int16u(Buffer+Buffer_Offset+4)&0xFFF0)==0x07F0) //14 bits and little endian bitstream
      && CC4(Buffer+Buffer_Offset)!=0x64582025 //TODO: 14 bit HD detection
      )
        Buffer_Offset++;
    if (Buffer_Offset+6>Buffer_Size)
    {
        //Parsing last bytes
        if (Buffer_Offset+6==Buffer_Size)
        {
            if (CC4(Buffer+Buffer_Offset)!=0x7FFE8001) //For 6 bytes instead of 4 (14 bits)
            {
                Buffer_Offset++;
                if (CC4(Buffer+Buffer_Offset)!=0x7FFE8001) //For 6 bytes instead of 4 (14 bits)
                {
                    Buffer_Offset++;
                    if (CC4(Buffer+Buffer_Offset)!=0x7FFE8001)
                    {
                        Buffer_Offset++;
                        if (CC3(Buffer+Buffer_Offset)!=0x7FFE80)
                        {
                            Buffer_Offset++;
                            if (CC2(Buffer+Buffer_Offset)!=0x7FFE)
                            {
                                Buffer_Offset++;
                                if (CC1(Buffer+Buffer_Offset)!=0x7F)
                                    Buffer_Offset++;
                            }
                        }
                    }
                }
            }
        }

        //Delay
        if (Frame_Count==0)
            Delay+=Buffer_Offset;

        return false;
    }

    //Configuring and Delay
    if (Frame_Count==0)
    {
        if (0) ;
        else if (BigEndian2int16u   (Buffer+Buffer_Offset)==0x7FFE) //16 bits and big    endian bitstream
        {
        }
        else if (LittleEndian2int16u(Buffer+Buffer_Offset)==0x7FFE) //16 bits and little endian bitstream
        {
            BigEndian=false;
        }
        else if (BigEndian2int16u   (Buffer+Buffer_Offset)==0x1FFF) //14 bits and big    endian bitstream
        {
            Word=false;
        }
        else if (LittleEndian2int16u(Buffer+Buffer_Offset)==0x1FFF) //14 bits and little endian bitstream
        {
            Word=false;
            BigEndian=false;
        }

        SyncCode=CC4(Buffer+Buffer_Offset);

        //14-bits and Little endian are not yet supported
        if (!Word || !BigEndian)
        {
            Finnished();
            return false;
        }

        //Delay
        Delay+=Buffer_Offset;
    }

    //Synched is OK
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_Dts::Detect_NonDTS ()
{
    //Element_Size
    if (Buffer_Size<4)
        return true; //Must wait for more data

    //Detect WAV files, the parser can't detect it easily, there is only 70 bytes of begining for sayint WAV
    if (CC4(Buffer)==CC4("RIFF"))
    {
        Finnished();
        return true;
    }

    //Seems OK
    return false;
}

} //NameSpace

#endif //MEDIAINFO_DTS_YES

