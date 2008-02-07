// File_MpegPs - Info for MPEG files
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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGPS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_MpegPs.h"
#include "MediaInfo/Multiple/File_Mpeg_Psi.h"
#if defined(MEDIAINFO_AVC_YES)
    #include "MediaInfo/Video/File_Avc.h"
#endif
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_MPEGV_YES)
    #include "MediaInfo/Video/File_Mpegv.h"
#endif
#if defined(MEDIAINFO_VC1_YES)
    #include "MediaInfo/Video/File_Vc1.h"
#endif
#if defined(MEDIAINFO_AC3_YES)
    #include "MediaInfo/Audio/File_Ac3.h"
#endif
#if defined(MEDIAINFO_DTS_YES)
    #include "MediaInfo/Audio/File_Dts.h"
#endif
#if defined(MEDIAINFO_MPEGA_YES)
    #include "MediaInfo/Audio/File_Mpega.h"
#endif
#if defined(MEDIAINFO_PCM_YES)
    #include "MediaInfo/Audio/File_Pcm.h"
#endif
#if defined(MEDIAINFO_LATM_YES)
    #include "MediaInfo/Audio/File_Latm.h"
#endif
#if defined(MEDIAINFO_RLE_YES)
    #include "MediaInfo/Image/File_Rle.h"
#endif
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const char* MpegPs_System_Fixed[]=
{
    "CBR",
    "VBR",
};

//---------------------------------------------------------------------------
const char* MpegPs_stream_id(int8u Element_Name)
{
         if (Element_Name>=0xC0
          && Element_Name<=0xDF) return "MPEG Audio";
    else if (Element_Name>=0xE0
          && Element_Name<=0xEF) return "MPEG Video";
    else if (Element_Name==0xB8) return "For all MPEG Audio streams";
    else if (Element_Name==0xB9) return "For all MPEG Video streams";
    else if (Element_Name==0xBD) return "Private 1";
    else if (Element_Name==0xBF) return "Private 2";
    else if (Element_Name==0xFD) return "Private HD";
    else                         return "";
}

//---------------------------------------------------------------------------
const char* MpegPs_Codec(int8u Element_Name)
{
         if (Element_Name>=0xC0
          && Element_Name<=0xDF) return "MPEG-A";
    else if (Element_Name>=0xE0
          && Element_Name<=0xEF) return "MPEG-V";
    else                         return "";
}

//---------------------------------------------------------------------------
int32u MpegPs_Default_stream_type(int8u Element_Name, int8u Mpeg_Version)
{
         if (Element_Name>=0xC0
          && Element_Name<=0xDF) return Mpeg_Version==0x02?0x04:0x03;
    else if (Element_Name>=0xE0
          && Element_Name<=0xEF) return Mpeg_Version==0x02?0x02:0x01;
    else                         return 0x00;
}

//---------------------------------------------------------------------------
const char* MpegPs_trick_mode_control_values[8]=
{
    "Fast forward",
    "Slow motion",
    "Freeze frame",
    "Fast reverse",
    "Slow reverse",
    "Reserved",
    "Reserved",
    "Reserved"
};

//---------------------------------------------------------------------------
extern const char* Mpeg_Psi_stream_type(int32u ID);
extern const char* Mpeg_Psi_stream_Codec(int8u ID);
extern stream_t    Mpeg_Psi_stream_Kind(int32u ID);

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_MpegPs::File_MpegPs()
:File__Analyze()
{
    //In
    FromTS=false;
    stream_type_FromTS=0; //No info
    MPEG_Version=0;

    //private_stream_1 specific
    private_stream_1_IsDvdVideo=false;

    //Count
    video_stream_Count=(int8u)-1;
    audio_stream_Count=(int8u)-1;
    private_stream_1_Count=(int8u)-1;
    private_stream_2_Count=true;

    //From packets
    program_mux_rate=0;

    //Default values
    Stream[0xBA].Searching_Payload=true;

    //Temp
    SizeToAnalyze=2*1024*1024;
    video_stream_Unlimited=false;
    video_stream_Unlimited_start_code=0xFF;
}

