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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_DTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Dts.h"
#include <ZenLib/Utils.h>
#include <ZenLib/BitStream.h>
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
    "Front: L C C R, Surround: L R",
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
    "4/2",
    "3/2",
    "2.2/2",
    "3.2/2",
    "2.2/4",
    "3.2/3",
};

const char*  DTS_ExtensionAudioDescriptor[]=
{
    "Channel Extension",
    "Reserved",
    "Frequency Extension",
    "Channel Extension + Frequency Extension",
    "",
    "",
    "",
    "",
    "",
    "",
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
    Word=true;
    BigEndian=true;

    //Temp - Unknown
    DTS_HD_Unknown_Size=0;
    SyncCode=0x7FFE;
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

    //DTS-HD: Saving offset
    size_t DTS_HD_Offset=Buffer_Offset;

    //Quick test of synchro
    if (Synched && CC4(Buffer+Buffer_Offset)!=SyncCode)
    {
        Info("DTS, Synchronisation lost"); //Should not //TODO: find a better way to detect DTS
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //DTS-HD: Saving difference
    if (Buffer_Offset-DTS_HD_Offset && Frame_Count>0)
        DTS_HD_Unknown_Size=Buffer_Offset-DTS_HD_Offset;

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_Dts::Header_Parse()
{
    //Parsing
    int16u Primary_Frame_Byte_Size_minus_1;
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
    Skip_SB(                                                    "ES");
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
    Header_Fill_Size(Primary_Frame_Byte_Size_minus_1+DTS_HD_Unknown_Size); //TODO: change this, for DTS-HD
    Header_Fill_Code(0, "Frame");
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
    Frame_Count++;

    //Name
    Element_Info(Ztring::ToZtring(Frame_Count));

    //Parsing
    Skip_XX(Element_Size,                                       "Data");

    //Filling
    if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
        Data_Parse_Fill();
}

//---------------------------------------------------------------------------
void File_Dts::Data_Parse_Fill()
{
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "DTS");
    if (DTS_HD_Unknown_Size) //DTS-HD, TODO : Find a better way
        Fill(Stream_General, 0, General_Format_Profile, "HD");
    Stream_Prepare(Stream_Audio);
    Fill(Stream_Audio, 0, Audio_Format, "DTS");
    if (DTS_HD_Unknown_Size) //DTS-HD, TODO : Find a better way
        Fill(Stream_Audio, 0, Audio_Format_Profile, "HD");
    Fill(Stream_Audio, 0, Audio_Codec, DTS_HD_Unknown_Size?"DTS-HD":"DTS");
    Fill(Stream_Audio, 0, Audio_BitRate_Mode, DTS_HD_Unknown_Size?"VBR":"CBR"); //DTS-HD, TODO : Find a better way

    if (ExtendedCoding && (ExtensionAudioDescriptor==2 || ExtensionAudioDescriptor==3))
        Fill(Stream_Audio, 0, Audio_SamplingRate, 96000);
    else
        Fill(Stream_Audio, 0, Audio_SamplingRate, DTS_SamplingRate[sample_frequency]);
    if (DTS_HD_Unknown_Size==0 && bit_rate<29)
        Fill(Stream_Audio, 0, Audio_BitRate, DTS_BitRate[bit_rate]);
    else if (bit_rate==29)
        Fill(Stream_Audio, 0, Audio_BitRate, "Open");
    else if (bit_rate==30)
        Fill(Stream_Audio, 0, Audio_BitRate, "Variable");
    else if (bit_rate==31)
        Fill(Stream_Audio, 0, Audio_BitRate, "LossLess");
    if (ExtendedCoding && (ExtensionAudioDescriptor==0 || ExtensionAudioDescriptor==3))
    {
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

    if (File_Offset+Buffer_Size<File_Size)
    {
        Info("DTS detected");
        Finnished();
    }
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

