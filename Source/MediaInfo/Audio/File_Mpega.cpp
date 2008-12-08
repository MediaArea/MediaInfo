// File_Mpega - Info for MPEG Audio files
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
//
// A good start : http://www.codeproject.com/audio/MPEGAudioInfo.asp
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGA_YES) || defined(MEDIAINFO_MPEGTS_YES) || defined(MEDIAINFO_MPEGPS_YES)
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#include "ZenLib/Conf.h"
using namespace ZenLib;

//---------------------------------------------------------------------------
const char* Mpega_Format_Profile_Version[4]=
{
    "Version 2.5",
    "",
    "Version 2",
    "Version 1"
};

//---------------------------------------------------------------------------
const char* Mpega_Format_Profile_Layer[4]=
{
    "",
    "Layer 3",
    "Layer 2",
    "Layer 1",
};

//---------------------------------------------------------------------------
const char* Mpega_Version[4]=
{
    "MPA2.5",
    "",
    "MPA2",
    "MPA1"
};

const char* Mpega_Version_String[4]=
{
    "MPEG-2.5 Audio",
    "",
    "MPEG-2 Audio",
    "MPEG-1 Audio",
};

const char* Mpega_Layer[4]=
{
    "",
    "L3",
    "L2",
    "L1",
};

const char* Mpega_Layer_String[4]=
{
    "",
    " layer 3",
    " layer 2",
    " layer 1",
};