//---------------------------------------------------------------------------
File_MpegPs::~File_MpegPs()
{
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Init()
{
    //TS specific
    if (FromTS)
    {
        Stream[0xBD].Searching_Payload=true;            //private_stream_1
        Stream[0xBD].Searching_TimeStamp_Start=true;    //private_stream_1
        Stream[0xBD].Searching_TimeStamp_End=true;      //private_stream_1
        Stream[0xFA].Searching_Payload=true;            //LATM
        Stream[0xFA].Searching_TimeStamp_Start=true;    //LATM
        Stream[0xFA].Searching_TimeStamp_End=true;      //LATM
        Stream[0xFD].Searching_Payload=true;            //audio_HD
        Stream[0xFD].Searching_TimeStamp_Start=true;    //audio_HD
        Stream[0xFD].Searching_TimeStamp_End=true;      //audio_HD
        for (int8u Pos=0xC0; Pos<=0xEF; Pos++)
        {
            Stream[Pos].Searching_Payload=true;         //audio_stream or video_stream
            Stream[Pos].Searching_TimeStamp_Start=true; //audio_stream or video_stream
            Stream[Pos].Searching_TimeStamp_End=true;   //audio_stream or video_stream
        }

        if (Mpeg_Psi_stream_Kind(stream_type_FromTS)==Stream_Video)
            video_stream_Count=1;
        else if (Mpeg_Psi_stream_Kind(stream_type_FromTS)==Stream_Audio)
        {
            if (stream_type_FromTS==0x03 || stream_type_FromTS==0x04) //specified as audio_stream
                audio_stream_Count=1;
            else
                private_stream_1_Count=1;
        }
    }
}

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Continue()
{
    //We can accept a lost of synchronisation loss
    Trusted*=2;

    //Integrity
    if (File_Offset==0 && Detect_NonMPEGPS())
        return;

    //Case of extraction from MPEG-TS files
    if (File_Offset==0 && Buffer_Size>=4 && ((CC4(Buffer)&0xFFFFFFF0)==0x000001E0 || (CC4(Buffer)&0xFFFFFFE0)==0x000001C0 || CC4(Buffer)==0x000001BD || CC4(Buffer)==0x000001FA || CC4(Buffer)==0x000001FD))
    {
        FromTS=true; //We want to anlyze this kind of file
        MPEG_Version=2; //By default, MPEG-TS is version 2
        Stream[CC1(Buffer+3)].Searching_Payload=true; //Activating the stream
    }
}

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Finalize()
{
    //For each stream
    std::map<int64u, ps_stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
        Read_Buffer_Finalize_PerStream(Temp);

    //For each private stream
    Temp=Stream_Private1.begin();
    while (Temp!=Stream_Private1.end())
        Read_Buffer_Finalize_PerStream(Temp);

    //Tags in MPEG Video
    if (Count_Get(Stream_Video)>0)
        Fill(Stream_General, 0, "Encoded_Library", Get(Stream_Video, 0, _T("Encoded_Library")));

    //Fill General
         if (!Video.empty() && Video[0](_T("Codec"))==_T("AVC"))
        Fill(Stream_General, 0, "Format", "MPEG-4PS");
    else if (!Video.empty() && Video[0](_T("Codec"))==_T("MPEG-4V"))
        Fill(Stream_General, 0, "Format", "MPEG-4PS");
    else if (MPEG_Version==2)
        Fill(Stream_General, 0, "Format", "MPEG-2PS");
    else if (MPEG_Version==1)
        Fill(Stream_General, 0, "Format", "MPEG-1PS");
}

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Finalize_PerStream(std::map<int64u, ps_stream>::iterator &Temp)
{
    //By the parser
    StreamKind_Last=Stream_Max;
    if (Temp->second.Parser)
    {
        Temp->second.Parser->Open_Buffer_Finalize();
        Merge(*Temp->second.Parser);
    }
    //By the stream_type
    if (StreamKind_Last==Stream_Max)
    {
        if (Temp->second.stream_type!=0 && Temp->first!=0xBD && Temp->first!=0xBF)
            Stream_Prepare(Mpeg_Psi_stream_Kind(Temp->second.stream_type));
    }
    //By StreamIsRegistred
    if (StreamKind_Last==Stream_Max)
    {
        if (Temp->second.StreamIsRegistred)
        {
            if ((Temp->first>=0xC0 && Temp->first<=0xDF) || (FromTS && Temp->first==0xFD))
                Stream_Prepare(Stream_Audio);
            if ((Temp->first>=0xE0 && Temp->first<=0xEF) || (!FromTS && Temp->first==0xFD))
                Stream_Prepare(Stream_Video);
        }
    }

    //More info
    if (StreamKind_Last!=Stream_Max) //Found by the Parser or stream_type
    {
        //Common
        if (!File_Name.empty())
            Fill("ID", Temp->first, 16);
        if (Get(StreamKind_Last, StreamPos_Last, _T("Codec")).empty() && Temp->second.stream_type!=0)
            Fill("Codec", Mpeg_Psi_stream_Codec(Temp->second.stream_type));

        if (Temp->second.TimeStamp_Start.PTS_Is_Valid && Temp->second.TimeStamp_End.PTS_Is_Valid)
        {
            //TimeStamp
            if (Temp->second.TimeStamp_End.PTS<Temp->second.TimeStamp_Start.PTS)
                Temp->second.TimeStamp_End.PTS+=0x200000000LL; //33 bits, cyclic
            int64u PlayTime=Temp->second.TimeStamp_End.PTS-Temp->second.TimeStamp_Start.PTS;
            if (PlayTime)
                Fill("PlayTime", PlayTime/90, 10, true);
        }
        if (Temp->second.TimeStamp_Start.PTS_Is_Valid)
            Fill("Delay", Temp->second.TimeStamp_Start.PTS/90, 10, true);
    }

    Temp++;
}
//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegPs::Header_Begin()
{
    //Video unlimited specific
    if (video_stream_Unlimited)
        return true; //Already tested, and this can be in a stream

    //Trailing 0xFF
    if (Synched)
    {
        while(Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset]==0xFF)
            Buffer_Offset++;
    }

    //Trailing 0x00
    if (Synched)
    {
        while(Buffer_Offset+4<=Buffer_Size && Buffer[Buffer_Offset]==0x00 && CC3(Buffer+Buffer_Offset)!=0x000001)
            Buffer_Offset++;
    }

    //Must have enough buffer for having header
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && !(CC3(Buffer+Buffer_Offset)==0x000001 && CC1(Buffer+Buffer_Offset+3)>=0xB9))
    {
        Trusted_IsNot("MPEG-PS, Synchronisation lost");
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //Quick search
    if (!Header_Parser_QuickSearch())
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_MpegPs::Header_Parse()
{
    //Video unlimited specific, we didn't wait for the end (because this is... unlimited)
    if (video_stream_Unlimited)
    {
        if (!Header_Parse_Fill_Size())
            //Next PS packet is not found, we will use all the buffer
            Header_Fill_Size(Buffer_Size); //All the buffer is used
        else
            //Next PS packet is found
            video_stream_Unlimited=false;

        //Parsing
        Buffer_Offset_Temp=0;
        Header_Fill_Code(video_stream_Unlimited_start_code, Ztring().From_CC1(video_stream_Unlimited_start_code));
        return;
    }

    //Parsing
    int8u start_code;
    Skip_B3(                                                    "synchro");
    Get_B1 (start_code,                                         "start_code");

    if (start_code!=0xB9 && start_code!=0xBA) //MPEG_program_end or pack_start have no PES
        Header_Parse_PES_packet(start_code);
    else if (!Header_Parse_Fill_Size()) //MPEG_program_end or pack_start specific
    {
        Element_WaitForMoreData();
        return;
    }

    //Filling
    Header_Fill_Code(start_code, Ztring().From_CC1(start_code));
}

//---------------------------------------------------------------------------
bool File_MpegPs::Header_Parse_Fill_Size()
{
    //Look for next Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if Header_Parse_Fill_Size() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset+(video_stream_Unlimited?0:4);
    while (Buffer_Offset_Temp+4<=Buffer_Size
        && !(CC3(Buffer+Buffer_Offset_Temp)==0x000001 && CC1(Buffer+Buffer_Offset_Temp+3)>=0xB9))
        Buffer_Offset_Temp++;

    //Must wait more data?
    if (Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (File_Offset+Buffer_Size==File_Size)
            Buffer_Offset_Temp=Buffer_Size; //We are sure that the next bytes are a start
        else
            return false;
    }

    //OK, we continue
    Header_Fill_Size(Buffer_Offset_Temp-Buffer_Offset);
    Buffer_Offset_Temp=0;
    return true;
}

//---------------------------------------------------------------------------
void File_MpegPs::Header_Parse_PES_packet(int8u start_code)
{
    //Parsing
    int16u PES_packet_length;
    Get_B2 (PES_packet_length,                                  "PES_packet_length");

    //Filling
    Header_Fill_Size(6+PES_packet_length);

    //Parsing
    switch (start_code)
    {
        //Header is only Size
        case 0xBB : //system_header_start
        case 0xBC : //program_stream_map
        case 0xBE : //padding_stream
        case 0xBF : //private_stream_2
        case 0xF0 : //ECM
        case 0xF1 : //EMM
        case 0xF2 : //DSMCC stream
        case 0xF8 : //ITU-T Rec. H .222.1 type E
        case 0xFF : //Program stream directory
            return;

        //Element with PES Header
        default :
            switch (MPEG_Version)
            {
                case 1  : Header_Parse_PES_packet_MPEG1(start_code); break;
                case 2  : Header_Parse_PES_packet_MPEG2(start_code); break;
                default : ; //We don't know what to parse...
            }
    }

    //Video unlimited specific
    if (PES_packet_length==0 && Element_Offset<Element_Size)
        if (!Header_Parse_Fill_Size())
        {
            //Next PS packet is not found, we will use all the buffer
            Header_Fill_Size(Buffer_Size-Buffer_Offset); //All the buffer is used
            video_stream_Unlimited=true;
            video_stream_Unlimited_start_code=start_code;
            Buffer_Offset_Temp=0; //We use the buffer
        }
}

//---------------------------------------------------------------------------
// Packet header data - MPEG-1
void File_MpegPs::Header_Parse_PES_packet_MPEG1(int8u start_code)
{
    int8u stuffing_byte;
    do
    {
        Peek_B1(stuffing_byte);
        if (stuffing_byte==0xFF)
            Skip_B1(                                            "stuffing_byte");
    }
    while(stuffing_byte==0xFF);

    if ((stuffing_byte&0xC0)==0x40)
    {
        BS_Begin();
        Mark_0();
        Mark_1();
        Skip_SB(                                                "STD_buffer_scale");
        Skip_S2(13,                                             "STD_buffer_size");
        BS_End();
        Peek_B1(stuffing_byte);
    }
    if ((stuffing_byte&0xF0)==0x20)
    {
        int16u PTS_29, PTS_14;
        int8u  PTS_32;
        Element_Begin("PTS");
        BS_Begin();
        Mark_0();
        Mark_0();
        Mark_1();
        Mark_0();
        Get_S1 ( 3, PTS_32,                                     "PTS_32");
        Mark_1();
        Get_S2 (15, PTS_29,                                     "PTS_29");
        Mark_1();
        Get_S2 (15, PTS_14,                                     "PTS_14");
        Mark_1();
        BS_End();

        //Filling
        Stream[start_code].TimeStamp_End.PTS_Is_Valid=true;
        Stream[start_code].TimeStamp_End.PTS=(((int64u)PTS_32)<<30)
                                                            | (((int64u)PTS_29)<<15)
                                                            | (((int64u)PTS_14));
        if (Stream[start_code].Searching_TimeStamp_Start)
        {
            Stream[start_code].TimeStamp_Start=Stream[start_code].TimeStamp_End;
            Stream[start_code].Searching_TimeStamp_Start=false;
        }
        Element_Info(Ztring().Duration_From_Milliseconds(Stream[start_code].TimeStamp_End.PTS/90));
        Element_End();
    }
    else if ((stuffing_byte&0xF0)==0x30)
    {
        int16u PTS_29, PTS_14, DTS_29, DTS_14;
        int8u  PTS_32, DTS_32;
        Element_Begin("PTS");
        BS_Begin();
        Mark_0();
        Mark_0();
        Mark_1();
        Mark_1();
        Get_S1 ( 3, PTS_32,                                     "PTS_32");
        Mark_1();
        Get_S2 (15, PTS_29,                                     "PTS_29");
        Mark_1();
        Get_S2 (15, PTS_14,                                     "PTS_14");

        //Filling
        Stream[start_code].TimeStamp_End.PTS_Is_Valid=true;
        Stream[start_code].TimeStamp_End.PTS=(((int64u)PTS_32)<<30)
                                           | (((int64u)PTS_29)<<15)
                                           | (((int64u)PTS_14));
        if (Stream[start_code].Searching_TimeStamp_Start)
        {
            Stream[start_code].TimeStamp_Start=Stream[start_code].TimeStamp_End;
            Stream[start_code].Searching_TimeStamp_Start=false;
        }
        Element_Info(Ztring().Duration_From_Milliseconds(Stream[start_code].TimeStamp_End.PTS/90));
        Element_End();

        Element_Begin("DTS");
        Mark_1();
        Mark_0();
        Mark_0();
        Mark_0();
        Mark_1();
        Get_S1 ( 3, DTS_32,                                     "DTS_32");
        Mark_1();
        Get_S2 (15, DTS_29,                                     "DTS_29");
        Mark_1();
        Get_S2 (15, DTS_14,                                     "DTS_14");
        Mark_1();
        BS_End();

        //Filling
        Stream[start_code].TimeStamp_End.DTS_Is_Valid=true;
        Stream[start_code].TimeStamp_End.DTS=(((int64u)DTS_32)<<30)
                                           | (((int64u)DTS_29)<<15)
                                           | (((int64u)DTS_14));
        Element_Info(Ztring().Duration_From_Milliseconds(Stream[start_code].TimeStamp_End.DTS/90));
        Element_End();
    }
    else
    {
        BS_Begin();
        Mark_0();
        Mark_0();
        Mark_0();
        Mark_0();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        BS_End();
    }
}

//---------------------------------------------------------------------------
// Packet header data - MPEG-2
void File_MpegPs::Header_Parse_PES_packet_MPEG2(int8u start_code)
{
    //Parsing
    BS_Begin();
    int8u PTS_DTS_flags, PES_header_data_length;
    bool ESCR_flag, ES_rate_flag, DSM_trick_mode_flag, additional_copy_info_flag, PES_CRC_flag, PES_extension_flag;
    Mark_1();
    Mark_0();
    Skip_S1(2,                                                  "PES_scrambling_control");
    Skip_SB(                                                    "PES_priority");
    Skip_SB(                                                    "data_alignment_indicator");
    Skip_SB(                                                    "copyright");
    Skip_SB(                                                    "original_or_copy");
    Get_S1 (2, PTS_DTS_flags,                                   "PTS_DTS_flags");
    Get_SB (ESCR_flag,                                          "ESCR_flag");
    Get_SB (ES_rate_flag,                                       "ES_rate_flag");
    Get_SB (DSM_trick_mode_flag,                                "DSM_trick_mode_flag");
    Get_SB (additional_copy_info_flag,                          "additional_copy_info_flag");
    Get_SB (PES_CRC_flag,                                       "PES_CRC_flag");
    Get_SB (PES_extension_flag,                                 "PES_extension_flag");
    BS_End();
    Get_B1 (PES_header_data_length,                             "PES_header_data_length");
    size_t Element_Pos_After_Data=Element_Offset+PES_header_data_length;

    //Options
    if (PTS_DTS_flags==0x2)
    {
        int16u PTS_29, PTS_14;
        int8u  PTS_32;
        Element_Begin("PTS_DTS_flags");
        Element_Begin("PTS");
        BS_Begin();
        Mark_0();
        Mark_0();
        Mark_1();
        Mark_0();
        Get_S1 ( 3, PTS_32,                                     "PTS_32");
        Mark_1();
        Get_S2 (15, PTS_29,                                     "PTS_29");
        Mark_1();
        Get_S2 (15, PTS_14,                                     "PTS_14");
        Mark_1();
        BS_End();

        //Filling
        Stream[start_code].TimeStamp_End.PTS_Is_Valid=true;
        Stream[start_code].TimeStamp_End.PTS=(((int64u)PTS_32)<<30)
                                           | (((int64u)PTS_29)<<15)
                                           | (((int64u)PTS_14));
        if (Stream[start_code].Searching_TimeStamp_Start)
        {
            Stream[start_code].TimeStamp_Start=Stream[start_code].TimeStamp_End;
            Stream[start_code].Searching_TimeStamp_Start=false;
        }
        Element_Info(Ztring().Duration_From_Milliseconds(Stream[start_code].TimeStamp_End.PTS/90));
        Element_End();
        Element_End();
    }
    if (PTS_DTS_flags==0x3)
    {
        int16u PTS_29, PTS_14, DTS_29, DTS_14;
        int8u  PTS_32, DTS_32;
        Element_Begin("PTS_DTS_flags");
        Element_Begin("PTS");
        BS_Begin();
        Mark_0();
        Mark_0();
        Mark_1();
        Mark_1();
        Get_S1 ( 3, PTS_32,                                     "PTS_32");
        Mark_1();
        Get_S2 (15, PTS_29,                                     "PTS_29");
        Mark_1();
        Get_S2 (15, PTS_14,                                     "PTS_14");

        //Filling
        Stream[start_code].TimeStamp_End.PTS_Is_Valid=true;
        Stream[start_code].TimeStamp_End.PTS=(((int64u)PTS_32)<<30)
                                           | (((int64u)PTS_29)<<15)
                                           | (((int64u)PTS_14));
        if (Stream[start_code].Searching_TimeStamp_Start)
        {
            Stream[start_code].TimeStamp_Start=Stream[start_code].TimeStamp_End;
            Stream[start_code].Searching_TimeStamp_Start=false;
        }
        Element_Info(Ztring().Duration_From_Milliseconds(Stream[start_code].TimeStamp_End.PTS/90));
        Element_End();

        Element_Begin("DTS");
        Mark_1();
        Mark_0();
        Mark_0();
        Mark_0();
        Mark_1();
        Get_S1 ( 3, DTS_32,                                     "DTS_32");
        Mark_1();
        Get_S2 (15, DTS_29,                                     "DTS_29");
        Mark_1();
        Get_S2 (15, DTS_14,                                     "DTS_14");
        Mark_1();
        BS_End();

        //Filling
        Stream[start_code].TimeStamp_End.DTS_Is_Valid=true;
        Stream[start_code].TimeStamp_End.DTS=(((int64u)DTS_32)<<30)
                                           | (((int64u)DTS_29)<<15)
                                           | (((int64u)DTS_14));
        Element_Info(Ztring().Duration_From_Milliseconds(Stream[start_code].TimeStamp_End.DTS/90));
        Element_End();
        Element_End();
    }
    if (ESCR_flag && Element_Offset<Element_Pos_After_Data)
    {
        Element_Begin("ESCR_flag");
        BS_Begin();
        int16u ESCR_29, ESCR_14, ESCR_extension;
        int8u  ESCR_32;
        Skip_S1( 2,                                             "reserved");
        Get_S1 ( 3, ESCR_32,                                    "PTS_32");
        Mark_1();
        Get_S2 (15, ESCR_29,                                    "PTS_29");
        Mark_1();
        Get_S2 (15, ESCR_14,                                    "PTS_14");
        Mark_1();
        Get_S2 (15, ESCR_extension,                             "ESCR_extension");
        Mark_1();
        BS_End();
        Element_End();
    }
    if (ES_rate_flag && Element_Offset<Element_Pos_After_Data)
    {
        Element_Begin("ES_rate_flag");
        BS_Begin();
        int32u ES_rate;
        Mark_1();
        Get_S3 (22, ES_rate,                                    "ES_rate");
        Mark_1();
        BS_End();
        Element_End();
    }
    if (DSM_trick_mode_flag && Element_Offset<Element_Pos_After_Data)
    {
        Element_Begin("DSM_trick_mode_flag");
        BS_Begin();
        int8u DSM_trick_mode_flag;
        Get_S1 (3, DSM_trick_mode_flag,                         "DSM_trick_mode_flag"); Param_Info(MpegPs_trick_mode_control_values[DSM_trick_mode_flag]);
        switch (DSM_trick_mode_flag)
        {
            case 0 :{ //fast_forward
                        Skip_S1(2,                              "field_id");
                        Skip_SB(                                "intra_slice_refresh");
                        Skip_S1(2,                              "frequency_truncation");
                    }
                    break;
            case 1 :{ //slow_motion
                        Skip_S1(5,                              "rep_cntrl");
                    }
                    break;
            case 2 :{ //freeze_frame
                        Skip_S1(2,                              "field_id");
                        Skip_S1(3,                              "reserved");
                    }
                    break;
            case 3 :{ //fast_reverse
                        Skip_S1(2,                              "field_id");
                        Skip_SB(                                "intra_slice_refresh");
                        Skip_S1(2,                              "frequency_truncation");
                    }
                    break;
            case 4 :{ //slow_reverse
                        Skip_S1(5,                              "rep_cntrl");
                    }
                    break;
            default:{
                        Skip_S1(5,                              "reserved");
                    }
        }
        BS_End();
        Element_End();
    }
    if (additional_copy_info_flag && Element_Offset<Element_Pos_After_Data)
    {
        Element_Begin("additional_copy_info_flag");
        BS_Begin();
        Mark_1();
        Skip_S1(7,                                              "additional_copy_info");
        BS_End();
        Element_End();
    }
    if (additional_copy_info_flag && Element_Offset<Element_Pos_After_Data)
    {
        Element_Begin("additional_copy_info_flag");
        Skip_B2(                                                "previous_PES_packet_CRC");
        Element_End();
    }
    if (PES_extension_flag && Element_Offset<Element_Pos_After_Data)
    {
        Element_Begin("PES_extension_flag");
        BS_Begin();
        bool PES_private_data_flag, pack_header_field_flag, program_packet_sequence_counter_flag, p_STD_buffer_flag, PES_extension_flag_2;
        Get_SB (PES_private_data_flag,                          "PES_private_data_flag");
        Get_SB (pack_header_field_flag,                         "pack_header_field_flag");
        Get_SB (program_packet_sequence_counter_flag,           "program_packet_sequence_counter_flag");
        Get_SB (p_STD_buffer_flag,                              "P-STD_buffer_flag");
        Skip_S1(3,                                              "reserved");
        Get_SB (PES_extension_flag_2,                           "PES_extension_flag_2");
        BS_End();
        if (PES_private_data_flag)
        {
            Element_Begin("PES_private_data_flag");
            Skip_B16(                                           "PES_private_data");
            Element_End();
        }
        if (pack_header_field_flag)
        {
            Element_Begin("pack_header_field_flag");
            int8u pack_field_length;
            Get_B1 (pack_field_length,                          "pack_field_length");
            Skip_XX(pack_field_length,                          "pack_header");
            Element_End();
        }
        if (program_packet_sequence_counter_flag)
        {
            Element_Begin("program_packet_sequence_counter_flag");
            BS_Begin();
            Mark_1();
            Skip_S1(7,                                          "program_packet_sequence_counter");
            Mark_1();
            Skip_SB(                                            "MPEG1_MPEG2_identifier");
            Skip_S1(6,                                          "original_stuff_length");
            BS_End();
            Element_End();
        }
        if (p_STD_buffer_flag)
        {
            Element_Begin("p_STD_buffer_flag");
            BS_Begin();
            Mark_0();
            Mark_1();
            Skip_SB(                                            "P-STD_buffer_scale");
            Skip_S2(13,                                         "P-STD_buffer_size");
            BS_End();
            Element_End();
        }
        if (PES_extension_flag_2)
        {
            Element_Begin("PES_extension_flag_2");
            int8u PES_extension_field_length;
            BS_Begin();
            Mark_1();
            Get_S1 (7, PES_extension_field_length,              "PES_extension_field_length");
            BS_End();
            Skip_XX(PES_extension_field_length,                 "reserved");
            Element_End();
        }
        Element_End();
    }
    if (Element_Pos_After_Data>Element_Offset)
        Skip_XX(Element_Pos_After_Data-Element_Offset,          "stuffing_bytes");
}

//---------------------------------------------------------------------------
void File_MpegPs::Data_Parse()
{
    //From TS
    if (FromTS && Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);

    //Needed?
    if (!Stream[Element_Code].Searching_Payload)
    {
        Skip_XX(Element_Size,                                   "data");
        return;
    }

    //Parsing
    switch (Element_Code)
    {
        case 0xB9 : MPEG_program_end(); break;
        case 0xBA : pack_start(); break;
        case 0xBB : system_header_start(); break;
        case 0xBC : program_stream_map(); break;
        case 0xBD : private_stream_1(); break;
        case 0xBE : padding_stream(); break;
        case 0xBF : private_stream_2(); break;
        case 0xFA : LATM(); break;
        case 0xFD : if (FromTS)
                        private_stream_1();
                    else
                        video_stream();
                    break;
        default:
                 if (Element_Code>=0xC0
                  && Element_Code<=0xDF) audio_stream();
            else if (Element_Code>=0xE0
                  && Element_Code<=0xEF) video_stream();
            else
                Trusted_IsNot("Unattended element!");
    }
}

//---------------------------------------------------------------------------
void File_MpegPs::Detect_EOF()
{
    //In case of problem with some streams
    if (!File_Name.empty() && File_Offset+Buffer_Offset>SizeToAnalyze)
    {
        video_stream_Count=0;
        audio_stream_Count=0;
        private_stream_1_Count=0;
        private_stream_2_Count=false;
    }

    //Jumping only if needed
    if (video_stream_Count>0 || audio_stream_Count>0 || private_stream_1_Count>0 || private_stream_2_Count==true)
        return;

    //No need anymore of this stream
    Stream[0xBB].Searching_Payload=false; //system_start

    //Jumping if needed
    if (File_Size>SizeToAnalyze && File_Offset+Buffer_Size<File_Size-SizeToAnalyze
     || FromTS)
    {
        //Reactivating interessant PS streams
        std::map<int64u, ps_stream>::iterator Temp=Stream.begin();
        while (Temp!=Stream.end())
        {
            //End timestamp is out of date
            Temp->second.TimeStamp_End.PTS_Is_Valid=false;
            Temp->second.TimeStamp_End.DTS_Is_Valid=false;
            Temp->second.Searching_TimeStamp_Start=false;

            Temp++;
        }

        //Reactivating interessant PS private streams
        Temp=Stream_Private1.begin();
        while (Temp!=Stream_Private1.end())
        {
            //End timestamp is out of date
            Temp->second.TimeStamp_End.PTS_Is_Valid=false;
            Temp->second.TimeStamp_End.DTS_Is_Valid=false;
            Temp->second.Searching_TimeStamp_Start=false;

            Temp++;
        }

        //Jumping
        Info("MPEG-PS, Jumping to end of file");
        if (FromTS)
            Finnished();
        else
            File_GoTo=File_Size-SizeToAnalyze;
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packet "B9"
void File_MpegPs::MPEG_program_end()
{
    Element_Name("MPEG_program_end");
}

//---------------------------------------------------------------------------
// Packet "BA"
void File_MpegPs::pack_start()
{
    Element_Name("pack_start");

    //Parsing
    int16u SysClock_29, SysClock_14;
    int8u Version, SysClock_32, Padding;
    BS_Begin();
    Peek_S1( 2, Version);
    if (Version==1)
    {
        //MPEG-2
        MPEG_Version=2;
        Mark_0();
        Mark_1();
        Get_S1 ( 3, SysClock_32,                                "system_clock_reference_base32");
        Mark_1();
        Get_S2 (15, SysClock_29,                                "system_clock_reference_base29");
        Mark_1();
        Get_S2 (15, SysClock_14,                                "system_clock_reference_base14");

        //Filling
        Stream[0xBA].TimeStamp_End.PTS_Is_Valid=true;
        Stream[0xBA].TimeStamp_End.PTS=(((int64u)SysClock_32)<<30)
                                     | (((int64u)SysClock_29)<<15)
                                     | (((int64u)SysClock_14));
        if (Stream[0xBA].Searching_TimeStamp_Start)
        {
            Stream[0xBA].TimeStamp_Start=Stream[0xBA].TimeStamp_End;
            Stream[0xBA].Searching_TimeStamp_Start=false;
        }
        Param_Info(Ztring().Duration_From_Milliseconds(Stream[0xBA].TimeStamp_End.PTS/90));

        Mark_1();
        Skip_S2( 9,                                             "system_clock_reference_extension");
        Mark_1();
        Get_S3 (22, program_mux_rate,                           "program_mux_rate"); Param_Info(program_mux_rate*400, " bps");
        Mark_1();
        Mark_1();
        Skip_S1( 5,                                             "reserved");
        Get_S1 ( 3, Padding,                                    "pack_stuffing_length");
        BS_End();
        if (Padding>0)
            Skip_XX(Padding,                                    "padding");
    }
    else
    {
        //MPEG-1
        MPEG_Version=1;
        Mark_0();
        Mark_0();
        Mark_1();
        Mark_0();
        Get_S1 ( 3, SysClock_32,                                "system_clock_reference_base32");
        Mark_1();
        Get_S2 (15, SysClock_29,                                "system_clock_reference_base29");
        Mark_1();
        Get_S2 (15, SysClock_14,                                "system_clock_reference_base14");

        //Filling
        Stream[0xBA].TimeStamp_End.PTS_Is_Valid=true;
        Stream[0xBA].TimeStamp_End.PTS=(((int64u)SysClock_32)<<30)
                                     | (((int64u)SysClock_29)<<15)
                                     | (((int64u)SysClock_14));
        if (Stream[0xBA].Searching_TimeStamp_Start)
        {
            Stream[0xBA].TimeStamp_Start=Stream[0xBA].TimeStamp_End;
            Stream[0xBA].Searching_TimeStamp_Start=false;
        }
        Param_Info(Ztring().Duration_From_Milliseconds(Stream[0xBA].TimeStamp_End.PTS/90));

        Mark_1();
        Mark_1();
        Get_S3(22, program_mux_rate,                            "mux_rate"); Param_Info(program_mux_rate*400, " bps");
        Mark_1();
        BS_End();
    }

    //Filling
    FILLING_BEGIN();
    Stream_Prepare(Stream_General);

    SizeToAnalyze=program_mux_rate*50*2; //standard delay between TimeStamps is 0.7s, we try 2s to be sure
    if (SizeToAnalyze>8*1024*1024)
        SizeToAnalyze=8*1024*1024; //Not too much
    if (SizeToAnalyze<1*1024*1024)
        SizeToAnalyze=1*1024*1024; //Not too less

    //Autorisation of other streams
    Stream[0xBA].Searching_Payload=false;           //We need not parse pack_start
    Stream[0xBB].Searching_Payload=true;            //system_header_start
    Stream[0xBD].Searching_Payload=true;            //private_stream_1
    Stream[0xBD].Searching_TimeStamp_Start=true;    //private_stream_1
    Stream[0xBD].Searching_TimeStamp_End=true;      //private_stream_1
    Stream[0xBF].Searching_Payload=true;            //private_stream_2
    Stream[0xFD].Searching_Payload=true;            //private_stream_1 or video_stream
    Stream[0xFD].Searching_TimeStamp_Start=true;    //private_stream_1 or video_stream
    Stream[0xFD].Searching_TimeStamp_End=true;      //private_stream_1 or video_stream
    for (int8u Pos=0xC0; Pos<=0xEF; Pos++)
    {
        Stream[Pos].Searching_Payload=true;         //audio_stream or video_stream
        Stream[Pos].Searching_TimeStamp_Start=true; //audio_stream or video_stream
        Stream[Pos].Searching_TimeStamp_End=true;   //audio_stream or video_stream
    }
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "BB"
void File_MpegPs::system_header_start()
{
    Element_Name("system_header_start");

    //If there is system_header_start, default value for private sections are false
    private_stream_1_Count=0;
    private_stream_2_Count=false;

    //Parsing
    int32u rate_bound;
    int8u  audio_bound, video_bound;
    BS_Begin();
    Mark_1();
    Get_S3 (22, rate_bound,                                     "rate_bound"); Param_Info(rate_bound*400, " bps");;
    Mark_1();
    Get_S1 ( 6, audio_bound,                                    "audio_bound");
    Info_SB(    fixed_flag,                                     "fixed_flag"); Param_Info(MpegPs_System_Fixed[fixed_flag]);
    Skip_SB(                                                    "CSPS_flag");
    Skip_SB(                                                    "system_audio_lock_flag");
    Skip_SB(                                                    "system_video_lock_flag");
    Mark_1();
    Get_S1 ( 5, video_bound,                                    "video_bound");
    Skip_SB(                                                    "packet_rate_restriction_flag");
    Skip_S1( 7,                                                 "reserved_byte");
    bool one=false;
    if (Element_IsNotFinnished())
        Peek_SB(one);
    while (one)
    {
        Element_Begin();
        int16u STD_buffer_size_bound;
        int8u stream_id;
        bool STD_buffer_bound_scale;
        Get_S1 ( 8, stream_id,                                  "stream_id"); Param_Info(MpegPs_stream_id(stream_id));
        Element_Name(Ztring().From_CC1(stream_id));
        Element_Info(MpegPs_stream_id(stream_id));
        Mark_1();
        Mark_1();
        Get_SB (    STD_buffer_bound_scale,                     "STD_buffer_bound_scale");
        Get_S2 (13, STD_buffer_size_bound,                      "STD_buffer_size_bound"); Param_Info(Ztring::ToZtring(STD_buffer_size_bound*(STD_buffer_bound_scale?1024:128)) + _T(" bytes"));
        Element_End();

        FILLING_BEGIN();
            if (stream_id==0xBD)
                private_stream_1_Count=(int8u)-1;
            if (stream_id==0xBF)
                private_stream_2_Count=true;
        FILLING_END();

        if (Element_IsNotFinnished())
            Peek_SB(one);
        else
            one=false;
    }
    BS_End();

    //Filling
    if (audio_stream_Count==(int8u)-1) //0xBB may be multipart
        audio_stream_Count=0;
    audio_stream_Count+=audio_bound;
    if (video_stream_Count==(int8u)-1) //0xBB may be multipart
        video_stream_Count=0;
    video_stream_Count+=video_bound;
    if (private_stream_1_Count>0 && program_mux_rate*50==SizeToAnalyze)
        SizeToAnalyze*=32; //If there is a private section, this may be DVD, with late data --> 10s minimum
    if (SizeToAnalyze>8*1024*1024)
        SizeToAnalyze=8*1024*1024;

    //Autorisation of other streams
    if ((private_stream_1_Count>0 || audio_stream_Count>0) && video_stream_Count>0) //0xBB may be multipart
        Stream[0xBB].Searching_Payload=false;
}

//---------------------------------------------------------------------------
// Packet "BC"
void File_MpegPs::program_stream_map()
{
    Element_Name("program_stream_map");
    MPEG_Version=2; //program_stream_map does NOT exist in MPEG-1 specs

    File_Mpeg_Psi Parser;
    Parser.From_TS=false;
    Open_Buffer_Init(&Parser, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(&Parser, Buffer+Buffer_Offset, (size_t)Element_Size);

    //Filling
    std::map<int32u, File_Mpeg_Psi::Stream>::iterator Streams_Temp=Parser.Streams.begin();
    while (Streams_Temp!=Parser.Streams.end())
    {
        Stream[Streams_Temp->first].stream_type=Streams_Temp->second.stream_type;
        Streams_Temp++;
    }
}

//---------------------------------------------------------------------------
// Packet "BD"
void File_MpegPs::private_stream_1()
{
    Element_Name("private_stream_1");

    //For TS streams, which does not have Start chunk
    if (FromTS)
    {
        video_stream_Count=0;
        audio_stream_Count=0;
        private_stream_1_Count=1;
        private_stream_2_Count=false;
        Stream_Private1[stream_type_FromTS].stream_type=stream_type_FromTS;
    }

    //From TS or PS?
    if (FromTS)
    {
        private_stream_1_ID=stream_type_FromTS;
        private_stream_1_Offset=0;
    }
    else
    {
        private_stream_1_ChooseID();
        if (private_stream_1_ID)
        {
            Element_Info(Ztring::ToZtring(private_stream_1_ID, 16));

            //Exists
            Stream_Private1[private_stream_1_ID].StreamIsRegistred=true;
        }
    }

    //New stream if needed
    if (Stream_Private1[private_stream_1_ID].Parser==NULL)
    {
        Stream_Private1[private_stream_1_ID].Parser=private_stream_1_ChooseParser();
        Stream_Private1[private_stream_1_ID].Searching_Payload=true;
    }
    if (Stream_Private1[private_stream_1_ID].Searching_TimeStamp_Start)
    {
        Stream_Private1[private_stream_1_ID].TimeStamp_Start=Stream_Private1[private_stream_1_ID].TimeStamp_End;
        Stream_Private1[private_stream_1_ID].Searching_TimeStamp_Start=false;
    }
    Stream_Private1[private_stream_1_ID].TimeStamp_End=Stream[0xBD].TimeStamp_End;

    //Needed?
    if (Stream_Private1[private_stream_1_ID].Parser->File_Offset==Stream_Private1[private_stream_1_ID].Parser->File_Size
     && (Stream_Private1[private_stream_1_ID].Parser2==NULL || Stream_Private1[private_stream_1_ID].Parser2->File_Offset==Stream_Private1[private_stream_1_ID].Parser->File_Size)
     && (Stream_Private1[private_stream_1_ID].Parser3==NULL || Stream_Private1[private_stream_1_ID].Parser3->File_Offset==Stream_Private1[private_stream_1_ID].Parser->File_Size))
    {
        Skip_XX(Element_Size-Element_Offset,                    "data");
        return;
    }

    private_stream_1_Element_Info();

    //Parsing
    Open_Buffer_Init(Stream_Private1[private_stream_1_ID].Parser, File_Size, File_Offset+Buffer_Offset+private_stream_1_Offset);
    Open_Buffer_Continue(Stream_Private1[private_stream_1_ID].Parser, Buffer+Buffer_Offset+private_stream_1_Offset, (size_t)(Element_Size-private_stream_1_Offset));

    //Testing other parsers in case of need
    if (Stream_Private1[private_stream_1_ID].Parser->Count_Get(Stream_Audio)==0
     && Stream_Private1[private_stream_1_ID].Parser->Count_Get(Stream_Text )==0)
    {
        Element_Begin("Testing AC3...");
        if (Stream_Private1[private_stream_1_ID].Parser3==NULL)
        {
            #if defined(MEDIAINFO_AC3_YES)
                Stream_Private1[private_stream_1_ID].Parser3=new File_Ac3;
                if (Stream_Private1[private_stream_1_ID].stream_type==0 || Stream_Private1[private_stream_1_ID].stream_type==0x06) //None or private
                    ((File_Ac3*)Stream_Private1[private_stream_1_ID].Parser3)->Frame_Count_Valid=1;
            #else
                Stream_Private1[private_stream_1_ID].Parser3=new File__Base;
            #endif
        }
        Open_Buffer_Init(Stream_Private1[private_stream_1_ID].Parser3, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Stream_Private1[private_stream_1_ID].Parser3, Buffer+Buffer_Offset, (size_t)Element_Size);

        if (Stream_Private1[private_stream_1_ID].Parser3->Count_Get(Stream_Audio)>0)
        {
            Element_Info("AC3 found, changing default parser");
            Element_End();
            delete Stream_Private1[private_stream_1_ID].Parser; Stream_Private1[private_stream_1_ID].Parser=Stream_Private1[private_stream_1_ID].Parser3; Stream_Private1[private_stream_1_ID].Parser3=NULL;
        }
        else
        {
            Element_End();
            Element_Begin("Testing DTS...");
            if (Stream_Private1[private_stream_1_ID].Parser2==NULL)
            {
                #if defined(MEDIAINFO_DTS_YES)
                    Stream_Private1[private_stream_1_ID].Parser2=new File_Dts;
                    if (Stream_Private1[private_stream_1_ID].stream_type==0 || Stream_Private1[private_stream_1_ID].stream_type==0x06) //None or private
                        ((File_Dts*)Stream_Private1[private_stream_1_ID].Parser3)->Frame_Count_Valid=1;
                #else
                    Stream_Private1[private_stream_1_ID].Parser2=new File__Base;
                #endif
            }
            Open_Buffer_Init(Stream_Private1[private_stream_1_ID].Parser2, File_Size, File_Offset+Buffer_Offset);
            Open_Buffer_Continue(Stream_Private1[private_stream_1_ID].Parser2, Buffer+Buffer_Offset, (size_t)Element_Size);

            if (Stream_Private1[private_stream_1_ID].Parser2->Count_Get(Stream_Audio)>0)
            {
                Element_Info("DTS found, changing default parser");
                delete Stream_Private1[private_stream_1_ID].Parser; Stream_Private1[private_stream_1_ID].Parser=Stream_Private1[private_stream_1_ID].Parser2; Stream_Private1[private_stream_1_ID].Parser2=NULL;
            }
            Element_End();
        }
    }
    if (Stream_Private1[private_stream_1_ID].Parser->Count_Get(Stream_Audio)>0 && (Stream_Private1[private_stream_1_ID].Parser2!=NULL || Stream_Private1[private_stream_1_ID].Parser3!=NULL))
    {
        delete Stream_Private1[private_stream_1_ID].Parser2; Stream_Private1[private_stream_1_ID].Parser2=NULL;
        delete Stream_Private1[private_stream_1_ID].Parser3; Stream_Private1[private_stream_1_ID].Parser3=NULL;
    }

    //Disabling this stream
    if (                                                         Stream_Private1[private_stream_1_ID].Parser->File_GoTo !=(int64u)-1
      && !(Stream_Private1[private_stream_1_ID].Parser3!=NULL && Stream_Private1[private_stream_1_ID].Parser3->File_GoTo==(int64u)-1)
      && !(Stream_Private1[private_stream_1_ID].Parser2!=NULL && Stream_Private1[private_stream_1_ID].Parser2->File_GoTo==(int64u)-1))
    {
        Stream_Private1[private_stream_1_ID].Searching_Payload=false;
        if (private_stream_1_Count>0)
            private_stream_1_Count--;
    }

    //Demux
    Demux(Buffer+Buffer_Offset+private_stream_1_Offset, (size_t)(Element_Size-private_stream_1_Offset), Ztring::ToZtring(Element_Code, 16)+_T(".")+Ztring::ToZtring(private_stream_1_ID, 16)+private_stream_1_ChooseExtension());
}

//---------------------------------------------------------------------------
void File_MpegPs::private_stream_1_ChooseID()
{
    //Init
    private_stream_1_Offset=0;

    //From TS
    if (stream_type_FromTS>0)
    {
        private_stream_1_ID=stream_type_FromTS;
        return;
    }

    //Trying VOB system
    private_stream_1_ID=0;
    private_stream_1_ChooseID_VOB();
    if (private_stream_1_ID)
        return;
}

//---------------------------------------------------------------------------
void File_MpegPs::private_stream_1_ChooseID_VOB()
{
    //Parsing
    int16u Next;
    int8u  CodecID, Count;
    Get_B1 (CodecID,                                        "CodecID");
    Get_B1 (Count,                                          "Count of next frame headers");
    Get_B2 (Next,                                           "Next frame offset minus 1");

    //Testing
    if (Count>0 && 4+(int64u)Next+4<=Element_Size)
    {
        //Subtitles
             if (CodecID>=0x20 && CodecID<=0x3F)
            ; //Seems to not work with subtitles, to be confirmed
        //AC3
        else if (CodecID>=0x80 && CodecID<=0x87)
        {
            if (CC2(Buffer+Buffer_Offset+4+Next)!=0x0B77 && CC2(Buffer+Buffer_Offset+3+Next)!=0x0B77 && CC2(Buffer+Buffer_Offset+2+Next)!=0x0B77)
                return;
        }
        //DTS
        else if (CodecID>=0x88 && CodecID<=0x8F)
        {
            if (CC4(Buffer+Buffer_Offset+4+Next)!=0x7FFE8001 && CC4(Buffer+Buffer_Offset+3+Next)!=0x7FFE8001 && CC4(Buffer+Buffer_Offset+2+Next)!=0x7FFE8001)
                return;
        }
        //DTS
        else if (CodecID>=0x98 && CodecID<=0x9F)
        {
            if (CC4(Buffer+Buffer_Offset+4+Next)!=0x7FFE8001 && CC4(Buffer+Buffer_Offset+3+Next)!=0x7FFE8001 && CC4(Buffer+Buffer_Offset+2+Next)!=0x7FFE8001)
                return;
        }
        //PCM
        else if (CodecID>=0xA0 && CodecID<=0xAF)
            ;
        //AC3+
        else if (CodecID>=0xC0 && CodecID<=0xCF)
        {
            if (CC2(Buffer+Buffer_Offset+4+Next)!=0x0B77 && CC2(Buffer+Buffer_Offset+3+Next)!=0x0B77 && CC2(Buffer+Buffer_Offset+2+Next)!=0x0B77)
                return;
        }

        private_stream_1_IsDvdVideo=true;
    }

    //Filling
    private_stream_1_ID=(int8u)CodecID;

    //Positionning
    private_stream_1_Offset=4;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::private_stream_1_ChooseParser()
{
    //Subtitles
         if (private_stream_1_ID>=0x20 && private_stream_1_ID<=0x3F)
        return private_stream_1_ChooseParser_RLE();
    //DTS-HD
    else if (FromTS && private_stream_1_ID>=0x86 && private_stream_1_ID<=0x87)
        return private_stream_1_ChooseParser_DTS();
    //AC3
    else if (private_stream_1_ID>=0x80 && private_stream_1_ID<=0x87)
        return private_stream_1_ChooseParser_AC3();
    //DTS
    else if (private_stream_1_ID>=0x88 && private_stream_1_ID<=0x8F)
        return private_stream_1_ChooseParser_DTS();
    //SDDS
    else if (private_stream_1_ID>=0x90 && private_stream_1_ID<=0x97)
        return private_stream_1_ChooseParser_DTS();
    //DTS
    else if (private_stream_1_ID>=0x98 && private_stream_1_ID<=0x9F)
        return private_stream_1_ChooseParser_DTS();
    //PCM
    else if (private_stream_1_ID>=0xA0 && private_stream_1_ID<=0xAF)
        return private_stream_1_ChooseParser_PCM();
    //AC3+
    else if (private_stream_1_ID>=0xC0 && private_stream_1_ID<=0xCF)
        return private_stream_1_ChooseParser_AC3();
    else
        return private_stream_1_ChooseParser_NULL();
}

//---------------------------------------------------------------------------
ZenLib::Char* File_MpegPs::private_stream_1_ChooseExtension()
{
    //Subtitles
         if (private_stream_1_ID>=0x20 && private_stream_1_ID<=0x3F)
        return _T(".sub");
    //DTS-HD
    else if (FromTS && private_stream_1_ID>=0x86 && private_stream_1_ID<=0x87)
        return _T(".dts");
    //AC3
    else if (private_stream_1_ID>=0x80 && private_stream_1_ID<=0x87)
        return _T(".ac3");
    //DTS
    else if (private_stream_1_ID>=0x88 && private_stream_1_ID<=0x8F)
        return _T(".dts");
    //SDDS
    else if (private_stream_1_ID>=0x90 && private_stream_1_ID<=0x97)
        return _T(".sdds");
    //DTS
    else if (private_stream_1_ID>=0x98 && private_stream_1_ID<=0x9F)
        return _T(".dts");
    //PCM
    else if (private_stream_1_ID>=0xA0 && private_stream_1_ID<=0xAF)
        return _T(".pcm");
    //AC3+
    else if (private_stream_1_ID>=0xC0 && private_stream_1_ID<=0xCF)
        return _T(".ac3");
    else
        return _T("");
}

//---------------------------------------------------------------------------
void File_MpegPs::private_stream_1_Element_Info()
{
    //Subtitles
         if (private_stream_1_ID>=0x20 && private_stream_1_ID<=0x3F)
        Element_Info("RLE");
    //DTS-HD
    else if (FromTS && private_stream_1_ID>=0x86 && private_stream_1_ID<=0x87)
        Element_Info("DTS");
    //AC3
    else if (private_stream_1_ID>=0x80 && private_stream_1_ID<=0x87)
        Element_Info("AC3");
    //DTS
    else if (private_stream_1_ID>=0x88 && private_stream_1_ID<=0x8F)
        Element_Info("DTS");
    //SDDS
    else if (private_stream_1_ID>=0x90 && private_stream_1_ID<=0x97)
        Element_Info("SDDS");
    //DTS
    else if (private_stream_1_ID>=0x98 && private_stream_1_ID<=0x9F)
        Element_Info("DTS");
    //PCM
    else if (private_stream_1_ID>=0xA0 && private_stream_1_ID<=0xAF)
        Element_Info("LPCM");
    //AC3+
    else if (private_stream_1_ID>=0xC0 && private_stream_1_ID<=0xCF)
        Element_Info("AC3");
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::private_stream_1_ChooseParser_RLE()
{
    //Filling
    #if defined(MEDIAINFO_RLE_YES)
        return new File_Rle();
    #else
        //Filling
        File__Analyze* Handle=new File__Analyze();
        Handle->Stream_Prepare(Stream_Text);
        Handle->Fill("Codec", "RLE");
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::private_stream_1_ChooseParser_AC3()
{
    //Filling
    #if defined(MEDIAINFO_AC3_YES)
        File__Analyze* Handle=new File_Ac3();
        ((File_Ac3*)Handle)->Frame_Count_Valid=1;
        return Handle;
    #else
        //Filling
        File__Analyze* Handle=new File__Analyze();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill("Codec", "AC3");
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::private_stream_1_ChooseParser_DTS()
{
    //Filling
    #if defined(MEDIAINFO_DTS_YES)
        File__Analyze* Handle=new File_Dts();
        ((File_Dts*)Handle)->Frame_Count_Valid=1;
        return Handle;
    #else
        //Filling
        File__Analyze* Handle=new File__Analyze();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill("Codec", "DTS");
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::private_stream_1_ChooseParser_SDDS()
{
    //Filling
    #if defined(MEDIAINFO_SDDS_YES)
        //Filling
        File__Analyze* Handle=new File__Analyze();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill("Codec", "SDDS");
        return Handle;
    #else
        //Filling
        File__Analyze* Handle=new File__Analyze();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill("Codec", "SDDS");
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::private_stream_1_ChooseParser_PCM()
{
    //Filling
    #if defined(MEDIAINFO_PCM_YES)
        return new File_Pcm();
    #else
        //Filling
        File__Analyze* Handle=new File__Analyze(); //PCM Signed 16 bits Big Endian, Interleavement is for 2 samples*2 channels L0-1/L0-0/R0-1/R0-0/L1-1/L1-0/R1-1/R1-0/L0-2/R0-2/L1-2/R1-2, http://wiki.multimedia.cx/index.php?title=PCM
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill("Codec", "LPCM (Big signed)");
        Handle->Fill("Resolution", 24);
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::private_stream_1_ChooseParser_NULL()
{
    //Filling
    File__Analyze* Handle=new File__Analyze();
    return Handle;
}

//---------------------------------------------------------------------------
// Packet "BE"
void File_MpegPs::padding_stream()
{
    Element_Name("padding_stream");

    Skip_XX(Element_Size,                                       "stuffing_bytes");
}

//---------------------------------------------------------------------------
// Packet "BF"
void File_MpegPs::private_stream_2()
{
    Element_Name("private_stream_2");

    //Filling
    Stream_Prepare(Stream_Menu);
    Fill("Codec", "DVD-Video");

    //Disabling this stream
    Stream[0xBF].Searching_Payload=false;
    private_stream_2_Count=false;
}

//---------------------------------------------------------------------------
void File_MpegPs::audio_stream()
{
    Element_Name("Audio");

    //For TS streams, which does not have Start chunk
    if (FromTS)
    {
        video_stream_Count=0;
        audio_stream_Count=1;
        private_stream_1_Count=0;
        private_stream_2_Count=false;
        Stream[Element_Code].stream_type=stream_type_FromTS;
    }

    //Exists
    Stream[Element_Code].StreamIsRegistred=true;

    //If we have no stream map --> Registering the stream as MPEG Audio
    if (Stream[Element_Code].stream_type==0)
    {
        if (MPEG_Version==2)
            Stream[Element_Code].stream_type=0x04; //MPEG-2 Audio
        else
            Stream[Element_Code].stream_type=0x03; //MPEG-1 Audio
    }

    //New stream if needed
    if (Stream[Element_Code].Parser==NULL)
    {
             if (Stream[Element_Code].stream_type==0x03 || Stream[Element_Code].stream_type==0x04)
        {
            #if defined(MEDIAINFO_MPEGA_YES)
                Stream[Element_Code].Parser=new File_Mpega;
                ((File_Mpega*)Stream[Element_Code].Parser)->Frame_Count_Valid=1;
            #else
                //Filling
                Stream[Element_Code].Parser=new File__Analyze();
                Stream[Element_Code].Parser->Stream_Prepare(Stream_Audio);
                if (Stream[Element_Code].stream_type==1)
                    Stream[Element_Code].Parser->Fill("Codec", "MPEG-1 Audio");
                if (Stream[Element_Code].stream_type==2)
                    Stream[Element_Code].Parser->Fill("Codec", "MPEG-2 Audio");
            #endif
        }
        else
        {
            Stream[Element_Code].Parser=new File__Analyze();
            Stream[Element_Code].Parser->Stream_Prepare(Stream_Audio);
        }
        //Stream[Element_Code].Parser=new File_Latm();
    }

    //Parsing
    Open_Buffer_Init(Stream[Element_Code].Parser, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(Stream[Element_Code].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);

    //Disabling this stream
    if (Stream[Element_Code].Parser->File_GoTo!=(int64u)-1 || Stream[Element_Code].Parser->Count_Get(Stream_Audio)>0)
    {
        Stream[Element_Code].Searching_Payload=false;
        if (audio_stream_Count>0)
            audio_stream_Count--;
    }

    //Demux
    Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(Element_Code, 16)+_T(".mpa"));
}

//---------------------------------------------------------------------------
void File_MpegPs::video_stream()
{
    Element_Name("Video");

    //For TS streams, which does not have Start chunk
    if (FromTS)
    {
        video_stream_Count=1;
        audio_stream_Count=0;
        private_stream_1_Count=0;
        private_stream_2_Count=false;
        Stream[Element_Code].stream_type=stream_type_FromTS;
    }

    //Exists
    Stream[Element_Code].StreamIsRegistred=true;

    //New stream if needed
    if (Stream[Element_Code].Parser==NULL)
    {
        if (Element_Code==0xFD)
        {
            #if defined(MEDIAINFO_VC1_YES)
                Stream[Element_Code].Parser=new File_Vc1;
                ((File_Vc1*)Stream[Element_Code].Parser)->Frame_Count_Valid=1;
                if (!FromTS)
                    ((File_Vc1*)Stream[Element_Code].Parser)->FrameIsAlwaysComplete=true;
            #else
                //Filling
                Stream[Element_Code].Parser=new File__Analyze();
                Stream[Element_Code].Parser->Stream_Prepare(Stream_Video);
                Stream[Element_Code].Parser->Fill("Codec", "VC-1");
            #endif
        }
        else if (Stream[Element_Code].stream_type==0x01 || Stream[Element_Code].stream_type==0x02 || Stream[Element_Code].stream_type==0x80 || Stream[Element_Code].stream_type==0x00)
        {
            #if defined(MEDIAINFO_MPEGV_YES)
                Stream[Element_Code].Parser=new File_Mpegv;
                ((File_Mpegv*)Stream[Element_Code].Parser)->MPEG_Version=MPEG_Version;
                ((File_Mpegv*)Stream[Element_Code].Parser)->Frame_Count_Valid=1;
                if (!FromTS)
                    ((File_Mpegv*)Stream[Element_Code].Parser)->FrameIsAlwaysComplete=true;
            #else
                //Filling
                Stream[Element_Code].Parser=new File__Analyze();
                Stream[Element_Code].Parser->Stream_Prepare(Stream_Video);
                if (Stream[Element_Code].stream_type==0x01)
                    Stream[Element_Code].Parser->Fill("Codec", "MPEG-1 Video");
                if (Stream[Element_Code].stream_type==0x02)
                    Stream[Element_Code].Parser->Fill("Codec", "MPEG-2 Video");
            #endif
        }
        else if (Stream[Element_Code].stream_type==0x10)
        {
            #if defined(MEDIAINFO_MPEG4V_YES)
                Stream[Element_Code].Parser=new File_Mpeg4v;
                ((File_Mpeg4v*)Stream[Element_Code].Parser)->Frame_Count_Valid=1;
                if (!FromTS)
                    ((File_Mpeg4v*)Stream[Element_Code].Parser)->FrameIsAlwaysComplete=true;
            #else
                //Filling
                Stream[Element_Code].Parser=new File__Analyze();
                Stream[Element_Code].Parser->Stream_Prepare(Stream_Video);
                Stream[Element_Code].Parser->Fill("Codec", "MPEG-4V");
            #endif
        }
        else if (Stream[Element_Code].stream_type==0x1B)
        {
            #if defined(MEDIAINFO_AVC_YES)
                Stream[Element_Code].Parser=new File_Avc;
                ((File_Avc*)Stream[Element_Code].Parser)->Frame_Count_Valid=2;
            #else
                //Filling
                Stream[Element_Code].Parser=new File__Analyze();
                Stream[Element_Code].Parser->Stream_Prepare(Stream_Video);
                Stream[Element_Code].Parser->Fill("Codec", "AVC");
            #endif
        }
        else
        {
            Stream[Element_Code].Parser=new File__Analyze();
            Stream[Element_Code].Parser->Stream_Prepare(Stream_Video);
        }
    }

    //Parsing
    Open_Buffer_Init(Stream[Element_Code].Parser, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(Stream[Element_Code].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);

    //Testing other parsers in case of need
    if (Stream[Element_Code].stream_type==0 && Stream[Element_Code].Parser->Count_Get(Stream_Video)==0)
    {
        bool WantShow1=Element_Show_Get();
        Element_Begin("Testing AVC...");
        if (Stream[Element_Code].Parser3==NULL)
        {
            #if defined(MEDIAINFO_AVC_YES)
                Stream[Element_Code].Parser3=new File_Avc;
                ((File_Avc*)Stream[Element_Code].Parser3)->Frame_Count_Valid=2;
            #else
                Stream[Element_Code].Parser3=new File__Base;
            #endif
        }
        Open_Buffer_Init(Stream[Element_Code].Parser3, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Stream[Element_Code].Parser3, Buffer+Buffer_Offset, (size_t)Element_Size);

        if (Stream[Element_Code].Parser3->Count_Get(Stream_Video)>0)
        {
            Element_Info("AVC found, changing default parser");
            Element_End();
            delete Stream[Element_Code].Parser; Stream[Element_Code].Parser=Stream[Element_Code].Parser3; Stream[Element_Code].Parser3=NULL;
        }
        else
        {
            Element_End();
            bool WantShow2=Element_Show_Get();
            if (WantShow1)
                Element_Show();
            Element_Begin("Testing MPEG-4 Video...");
            if (Stream[Element_Code].Parser2==NULL)
            {
                #if defined(MEDIAINFO_MPEG4V_YES)
                    Stream[Element_Code].Parser2=new File_Mpeg4v;
                #else
                    Stream[Element_Code].Parser2=new File__Base;
                #endif
            }
            Open_Buffer_Init(Stream[Element_Code].Parser2, File_Size, File_Offset+Buffer_Offset);
            Open_Buffer_Continue(Stream[Element_Code].Parser2, Buffer+Buffer_Offset, (size_t)Element_Size);

            if (Stream[Element_Code].Parser2->Count_Get(Stream_Video)>0)
            {
                Element_Info("MPEG-4 Video found, changing default parser");
                delete Stream[Element_Code].Parser; Stream[Element_Code].Parser=Stream[Element_Code].Parser2; Stream[Element_Code].Parser2=NULL;
            }
            Element_End();
            if (WantShow1 || WantShow2)
                Element_Show();
        }
    }
    if (Stream[Element_Code].Parser->Count_Get(Stream_Video)>0 && (Stream[Element_Code].Parser2!=NULL || Stream[Element_Code].Parser3!=NULL))
    {
        delete Stream[Element_Code].Parser2; Stream[Element_Code].Parser2=NULL;
        delete Stream[Element_Code].Parser3; Stream[Element_Code].Parser3=NULL;
    }

    //Disabling this stream
    if (                                                        Stream[Element_Code].Parser->File_GoTo !=(int64u)-1
      && !(Stream[Element_Code].Parser3!=NULL && Stream[Element_Code].Parser3->File_GoTo==(int64u)-1)
      && !(Stream[Element_Code].Parser2!=NULL && Stream[Element_Code].Parser2->File_GoTo==(int64u)-1))
    {
        Stream[Element_Code].Searching_Payload=false;
        if (video_stream_Count>0)
            video_stream_Count--;
    }

    //Demux
    Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(Element_Code, 16)+_T(".mpv"));
}

//---------------------------------------------------------------------------
// Packet "FA"
void File_MpegPs::LATM()
{
    Element_Name("LATM");

    //For TS streams, which does not have Start chunk
    if (FromTS)
    {
        video_stream_Count=0;
        audio_stream_Count=1;
        private_stream_1_Count=0;
        private_stream_2_Count=false;
        Stream[Element_Code].stream_type=stream_type_FromTS;
    }

    //Exists
    Stream[Element_Code].StreamIsRegistred=true;

    //Parsing
    /*
    size_t Frame_Size=0;
    int8u  Size_Partial;
    Element_Begin("Size");
    do
    {
        Get_B1(Size_Partial,                                    "Size");
        Frame_Size+=Size_Partial;
    }
    while (Size_Partial==0xFF && Element_Offset<Element_Size);
    Element_Info(Ztring::ToZtring(Frame_Size)+_T(" bytes"));
    Element_End();
    Skip_XX(Frame_Size,                                         "LATM data?");
    Skip_XX(Element_Size-Element_Offset,                        "Data (To be parsed)");
    */

    //Filling
    Stream[Element_Code].Parser=new File__Analyze();
    Stream[Element_Code].Parser->Stream_Prepare(Stream_Audio);
    Stream[Element_Code].Parser->Fill("Codec", "AAC");

    //Disabling this stream
    Stream[Element_Code].Searching_Payload=false;
    if (audio_stream_Count>0)
        audio_stream_Count--;

    //Demux
    Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(Element_Code, 16)+_T(".latm"));
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegPs::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+4<=Buffer_Size
      && !(CC3(Buffer+Buffer_Offset  )==0x000001
        && CC1(Buffer+Buffer_Offset+3)>=0xB9))
        Buffer_Offset++;
    if (Buffer_Offset+4>Buffer_Size)
    {
        //Parsing last bytes
        if (Buffer_Offset+4==Buffer_Size)
        {
            if (CC3(Buffer+Buffer_Offset)!=0x000001)
            {
                Buffer_Offset++;
                if (CC3(Buffer+Buffer_Offset)!=0x000001)
                {
                    Buffer_Offset++;
                    if (CC2(Buffer+Buffer_Offset)!=0x0000)
                    {
                        Buffer_Offset++;
                        if (CC1(Buffer+Buffer_Offset)!=0x00)
                            Buffer_Offset++;
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

//---------------------------------------------------------------------------
bool File_MpegPs::Header_Parser_QuickSearch()
{
    while (           Buffer_Offset+4<=Buffer_Size
      &&   CC3(Buffer+Buffer_Offset)==0x000001)
    {
        //Getting start_code
        int8u start_code=CC1(Buffer+Buffer_Offset+3);

        //Searching start
        if (Stream[start_code].Searching_Payload)
        {
            if (start_code!=0xBD || !private_stream_1_IsDvdVideo) //Not (private_stream_1 and IsDvdVideo)
                return true;

            //private_stream_1 and IsDvdVideo, looking for substream ID
            size_t Data_Offset=CC1(Buffer+Buffer_Offset+8);
            int8u  private_stream_1_ID=CC1(Buffer+Buffer_Offset+9+Data_Offset);
            if (Stream_Private1.find(private_stream_1_ID)==Stream_Private1.end() || Stream_Private1[private_stream_1_ID].Searching_Payload)
                return true;
        }

        //Searching TimeStamp_End
        if (Stream[start_code].Searching_TimeStamp_End)
        {
            switch(start_code)
            {
                //Element with no PES Header
                case 0xB9 : //MPEG_program_end
                case 0xBA : //pack_start
                case 0xBB : //system_header_start
                case 0xBC : //program_stream_map
                case 0xBE : //padding_stream
                case 0xBF : //private_stream_2
                case 0xF0 : //ECM
                case 0xF1 : //EMM
                case 0xF2 : //DSMCC stream
                case 0xF8 : //ITU-T Rec. H .222.1 type E
                case 0xFF : //Program stream directory
                    break;

                //Element with PES Header
                default :
                    if (MPEG_Version==1)
                    {
                        size_t Buffer_Offset_Temp=Buffer_Offset+6;
                        while(Buffer_Offset_Temp<=Buffer_Size && CC1(Buffer+Buffer_Offset_Temp)==0xFF)
                            Buffer_Offset_Temp++;
                        if (Buffer_Offset_Temp>=Buffer_Size)
                            return false; //Not enough data
                        if (Buffer_Offset_Temp<Buffer_Size && (CC1(Buffer+Buffer_Offset_Temp)&0xF0)!=0x00)
                            return true; //With a PTS
                    }
                    if (MPEG_Version==2)
                    {
                        if (Buffer_Offset+7>Buffer_Size)
                            return false; //Not enough buffer
                        if ((CC1(Buffer+Buffer_Offset+7)&0xC0)!=0x00)
                            return true; //With a PTS
                    }
            }
        }

        //Getting size
        switch(start_code)
        {
            //No size
            case 0xB9 : //MPEG_program_end
            case 0xBA : //pack_start
                Buffer_Offset+=4;
                while(Buffer_Offset+4<=Buffer_Size && !(CC3(Buffer+Buffer_Offset)==0x000001 && CC1(Buffer+Buffer_Offset+3)>=0xB9))
                    Buffer_Offset++;
                //Parsing last bytes
                if (Buffer_Offset+3==Buffer_Size)
                {
                    if (CC3(Buffer+Buffer_Offset)!=0x000001)
                    {
                        Buffer_Offset++;
                        if (CC2(Buffer+Buffer_Offset)!=0x0000)
                        {
                            Buffer_Offset++;
                            if (CC1(Buffer+Buffer_Offset)!=0x00)
                                Buffer_Offset++;
                        }
                    }
                }
                break;

            //Element with size
            default :
                if (Buffer_Offset+6>=Buffer_Size)
                    return false; //Not enough data
                int16u Size=CC2(Buffer+Buffer_Offset+4);
                if (Size>0)
                    Buffer_Offset+=6+Size;
                else
                {
                    Buffer_Offset+=6;
                    while(Buffer_Offset+4<=Buffer_Size && !(CC3(Buffer+Buffer_Offset)==0x000001 && CC1(Buffer+Buffer_Offset+3)>=0xB9))
                        Buffer_Offset++;
                    //Parsing last bytes
                    if (Buffer_Offset+3==Buffer_Size)
                    {
                        if (CC3(Buffer+Buffer_Offset)!=0x000001)
                        {
                            Buffer_Offset++;
                            if (CC2(Buffer+Buffer_Offset)!=0x0000)
                            {
                                Buffer_Offset++;
                                if (CC1(Buffer+Buffer_Offset)!=0x00)
                                    Buffer_Offset++;
                            }
                        }
                    }
                }
        }
    }

    if (Buffer_Offset+4>Buffer_Size)
    {
        Synched=false;
        Synchronize();
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool File_MpegPs::Detect_NonMPEGPS ()
{
    //File_Size
    if (File_Size<=188*4)
        return false; //We can't do detection

    //Element_Size
    if (Buffer_Size<=188*4)
        return true; //Must wait for more data

    //Detect mainly DAT files, and the parser is not enough precise to detect them later
    if (CC4(Buffer)==CC4("RIFF"))
    {
        Finnished();
        return true;
    }

    //Detect TS files, and the parser is not enough precise to detect them later
    while (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset)!=0x47) //Look for first Sync word
        Buffer_Offset++;
    if (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset+188)==0x47 && CC1(Buffer+Buffer_Offset+188*2)==0x47 && CC1(Buffer+Buffer_Offset+188*3)==0x47)
    {
        Finnished();
        return true;
    }
    Buffer_Offset=0;

    //Seems OK
    return false;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegPs::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("OveralBitRate", "R");
            Fill_HowTo("PlayTime", "R");
            Fill_HowTo("Encoded_Application", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Width", "R");
            Fill_HowTo("Height", "R");
            Fill_HowTo("DisplayAspectRatio", "R");
            Fill_HowTo("FrameRate", "R");
            Fill_HowTo("Delay", "R");
            break;
        case (Stream_Audio) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("Resolution", "R");
            Fill_HowTo("Delay", "R");
            break;
        case (Stream_Text) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("Delay", "R");
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

} //NameSpace

#endif //MEDIAINFO_MPEGPS_YES

