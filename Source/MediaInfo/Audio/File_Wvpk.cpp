// File_Wvpk - Info for WavPack files
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
// Source : http://www.wavpack.com/file_format.txt
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
#if defined(MEDIAINFO_WVPK_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Wvpk.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const int16u Wvpk_Resolution[]=
{
     8,
    16,
    24,
    32,
};

//---------------------------------------------------------------------------
const int32u Wvpk_SamplingRate[]=
{
      6000,
      8000,
      9600,
     11025,
     12000,
     16000,
     22050,
     24000,
     32000,
     44100,
     48000,
     64000,
     88200,
     96000,
    192000,
         0,
};

//---------------------------------------------------------------------------
const char* Wvpk_id(int8u ID)
{
    switch (ID)
    {
        case 0x00 : return "could be used to pad WavPack blocks";
        case 0x02 : return "decorrelation terms & deltas";
        case 0x03 : return "initial decorrelation weights";
        case 0x04 : return "decorrelation sample history";
        case 0x05 : return "initial entropy variables";
        case 0x06 : return "entropy variables specific to hybrid mode";
        case 0x07 : return "info needed for hybrid lossless (wvc) mode";
        case 0x08 : return "specific info for floating point decode";
        case 0x09 : return "specific info for decoding integers > 24 bits";
        case 0x0A : return "normal compressed audio bitstream (wv file)";
        case 0x0B : return "correction file bitstream (wvc file)";
        case 0x0C : return "special extended bitstream for floating point data or integers > 24 bit";
        case 0x0D : return "contains channel count and channel_mask";
        case 0x21 : return "RIFF header for .wav files (before audio)";
        case 0x22 : return "RIFF trailer for .wav files (after audio)";
        case 0x25 : return "some encoding details for info purposes";
        case 0x26 : return "16-byte MD5 sum of raw audio data";
        case 0x27 : return "non-standard sampling rate info";
        default:    return "";
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Wvpk::File_Wvpk()
:File__Analyze(), File__Tags_Helper()
{
    //File__Tags_Helper
    Base=this;

    //Configuration
    Frame_Count_Valid=2;

    //Temp - Global
    Frame_Count=0;

    //Temp - Technical info
    total_samples_FirstFrame=0xFFFFFFFF;
    block_index_FirstFrame=0;
    block_index_LastFrame=0;
    SamplingRate=(int8u)-1;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wvpk::Read_Buffer_Continue()
{
    //Tags
    if (!File__Tags_Helper::Read_Buffer_Continue())
        return;
}

//---------------------------------------------------------------------------
void File_Wvpk::Read_Buffer_Finalize()
{
    //Duration
    if (SamplingRate<15)
    {
        int64u Duration=(((int64u)(block_index_LastFrame+block_samples_LastFrame-block_index_FirstFrame))*1000/Wvpk_SamplingRate[SamplingRate]); //Don't forget the last frame with block_samples...
        int64u CompressedSize=File_Size-File_BeginTagSize-File_EndTagSize;
        int64u UncompressedSize=Duration*(mono?1:2)*Wvpk_Resolution[(resolution1?1:0)*2+(resolution0?1:0)]*Wvpk_SamplingRate[SamplingRate]/8/1000;
        float32 CompressionRatio=((float32)UncompressedSize)/CompressedSize;
        Fill(Stream_Audio, 0, Audio_StreamSize, CompressedSize, 3, true);
        Fill(Stream_Audio, 0, Audio_Duration, Duration, 10, true);
        Fill(Stream_Audio, 0, Audio_CompressionRatio, CompressionRatio, 3, true);
        Fill(Stream_Audio, 0, Audio_BitRate_Mode, "VBR");
    }

    //Tags
    File__Tags_Helper::Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Wvpk::Header_Begin()
{
    if (!File__Tags_Helper::Header_Begin())
        return false;

    //Must have enough buffer for having header
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && CC4(Buffer+Buffer_Offset)!=CC4("wvpk"))
    {
        Trusted_IsNot("WavPack, Synchronisation lost");
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Wvpk::Header_Parse()
{
    //Parsing
    int32u ckSize;
    Skip_C4(                                                    "ckID");
    Get_L4 (ckSize,                                             "ckSize");

    //Filling
    Header_Fill_Size(8+ckSize);
    Header_Fill_Code(0, "Block");
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wvpk::Data_Parse()
{
    //Counting
    Frame_Count++;

    //Parsing
    int16u version;
    Element_Begin("Header", 32);
    Get_L2 (version,                                            "version");
    if (version/0x100==0x4)
    {
        int32u total_samples, block_index, block_samples, flags;
        Skip_L1(                                                "track_no");
        Skip_L1(                                                "index_no");
        Get_L4 (total_samples,                                  "total_samples");
        Get_L4 (block_index,                                    "block_index");
        Get_L4 (block_samples,                                  "block_samples");
        if (block_samples!=0) //empty frames have other values empty
        {
            if (block_index==0) //Only the frame with block_index==0
                total_samples_FirstFrame=total_samples; //Note: total_samples is not trustable for a cutted file
            if (Frame_Count==1)
                block_index_FirstFrame=block_index; //Save the block_index of the first block
            block_index_LastFrame=block_index;
            block_samples_LastFrame=block_samples;
            Get_L4 (flags,                                      "flags");
                Get_Flags (flags,  0, resolution0,              "resolution0");
                Get_Flags (flags,  1, resolution1,              "resolution1");
                Get_Flags (flags,  2, mono,                     "mono");
                Get_Flags (flags,  3, hybrid,                   "hybrid");
                Get_Flags (flags,  4, joint_stereo,             "joint stereo");
                Get_Flags (flags,  5, cross_channel_decorrelation, "cross-channel decorrelation");
                Skip_Flags(flags,  6,                           "hybrid noise shaping");
                Skip_Flags(flags,  7,                           "floating point data");
                Skip_Flags(flags,  8,                           "extended size integers");
                Skip_Flags(flags,  9,                           "hybrid mode parameters control bitrate");
                Skip_Flags(flags, 10,                           "hybrid noise balanced between channels");
                Skip_Flags(flags, 11,                           "initial block in sequence");
                Skip_Flags(flags, 12,                           "final block in sequence");
                Skip_Flags(flags, 13,                           "amount of data left-shift after decode");
                Skip_Flags(flags, 14,                           "amount of data left-shift after decode");
                Skip_Flags(flags, 15,                           "amount of data left-shift after decode");
                Skip_Flags(flags, 16,                           "amount of data left-shift after decode");
                Skip_Flags(flags, 17,                           "amount of data left-shift after decode");
                Skip_Flags(flags, 18,                           "maximum magnitude of decoded data");
                Skip_Flags(flags, 19,                           "maximum magnitude of decoded data");
                Skip_Flags(flags, 20,                           "maximum magnitude of decoded data");
                Skip_Flags(flags, 21,                           "maximum magnitude of decoded data");
                Skip_Flags(flags, 22,                           "maximum magnitude of decoded data");
                Skip_Flags(flags, 23,                           "sampling rate");
                Skip_Flags(flags, 24,                           "sampling rate");
                Skip_Flags(flags, 25,                           "sampling rate");
                Skip_Flags(flags, 26,                           "sampling rate"); SamplingRate=(int8u)(((flags>>23)&0xF)); Element_Info(Wvpk_SamplingRate[SamplingRate]);
                Skip_Flags(flags, 27,                           "reserved");
                Skip_Flags(flags, 28,                           "reserved");
                Skip_Flags(flags, 29,                           "use IIR for negative hybrid noise shaping");
                Skip_Flags(flags, 30,                           "false stereo");
                Skip_Flags(flags, 31,                           "reserved");
        }
        else
        {
            Skip_L4(                                            "flags_empty");

            //Counting
            Frame_Count--; //This is not a real frame
        }
        Skip_L4(                                                "crc");
        Element_End();

        //Sub-block
        int32u word_size;
        int8u id;
        while (Element_Offset<Element_Size)
        {
            Element_Begin();
            Get_L1 (id,                                         "id"); Element_Info(Wvpk_id(id&0x2F));
            if (id&0x80)
            {
                Get_L3 (word_size,                              "word_size");
            }
            else
            {
                int8u word_size1;
                Get_L1 (word_size1,                             "word_size");
                word_size=word_size1;
            }
            Element_Name(Wvpk_id(id&0x2F));
            Skip_XX(word_size*2,                                "data");
            Element_End();
        }
    }

    //Filling
    if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
        Data_Parse_Fill();
}

//---------------------------------------------------------------------------
void File_Wvpk::Data_Parse_Fill()
{
    //Filling
    if (Count_Get(Stream_General)==0)
    {
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "WavPack");
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, Audio_Format, "WavPack");
        Fill(Stream_Audio, 0, Audio_Codec, "Wavpack");
    }

    Fill(Stream_Audio, 0, Audio_Resolution, Wvpk_Resolution[(resolution1?1:0)*2+(resolution0?1:0)]);
    Fill(Stream_Audio, StreamPos_Last, Audio_Channel_s_, mono?1:2);
    if (SamplingRate<15)
    {
        Fill(Stream_Audio, StreamPos_Last, Audio_SamplingRate, Wvpk_SamplingRate[SamplingRate]);
        if (total_samples_FirstFrame!=0xFFFFFFFF) //--> this is a valid value
            Fill(Stream_Audio, 0, Audio_Duration, ((int64u)total_samples_FirstFrame)*1000/Wvpk_SamplingRate[SamplingRate]);
    }
    Fill(Stream_Audio, 0, Audio_Format_Settings, hybrid?"Hybrid lossy":"Lossless");
    Fill(Stream_Audio, 0, Audio_Codec_Settings, hybrid?"hybrid lossy":"lossless");

    //Going to end of file
    if (File_Size>512*1024)
        File__Tags_Helper::Data_GoTo(File_Size-512*1024, "WavPack");
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Wvpk::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+8<=Buffer_Size)
    {
        while (Buffer_Offset+8<=Buffer_Size
            && CC4(Buffer+Buffer_Offset)!=CC4("wvpk"))
        {
            //Tags
            if (File_Offset+Buffer_Offset+File_EndTagSize==File_Size && !File__Tags_Helper::Header_Begin())
                return false;

            Buffer_Offset++;
        }

        if (Buffer_Offset+8<=Buffer_Size)//Testing if size is coherant
        {
            //Testing next start, to be sure
            size_t Size=LittleEndian2int32u(Buffer+Buffer_Offset+4)+8;
            if (File_Offset+Buffer_Offset+Size!=File_Size-File_EndTagSize)
            {
                if (Buffer_Offset+Size+8>Buffer_Size)
                    return false; //Need more data

                //Testing
                if (CC4(Buffer+Buffer_Offset+Size)!=CC4("wvpk"))
                    Buffer_Offset++;
                else
                    break; //while()
            }
            else
                Buffer_Offset++;
        }
    }
    if (Buffer_Offset+8>Buffer_Size)
    {
        //Parsing last bytes
        if (Buffer_Offset+7==Buffer_Size)
        {
            if (CC4(Buffer+Buffer_Offset)!=CC4("wvpk"))
            {
                Buffer_Offset++;
                if (CC4(Buffer+Buffer_Offset)!=CC4("wvpk"))
                {
                    Buffer_Offset++;
                    if (CC4(Buffer+Buffer_Offset)!=CC4("wvpk"))
                    {
                        Buffer_Offset++;
                        if (CC4(Buffer+Buffer_Offset)!=CC4("wvpk"))
                        {
                            Buffer_Offset++;
                            if (CC3(Buffer+Buffer_Offset)!=CC3("wvp"))
                            {
                                Buffer_Offset++;
                                if (CC2(Buffer+Buffer_Offset)!=CC2("wv"))
                                {
                                    Buffer_Offset++;
                                    if (CC1(Buffer+Buffer_Offset)!=CC1("w"))
                                        Buffer_Offset++;
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    }

    //Synched is OK
    Synched=true;
    return true;
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_WVPK_YES