const int16u Mpega_BitRate[4][4][16]=
{
    {{0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio 2.5 layer X
     {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160,   0},  //MPEG Audio 2.5 layer 3
     {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160,   0},  //MPEG Audio 2.5 layer 2
     {0,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256,   0}}, //MPEG Audio 2.5 layer 1
    {{0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio X layer X
     {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio X layer 3
     {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio X layer 2
     {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}}, //MPEG Audio X layer 1
    {{0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio 2 layer X
     {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160,   0},  //MPEG Audio 2 layer 3
     {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160,   0},  //MPEG Audio 2 layer 2
     {0,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256,   0}}, //MPEG Audio 2 layer 1
    {{0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio 1 layer X
     {0,  32,  40,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320,   0},  //MPEG Audio 1 layer 3
     {0,  32,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384,   0},  //MPEG Audio 1 layer 2
     {0,  32,  64,  96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448,   0}}, //MPEG Audio 1 layer 1
};

const int16u Mpega_SamplingRate[4][4]=
{
    {11025, 12000,  8000, 0}, //MPEG Audio 2.5
    {    0,     0,     0, 0}, //MPEG Audio X
    {22050, 24000, 16000, 0}, //MPEG Audio 2
    {44100, 48000, 32000, 0}, //MPEG Audio 1
};

const int16u Mpega_Channels[4]=
{
    2,
    2,
    2,
    1,
};

const char* Mpega_Codec_Profile[4]=
{
    "",
    "Joint stereo",
    "Dual mono",
    "",
};

const char* Mpega_Codec_Profile_Extension[]=
{
    "",
    "Intensity Stereo",
    "MS Stereo",
    "Intensity Stereo + MS Stereo",
};

const char* Mpega_Emphasis[]=
{
    "",
    "50/15ms",
    "Reserved",
    "CCITT",
};

const char* Lame_BitRate_Mode[]=
{
    "",
    "CBR",
    "VBR",
    "VBR",
    "VBR",
    "VBR",
    "VBR",
    "",
    "CBR",
    "VBR",
    "",
    "",
    "",
    "",
    "",
    "",
};

const char* Lame_Method[]=
{
    "",
    "CBR",
    "ABR",
    "VBR (rh)",
    "VBR (mtrh)",
    "VBR (rh)",
    "VBR",
    "",
    "CBR (2-pass)",
    "ABR (2-pass)",
    "",
    "",
    "",
    "",
    "",
    "",
};


const int8u Mpega_Coefficient[4][4] = //Samples per Frame / 8
{
    {  0,  72, 144,  12}, //MPEG Audio 2.5
    {  0,   0,   0,   0}, //MPEG Audio X
    {  0,  72, 144,  12}, //MPEG Audio 2
    {  0, 144, 144,  12}, //MPEG Audio 1
};

const int8u Mpega_SlotSize[4]= //A frame is coposed of slots
{
    0, // Layer X
    1, // Layer3
    1, // Layer2
    4, // Layer1
};                       
//---------------------------------------------------------------------------
} //NameSpace

//---------------------------------------------------------------------------
#endif //...
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGA_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Mpega.h"
#include "ZenLib/BitStream.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpega::File_Mpega()
:File__Analyze(), File__Tags_Helper()
{
    //File__Tags_Helper
    Base=this;

    //Configuration
    File_MaximumOffset=128*1024;

    //In
    Frame_Count_Valid=16;
    FrameIsAlwaysComplete=false;

    //Out
    Delay=0;

    //Temp - Global
    Frame_Count=0;
    LastSync_Offset=(int64u)-1;

    //Temp - BitStream info
    Block_Count[0]=0;
    Block_Count[1]=0;
    Block_Count[2]=0;
    Channels_Count[0]=0;
    Channels_Count[1]=0;
    Channels_Count[2]=0;
    Channels_Count[3]=0;
    Extension_Count[0]=0;
    Extension_Count[1]=0;
    Extension_Count[2]=0;
    Extension_Count[3]=0;
    Emphasis_Count[0]=0;
    Emphasis_Count[1]=0;
    Emphasis_Count[2]=0;
    Emphasis_Count[3]=0;
    Scfsi=0;
    Scalefac=0;
    Reservoir=0;
    Reservoir_Max=0;

    //Temp - VBR handling
    VBR_Frames=0;
    VBR_FileSize=0;

    //Temp - Tags
    Xing_Scale=0;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpega::Read_Buffer_Continue()
{
    //Tags
    if (!File__Tags_Helper::Read_Buffer_Continue())
        return;

    //Integrity
    if (File_Offset==0 && Detect_NonMPEGA())
        return;
}

//---------------------------------------------------------------------------
void File_Mpega::Read_Buffer_Finalize()
{
    if (Count_Get(Stream_Audio)==0)
        return;

    //VBR detection without header
    if (VBR_Frames==0)
    {
        //How much kinds of bitrates?
        if (BitRate_Count.size()>1)
            BitRate_Mode=_T("VBR");
    }

    //VBR header
    int32u BitRate;
    if (VBR_Frames==0 && BitRate_Mode!=_T("VBR"))
    {
        BitRate=Mpega_BitRate[ID][layer][bitrate_index]*1000;
        BitRate_Mode=_T("CBR");
    }
    else if (VBR_Frames>0)
    {
        float32 FrameLength=((float32)(VBR_FileSize?VBR_FileSize:File_Size-File_EndTagSize-File_BeginTagSize))/VBR_Frames;
        size_t Divider;
        if (ID==3 && layer==3) //MPEG 1 layer 1
             Divider=384/8;
        else if ((ID==2 || ID==0) && layer==1) //MPEG 2 or 2.5 layer 3
            Divider=576/8;
        else
            Divider=1152/8;
        BitRate=(int32u)(FrameLength*Mpega_SamplingRate[ID][sampling_frequency]/Divider);
        BitRate_Mode=_T("VBR");
    }
    else
        BitRate=0;

    //Bitrate VBR
    if (BitRate_Count.size()>1)
    {
        Ztring BitRate_VBR;
        if (!BitRate_VBR.empty())
            BitRate_VBR+=_T(' ');
        BitRate_VBR+=Ztring::ToZtring(8);
        BitRate_VBR+=_T(':');
        BitRate_VBR+=Ztring::ToZtring(BitRate_Count[8]);
        //Fill("BitRate_VBR", Ztring::ToZtring(BitRate_Count[8]));
    }

    //Reservoir
    //Fill("Reservoir_Avg", Reservoir/Frame_Count);
    //Fill("Reservoir_Max", Reservoir_Max);
    //size_t Granules=(Mpeg==3?2:1);
    //size_t Ch=Mpega_Channels[Channels];
    //Fill("Scalefactors", Ztring::ToZtring(Scalefac*100/(Granules*Ch*Frame_Count))+_T('%'));

    //Filling
    if ((File_Size!=(int64u)-1 || LastSync_Offset!=(int64u)-1) && VBR_FileSize==0)
    {
        //We calculate VBR_FileSize from the last synch or File_Size
        if (LastSync_Offset!=(int64u)-1)
        {
            VBR_FileSize=LastSync_Offset;
            VBR_FileSize-=File_BeginTagSize;
        }
        else
        {
            VBR_FileSize=File_Size;
            VBR_FileSize-=File_BeginTagSize;
            VBR_FileSize-=File_EndTagSize;
        }
    }
    if (BitRate>0 && !File_Name.empty())
        Fill(Stream_General, 0, General_Duration, VBR_FileSize*8*1000/BitRate);
    Fill(Stream_General, 0, General_Encoded_Library, Encoded_Library);
    if (BitRate>0)
    {
        Fill(Stream_General, 0, General_OverallBitRate, BitRate);
        Fill(Stream_Audio, 0, Audio_BitRate, BitRate);
        if (Delay>100 && BitRate>0)
            Fill(Stream_Audio, 0, Audio_Delay, (float)Delay*8*1000/BitRate, 0);
    }
    Fill(Stream_Audio, 0, Audio_BitRate_Mode, BitRate_Mode);
    Fill(Stream_Audio, 0, Audio_BitRate_Minimum, BitRate_Minimum);
    Fill(Stream_Audio, 0, Audio_BitRate_Nominal, BitRate_Nominal);
    if (Encoded_Library.empty())
        Encoded_Library_Guess();
    Fill(Stream_Audio, 0, Audio_Encoded_Library, Encoded_Library);
    Fill(Stream_Audio, 0, Audio_Encoded_Library_Settings, Encoded_Library_Settings);

    //Tags
    File__Tags_Helper::Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpega::Header_Begin()
{
    if (!File__Tags_Helper::Header_Begin())
        return false;

    //Must have enough buffer for having header
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Some file have the padding bit set but there is no padding
    //if ((CC2(Buffer+Buffer_Offset+Element_Size)&0xFFE0)!=0xFFE0)// && (Buffer[Buffer_Offset+1]&0x18)!=0x08 && (Buffer[Buffer_Offset+1]&0x06)!=0x00)
    //    Element_Size--;

    //Quick test of synchro
    if (Synched && ((CC2(Buffer+Buffer_Offset)&0xFFE0)!=0xFFE0 || (CC1(Buffer+Buffer_Offset+2)&0xF0)==0xF0 || (CC1(Buffer+Buffer_Offset+2)&0x0C)==0x0C))
    {
        Trusted_IsNot("MPEG-A, Synchronisation lost");
        Frame_Count=0; //Reset the frame count, we try to find x consecutive frames
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Mpega::Header_Parse()
{
    //Parsing
    BS_Begin();
    Skip_S2(11,                                                 "syncword");
    Get_S1 (2, ID,                                              "ID"); Param_Info(Mpega_Version[ID]);
    Get_S1 (2, layer,                                           "layer"); Param_Info(Mpega_Layer[layer]);
    Get_SB (   protection_bit,                                  "protection_bit");
    Get_S1 (4, bitrate_index,                                   "bitrate_index"); Param_Info(Mpega_BitRate[ID][layer][bitrate_index], " Kbps");
    Get_S1 (2, sampling_frequency,                              "sampling_frequency"); Param_Info(Mpega_SamplingRate[ID][sampling_frequency], " Hz");
    Get_SB (   padding_bit,                                     "padding_bit");
    Skip_SB(                                                    "private_bit");
    Get_S1 (2, mode,                                            "mode"); Param_Info(Mpega_Channels[mode], " channels"); Param_Info(Mpega_Codec_Profile[mode]);
    Get_S1 (2, mode_extension,                                  "mode_extension"); Param_Info(Mpega_Codec_Profile_Extension[mode_extension]);
    Get_SB (   copyright,                                       "copyright");
    Get_SB (   original_home,                                   "original_home");
    Get_S1 (2, emphasis,                                        "emphasis"); Param_Info(Mpega_Emphasis[emphasis]);
    BS_End();

    //Coherancy
    if (Mpega_SamplingRate[ID][sampling_frequency]==0 || Mpega_Coefficient[ID][layer]==0 || Mpega_BitRate[ID][layer][bitrate_index]==0 || Mpega_SlotSize[layer]==0)
    {
        Element_Offset=1;
        Header_Fill_Size(1);
        Header_Fill_Code(0, "False start");
        Synched=false;
        return;
    }

    //Filling
    int64u Size=(Mpega_Coefficient[ID][layer]*Mpega_BitRate[ID][layer][bitrate_index]*1000/Mpega_SamplingRate[ID][sampling_frequency]+(padding_bit?1:0))*Mpega_SlotSize[layer];
    Header_Fill_Size(Size);
    Header_Fill_Code(0, "Frame");

    //Filling error detection
    sampling_frequency_Count[sampling_frequency]++;
    mode_Count[mode]++;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpega::Data_Parse()
{
    //If false start
    if (Element_Size==0)
    {
        Element_DoNotShow();
        return;
    }

    //VBR and library headers
    if (Frame_Count<3) //No need to do it too much
    {
        if (Header_Xing()) return;
        if (Header_VBRI()) return;
    }

    //Counting
    if (File_Offset+Buffer_Offset+Element_Size==File_Size-File_EndTagSize)
        Frame_Count_Valid=Frame_Count; //Finalize MPEG Audio frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;

    //Name
    Element_Info(Ztring::ToZtring(Frame_Count));

    //LAME
    if (Encoded_Library.empty() && (Frame_Count<Frame_Count_Valid || File_Offset+Buffer_Offset+Element_Size==File_Size-File_EndTagSize)) //Can be elsewhere... At the start, or end frame
        Header_Encoders();

    //Filling
    BitRate_Count[Mpega_BitRate[ID][layer][bitrate_index]]++;
    Channels_Count[mode]++;
    Extension_Count[mode_extension]++;
    Emphasis_Count[emphasis]++;

    //Parsing
    int16u main_data_end;
    if (protection_bit)
        Skip_B2(                                                "crc_check");
    BS_Begin();
    if (ID==3) //MPEG-1
        Get_S2 (9, main_data_end,                               "main_data_end");
    else
        Get_S2 (8, main_data_end,                               "main_data_end");
    if ((int32u)main_data_end>Reservoir_Max)
        Reservoir_Max=main_data_end;
    Reservoir+=main_data_end;
    if (ID==3) //MPEG-1
    {
        if (mode==3) //Mono
            Skip_S1(5,                                          "private_bits");
        else
            Skip_S1(3,                                          "private_bits");
    }
    else
    {
        if (mode==3) //Mono
            Skip_S1(1,                                          "private_bits");
        else
            Skip_S1(2,                                          "private_bits");
    }
    if (ID==3) //MPEG-1
    {
        Element_Begin("scfsi");
        for(int8u ch=0; ch<Mpega_Channels[mode]; ch++)
            for(int8u scfsi_band=0; scfsi_band<4; scfsi_band++)
            {
                bool scfsi;
                Get_SB (   scfsi,                               "scfsi");
                if (scfsi)
                    Scfsi++;
            }
        Element_End();
    }
    for(int8u gr=0; gr<(ID==3?2:1); gr++)
    {
        Element_Begin("granule");
        for(int8u ch=0; ch<Mpega_Channels[mode]; ch++)
        {
            Element_Begin("channel");
            Skip_S2(12,                                         "part2_3_length");
            Skip_S2(9,                                          "big_values");
            Skip_S1(8,                                          "global_gain");
            if (ID==3) //MPEG-1
                Skip_S1(4,                                      "scalefac_compress");
            else
                Skip_S2(9,                                      "scalefac_compress");
            bool blocksplit_flag;
            Get_SB (   blocksplit_flag,                         "blocksplit_flag");
            if (blocksplit_flag==1)
            {
                int8u block_type;
                bool  mixed_block_flag;
                Get_S1 (2, block_type,                          "block_type");
                Get_SB (   mixed_block_flag,                    "mixed_block_flag");
                for (int8u region=0; region<2; region++)
                    Skip_S1(5,                                  "table_select");
                for (int8u window=0; window<3; window++)
                    Skip_S1(3,                                  "subblock_gain");
                if (block_type == 2)
                {
                    if (mixed_block_flag==1)
                    {
                        Param_Info("Mixed");
                        Block_Count[2]++; //Mixed
                    }
                    else
                    {
                        Param_Info("Short");
                        Block_Count[1]++; //Short
                    }
                }
                else
                {
                    Param_Info("Long");
                    Block_Count[0]++; //Long
                }
            }
            else
            {
                for (int8u region=0; region<3; region++)
                    Skip_S1(5,                                  "table_select");
                Skip_S1(4,                                      "region0_count");
                Skip_S1(3,                                      "region1_count");
                Param_Info("Long");
                Block_Count[0]++; //Long
            }
            if (ID==3) //MPEG-1
                Skip_SB(                                        "preflag");
            bool scalefac;
            Get_SB (   scalefac,                                "scalefac_scale");
            if (scalefac)
                Scalefac++;
            Skip_SB(                                            "count1table_select");
            Element_End();
        } //channels
        Element_End();
    } //granules
    BS_End();
    Skip_XX(Element_Size-Element_Offset,                        "data");

    FILLING_BEGIN();
        //Filling
        LastSync_Offset=File_Offset+Buffer_Offset+Element_Size;
        if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
            Data_Parse_Fill();

        //Detect Id3v1 tags inside a frame
        if (File_Offset+Buffer_Offset+(size_t)Element_Size>File_Size-File_EndTagSize)
            File__Analyze::Data_GoTo(File_Size-File_EndTagSize, "Tags inside a frame, parsing the tags");
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpega::Data_Parse_Fill()
{
    //Testing errors
    if (sampling_frequency_Count.size()>1 || mode_Count.size()>1)
    {
        //Finished();
        //return; //More than one sampling_frequency of mode, not normal
    }

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "MPEG Audio");
    Stream_Prepare(Stream_Audio);
    Fill(Stream_Audio, 0, Audio_Format, "MPEG Audio");
    Fill(Stream_Audio, 0, Audio_Format_Version, Mpega_Format_Profile_Version[ID]);
    Fill(Stream_Audio, 0, Audio_Format_Profile, Mpega_Format_Profile_Layer[layer]);
    Fill(Stream_Audio, 0, Audio_Format_Settings, Mpega_Codec_Profile[mode]);
    Fill(Stream_Audio, 0, Audio_Codec, Ztring(Mpega_Version[ID])+Ztring(Mpega_Layer[layer]));
    Fill(Stream_Audio, 0, Audio_Codec_String, Ztring(Mpega_Version_String[ID])+Ztring(Mpega_Layer_String[layer]));
    Fill(Stream_Audio, 0, Audio_SamplingRate, Mpega_SamplingRate[ID][sampling_frequency]);
    Fill(Stream_Audio, 0, Audio_Channel_s_, Mpega_Channels[mode]);
    Fill(Stream_Audio, 0, Audio_Codec_Profile, Mpega_Codec_Profile[mode]);
    Fill(Stream_Audio, 0, Audio_Resolution, 16);

    //Jumping
    File__Tags_Helper::Data_GoTo(File_Size-(File_Size>=16*1024?16*1024:0), "MPEG-A");
    LastSync_Offset=(int64u)-1;
}

//---------------------------------------------------------------------------
bool File_Mpega::Header_Xing()
{
    int32u Xing_Header_Offset;
    if (ID==3) //MPEG-1
        if (mode==3) //Mono
            Xing_Header_Offset=21-4;
        else
            Xing_Header_Offset=36-4;
    else //MPEG-2 or 2.5
        if (mode==3) //Mono
            Xing_Header_Offset=13-4;
        else
            Xing_Header_Offset=21-4;
    if (Buffer_Offset+Xing_Header_Offset+128<Buffer_Size)
    {
        const int8u* Xing_Header=Buffer+Buffer_Offset+Xing_Header_Offset;
        if (CC4(Xing_Header)==CC4("Xing") || CC4(Xing_Header)==CC4("Info"))
        {
            //This is a "tag"
            File_BeginTagSize+=4+Element_Size;
            Element_Info("Tag (Xing)");

            //Parsing
            Element_Begin("Xing header");
            Skip_XX(Xing_Header_Offset,                         "Junk");
            int32u Flags;
            bool FrameCount, FileSize, TOC, Scale, Lame;
            Skip_C4(                                            "Xing");
            Get_B4 (Flags,                                      "Flags");
                Get_Flags(Flags, 0, FrameCount,                 "FrameCount");
                Get_Flags(Flags, 1, FileSize,                   "FileSize");
                Get_Flags(Flags, 2, TOC,                        "TOC");
                Get_Flags(Flags, 3, Scale,                      "Scale");
                Get_Flags(Flags, 4, Lame,                       "Lame");
            int32u Xing_Header_Size=8
                                   +(FrameCount?  4:0)    //FrameCount
                                   +(FileSize?    4:0)    //FileSize
                                   +(TOC?       100:0)    //TOC
                                   +(Scale?       4:0)    //Scale
                                   +(Lame?      348:0);   //Lame
            Element_End();
            //Element size
            if (Xing_Header_Size>Element_Size-Xing_Header_Offset)
                return false; //Error tag size

            //Parsing
            if (FrameCount)
                Get_B4 (VBR_Frames,                             "FrameCount"); //FrameCount exclude this frame
            if (FileSize)
            {
                int32u VBR_FileSize_Temp;
                Get_B4 (VBR_FileSize_Temp,                      "FileSize");
                if (VBR_FileSize_Temp>4+Element_Size)
                   VBR_FileSize=VBR_FileSize_Temp-4-Element_Size; //FileSize include the Xing element
            }
            if (TOC)
                Skip_XX(100,                                    "TOC");
            if (Scale)
                Get_B4 (Xing_Scale,                             "Scale");
            Ztring Lib;
            Peek_Local(4, Lib);
            if (Lame || Lib==_T("LAME") || Lib==_T("GOGO"))
                Header_Encoders_Lame();

            if (CC4(Xing_Header)==CC4("Info"))
                VBR_Frames=0; //This is not a VBR file

            //Clearing Error detection
            sampling_frequency_Count.clear();
            mode_Count.clear();

            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
bool File_Mpega::Header_VBRI()
{
    const size_t Fraunhofer_Header_Offset=36-4;
    if (Buffer_Offset+Fraunhofer_Header_Offset+32<Buffer_Size)
    {
        const int8u* Fraunhofer_Header=Buffer+Buffer_Offset+Fraunhofer_Header_Offset;
        if (CC4(Fraunhofer_Header)==CC4("VBRI") && CC2(Fraunhofer_Header+4)==0x0001) //VBRI v1 only
        {
            //This is a "tag"
            File_BeginTagSize+=4+Element_Size;

            Element_Name("Tag (VBRI)");

            //Parsing
            int32u VBR_FileSize_Temp;
            Skip_XX(Fraunhofer_Header_Offset,                   "Junk");
            Skip_C4(                                            "VBRI");
            Skip_B2(                                            "Version");
            Skip_B4(                                            "Unknown");
            Get_B4 (VBR_FileSize_Temp,                          "FileSize");
            Get_B4 (VBR_Frames,                                 "Frames");
            VBR_FileSize=VBR_FileSize_Temp;

            //Clearing Error detection
            sampling_frequency_Count.clear();
            mode_Count.clear();

            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
bool File_Mpega::Header_Encoders()
{
    std::string BufferS((const char*)(Buffer+Buffer_Offset), (size_t)Element_Size);
    size_t Buffer_Pos;

    //Lame
    Buffer_Pos=BufferS.find("LAME");
    if (Buffer_Pos!=std::string::npos && Buffer_Pos<=Element_Size-8)
    {
        Element_Info("With tag (Lame)");
        Element_Offset=Buffer_Pos;
        if (Element_Offset+20<=Element_Size)
            Get_Local(20, Encoded_Library,                      "Encoded_Library");
        else
            Get_Local( 8, Encoded_Library,                      "Encoded_Library");
        Encoded_Library.Trim(_T('A'));
        Encoded_Library.Trim(_T('U'));
        Encoded_Library.Trim(0xAA);
        Element_Offset=0; //Reseting it
        return true;
    }

    //RCA
    Buffer_Pos=BufferS.find("RCA mp3PRO Encoder");
    if (Buffer_Pos!=std::string::npos && Buffer_Pos<Element_Size-23)
    {
        Element_Info("With tag (RCA)");
        Encoded_Library="RCA ";
        Encoded_Library+=Ztring((const char*)(Buffer+Buffer_Offset+18), 5);
        return true;
    }

    //Thomson
    Buffer_Pos=BufferS.find("THOMSON mp3PRO Encoder");
    if (Buffer_Pos!=std::string::npos && Buffer_Pos<Element_Size-29)
    {
        Element_Info("With tag (Thomson)");
        Encoded_Library="Thomson ";
        Encoded_Library+=Ztring((const char*)(Buffer+Buffer_Offset+22), 6);
        return true;
    }

    //Gogo (old)
    Buffer_Pos=BufferS.find("MPGE");
    if (Buffer_Pos!=std::string::npos)
    {
        Element_Info("With tag (Gogo)");
        Encoded_Library="Gogo <3.0";
        return true;
    }

    //Gogo (new)
    Buffer_Pos=BufferS.find("GOGO");
    if (Buffer_Pos!=std::string::npos)
    {
        Element_Info("With tag (Gogo)");
        Encoded_Library="Gogo >=3.0";
        return true;
    }

    return false;
}

void File_Mpega::Header_Encoders_Lame()
{
    Peek_Local(8, Encoded_Library);
    if (Encoded_Library>=_T("LAME3.90") && Element_IsNotFinished())
    {
        int8u Flags, BitRate;
        Get_Local(9, Encoded_Library,                           "Encoded_Library");
        Param_Info(Ztring(_T("V"))+Ztring::ToZtring(Xing_Scale&0x0F));
        Param_Info(Ztring(_T("q"))+Ztring::ToZtring((Xing_Scale>>8)&0x0F));
        Get_B1 (Flags,                                          "Flags");
        if ((Flags&0xF0)<=0x20) //Rev. 0 or 1, http://gabriel.mp3-tech.org/mp3infotag.html and Rev. 2 was seen.
        {
            Param_Info(Lame_Method[Flags&0x0F]);
            BitRate_Mode=Lame_BitRate_Mode[Flags&0x0F];
            Encoded_Library_Settings.From_Local(Lame_Method[Flags&0x0F]);
            if ((Flags&0x0F)==1 || (Flags&0x0F)==8) //2 possible values for CBR
                VBR_Frames=0;
        }
        Info_B1(lowpass,                                        "Lowpass filter value"); Param_Info(lowpass*100, " Hz");
        Skip_B4(                                                "Peak signal amplitude");
        Skip_B2(                                                "Radio Replay Gain");
        Skip_B2(                                                "Audiophile Replay Gain");
        Get_B1 (Flags,                                          "Encoding Flags"); Param_Info(Ztring(_T("ATH Type="))+Ztring::ToZtring(Flags&0x0F));
            Skip_Flags(Flags, 4,                                "nspsytune");
            Skip_Flags(Flags, 5,                                "nssafejoint");
            Skip_Flags(Flags, 6,                                "nogap (after)");
            Skip_Flags(Flags, 7,                                "nogap (before)");
        Get_B1 (BitRate,                                        "BitRate");
        if (BitRate!=0 && BitRate!=0xFF)
        {
            switch (Flags&0x0F)
            {
                case  2 :
                case  9 : //ABR
                    BitRate_Nominal.From_Number(BitRate*1000); break;
                default :
                    BitRate_Minimum.From_Number(BitRate*1000); break;
            }
        }
    }
    else
        Get_Local(20, Encoded_Library,                          "Encoded_Library");
}

void File_Mpega::Encoded_Library_Guess()
{
    return; //TODO: Not yet enough precise

    if (Block_Count[1]==0) //No short blocks
    {
        if (mode==2) //Dual Mono
        {
            if (Scfsi>0) //scfsi used
                {}
            else //no scfsi
            {
                if (Scalefac>0) //scalefacors used
                    {}
                else //scalefacors not used
                    Encoded_Library="Shine";
            }
        }
        else //Other than dual mono
        {
            if (Extension_Count[1]>0 || Extension_Count[3]>0) //Intensity Stereo
                Encoded_Library="Xing (very old)";
            else //No Intensity Stereo
            {
                if (Scfsi>0) //Scfsi used
                    Encoded_Library="Xing (new)";
                else //Scsfi not used
                {
                    if (Channels_Count[2]>0) //Joint Stereo
                    {
                        if (Channels_Count[0]>0) //also includes no Joint Stereo frames
                        {
                            if (padding_bit) //Padding
                            {
                                if (original_home)
                                    Encoded_Library="FhG (l3enc)";
                                else
                                    Encoded_Library="FhG (fastenc or mp3enc)";
                            }
                            else //No padding
                                Encoded_Library="FhG (ACM or producer pro)";
                        }
                        else //No stereo frames: joint stereo was forced
                        {
                            if (padding_bit && !original_home && !copyright)
                                Encoded_Library="QDesign (fast mode)";
                        }
                    }
                    else
                    {
                        if (Channels_Count[0]>0 && Scalefac==0 && !original_home) //Stereo
                            Encoded_Library="Plugger";
                        else
                            Encoded_Library="Xing (old)";
                    }
                }
            }
        }
    }
    else //Short blocks
    {
        if (Scfsi)  //scfsi used
        {
            if (Scalefac>0) //Scalefactor used
                Encoded_Library="Gogo (after 3.0)"; //Could be lame, but with a label, detected elsewhere before
            else
                Encoded_Library="Lame (old) or m3e";
        }
        else //Scfsi not used
        {
            if (Scalefac>0) //Scalefactor used
            {
                if (padding_bit)
                {
                    if (original_home)
                    {
                       /*
                            //10 last bytes
                            int sum = get_final_sum(data);
                            if (sum==0)
                                return guess = _T("FhG (fastenc, low quality mode)");
                            else if (sum==10 * 0xFF)
                                return guess = _T("FhG (l3enc)");
                            else if (sum==5 * 0x20)
                                return guess = _T("FhG (fastenc, medium or high quality mode)");
                            else
                                return guess = _T("FhG (l3enc or fastenc)");
                        */
                    }
                    else
                    {
                        if (Channels_Count[1]>0 && Extension_Count[1]>0)        //Joint Stereo and some Intensity Stereo
                            Encoded_Library="Thomson mp3PRO Encoder";
                        else
                            Encoded_Library="FhG (fastenc or mp3enc)";
                    }
                }
                else //No padding
                {
                    if (BitRate_Mode.find(_T("VBR"))==0) //VBR
                        Encoded_Library="FhG (fastenc)";
                    else
                        Encoded_Library="FhG (ACM or producer pro)";
                }
            }
            else //scalefactors not used
            {
                if (Channels_Count[1]>0) //Joint Stereo
                {
                    if (padding_bit && !original_home && !copyright)
                        Encoded_Library="QDesign";
                }
                else //Joint Stereo not used
                {
                    if (BitRate_Mode.find(_T("VBR"))==0) //VBR
                        Encoded_Library="Lame (old)";
                    else //CBR
                    {

                        if (mode==2) //Dual Mono
                        {
                            if (padding_bit)
                                Encoded_Library="Blade";
                            else
                                Encoded_Library="dist10 encoder or other encoder";
                        }
                        else //Stereo or Mono
                        {
                            /*
                            if (data.av_reservoir < 40 && !data.vbr) //ISO based encoders are unable to properly use bit reservoir... average reservoir usage is about 10
                            {
                                if (data.padding)
                                    return guess = _T("Blade");
                                else
                                    return guess = _T("dist10 encoder or other encoder");
                            }
                            else
                                return guess = _T("Gogo (before 3.0)");
                            */
                        }
                    }
                }
            }
        }
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpega::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+4<=Buffer_Size)
    {
        while (Buffer_Offset+4<=Buffer_Size)
        {
            if ((CC2(Buffer+Buffer_Offset+0)&0xFFE0)==0xFFE0 && (CC1(Buffer+Buffer_Offset+2)&0xF0)!=0xF0 && (CC1(Buffer+Buffer_Offset+2)&0x0C)!=0x0C)
                break; //while()

            //Tags
            if (File_Offset+Buffer_Offset+File_EndTagSize==File_Size && !File__Tags_Helper::Header_Begin())
                return false;

            Buffer_Offset++;
        }

        if (Buffer_Offset+4<=Buffer_Size)//Testing if size is coherant
        {
            //Retrieving some info
            int8u ID                =(CC1(Buffer+Buffer_Offset+1)>>3)&0x03;
            int8u layer             =(CC1(Buffer+Buffer_Offset+1)>>1)&0x03;
            int8u bitrate_index     =(CC1(Buffer+Buffer_Offset+2)>>4)&0x0F;
            int8u sampling_frequency=(CC1(Buffer+Buffer_Offset+2)>>2)&0x03;
            int8u padding_bit       =(CC1(Buffer+Buffer_Offset+2)>>1)&0x01;
            //Coherancy
            if (Mpega_SamplingRate[ID][sampling_frequency]==0 || Mpega_Coefficient[ID][layer]==0 || Mpega_BitRate[ID][layer][bitrate_index]==0 || Mpega_SlotSize[layer]==0)
                Buffer_Offset++; //False start
            else
            {
                //Testing next start, to be sure
                size_t Size=(Mpega_Coefficient[ID][layer]*Mpega_BitRate[ID][layer][bitrate_index]*1000/Mpega_SamplingRate[ID][sampling_frequency]+(padding_bit?1:0))*Mpega_SlotSize[layer];
                if (IsSub && Buffer_Offset+Size==Buffer_Size)
                    break;
                if (File_Offset+Buffer_Offset+Size!=File_Size-File_EndTagSize)
                {
                    if (Buffer_Offset+Size+4>Buffer_Size)
                    {
                        //Delay
                        if (Frame_Count==0)
                            Delay+=Buffer_Offset;
                        return false; //Need more data
                    }

                    //Testing
                    if ((CC2(Buffer+Buffer_Offset+Size)&0xFFE0)!=0xFFE0 || (CC1(Buffer+Buffer_Offset+Size+2)&0xF0)==0xF0 || (CC1(Buffer+Buffer_Offset+Size+2)&0x0C)==0x0C)
                        Buffer_Offset++;
                    else
                    {
                        //Retrieving some info
                        int8u ID                =(CC1(Buffer+Buffer_Offset+Size+1)>>3)&0x03;
                        int8u layer             =(CC1(Buffer+Buffer_Offset+Size+1)>>1)&0x03;
                        int8u bitrate_index     =(CC1(Buffer+Buffer_Offset+Size+2)>>4)&0x0F;
                        int8u sampling_frequency=(CC1(Buffer+Buffer_Offset+Size+2)>>2)&0x03;
                        int8u padding_bit       =(CC1(Buffer+Buffer_Offset+Size+2)>>1)&0x01;
                        //Coherancy
                        if (Mpega_SamplingRate[ID][sampling_frequency]==0 || Mpega_Coefficient[ID][layer]==0 || Mpega_BitRate[ID][layer][bitrate_index]==0 || Mpega_SlotSize[layer]==0)
                            Buffer_Offset++; //False start
                        else
                        {
                            //Testing next start, to be sure
                            size_t Size2=(Mpega_Coefficient[ID][layer]*Mpega_BitRate[ID][layer][bitrate_index]*1000/Mpega_SamplingRate[ID][sampling_frequency]+(padding_bit?1:0))*Mpega_SlotSize[layer];
                            if (FrameIsAlwaysComplete && Buffer_Offset+Size+Size2==Buffer_Size)
                                break;
                            if (File_Offset+Buffer_Offset+Size+Size2!=File_Size-File_EndTagSize)
                            {
                                if (Buffer_Offset+Size+Size2+4>Buffer_Size)
                                {
                                    //Delay
                                    if (Frame_Count==0)
                                        Delay+=Buffer_Offset;
                                    return false; //Need more data
                                }

                                //Testing
                                if ((CC2(Buffer+Buffer_Offset+Size+Size2)&0xFFE0)!=0xFFE0 || (CC1(Buffer+Buffer_Offset+Size+Size2+2)&0xF0)==0xF0 || (CC1(Buffer+Buffer_Offset+Size+Size2+2)&0x0C)==0x0C)
                                    Buffer_Offset++;
                                else
                                    break; //while()
                            }
                            else
                                break; //while()
                        }
                    }
                }
                else
                    break; //while()
            }
        }
    }
    if (Buffer_Offset+4>Buffer_Size)
    {
        //Parsing last bytes
        if (Buffer_Offset+3==Buffer_Size)
        {
            if ((CC2(Buffer+Buffer_Offset)&0xFFE0)!=0xFFE0)
            {
                Buffer_Offset++;
                if ((CC2(Buffer+Buffer_Offset)&0xFFE0)!=0xFFE0)
                {
                    Buffer_Offset++;
                    if (CC1(Buffer+Buffer_Offset)!=0xFF)
                        Buffer_Offset++;
                }
            }
        }

        //Delay
        if (Frame_Count==0)
            Delay+=Buffer_Offset;

        return false;
    }

    //Delay
    if (Frame_Count==0)
        Delay+=Buffer_Offset;

    //Synched is OK
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_Mpega::Detect_NonMPEGA ()
{
    //File_Size
    if (File_Size<4)
        return false; //We can't do detection

    //Element_Size
    if (Buffer_Size<4)
        return true; //Must wait for more data

    //Detect WAV files, the parser can't detect it easily, there is only 70 bytes of begining for sayint WAV
    if (CC4(Buffer)==CC4("RIFF"))
    {
        Finished();
        return true;
    }

    //Detect SWF files (one case with no possibility to detect false-positive...
    if (CC3(Buffer)==CC3("FWS"))
    {
        Finished();
        return true;
    }

    //Detect FLV files (one case with no possibility to detect false-positive...
    if (CC3(Buffer)==CC3("FLV"))
    {
        Finished();
        return true;
    }

    //Seems OK
    return false;
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES

