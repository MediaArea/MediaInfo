// File_MpegPs - Info for MPEG files
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
#if defined(MEDIAINFO_AVSV_YES)
    #include "MediaInfo/Video/File_AvsV.h"
#endif
#if defined(MEDIAINFO_DIRAC_YES)
    #include "MediaInfo/Video/File_Dirac.h"
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
#if defined(MEDIAINFO_ADTS_YES)
    #include "MediaInfo/Audio/File_Adts.h"
#endif
#if defined(MEDIAINFO_PCM_YES)
    #include "MediaInfo/Audio/File_Pcm.h"
#endif
#if defined(MEDIAINFO_AES3_YES)
    #include "MediaInfo/Audio/File_Aes3.h"
#endif
#if defined(MEDIAINFO_LATM_YES)
    #include "MediaInfo/Audio/File_Latm.h"
#endif
#if defined(MEDIAINFO_PS2A_YES)
    #include "MediaInfo/Audio/File_Ps2Audio.h"
#endif
#if defined(MEDIAINFO_RLE_YES)
    #include "MediaInfo/Image/File_Rle.h"
#endif
#if defined(MEDIAINFO_PGS_YES)
    #include "MediaInfo/Text/File_Pgs.h"
#endif
#include "MediaInfo/File_Unknown.h"
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
const char* MpegPs_stream_id_extension(int8u stream_id_extension)
{
    switch (stream_id_extension)
    {
        case 0x00 : return "IPMP Control Information Streams"; //ISO/IEC 13818-11
        case 0x01 : return "IPMP Streams";                     //ISO/IEC 13818-11
        default :
                 if (stream_id_extension>=0x02
                  && stream_id_extension<=0x11) return "ISO/IEC 14496-17 text Streams";
            else if (stream_id_extension>=0x12
                  && stream_id_extension<=0x21) return "ISO/IEC 23002-3 auxiliary video data Streams";
            else if (stream_id_extension>=0x55
                  && stream_id_extension<=0x5F) return "VC-1";
            else if (stream_id_extension>=0x60
                  && stream_id_extension<=0x6F) return "Dirac";
            else if (stream_id_extension==0x71) return "Audio";
            else if (stream_id_extension==0x72) return "Audio Ext";
            else if (stream_id_extension==0x76) return "Audio";
            else if (stream_id_extension>=0x75
                  && stream_id_extension<=0x7F) return "VC-1";
            else                                return "";
    }
}

//---------------------------------------------------------------------------
extern const char* Mpeg_Psi_stream_type_Format(int8u stream_type, int32u format_identifier);
extern const char* Mpeg_Psi_stream_type_Codec(int8u stream_type, int32u format_identifier);
extern stream_t    Mpeg_Psi_stream_type_StreamKind(int32u stream_type, int32u format_identifier);
extern const char* Mpeg_Psi_stream_type_Info(int8u stream_type, int32u format_identifier);

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_MpegPs::File_MpegPs()
:File__Analyze()
{
    //Configuration
    MustSynchronize=true;
    Buffer_TotalBytes_FirstSynched_Max=64*1024;
    Trusted_Multiplier=2;

    //In
    FromTS=false;
    stream_type_FromTS=0x00; //No info
    descriptor_tag_FromTS=0x00; //No info
    format_identifier_FromTS=0x00000000; //No info
    MPEG_Version=0; //No info
    Searching_TimeStamp_Start=true;

    //Out
    HasTimeStamps=false;

    //Temp
    SizeToAnalyze=8*1024*1024;
    video_stream_Unlimited=false;
    Parsing_End_ForDTS=false;

    //From packets
    program_mux_rate=(int32u)-1;

    BookMark_Set(); //for stream parsing in phase 2
}

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegPs::FileHeader_Begin()
{
    if (!File__Analyze::FileHeader_Begin_0x000001())
        return false;

    if (!MustSynchronize)
        Synched_Init();

    //All should be OK
    return true;
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegPs::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+4<=Buffer_Size)
    {
        while (Buffer_Offset+4<=Buffer_Size && Buffer[Buffer_Offset]!=0x00)
            Buffer_Offset++;
        if (Buffer_Offset+4<=Buffer_Size && Buffer[Buffer_Offset+1]==0x00)
            if (Buffer[Buffer_Offset+2]==0x01 && Buffer[Buffer_Offset+3]>=0xB9)
                break;
        Buffer_Offset++;
    }

    //Parsing last bytes if needed
    if (Buffer_Offset+4>Buffer_Size)
    {
        if (Buffer_Offset+3==Buffer_Size && CC3(Buffer+Buffer_Offset)!=0x000001)
            Buffer_Offset++;
        if (Buffer_Offset+2==Buffer_Size && CC2(Buffer+Buffer_Offset)!=0x0000)
            Buffer_Offset++;
        if (Buffer_Offset+1==Buffer_Size && CC1(Buffer+Buffer_Offset)!=0x00)
            Buffer_Offset++;
        return false;
    }

    //Synched is OK
    return true;
}

//---------------------------------------------------------------------------
bool File_MpegPs::Synched_Test()
{
    //Video unlimited specific
    if (video_stream_Unlimited)
        return true; //Already tested, and this can be in a Streams

    //Trailing 0xFF
    while(Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset]==0xFF)
        Buffer_Offset++;

    //Trailing 0x00
    while(Buffer_Offset+3<=Buffer_Size
       && Buffer[Buffer_Offset+2]==0x00
       && Buffer[Buffer_Offset+1]==0x00
       && Buffer[Buffer_Offset  ]==0x00)
        Buffer_Offset++;

    //Must have enough buffer for having header
    if (Buffer_Offset+3>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Buffer[Buffer_Offset  ]!=0x00
     || Buffer[Buffer_Offset+1]!=0x00
     || Buffer[Buffer_Offset+2]!=0x01)
        Synched=false;

    //Quick search
    if (Synched && !Header_Parser_QuickSearch())
        return false;

    //We continue
    return true;
}

//---------------------------------------------------------------------------
void File_MpegPs::Synched_Init()
{
    //private_stream_1 specific
    private_stream_1_ID=0x00;
    private_stream_1_Offset=0;
    private_stream_1_IsDvdVideo=false;

    //Count
    video_stream_Count=(int8u)-1;
    audio_stream_Count=(int8u)-1;
    private_stream_1_Count=(int8u)-1;
    private_stream_2_Count=true;
    extension_stream_Count=(int8u)-1;

    //From packets
    program_mux_rate=0;

    //Default values
    Streams.resize(0x100);
    Streams_Private1.resize(0x100);
    Streams_Extension.resize(0x100);
    Streams[0xBA].Searching_Payload=true;

    //Temp
    stream_id_extension=0x55; //Default is set to VC-1, should never happens, but happens sometimes
    video_stream_Unlimited_start_code=0xFF;

    //Case of extraction from MPEG-TS files
    if (File_Offset==0 && Buffer_Size>=4 && ((CC4(Buffer)&0xFFFFFFF0)==0x000001E0 || (CC4(Buffer)&0xFFFFFFE0)==0x000001C0 || CC4(Buffer)==0x000001BD || CC4(Buffer)==0x000001FA || CC4(Buffer)==0x000001FD))
    {
        FromTS=true; //We want to anlyze this kind of file
        MPEG_Version=2; //By default, MPEG-TS is version 2
        Streams[Buffer[3]].Searching_Payload=true; //Activating the Streams
    }

    //TS specific
    if (FromTS)
    {
        Streams[0xBD].Searching_Payload=true;            //private_stream_1
        Streams[0xBD].Searching_TimeStamp_Start=true;    //private_stream_1
        Streams[0xBD].Searching_TimeStamp_End=true;      //private_stream_1
        Streams[0xBF].Searching_Payload=true;            //private_stream_2
        Streams[0xBF].Searching_TimeStamp_Start=true;    //private_stream_2
        Streams[0xBF].Searching_TimeStamp_End=true;      //private_stream_2
        for (int8u Pos=0xC0; Pos<=0xEF; Pos++)
        {
            Streams[Pos].Searching_Payload=true;         //audio_stream or video_stream
            Streams[Pos].Searching_TimeStamp_Start=true; //audio_stream or video_stream
            Streams[Pos].Searching_TimeStamp_End=true;   //audio_stream or video_stream
        }
        Streams[0xFA].Searching_Payload=true;            //LATM
        Streams[0xFA].Searching_TimeStamp_Start=true;    //LATM
        Streams[0xFA].Searching_TimeStamp_End=true;      //LATM
        Streams[0xFD].Searching_Payload=true;            //extension_stream
        Streams[0xFD].Searching_TimeStamp_Start=true;    //extension_stream
        Streams[0xFD].Searching_TimeStamp_End=true;      //extension_stream
    }
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Unsynched()
{
    Searching_TimeStamp_Start=false;

    if (Streams.empty())
       return;

    //No need anymore of this Streams
    Streams[0xBB].Searching_Payload=false; //system_start

    //Reactivating interessant PS streams
    for (size_t StreamID=0; StreamID<0x100; StreamID++)
    {
        //End timestamp is out of date
        Streams[StreamID].TimeStamp_End.PTS.File_Pos=(int64u)-1;
        Streams[StreamID].TimeStamp_End.DTS.File_Pos=(int64u)-1;
        Streams[StreamID].TimeStamp_End.PTS.TimeStamp=(int64u)-1;
        Streams[StreamID].TimeStamp_End.DTS.TimeStamp=(int64u)-1;
        Streams[StreamID].Searching_TimeStamp_Start=false;
        Streams_Private1[StreamID].TimeStamp_End.PTS.File_Pos=(int64u)-1;
        Streams_Private1[StreamID].TimeStamp_End.DTS.File_Pos=(int64u)-1;
        Streams_Private1[StreamID].TimeStamp_End.PTS.TimeStamp=(int64u)-1;
        Streams_Private1[StreamID].TimeStamp_End.DTS.TimeStamp=(int64u)-1;
        Streams_Private1[StreamID].Searching_TimeStamp_Start=false;
        Streams_Extension[StreamID].TimeStamp_End.PTS.File_Pos=(int64u)-1;
        Streams_Extension[StreamID].TimeStamp_End.DTS.File_Pos=(int64u)-1;
        Streams_Extension[StreamID].TimeStamp_End.PTS.TimeStamp=(int64u)-1;
        Streams_Extension[StreamID].TimeStamp_End.DTS.TimeStamp=(int64u)-1;
        Streams_Extension[StreamID].Searching_TimeStamp_Start=false;
    }
    video_stream_Unlimited=false;
}

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Finalize()
{
    if (Streams.empty())
        return; //Not initialized
    if (!IsAccepted)
        Accept("MPEG-PS");

    PTS=0; //Will be used for BitRate calculation
    DTS=0; //Will be used for Duration calculation

    //For each Streams
    for (size_t StreamID=0; StreamID<0x100; StreamID++)
        Read_Buffer_Finalize_PerStream(StreamID, Streams[StreamID]);

    //For each private Streams
    for (size_t StreamID=0; StreamID<0x100; StreamID++)
        Read_Buffer_Finalize_PerStream(StreamID, Streams_Private1[StreamID]);

    //For each extesnion Streams
    for (size_t StreamID=0; StreamID<0x100; StreamID++)
        Read_Buffer_Finalize_PerStream(StreamID, Streams_Extension[StreamID]);

    //Fill General
    if (Count_Get(Stream_General)==0)
    {
        if (Count_Get(Stream_Video) || Count_Get(Stream_Audio))
            Stream_Prepare(Stream_General);
        else
        {
            Reject("MPEG-PS");
            return;
        }
    }
    Fill(Stream_General, 0, General_Format, "MPEG-PS");

    //Tags in MPEG Video
    if (Count_Get(Stream_Video)>0)
        Fill(Stream_General, 0, General_Encoded_Library, Retrieve(Stream_Video, 0, Video_Encoded_Library));

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
    {
        Streams.clear();
        Streams_Private1.clear();
        Streams_Extension.clear();
    }

    //Bitrate coherancy
    if (!IsSub && PTS>0 && PTS!=(int64u)-1 && DTS!=0 && File_Size!=(int64u)-1)
    {
        int64u BitRate_FromDuration=File_Size*8000*90/DTS;
        int64u BitRate_FromBitRates=PTS;

        if (BitRate_FromDuration>=BitRate_FromBitRates*3
         || BitRate_FromDuration<=BitRate_FromBitRates/10)
        {
            //Clearing durations
            for (size_t StreamKind=0; StreamKind<=Stream_Text; StreamKind++)
                for (size_t StreamPos=0; StreamPos<Count_Get((stream_t)StreamKind); StreamPos++)
                    Fill((stream_t)StreamKind, StreamPos, "Duration", "", Unlimited, true, true);
        }
    }
}

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Finalize_PerStream(size_t StreamID, ps_stream &Temp)
{
    //By the parser
    StreamKind_Last=Stream_Max;
    if (Temp.Parser /*&& Temp.Parser->Count_Get(Stream_General)>0*/) //Verifying it is really detected, not only a header
    {
        Open_Buffer_Finalize(Temp.Parser);
        Merge(*Temp.Parser);
    }
    //By the TS stream_type
    if (StreamKind_Last==Stream_Max)
    {
        //Disabling stream_private_1 if needed (will be done by Streams_Private1 object)
        if (Temp.stream_type!=0 && StreamID==0xBD)
        {
            bool StreamIsDetected=false;
            for (size_t Pos=0; Pos<Streams_Private1.size(); Pos++)
                if (Streams_Private1[Pos].Parser)
                    StreamIsDetected=true;
            if (StreamIsDetected)
                Temp.stream_type=0;
        }

        if (Temp.stream_type!=0)
            Stream_Prepare(Mpeg_Psi_stream_type_StreamKind(Temp.stream_type, 0x00000000));
    }
    //By StreamIsRegistred
    if (StreamKind_Last==Stream_Max)
    {
        if (Temp.StreamIsRegistred)
        {
            if (StreamID>=0xC0 && StreamID<=0xDF)
                Stream_Prepare(Stream_Audio);
            if (StreamID>=0xE0 && StreamID<=0xEF)
                Stream_Prepare(Stream_Video);
        }
    }

    //More info
    if (StreamKind_Last!=Stream_Max) //Found
    {
        //Special cases
        if (StreamKind_Last==Stream_Text && Temp.Parser && Temp.Parser->Count_Get(Stream_Video))
        {
            StreamKind_Last=Stream_Video;
            StreamPos_Last=Count_Get(Stream_Video)-1;
        }

        //Common
        Fill(StreamKind_Last, StreamPos_Last, "ID", StreamID);
        Ztring ID_String; ID_String.From_Number(StreamID); ID_String+=_T(" (0x"); ID_String+=Ztring::ToZtring(StreamID, 16); ID_String+=_T(")");
        Fill(StreamKind_Last, StreamPos_Last, "ID/String", ID_String); //TODO: merge with Decimal_Hexa in file_MpegTs
        if (Retrieve(StreamKind_Last, StreamPos_Last, "Format").empty() && Temp.stream_type!=0)
            Fill(StreamKind_Last, StreamPos_Last, "Format", Mpeg_Psi_stream_type_Format(Temp.stream_type, 0x0000));
        if (Retrieve(StreamKind_Last, StreamPos_Last, "Codec").empty() && Temp.stream_type!=0)
            Fill(StreamKind_Last, StreamPos_Last, "Codec", Mpeg_Psi_stream_type_Codec(Temp.stream_type, 0x0000));

        int64u Start=(int64u)-1, End=(int64u)-1;
        if (Temp.TimeStamp_Start.DTS.TimeStamp!=(int64u)-1 && Temp.TimeStamp_End.DTS.TimeStamp!=(int64u)-1)
        {
            Start=Temp.TimeStamp_Start.DTS.TimeStamp;
            End=Temp.TimeStamp_End.DTS.TimeStamp;
        }
        else if (Temp.TimeStamp_Start.PTS.TimeStamp!=(int64u)-1 && Temp.TimeStamp_End.PTS.TimeStamp!=(int64u)-1)
        {
            Start=Temp.TimeStamp_Start.PTS.TimeStamp;
            End=Temp.TimeStamp_End.PTS.TimeStamp;
        }
        if (Start!=(int64u)-1 && End!=(int64u)-1)
        {
            //TimeStamp
            if (End<Start)
                End+=0x200000000LL; //33 bits, cyclic
            int64u Duration=End-Start;
            if (File_Size!=(int64u)-1 && File_Size>1024*1024*16 && End>=0x200000000LL)
            {
                //Testing coherancy
                if (Duration/90>16*3600*1000)
                    Duration=0; //Disabling it
            }
            if (Duration)
            {
                if (StreamKind_Last==Stream_Video)
                {
                    float64 FrameRate=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate).To_float64();
                    if (FrameRate!=0)
                        Duration+=Ztring::ToZtring(Temp.FrameCount_AfterLast_TimeStamp_End*90*1000/FrameRate, 0).To_int64u();
                }
                if (Duration)
                {
                    Fill(StreamKind_Last, StreamPos_Last, "Duration", Duration/90, 10, true);
                    if (Duration>DTS)
                        DTS=Duration; //Saving maximum Duration
                }
            }
        }
        if (Temp.TimeStamp_Start.PTS.TimeStamp!=(int64u)-1)
        {
            if (StreamKind_Last==Stream_Video)
            {
                Fill(StreamKind_Last, StreamPos_Last, "Delay_Original", Retrieve(StreamKind_Last, StreamPos_Last, "Delay"));
                Fill(StreamKind_Last, StreamPos_Last, "Delay_Original_Settings", Retrieve(StreamKind_Last, StreamPos_Last, "Delay_Settings"));
            }
            Fill(StreamKind_Last, StreamPos_Last, "Delay", Temp.TimeStamp_Start.PTS.TimeStamp/90, 10, true);
            Fill(StreamKind_Last, StreamPos_Last, "Delay_Settings", "", Unlimited, true, true);
        }

        //Special cases
        if (StreamKind_Last==Stream_Video && Temp.Parser && Temp.Parser->Count_Get(Stream_Text))
        {
            //Video and Text are together
            size_t Text_Count=Temp.Parser->Count_Get(Stream_Text);
            for (size_t Text_Pos=0; Text_Pos<Text_Count; Text_Pos++)
            {
                size_t Pos=Count_Get(Stream_Text)-Text_Count+Text_Pos;
                Ztring MuxingMode=Retrieve(Stream_Text, Pos, "MuxingMode");
                Fill(Stream_Text, Pos, "MuxingMode", Ztring(_T("MPEG Video / "))+MuxingMode, true);
                if (!IsSub)
                    Fill(Stream_Text, Pos, "MuxingMode_MoreInfo", _T("Muxed in Video #")+Ztring().From_Number(StreamPos_Last+1));
                Fill(Stream_Text, Pos, Text_StreamSize, 0);
                Ztring ID=Retrieve(Stream_Text, Pos, Text_ID);
                Fill(Stream_Text, Pos, Text_ID, Retrieve(Stream_Video, StreamPos_Last, Video_ID)+_T("-")+ID, true);
                Fill(Stream_Text, Pos, Text_ID_String, Retrieve(Stream_Video, StreamPos_Last, Video_ID_String)+_T("-")+ID, true);
                Fill(Stream_Text, Pos, Text_Delay, Retrieve(Stream_Video, StreamPos_Last, Video_Delay), true);
            }
        }
    }

    //Bitrate calculation
    if (PTS!=(int64u)-1 && (StreamKind_Last==Stream_Video || StreamKind_Last==Stream_Audio))
    {
        int64u BitRate=Retrieve(StreamKind_Last, StreamPos_Last, "BitRate").To_int64u();
        if (BitRate==0)
            BitRate=Retrieve(StreamKind_Last, StreamPos_Last, "BitRate_Nominal").To_int64u();
        if (BitRate==0)
            PTS=(int64u)-1;
        else
            PTS+=BitRate; //Saving global BitRate
    }
}

//***************************************************************************
// Buffer - Par element
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegPs::Header_Parse()
{
    //Reinit
    PTS=(int64u)-1;
    DTS=(int64u)-1;

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
        && (Buffer[Buffer_Offset_Temp  ]!=0x00
         || Buffer[Buffer_Offset_Temp+1]!=0x00
         || Buffer[Buffer_Offset_Temp+2]!=0x01
         || Buffer[Buffer_Offset_Temp+3]< 0xB9))
    {
        Buffer_Offset_Temp+=2;
        while(Buffer_Offset_Temp<Buffer_Size && Buffer[Buffer_Offset_Temp]!=0x00)
            Buffer_Offset_Temp+=2;
        if (Buffer_Offset_Temp<Buffer_Size && Buffer[Buffer_Offset_Temp-1]==0x00 || Buffer_Offset_Temp>=Buffer_Size)
            Buffer_Offset_Temp--;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (File_Offset+Buffer_Size==File_Size)
            Buffer_Offset_Temp=Buffer_Size; //We are sure that the next bytes are a start
        else
            return false;
    }

    //Specific case
    if (video_stream_Unlimited && Buffer_Offset_Temp==Buffer_Offset) //Latest chunk is finished, we must parse the next one
    {
        video_stream_Unlimited=false;
        Header_Parse_Fill_Size();
        return true;
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
        case 0xF2 : //DSMCC Streams
        case 0xF8 : //ITU-T Rec. H .222.1 type E
        case 0xFF : //Program Streams directory
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
        PTS=(((int64u)PTS_32)<<30)
          | (((int64u)PTS_29)<<15)
          | (((int64u)PTS_14));
        if (Streams[start_code].Searching_TimeStamp_End && start_code!=0xBD && start_code!=0xFD) //0xBD and 0xFD can contain multiple streams, TimeStamp management is in Streams management
        {
            Streams[start_code].TimeStamp_End.PTS.TimeStamp=PTS;
        }
        if (Searching_TimeStamp_Start && Streams[start_code].Searching_TimeStamp_Start && start_code!=0xBD && start_code!=0xFD) //0xBD and 0xFD can contain multiple streams, TimeStamp management is in Streams management
        {
            Streams[start_code].TimeStamp_Start.PTS.TimeStamp=PTS;
            Streams[start_code].Searching_TimeStamp_Start=false;
        }
        Element_Info_From_Milliseconds(PTS/90);
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
        PTS=(((int64u)PTS_32)<<30)
          | (((int64u)PTS_29)<<15)
          | (((int64u)PTS_14));
        if (Streams[start_code].Searching_TimeStamp_End)
        {
            Streams[start_code].TimeStamp_End.PTS.TimeStamp=PTS;
        }
        if (Searching_TimeStamp_Start && Streams[start_code].Searching_TimeStamp_Start)
        {
            Streams[start_code].TimeStamp_Start.PTS.TimeStamp=PTS;
        }
        Element_Info_From_Milliseconds(PTS/90);
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
        DTS=(((int64u)DTS_32)<<30)
          | (((int64u)DTS_29)<<15)
          | (((int64u)DTS_14));
        if (Streams[start_code].Searching_TimeStamp_End)
        {
            Streams[start_code].TimeStamp_End.DTS.File_Pos=File_Offset+Buffer_Offset;
            Streams[start_code].TimeStamp_End.DTS.TimeStamp=DTS;
        }
        if (Searching_TimeStamp_Start && Streams[start_code].Searching_TimeStamp_Start)
        {
            Streams[start_code].TimeStamp_Start.DTS.TimeStamp=DTS;
            Streams[start_code].Searching_TimeStamp_Start=false;
        }
        Element_Info_From_Milliseconds(Streams[start_code].TimeStamp_End.DTS.TimeStamp/90);
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
    int64u Element_Pos_After_Data=Element_Offset+PES_header_data_length;

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
        Mark_1_NoTrustError(); //Is "0" in one sample
        Mark_0_NoTrustError(); //Is "1" in one sample
        Get_S1 ( 3, PTS_32,                                     "PTS_32");
        Mark_1();
        Get_S2 (15, PTS_29,                                     "PTS_29");
        Mark_1();
        Get_S2 (15, PTS_14,                                     "PTS_14");
        Mark_1();
        BS_End();

        //Filling
        PTS=(((int64u)PTS_32)<<30)
          | (((int64u)PTS_29)<<15)
          | (((int64u)PTS_14));
        if (Streams[start_code].Searching_TimeStamp_End)
        {
            Streams[start_code].TimeStamp_End.PTS.File_Pos=File_Offset+Buffer_Offset;
            Streams[start_code].TimeStamp_End.PTS.TimeStamp=PTS;
        }
        if (Searching_TimeStamp_Start && Streams[start_code].Searching_TimeStamp_Start)
        {
            Streams[start_code].TimeStamp_Start.PTS.TimeStamp=PTS;
            Streams[start_code].Searching_TimeStamp_Start=false;
        }
        Element_Info_From_Milliseconds(PTS/90);
        Element_End();
        Element_End();
        HasTimeStamps=true;
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
        Mark_1_NoTrustError(); //Is "0" in one sample
        Mark_1_NoTrustError(); //Is "0" in one sample
        Get_S1 ( 3, PTS_32,                                     "PTS_32");
        Mark_1();
        Get_S2 (15, PTS_29,                                     "PTS_29");
        Mark_1();
        Get_S2 (15, PTS_14,                                     "PTS_14");

        //Filling
        PTS=(((int64u)PTS_32)<<30)
          | (((int64u)PTS_29)<<15)
          | (((int64u)PTS_14));
        if (Streams[start_code].Searching_TimeStamp_End)
        {
            Streams[start_code].TimeStamp_End.PTS.File_Pos=File_Offset+Buffer_Offset;
            Streams[start_code].TimeStamp_End.PTS.TimeStamp=PTS;
        }
        if (Searching_TimeStamp_Start && Streams[start_code].Searching_TimeStamp_Start)
        {
            Streams[start_code].TimeStamp_Start.PTS.TimeStamp=PTS;
        }
        Element_Info_From_Milliseconds(PTS/90);
        Element_End();

        Element_Begin("DTS");
        Mark_1();
        Mark_0();
        Mark_0();
        Mark_0();
        Mark_1_NoTrustError(); //Is "1" in one sample
        Get_S1 ( 3, DTS_32,                                     "DTS_32");
        Mark_1();
        Get_S2 (15, DTS_29,                                     "DTS_29");
        Mark_1();
        Get_S2 (15, DTS_14,                                     "DTS_14");
        Mark_1();
        BS_End();

        //Filling
        DTS=(((int64u)DTS_32)<<30)
          | (((int64u)DTS_29)<<15)
          | (((int64u)DTS_14));
        if (Streams[start_code].Searching_TimeStamp_End)
        {
            Streams[start_code].TimeStamp_End.DTS.File_Pos=File_Offset+Buffer_Offset;
            Streams[start_code].TimeStamp_End.DTS.TimeStamp=DTS;
        }
        if (Searching_TimeStamp_Start && Streams[start_code].Searching_TimeStamp_Start)
        {
            Streams[start_code].TimeStamp_Start.DTS.TimeStamp=DTS;
            Streams[start_code].Searching_TimeStamp_Start=false;
        }
        Element_Info_From_Milliseconds(Streams[start_code].TimeStamp_End.DTS.TimeStamp/90);
        Element_End();
        Element_End();
        HasTimeStamps=true;
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
            Skip_SB(                                            "Should be 1"); //But I saw a file with "0"
            Skip_SB(                                            "P-STD_buffer_scale");
            Skip_S2(13,                                         "P-STD_buffer_size");
            BS_End();
            Element_End();
        }
        if (PES_extension_flag_2)
        {
            Element_Begin("PES_extension_flag_2");
            int8u PES_extension_field_length;
            bool stream_id_extension_flag;
            BS_Begin();
            Mark_1();
            Get_S1 (7, PES_extension_field_length,              "PES_extension_field_length");
            Get_SB (stream_id_extension_flag,                   "stream_id_extension_flag");
            if (stream_id_extension_flag==0)
            {
                Get_S1 (7, stream_id_extension,                 "stream_id_extension"); Param_Info(MpegPs_stream_id_extension(stream_id_extension));
            }
            else
            {
                //This should not, but I found a file with stream_id_extension_flag=1 and a real code...
                Get_S1 (7, stream_id_extension,                 "stream_id_extension"); Param_Info(MpegPs_stream_id_extension(stream_id_extension));
            }
            BS_End();
            if (PES_extension_field_length-1>0)
                Skip_XX(PES_extension_field_length-1,           "reserved");
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
    if (!Streams[start_code].Searching_Payload)
    {
        Skip_XX(Element_Size,                                   "data");
        Element_DoNotShow();
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
        case 0xFD : extension_stream();
                    break;
        default:
                 if (Element_Code>=0xC0
                  && Element_Code<=0xDF) audio_stream();
            else if (Element_Code>=0xE0
                  && Element_Code<=0xEF) video_stream();
            else
                Trusted_IsNot("Unattended element!");
    }

    //Jumping to the last DTS if needed
    //if (!Parsing_End_ForDTS && File_Offset+Buffer_Offset==File_Size)
    //    Jump_DTS();
}

//---------------------------------------------------------------------------
void File_MpegPs::Detect_EOF()
{
    //In case of problem with some streams
    if (Buffer_TotalBytes>Buffer_TotalBytes_FirstSynched+SizeToAnalyze)
    {
        if (!IsAccepted)
        {
            Reject("MPEG-PS");
            return;
        }

        video_stream_Count=0;
        audio_stream_Count=0;
        private_stream_1_Count=0;
        private_stream_2_Count=false;
        extension_stream_Count=0;
    }

    //Jumping only if needed
    if (Streams.empty() || video_stream_Count>0 || audio_stream_Count>0 || private_stream_1_Count>0 || private_stream_2_Count==true || extension_stream_Count>0)
        return;

    //Jumping if needed
    IsFilled=true;
    if (File_Size>SizeToAnalyze && File_Offset+Buffer_Size<File_Size-SizeToAnalyze)
    {
        //Jumping
        if (!IsAccepted)
            Accept("MPEG-PS");
        if (IsSub)
            Finish("MPEG-PS");
        else
        {
            GoToFromEnd(SizeToAnalyze, "MPEG-PS");
            Read_Buffer_Unsynched();
        }
    }
}

//---------------------------------------------------------------------------
//Jumping to the last DTS if needed
bool File_MpegPs::BookMark_Needed()
{
    if (IsSub || Streams.empty())
        return false;

    int64u ToJump=(int64u)-1;
    for (size_t StreamID=0; StreamID<0x100; StreamID++)
    {
        //End timestamp is out of date
        if (Streams[StreamID].TimeStamp_End.PTS.File_Pos!=(int64u)-1)
        {
            if (Streams[StreamID].TimeStamp_End.PTS.File_Pos<ToJump)
                ToJump=Streams[StreamID].TimeStamp_End.PTS.File_Pos;
            Streams[StreamID].Searching_Payload=true;
        }
        if (Streams[StreamID].TimeStamp_End.DTS.File_Pos!=(int64u)-1)
        {
            if (Streams[StreamID].TimeStamp_End.DTS.File_Pos<ToJump)
                ToJump=Streams[StreamID].TimeStamp_End.DTS.File_Pos;
            Streams[StreamID].Searching_Payload=true;
        }
        if (Streams_Private1[StreamID].TimeStamp_End.PTS.File_Pos!=(int64u)-1)
        {
            if (Streams_Private1[StreamID].TimeStamp_End.PTS.File_Pos<ToJump)
                ToJump=Streams_Private1[StreamID].TimeStamp_End.PTS.File_Pos;
            Streams_Private1[StreamID].Searching_Payload=true;
        }
        if (Streams_Private1[StreamID].TimeStamp_End.DTS.File_Pos!=(int64u)-1)
        {
            if (Streams_Private1[StreamID].TimeStamp_End.DTS.File_Pos<ToJump)
                ToJump=Streams_Private1[StreamID].TimeStamp_End.DTS.File_Pos;
            Streams_Private1[StreamID].Searching_Payload=true;
        }
        if (Streams_Extension[StreamID].TimeStamp_End.PTS.File_Pos!=(int64u)-1)
        {
            if (Streams_Extension[StreamID].TimeStamp_End.PTS.File_Pos<ToJump)
                ToJump=Streams_Extension[StreamID].TimeStamp_End.PTS.File_Pos;
            Streams_Extension[StreamID].Searching_Payload=true;
        }
        if (Streams_Extension[StreamID].TimeStamp_End.DTS.File_Pos!=(int64u)-1)
        {
            if (Streams_Extension[StreamID].TimeStamp_End.DTS.File_Pos<ToJump)
                ToJump=Streams_Extension[StreamID].TimeStamp_End.DTS.File_Pos;
            Streams_Extension[StreamID].Searching_Payload=true;
        }
    }
    if (IsAccepted && ToJump!=(int64u)-1)
    {
        Info("MPEG-PS, Jumping to nearly end of file");
        Parsing_End_ForDTS=true;
        File_GoTo=ToJump;
        return true;
    }

    return false;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packet "B9"
void File_MpegPs::MPEG_program_end()
{
    Element_Name("MPEG_program_end");

    //Filling
    Synched=false; //We don't know what can be after
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
        Streams[0xBA].TimeStamp_End.PTS.TimeStamp=(((int64u)SysClock_32)<<30)
                                                | (((int64u)SysClock_29)<<15)
                                                | (((int64u)SysClock_14));
        if (Searching_TimeStamp_Start && Streams[0xBA].Searching_TimeStamp_Start)
        {
            Streams[0xBA].TimeStamp_Start=Streams[0xBA].TimeStamp_End;
            Streams[0xBA].Searching_TimeStamp_Start=false;
        }
        Param_Info_From_Milliseconds(Streams[0xBA].TimeStamp_End.PTS.TimeStamp/90);

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
        Streams[0xBA].TimeStamp_End.PTS.TimeStamp=(((int64u)SysClock_32)<<30)
                                                | (((int64u)SysClock_29)<<15)
                                                | (((int64u)SysClock_14));
        if (Searching_TimeStamp_Start && Streams[0xBA].Searching_TimeStamp_Start)
        {
            Streams[0xBA].TimeStamp_Start=Streams[0xBA].TimeStamp_End;
            Streams[0xBA].Searching_TimeStamp_Start=false;
        }
        Param_Info_From_Milliseconds(Streams[0xBA].TimeStamp_End.PTS.TimeStamp/90);

        Mark_1();
        Mark_1();
        Get_S3(22, program_mux_rate,                            "mux_rate"); Param_Info(program_mux_rate*400, " bps");
        Mark_1();
        BS_End();
    }

    //Filling
    FILLING_BEGIN();
        if (Element_Offset!=Element_Size)
            return; //TODO: Should be done in FILLING_BEGIN later

        Stream_Prepare(Stream_General);

        SizeToAnalyze=program_mux_rate*50*2; //standard delay between TimeStamps is 0.7s, we try 2s to be sure
        if (SizeToAnalyze>16*1024*1024)
            SizeToAnalyze=16*1024*1024; //Not too much
        if (SizeToAnalyze<2*1024*1024)
            SizeToAnalyze=2*1024*1024; //Not too less

        //Autorisation of other streams
        Streams[0xB9].Searching_Payload=true;            //MPEG_program_end
        Streams[0xBA].Searching_Payload=false;           //We need not parse pack_start
        Streams[0xBB].Searching_Payload=true;            //system_header_start
        Streams[0xBD].Searching_Payload=true;            //private_stream_1
        Streams[0xBD].Searching_TimeStamp_Start=true;    //private_stream_1
        Streams[0xBD].Searching_TimeStamp_End=true;      //private_stream_1
        Streams[0xBF].Searching_Payload=true;            //private_stream_2
        Streams[0xFD].Searching_Payload=true;            //private_stream_1 or video_stream
        Streams[0xFD].Searching_TimeStamp_Start=true;    //private_stream_1 or video_stream
        Streams[0xFD].Searching_TimeStamp_End=true;      //private_stream_1 or video_stream
        for (int8u Pos=0xC0; Pos<=0xEF; Pos++)
        {
            Streams[Pos].Searching_Payload=true;         //audio_stream or video_stream
            Streams[Pos].Searching_TimeStamp_Start=true; //audio_stream or video_stream
            Streams[Pos].Searching_TimeStamp_End=true;   //audio_stream or video_stream
        }
        if (!IsAccepted)
            Accept("MPEG-PS");
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
    if (Element_IsNotFinished())
        Peek_SB(one);
    while (one)
    {
        Element_Begin();
        int16u STD_buffer_size_bound;
        int8u stream_id, stream_id_extension=0;
        bool STD_buffer_bound_scale;
        Get_S1 ( 8, stream_id,                                  "stream_id"); Param_Info(MpegPs_stream_id(stream_id));
        Element_Name(Ztring().From_CC1(stream_id));
        Element_Info(MpegPs_stream_id(stream_id));
        if (stream_id==0xB7)
        {
            Mark_1();
            Mark_1();
            Mark_0();
            Mark_0();
            Mark_0();
            Mark_0();
            Mark_0();
            Mark_0();
            Mark_0();
            Get_S1 (8, stream_id_extension,                     "stream_id_extension");
            Mark_1();
            Mark_0();
            Mark_1();
            Mark_1();
            Mark_0();
            Mark_1();
            Mark_1();
            Mark_0();
        }
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
            if (stream_id==0xFD)
                extension_stream_Count=(int8u)-1;
        FILLING_END();

        if (Element_IsNotFinished())
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
        Streams[0xBB].Searching_Payload=false;
    Streams[0xBC].Searching_Payload=true;            //program_stream_map
}

//---------------------------------------------------------------------------
// Packet "BC"
void File_MpegPs::program_stream_map()
{
    Element_Name("program_stream_map");
    MPEG_Version=2; //program_stream_map does NOT exist in MPEG-1 specs

    File_Mpeg_Psi Parser;
    Parser.From_TS=false;
    Parser.Complete_Stream=new complete_stream;
    Parser.Complete_Stream->Streams.resize(0x100);
    Open_Buffer_Init(&Parser);
    Open_Buffer_Continue(&Parser, Buffer+Buffer_Offset, (size_t)Element_Size);
    Open_Buffer_Finalize(&Parser);

    //Filling
    for (int8u Pos=0; Pos<0xFF; Pos++)
        if (Parser.Complete_Stream->Streams[Pos].stream_type)
            Streams[Pos].stream_type=Parser.Complete_Stream->Streams[Pos].stream_type;
    delete Parser.Complete_Stream; //Parser.Complete_Stream=NULL;
}

//---------------------------------------------------------------------------
// Packet "BD"
void File_MpegPs::private_stream_1()
{
    Element_Name("private_stream_1");

    //For TS streams, which does not have Start chunk
    if (FromTS)
    {
        //From TS
        video_stream_Count=0;
        audio_stream_Count=0;
        private_stream_1_Count=1;
        private_stream_2_Count=false;
        extension_stream_Count=0;
        private_stream_1_ID=stream_type_FromTS;
        private_stream_1_Offset=0;
        Streams_Private1[private_stream_1_ID].stream_type=stream_type_FromTS;
    }
    else
    {
        //From PS, trying DVD system
        private_stream_1_ID=0;
        private_stream_1_Offset=0;
        private_stream_1_Choose_DVD_ID();
        if (private_stream_1_ID)
        {
            Element_Info(Ztring::ToZtring(private_stream_1_ID, 16));

            //Exists
            Streams_Private1[private_stream_1_ID].StreamIsRegistred=true;
        }
    }

    //New Streams if needed
    if (Streams_Private1[private_stream_1_ID].Parser==NULL)
    {
        Streams_Private1[private_stream_1_ID].Parser=private_stream_1_ChooseParser();
        Streams_Private1[private_stream_1_ID].Searching_TimeStamp_Start=true;
        Open_Buffer_Init(Streams_Private1[private_stream_1_ID].Parser);
        if (!IsAccepted)
            Data_Accept("MPEG-PS");
    }
    if (Searching_TimeStamp_Start && Streams_Private1[private_stream_1_ID].Searching_TimeStamp_Start)
    {
        Streams_Private1[private_stream_1_ID].TimeStamp_Start=Streams[0xBD].TimeStamp_Start;
        Streams_Private1[private_stream_1_ID].Searching_TimeStamp_Start=false;
    }
    Streams_Private1[private_stream_1_ID].TimeStamp_End=Streams[0xBD].TimeStamp_End;

    //Needed?
    if (Streams_Private1[private_stream_1_ID].Parser->IsFinished)
    {
        Skip_XX(Element_Size-Element_Offset,                    "data");
        return;
    }

    private_stream_1_Element_Info();

    //Specific
    #if defined(MEDIAINFO_AES3_YES)
        if (FromTS && format_identifier_FromTS==0x42535344 && PTS!=(int64u)-1) //"BSSD"
            ((File_Aes3*)Streams_Private1[private_stream_1_ID].Parser)->PTS=PTS;
    #endif

    //Parsing
    if (Parsing_End_ForDTS && ((Streams_Private1[private_stream_1_ID].TimeStamp_End.DTS.TimeStamp!=(int64u)-1 && DTS!=(int64u)-1) || (Streams_Private1[private_stream_1_ID].TimeStamp_End.PTS.TimeStamp!=(int64u)-1 && PTS!=(int64u)-1)))
        Streams_Private1[private_stream_1_ID].FrameCount_AfterLast_TimeStamp_End=0;
    Open_Buffer_Continue(Streams_Private1[private_stream_1_ID].Parser, Buffer+Buffer_Offset+private_stream_1_Offset, (size_t)(Element_Size-private_stream_1_Offset));
    if (Parsing_End_ForDTS)
        Streams_Private1[private_stream_1_ID].FrameCount_AfterLast_TimeStamp_End+=Streams_Private1[private_stream_1_ID].Parser->Frame_Count_InThisBlock;

    //Testing other parsers in case of need
    if (Streams_Private1[private_stream_1_ID].Parser->Count_Get(Stream_Audio)==0
     && Streams_Private1[private_stream_1_ID].Parser->Count_Get(Stream_Text )==0)
    {
        Element_Begin("Testing AC3...");
        if (Streams_Private1[private_stream_1_ID].Parser3==NULL)
        {
            #if defined(MEDIAINFO_AC3_YES)
                Streams_Private1[private_stream_1_ID].Parser3=new File_Ac3;
                if (Streams_Private1[private_stream_1_ID].stream_type==0 || Streams_Private1[private_stream_1_ID].stream_type==0x06) //None or private
                    ((File_Ac3*)Streams_Private1[private_stream_1_ID].Parser3)->Frame_Count_Valid=2;
            #else
                Streams_Private1[private_stream_1_ID].Parser3=new File__Analyze;
            #endif
            Open_Buffer_Init(Streams_Private1[private_stream_1_ID].Parser3);
        }
        Open_Buffer_Continue(Streams_Private1[private_stream_1_ID].Parser3, Buffer+Buffer_Offset, (size_t)Element_Size);

        if (Streams_Private1[private_stream_1_ID].Parser3->Count_Get(Stream_Audio)>0)
        {
            Element_Info("AC3 found, changing default parser");
            Element_End();
            delete Streams_Private1[private_stream_1_ID].Parser; Streams_Private1[private_stream_1_ID].Parser=Streams_Private1[private_stream_1_ID].Parser3; Streams_Private1[private_stream_1_ID].Parser3=NULL;
        }
        else
        {
            Element_End();
            Element_Begin("Testing DTS...");
            if (Streams_Private1[private_stream_1_ID].Parser2==NULL)
            {
                #if defined(MEDIAINFO_DTS_YES)
                    Streams_Private1[private_stream_1_ID].Parser2=new File_Dts;
                    if (Streams_Private1[private_stream_1_ID].stream_type==0 || Streams_Private1[private_stream_1_ID].stream_type==0x06) //None or private
                        ((File_Dts*)Streams_Private1[private_stream_1_ID].Parser3)->Frame_Count_Valid=2;
                #else
                    Streams_Private1[private_stream_1_ID].Parser2=new File__Analyze;
                #endif
                Open_Buffer_Init(Streams_Private1[private_stream_1_ID].Parser2);
            }
            Open_Buffer_Continue(Streams_Private1[private_stream_1_ID].Parser2, Buffer+Buffer_Offset, (size_t)Element_Size);

            if (Streams_Private1[private_stream_1_ID].Parser2->Count_Get(Stream_Audio)>0)
            {
                Element_Info("DTS found, changing default parser");
                delete Streams_Private1[private_stream_1_ID].Parser; Streams_Private1[private_stream_1_ID].Parser=Streams_Private1[private_stream_1_ID].Parser2; Streams_Private1[private_stream_1_ID].Parser2=NULL;
            }
            Element_End();
        }
    }
    if (Streams_Private1[private_stream_1_ID].Parser->Count_Get(Stream_Audio)>0 && (Streams_Private1[private_stream_1_ID].Parser2!=NULL || Streams_Private1[private_stream_1_ID].Parser3!=NULL))
    {
        delete Streams_Private1[private_stream_1_ID].Parser2; Streams_Private1[private_stream_1_ID].Parser2=NULL;
        delete Streams_Private1[private_stream_1_ID].Parser3; Streams_Private1[private_stream_1_ID].Parser3=NULL;
    }

    //Disabling this Streams
    if (Streams_Private1[private_stream_1_ID].Parser->IsFinished || (!Parsing_End_ForDTS && Streams_Private1[private_stream_1_ID].Parser->IsFilled))
    {
        Streams_Private1[private_stream_1_ID].Searching_Payload=false;
        if (private_stream_1_Count>0)
            private_stream_1_Count--;
    }

    //Demux
    Element_Show();
    Demux(Buffer+Buffer_Offset+private_stream_1_Offset, (size_t)(Element_Size-private_stream_1_Offset), Ztring::ToZtring(Element_Code, 16)+_T(".")+Ztring::ToZtring(private_stream_1_ID, 16)+private_stream_1_ChooseExtension());
}

//---------------------------------------------------------------------------
void File_MpegPs::private_stream_1_Choose_DVD_ID()
{
    private_stream_1_IsDvdVideo=false;

    if (Element_Size<4)
        return;

    //Parsing
    int8u  CodecID;
    Get_B1 (CodecID,                                            "CodecID");

    //Testing
    //Subtitles (CVD)
         if (CodecID>=0x00 && CodecID<=0x0F)
    {
        private_stream_1_IsDvdVideo=true;
        private_stream_1_Offset=1;
    }
    //Subtitles (DVD)
    else if (CodecID>=0x20 && CodecID<=0x3F)
    {
        private_stream_1_IsDvdVideo=true;
        private_stream_1_Offset=1;
    }
    //Subtitles (SVCD)
    else if (CodecID>=0x70 && CodecID<=0x7F)
    {
        private_stream_1_IsDvdVideo=true;
        private_stream_1_Offset=1;
    }
    //AC-3 (OTA?)
    else if (CodecID==0x80 && CC3(Buffer+Buffer_Offset+1)==0x000000)
    {
        private_stream_1_IsDvdVideo=true; //Not sure
        private_stream_1_Offset=4;
    }
    //PCM
    else if (CodecID>=0xA0 && CodecID<=0xAF && Element_Size>=7 && Buffer[Buffer_Offset+6]==0x80)
    {
        private_stream_1_IsDvdVideo=true;
        private_stream_1_Offset=1;
    }
    //PS2-MPG
    else if (CodecID==0xFF)
    {
        int16u StreamID;
        int8u  SubID;
        Get_B1 (SubID,                                          "CodeID (part 2)");
        Get_B2 (StreamID,                                       "Stream ID");

             if ((SubID&0xFE)==0xA0) //0xFFA0 or 0xFFA1
        {
            //PS2-MPG PCM/ADPCM
            private_stream_1_Offset=4;
            private_stream_1_ID=(int8u)StreamID; //ID is maybe 2 byte long, but private_stream_1_ID is an int8u
            return;
        }
        else if (SubID==0x90) //0xFF90
        {
            //PS2-MPG AC-3 or subtitles
            private_stream_1_Offset=4;
            private_stream_1_ID=(int8u)StreamID; //ID is maybe 2 byte long, but private_stream_1_ID is an int8u
            return;
        }
        else
            return;
    }
    else
    {
        int16u Next;
        int8u  Count;
        Get_B1 (Count,                                          "Count of next frame headers");
        Get_B2 (Next,                                           "Next frame offset minus 1");

        if (Count>0 && 4+(int64u)Next+4<=Element_Size)
        {
            //Subtitles (CVD)
                 if (CodecID>=0x00 && CodecID<=0x0F)
                ; //Seems to not work with subtitles, to be confirmed
            //Subtitles (DVD)
                 if (CodecID>=0x20 && CodecID<=0x3F)
                ; //Seems to not work with subtitles, to be confirmed
            //Subtitles (SVCD)
                 if (CodecID>=0x70 && CodecID<=0x7F)
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
            //MLP
            else if (CodecID>=0xB0 && CodecID<=0xBF)
            {
                if (CC2(Buffer+Buffer_Offset+4+Next)!=0x0B77 && CC2(Buffer+Buffer_Offset+3+Next)!=0x0B77 && CC2(Buffer+Buffer_Offset+2+Next)!=0x0B77)
                    return;
            }
            //AC3+
            else if (CodecID>=0xC0 && CodecID<=0xCF)
            {
                if (CC2(Buffer+Buffer_Offset+4+Next)!=0x0B77 && CC2(Buffer+Buffer_Offset+3+Next)!=0x0B77 && CC2(Buffer+Buffer_Offset+2+Next)!=0x0B77)
                    return;
            }
            else
                return;

            private_stream_1_IsDvdVideo=true;
            private_stream_1_Offset=4;
        }
    }

    //Filling
    private_stream_1_ID=CodecID;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::private_stream_1_ChooseParser()
{
    if (FromTS)
    {
        if (format_identifier_FromTS==0x42535344) //"BSSD"
        {
            return ChooseParser_AES3(); //AES3 (SMPTE 320M)
        }
        switch (stream_type_FromTS)
        {
            case 0x80 : return ChooseParser_PCM(); //PCM
            case 0x81 :
            case 0x83 :
            case 0x84 :
            case 0x87 :
            case 0xA1 : return ChooseParser_AC3(); //AC3/AC3+
            case 0x82 :
            case 0x85 :
            case 0x86 :
            case 0xA2 : return ChooseParser_DTS(); //DTS
            case 0x90 : return ChooseParser_PGS(); //PGS from Bluray
            case 0xEA : return ChooseParser_NULL(); //VC1()
            default   : switch (descriptor_tag_FromTS)
                        {
                            case 0x56 : return ChooseParser_NULL(); //Teletext
                            case 0x59 : return ChooseParser_NULL(); //DVB Subtiles
                            case 0x6A :
                            case 0x7A :
                            case 0x81 : return ChooseParser_AC3(); //AC3/AC3+
                            case 0x7B : return ChooseParser_DTS(); //DTS
                            case 0x7C : return ChooseParser_AAC(); //AAC
                            default   :      if (Element_Size>2 && CC2(Buffer+Buffer_Offset)==0x0B77)
                                            return ChooseParser_AC3(); //AC3/AC3+
                                        else if (Element_Size>4 && CC4(Buffer+Buffer_Offset)==0x7FFE8001)
                                            return ChooseParser_DTS(); //DTS
                                        else
                                            return ChooseParser_NULL();
                        }
        }
    }
    else if (private_stream_1_IsDvdVideo)
    {
        //Subtitles (CVD)
             if (private_stream_1_ID>=0x00 && private_stream_1_ID<=0x0F)
            return ChooseParser_RLE();
        //Subtitles (DVD)
             if (private_stream_1_ID>=0x20 && private_stream_1_ID<=0x3F)
            return ChooseParser_RLE();
        //Subtitles (SVCD)
             if (private_stream_1_ID>=0x70 && private_stream_1_ID<=0x7F)
            return ChooseParser_RLE();
        //AC3
        else if (private_stream_1_ID>=0x80 && private_stream_1_ID<=0x87)
            return ChooseParser_AC3();
        //DTS
        else if (private_stream_1_ID>=0x88 && private_stream_1_ID<=0x8F)
            return ChooseParser_DTS();
        //SDDS
        else if (private_stream_1_ID>=0x90 && private_stream_1_ID<=0x97)
            return ChooseParser_DTS();
        //DTS
        else if (private_stream_1_ID>=0x98 && private_stream_1_ID<=0x9F)
            return ChooseParser_DTS();
        //PCM
        else if (private_stream_1_ID>=0xA0 && private_stream_1_ID<=0xAF)
            return ChooseParser_PCM();
        //AC3+
        else if (private_stream_1_ID>=0xC0 && private_stream_1_ID<=0xCF)
            return ChooseParser_AC3();
        else
            return ChooseParser_NULL();
    }
    else
    {
             if (Element_Size>2 && CC2(Buffer+Buffer_Offset)==0x0B77)
            return ChooseParser_AC3(); //AC3/AC3+
        else if (Element_Size>4 && CC4(Buffer+Buffer_Offset)==0x7FFE8001)
            return ChooseParser_DTS(); //DTS
        else if (Element_Size>2 && (CC2(Buffer+Buffer_Offset)&0xFFFE)==0xFFA0) //0xFFA0 or 0xFFA1
            return ChooseParser_PS2(); //PS2-MPG PCM/ADPCM
        else if (Element_Size>6 && CC2(Buffer+Buffer_Offset)==0xFF90 && CC2(Buffer+Buffer_Offset+4)==0x0B77)
            return ChooseParser_AC3(); //PS2-MPG AC-3
        else if (Element_Size>6 && CC2(Buffer+Buffer_Offset)==0xFF90 && CC2(Buffer+Buffer_Offset+4)==0x0000)
            return ChooseParser_RLE(); //PS2-MPG Subtitles
        else
            return ChooseParser_NULL();
    }
}

//---------------------------------------------------------------------------
const ZenLib::Char* File_MpegPs::private_stream_1_ChooseExtension()
{
    if (FromTS)
    {
        switch (private_stream_1_ID)
        {
            case 0x80 : return _T(".pcm"); //PCM
            case 0x81 : return _T(".ac3"); //AC3
            case 0x83 :
            case 0x87 : return _T(".dd+"); //AC3+
            case 0x86 : return _T(".dts"); //DTS
            case 0xEA : return _T(".vc1"); //DTS
            default   : return _T(".raw");
        }
    }
    else
    {
        //Subtitles
             if (private_stream_1_ID>=0x20 && private_stream_1_ID<=0x3F)
            return _T(".sub");
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
        //MLP
        else if (private_stream_1_ID>=0xB0 && private_stream_1_ID<=0xBF)
            return _T(".dd+");
        //AC3+
        else if (private_stream_1_ID>=0xC0 && private_stream_1_ID<=0xCF)
            return _T(".dd+");
        else
            return _T(".raw");
    }
}

//---------------------------------------------------------------------------
void File_MpegPs::private_stream_1_Element_Info()
{
    if (FromTS)
    {
        switch (private_stream_1_ID)
        {
            case 0x80 : Element_Info("PCM"); return;
            case 0x81 : Element_Info("AC3"); return;
            case 0x83 :
            case 0x87 : Element_Info("AC3+"); return;
            case 0x86 : Element_Info("DTS"); return;
            case 0xEA : Element_Info("VC1"); return;
            default   : return;
        }
    }
    else
    {
        //Subtitles
             if (private_stream_1_ID>=0x20 && private_stream_1_ID<=0x3F)
            Element_Info("RLE");
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
        //MLP
        else if (private_stream_1_ID>=0xB0 && private_stream_1_ID<=0xBF)
            Element_Info("MLP");
        //AC3+
        else if (private_stream_1_ID>=0xC0 && private_stream_1_ID<=0xCF)
            Element_Info("AC3+");
        //PS2
        else if (private_stream_1_ID>=0xC0 && private_stream_1_ID<=0xCF)
            Element_Info("PS2");
    }
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
    if (FromTS)
    {
        switch (format_identifier_FromTS)
        {
            case 0x54534856 : //TSHV
                                switch (stream_type_FromTS)
                                {
                                    case 0xA0 : private_stream_2_TSHV_A0(); break;
                                    case 0xA1 : private_stream_2_TSHV_A1(); break;
                                    default   : Skip_XX(Element_Size, "Unknown");
                                }
            default         : Skip_XX(Element_Size,             "Unknown");
        }

        //Disabling the program
        if (!IsAccepted)
            Data_Accept("MPEG-PS");
    }
    else //DVD?
    {
        Stream_Prepare(Stream_Menu);
        Fill(Stream_Menu, StreamPos_Last, Menu_Format, "DVD-Video");
        Fill(Stream_Menu, StreamPos_Last, Menu_Codec, "DVD-Video");

        //Disabling this Stream
        Streams[0xBF].Searching_Payload=false;
        private_stream_2_Count=false;
    }
}

//---------------------------------------------------------------------------
void File_MpegPs::private_stream_2_TSHV_A0()
{
    Element_Name("Digital Video A0");

    //Parsing
    Skip_XX(Element_Size,                                       "Unknown");

    //Filling
    IsFilled=true;
    Finish("MPEG-PS");
}

//---------------------------------------------------------------------------
void File_MpegPs::private_stream_2_TSHV_A1()
{
    Element_Name("Digital Video A1");

    //Parsing
    int8u day, month, year, second, minute, hour;
    Skip_XX(31,                                                 "Unknown");
    BS_Begin();
    Skip_S1(2,                                                  "Unknown");
    Skip_S1(6,                                                  "timecode_frame");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(7,                                                  "timecode_second");
    Skip_S1(1,                                                  "Unknown");
    Skip_S1(7,                                                  "timecode_minute");
    Skip_S1(2,                                                  "Unknown");
    Skip_S1(6,                                                  "timecode_hour");
    Skip_S1(8,                                                  "Unknown");
    Skip_S1(2,                                                  "Unknown");
    Get_S1 (6, day,                                             "day");
    Skip_S1(3,                                                  "Unknown");
    Get_S1 (5, month,                                           "month");
    Get_S1 (8, year,                                            "year");
    Skip_S1(8,                                                  "Unknown");
    Skip_S1(1,                                                  "Unknown");
    Get_S1 (7, second,                                          "second");
    Skip_S1(1,                                                  "Unknown");
    Get_S1 (7, minute,                                          "minute");
    Skip_S1(2,                                                  "Unknown");
    Get_S1 (6, hour,                                            "hour");
    Skip_S1(2,                                                  "Unknown");
    Skip_S1(1,                                                  "scene_start");
    Skip_S1(5,                                                  "Unknown");
    BS_End();
    Skip_XX(Element_Size-Element_Offset,                        "Unknown");

    FILLING_BEGIN();
        Ztring Date_Time=Ztring().Date_From_Numbers(year/0x10*10+year%0x10, month/0x10*10+month%0x10, day/0x10*10+day%0x10, hour/0x10*10+hour%0x10, minute/0x10*10+minute%0x10, second/0x10*10+second%0x10);
        if (Retrieve(Stream_General, 0, General_Encoded_Date).empty())
        {
            Fill(Stream_General, 0, General_Encoded_Date, Date_Time);
            Fill(Stream_General, 0, General_Duration_Start, Date_Time);
        }
        Fill(Stream_General, 0, General_Duration_End, Date_Time, true);
    FILLING_END();
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
        extension_stream_Count=0;
        Streams[start_code].stream_type=stream_type_FromTS;
    }

    //Exists
    Streams[start_code].StreamIsRegistred=true;

    //If we have no Streams map --> Registering the Streams as MPEG Audio
    if (Streams[start_code].stream_type==0)
    {
        if (MPEG_Version==2)
            Streams[start_code].stream_type=0x04; //MPEG-2 Audio
        else
            Streams[start_code].stream_type=0x03; //MPEG-1 Audio
    }

    //New Streams if needed
    if (Streams[start_code].Parser==NULL)
    {
             if (Streams[start_code].stream_type==0x03 || Streams[start_code].stream_type==0x04)
            Streams[start_code].Parser=ChooseParser_Mpega();
        else if (Streams[start_code].stream_type==0x0F)
            Streams[start_code].Parser=ChooseParser_Adts();
        else
        {
            Streams[start_code].Parser=ChooseParser_NULL();
            Open_Buffer_Init(Streams[start_code].Parser);
            Streams[start_code].Parser->Stream_Prepare(Stream_Audio); //We are sure this is audio
        }
        Open_Buffer_Init(Streams[start_code].Parser);
        if (!IsAccepted)
            Data_Accept("MPEG-PS");
    }

    //Parsing
    if (Parsing_End_ForDTS && ((Streams[start_code].TimeStamp_End.DTS.TimeStamp!=(int64u)-1 && DTS!=(int64u)-1) || (Streams[start_code].TimeStamp_End.PTS.TimeStamp!=(int64u)-1 && PTS!=(int64u)-1)))
        Streams[start_code].FrameCount_AfterLast_TimeStamp_End=0;
    Open_Buffer_Continue(Streams[start_code].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);
    if (Parsing_End_ForDTS)
        Streams[start_code].FrameCount_AfterLast_TimeStamp_End+=Streams[start_code].Parser->Frame_Count_InThisBlock;

    //Testing other parsers in case of need
    if ((stream_type_FromTS==0 || stream_type_FromTS==6) && Streams[start_code].Parser->Count_Get(Stream_Audio)==0)
    {
        bool WantShow2=Element_Show_Get();
        Element_Begin("Testing ADTS...");
        if (Streams[start_code].Parser2==NULL)
        {
            #if defined(MEDIAINFO_ADTS_YES)
                Streams[start_code].Parser2=new File_Adts;
            #else
                Streams[start_code].Parser2=new File__Analyze;
            #endif
            Open_Buffer_Init(Streams[start_code].Parser2);
        }
        Open_Buffer_Continue(Streams[start_code].Parser2, Buffer+Buffer_Offset, (size_t)Element_Size);

        if (Streams[start_code].Parser2->Count_Get(Stream_Audio)>0)
        {
            Element_Info("ADTS Video found, changing default parser");
            delete Streams[start_code].Parser; Streams[start_code].Parser=Streams[start_code].Parser2; Streams[start_code].Parser2=NULL;
        }
        Element_End();
        if (WantShow2)
            Element_Show();
    }
    if (Streams[start_code].Parser->Count_Get(Stream_Audio)>0 && Streams[start_code].Parser2!=NULL)
    {
        delete Streams[start_code].Parser2; Streams[start_code].Parser2=NULL;
    }

    //Disabling this Streams
    if (Streams[start_code].Parser->IsFinished || (!Parsing_End_ForDTS && Streams[start_code].Parser->IsFilled))
    {
        Streams[start_code].Searching_Payload=false;
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
        extension_stream_Count=0;
        Streams[start_code].stream_type=stream_type_FromTS;
    }

    //Exists
    Streams[start_code].StreamIsRegistred=true;

    //If we have no Streams map --> Registering the Streams as MPEG Video
    if (Streams[start_code].stream_type==0)
    {
        if (MPEG_Version==2)
            Streams[start_code].stream_type=0x02; //MPEG-2 Video
        else
            Streams[start_code].stream_type=0x01; //MPEG-1 Video
    }

    //New Streams if needed
    if (Streams[start_code].Parser==NULL)
    {
             if (Streams[start_code].stream_type==0x01 || Streams[start_code].stream_type==0x02 || Streams[start_code].stream_type==0x80 || Streams[start_code].stream_type==0x00)
            Streams[start_code].Parser=ChooseParser_Mpegv();
        else if (Streams[start_code].stream_type==0x10)
            Streams[start_code].Parser=ChooseParser_Mpeg4v();
        else if (Streams[start_code].stream_type==0x1B)
            Streams[start_code].Parser=ChooseParser_Avc();
        else
            Streams[start_code].Parser=ChooseParser_Mpegv(); //Trying by default
        Streams[start_code].Parser->ShouldContinueParsing=true;
        Open_Buffer_Init(Streams[start_code].Parser);
        if (!IsAccepted)
            Data_Accept("MPEG-PS");
    }

    //PTS/DTS
    if (PTS!=(int64u)-1)
        Streams[start_code].Parser->PTS=PTS*1000000/90;
    if (DTS!=(int64u)-1)
        Streams[start_code].Parser->DTS=DTS*1000000/90;

    //Parsing
    if (Parsing_End_ForDTS && ((Streams[start_code].TimeStamp_End.DTS.TimeStamp!=(int64u)-1 && DTS!=(int64u)-1) || (Streams[start_code].TimeStamp_End.PTS.TimeStamp!=(int64u)-1 && PTS!=(int64u)-1)))
        Streams[start_code].FrameCount_AfterLast_TimeStamp_End=0;
    Open_Buffer_Continue(Streams[start_code].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);
    if (Parsing_End_ForDTS)
        Streams[start_code].FrameCount_AfterLast_TimeStamp_End+=Streams[start_code].Parser->Frame_Count_InThisBlock;

    //Testing other parsers in case of need
    if ((stream_type_FromTS==0 || stream_type_FromTS==6) && Streams[start_code].Parser->Count_Get(Stream_Video)==0)
    {
        bool WantShow1=Element_Show_Get();
        Element_Begin("Testing AVC...");
        if (Streams[start_code].Parser3==NULL)
        {
            #if defined(MEDIAINFO_AVC_YES)
                Streams[start_code].Parser3=new File_Avc;
            #else
                Streams[start_code].Parser3=new File__Analyze;
            #endif
            Open_Buffer_Init(Streams[start_code].Parser3);
        }
        if (!Streams[start_code].Parser3->IsFinished)
        {
            Open_Buffer_Continue(Streams[start_code].Parser3, Buffer+Buffer_Offset, (size_t)Element_Size);
        }

        if (Streams[start_code].Parser3 && Streams[start_code].Parser3->Count_Get(Stream_Video)>0)
        {
            Element_Info("AVC found, changing default parser");
            Element_End();
            delete Streams[start_code].Parser; Streams[start_code].Parser=Streams[start_code].Parser3; Streams[start_code].Parser3=NULL;
        }
        else
        {
            Element_End();
            //bool WantShow2=Element_Show_Get();
            if (WantShow1)
                Element_Show();
            Element_Begin("Testing MPEG-4 Video...");
            if (Streams[start_code].Parser2==NULL)
            {
                #if defined(MEDIAINFO_MPEG4V_YES)
                    Streams[start_code].Parser2=new File_Mpeg4v;
                #else
                    Streams[start_code].Parser2=new File__Analyze;
                #endif
                Open_Buffer_Init(Streams[start_code].Parser2);
            }
            if (!Streams[start_code].Parser2->IsFinished)
            {
                Open_Buffer_Continue(Streams[start_code].Parser2, Buffer+Buffer_Offset, (size_t)Element_Size);
            }

            if (Streams[start_code].Parser2 && Streams[start_code].Parser2->Count_Get(Stream_Video)>0)
            {
                Element_Info("MPEG-4 Video found, changing default parser");
                delete Streams[start_code].Parser; Streams[start_code].Parser=Streams[start_code].Parser2; Streams[start_code].Parser2=NULL;
                Element_End();
            }
            else
            {
                Element_End();
                //bool WantShow3=Element_Show_Get();
                //if (WantShow1 || WantShow2)
                    Element_Show();
                Element_Begin("Testing AVS Video...");
                if (Streams[start_code].Parser4==NULL)
                {
                    #if defined(MEDIAINFO_AVSV_YES)
                        Streams[start_code].Parser4=new File_AvsV;
                    #else
                        Streams[start_code].Parser4=new File__Analyze;
                    #endif
                    Open_Buffer_Init(Streams[start_code].Parser4);
                }
                if (!Streams[start_code].Parser4->IsFinished)
                {
                    Open_Buffer_Continue(Streams[start_code].Parser4, Buffer+Buffer_Offset, (size_t)Element_Size);
                }

                if (Streams[start_code].Parser4 && Streams[start_code].Parser4->Count_Get(Stream_Video)>0)
                {
                    Element_Info("AVS Video found, changing default parser");
                    delete Streams[start_code].Parser; Streams[start_code].Parser=Streams[start_code].Parser4; Streams[start_code].Parser4=NULL;
                }
                Element_End();
                //if (WantShow1 || WantShow2 || WantShow3)
                    Element_Show();
            }
        }
    }
    if (Streams[start_code].Parser->Count_Get(Stream_Video)>0 && (Streams[start_code].Parser2!=NULL || Streams[start_code].Parser3!=NULL || Streams[start_code].Parser4!=NULL))
    {
        delete Streams[start_code].Parser2; Streams[start_code].Parser2=NULL;
        delete Streams[start_code].Parser3; Streams[start_code].Parser3=NULL;
        delete Streams[start_code].Parser4; Streams[start_code].Parser4=NULL;
    }

    //Disabling this Streams
    if (Streams[start_code].Parser->IsFinished || (!Parsing_End_ForDTS && Streams[start_code].Parser->IsFilled))
    {
        Streams[start_code].Searching_Payload=false;
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
        extension_stream_Count=0;
        Streams[start_code].stream_type=stream_type_FromTS;
    }

    //Exists
    Streams[start_code].StreamIsRegistred=true;
    if (!IsAccepted)
        Data_Accept("MPEG-PS");

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
    Streams[start_code].Parser=new File__Analyze();
    Open_Buffer_Init(Streams[start_code].Parser);
    Streams[start_code].Parser->Stream_Prepare(Stream_Audio);
    Streams[start_code].Parser->Fill(Stream_Audio, StreamPos_Last, Audio_Format, "AAC");
    Streams[start_code].Parser->Fill(Stream_Audio, StreamPos_Last, Audio_Codec, "AAC");
    Streams[start_code].Parser->Fill(Stream_Audio, StreamPos_Last, Audio_MuxingMode, "LATM");
    Streams[start_code].Parser->Accept();
    Streams[start_code].Parser->Finish();

    //Disabling this Streams
    Streams[start_code].Searching_Payload=false;
    if (audio_stream_Count>0)
        audio_stream_Count--;

    //Demux
    Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(Element_Code, 16)+_T(".latm"));
}

//---------------------------------------------------------------------------
// Packet "FD"
void File_MpegPs::extension_stream()
{
    Element_Name("With Extension");
    Element_Info(MpegPs_stream_id_extension(stream_id_extension));

    //For TS streams, which does not have Start chunk
    if (FromTS)
    {
        video_stream_Count=0;
        audio_stream_Count=0;
        private_stream_1_Count=0;
        private_stream_2_Count=false;
        extension_stream_Count=1;
    }

    //Exists
    Streams[start_code].StreamIsRegistred=true;

    //New Streams if needed
    int8u Extension=stream_id_extension;
    if (Extension==0x72)
        Extension=0x71; //0x72 is extension of 0x71 (example : DTS-HD)
    if (Extension==0x76)
        Extension=0x71; //0x76 is extension of 0x71 (example : TrueHD)
    if (Streams_Extension[Extension].Parser==NULL)
    {
            if ((Extension>=0x55 && Extension<=0x5F)
             || (Extension==0x75 && Extension<=0x7F))
             Streams_Extension[Extension].Parser=ChooseParser_VC1();
        else if (Extension>=0x60 && Extension<=0x6F)
             Streams_Extension[Extension].Parser=ChooseParser_Dirac();
        else if (Extension==0x71)
            Streams_Extension[Extension].Parser=private_stream_1_ChooseParser();
        else
            Streams_Extension[Extension].Parser=new File__Analyze();
        Streams_Extension[Extension].Searching_TimeStamp_Start=true;
        Open_Buffer_Init(Streams_Extension[Extension].Parser);
        if (!IsAccepted)
            Data_Accept("MPEG-PS");
    }
    if (Searching_TimeStamp_Start && Streams_Extension[Extension].Searching_TimeStamp_Start)
    {
        Streams_Extension[Extension].TimeStamp_Start=Streams[0xFD].TimeStamp_Start;
        Streams_Extension[Extension].Searching_TimeStamp_Start=false;
    }
    Streams_Extension[Extension].TimeStamp_End=Streams[0xFD].TimeStamp_End;

    //Parsing
    if (Parsing_End_ForDTS && ((Streams_Extension[Extension].TimeStamp_End.DTS.TimeStamp!=(int64u)-1 && DTS!=(int64u)-1) || (Streams_Extension[Extension].TimeStamp_End.PTS.TimeStamp!=(int64u)-1 && PTS!=(int64u)-1)))
        Streams_Extension[Extension].FrameCount_AfterLast_TimeStamp_End=0;
    Open_Buffer_Continue(Streams_Extension[Extension].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);
    if (Parsing_End_ForDTS)
        Streams_Extension[Extension].FrameCount_AfterLast_TimeStamp_End+=Streams_Extension[Extension].Parser->Frame_Count_InThisBlock;

    //Disabling this Streams
    if (Streams_Extension[Extension].Parser->IsFinished ||(!Parsing_End_ForDTS && Streams_Extension[Extension].Parser->IsFilled))
    {
        Streams_Extension[Extension].Searching_Payload=false;
        if (extension_stream_Count>0)
            extension_stream_Count--;
    }

    //Demux
    Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(Element_Code, 16)+_T('.')+Ztring::ToZtring(stream_id_extension, 16)+extension_stream_ChooseExtension());
}

//---------------------------------------------------------------------------
const ZenLib::Char* File_MpegPs::extension_stream_ChooseExtension()
{
    //AC3
        if ((stream_id_extension>=0x55 && stream_id_extension<=0x5F)
         || (stream_id_extension==0x75 && stream_id_extension<=0x7F))
        return _T(".vc1");
    //AC3+
    else if (stream_id_extension>=0x60 && stream_id_extension<=0x6F)
        return _T(".dirac");
    else if (stream_id_extension==0x71)
        return private_stream_1_ChooseExtension();
    else
        return _T(".raw");
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegPs::Header_Parser_QuickSearch()
{
    while (           Buffer_Offset+4<=Buffer_Size
      &&   CC3(Buffer+Buffer_Offset)==0x000001)
    {
        //Getting start_code
        int8u start_code=Buffer[Buffer_Offset+3];

        //Searching start
        if (Streams[start_code].Searching_Payload)
        {
            if (start_code!=0xBD || !private_stream_1_IsDvdVideo) //Not (private_stream_1 and IsDvdVideo)
                return true;

            //private_stream_1 and IsDvdVideo, looking for substream ID
            if (Buffer_Offset+9>Buffer_Size)
                return false; //Need more data
            size_t Data_Offset=Buffer[Buffer_Offset+8];
            if (Buffer_Offset+9+Data_Offset>Buffer_Size)
                return false; //Need more data
            int8u  private_stream_1_ID=Buffer[Buffer_Offset+9+Data_Offset];
            if (!Streams_Private1[private_stream_1_ID].StreamIsRegistred || Streams_Private1[private_stream_1_ID].Searching_Payload)
                return true;
        }

        //Searching TimeStamp_End
        if (Streams[start_code].Searching_TimeStamp_End)
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
                case 0xF2 : //DSMCC Streams
                case 0xF8 : //ITU-T Rec. H .222.1 type E
                case 0xFF : //Program Streams directory
                    break;

                //Element with PES Header
                default :
                    if (MPEG_Version==1)
                    {
                        size_t Buffer_Offset_Temp=Buffer_Offset+6;
                        while(Buffer_Offset_Temp<=Buffer_Size && Buffer[Buffer_Offset_Temp]==0xFF)
                        {
                            Buffer_Offset_Temp++;
                            if (Buffer_Offset_Temp+1>=Buffer_Size)
                                return false; //Not enough data
                        }
                        if (Buffer_Offset_Temp+1>=Buffer_Size)
                            return false; //Not enough data
                        if (Buffer_Offset_Temp<Buffer_Size && (Buffer[Buffer_Offset_Temp]&0xF0)!=0x00)
                            return true; //With a PTS
                    }
                    if (MPEG_Version==2)
                    {
                        if (Buffer_Offset+8>Buffer_Size)
                            return false; //Not enough buffer
                        if ((Buffer[Buffer_Offset+7]&0xC0)!=0x00)
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
                while(Buffer_Offset+4<=Buffer_Size && !(CC3(Buffer+Buffer_Offset)==0x000001 && Buffer[Buffer_Offset+3]>=0xB9))
                {
                    Buffer_Offset+=2;
                    while(Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset]!=0x00)
                        Buffer_Offset+=2;
                    if (Buffer_Offset<Buffer_Size && Buffer[Buffer_Offset-1]==0x00 || Buffer_Offset>=Buffer_Size)
                        Buffer_Offset--;
                }
                //Parsing last bytes if needed
                if (Buffer_Offset+4>Buffer_Size)
                {
                    if (Buffer_Offset+3==Buffer_Size && CC3(Buffer+Buffer_Offset)!=0x000001)
                        Buffer_Offset++;
                    if (Buffer_Offset+2==Buffer_Size && CC2(Buffer+Buffer_Offset)!=0x0000)
                        Buffer_Offset++;
                    if (Buffer_Offset+1==Buffer_Size && CC1(Buffer+Buffer_Offset)!=0x00)
                        Buffer_Offset++;
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
                    while(Buffer_Offset+4<=Buffer_Size && !(CC3(Buffer+Buffer_Offset)==0x000001 && Buffer[Buffer_Offset+3]>=0xB9))
                        Buffer_Offset++;
                    if (Buffer_Offset+4>Buffer_Size)
                    {
                        if (Buffer_Offset+3==Buffer_Size && CC3(Buffer+Buffer_Offset)!=0x000001)
                            Buffer_Offset++;
                        if (Buffer_Offset+2==Buffer_Size && CC2(Buffer+Buffer_Offset)!=0x0000)
                            Buffer_Offset++;
                        if (Buffer_Offset+1==Buffer_Size && CC1(Buffer+Buffer_Offset)!=0x00)
                            Buffer_Offset++;
                    }
                }
        }
    }

    if (Buffer_Offset+4<=Buffer_Size)
        Trusted_IsNot("MPEG-PS, Synchronisation lost");
    Synched=false;
    return Synchronize();
}

//***************************************************************************
// Parsers
//***************************************************************************

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_Mpegv()
{
    //Filling
    #if defined(MEDIAINFO_MPEGV_YES)
        File__Analyze* Handle=new File_Mpegv;
        ((File_Mpegv*)Handle)->MPEG_Version=MPEG_Version;
        ((File_Mpegv*)Handle)->Frame_Count_Valid=32;
        ((File_Mpegv*)Handle)->ShouldContinueParsing=true;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Video);
        Handle->Fill(Stream_Video, 0, Video_Format, "MPEG Video");
        switch (stream_type_FromTS)
        {
            case 0x01 : Handle->Fill(Stream_Video, 0, Video_Codec, "MPEG-1V");
                        Handle->Fill(Stream_Video, 0, Video_Format_Version, "Version 1"); break;
            case 0x02 : Handle->Fill(Stream_Video, 0, Video_Codec, "MPEG-2V");
                        Handle->Fill(Stream_Video, 0, Video_Format_Version, "Version 2"); break;
            default   : Handle->Fill(Stream_Video, 0, Video_Codec, "MPEG-V");
        }
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_Mpeg4v()
{
    //Filling
    #if defined(MEDIAINFO_MPEG4V_YES)
        File__Analyze* Handle=new File_Mpeg4v;
        ((File_Mpeg4v*)Handle)->Frame_Count_Valid=1;
        if (!FromTS)
            ((File_Mpeg4v*)Handle)->FrameIsAlwaysComplete=true;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Video);
        Handle->Fill(Stream_Video, 0, Video_Codec, "MPEG-4V");
        Handle->Fill(Stream_Video, 0, Video_Format, "MPEG-4 Visual");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_Avc()
{
    //Filling
    #if defined(MEDIAINFO_AVC_YES)
        File__Analyze* Handle=new File_Avc;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Video);
        Handle->Fill(Stream_Video, 0, Video_Codec,  "AVC");
        Handle->Fill(Stream_Video, 0, Video_Format, "AVC");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_VC1()
{
    //Filling
    #if defined(MEDIAINFO_VC1_YES)
        File__Analyze* Handle=new File_Vc1;
        ((File_Vc1*)Handle)->Frame_Count_Valid=30;
        if (!FromTS)
            ((File_Vc1*)Handle)->FrameIsAlwaysComplete=true;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Video);
        Handle->Fill(Stream_Video, 0, Video_Codec,  "VC-1");
        Handle->Fill(Stream_Video, 0, Video_Format, "VC-1");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_Dirac()
{
    //Filling
    #if defined(MEDIAINFO_DIRAC_YES)
        File__Analyze* Handle=new File_Dirac;
        ((File_Dirac*)Handle)->Frame_Count_Valid=1;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Video);
        Handle->Fill(Stream_Video, 0, Video_Codec,  "Dirac");
        Handle->Fill(Stream_Video, 0, Video_Format, "Dirac");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_Mpega()
{
    //Filling
    #if defined(MEDIAINFO_MPEGA_YES)
        File__Analyze* Handle=new File_Mpega;
        ((File_Mpega*)Handle)->Frame_Count_Valid=1;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Format, "MPEG Audio");
        switch (stream_type_FromTS)
        {
            case 0x03 : Handle->Fill(Stream_Audio, 0, Audio_Codec,  "MPEG-1A");
                        Handle->Fill(Stream_Audio, 0, Audio_Format_Version, "Version 1"); break;
            case 0x04 : Handle->Fill(Stream_Audio, 0, Audio_Codec,  "MPEG-2A");
                        Handle->Fill(Stream_Audio, 0, Audio_Format_Version, "Version 2"); break;
            default   : Handle->Fill(Stream_Audio, 0, Audio_Codec,  "MPEG-A"); break;
        }
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_Adts()
{
    //Filling
    #if defined(MEDIAINFO_ADTS_YES)
        File__Analyze* Handle=new File_Adts;
        ((File_Adts*)Handle)->Frame_Count_Valid=1;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Codec,  "AAC");
        Handle->Fill(Stream_Audio, 0, Audio_Format, "AAC");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_AC3()
{
    //Filling
    #if defined(MEDIAINFO_AC3_YES)
        File__Analyze* Handle=new File_Ac3();
        ((File_Ac3*)Handle)->Frame_Count_Valid=2; //2 frames to be sure
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Format, private_stream_1_ID==0x83?"E-AC-3":"AC-3");
        Handle->Fill(Stream_Audio, 0, Audio_Codec, private_stream_1_ID==0x83?"AC3+":"AC3");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_DTS()
{
    //Filling
    #if defined(MEDIAINFO_DTS_YES)
        File__Analyze* Handle=new File_Dts();
        ((File_Dts*)Handle)->Frame_Count_Valid=2;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Format, "DTS");
        Handle->Fill(Stream_Audio, 0, Audio_Codec, "DTS");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_SDDS()
{
    //Filling
    #if defined(MEDIAINFO_SDDS_YES)
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, StreamPos_Last, Audio_Format, "SDDS");
        Handle->Fill(Stream_Audio, StreamPos_Last, Audio_Codec,  "SDDS");
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Format, "SDDS");
        Handle->Fill(Stream_Audio, 0, Audio_Codec,  "SDDS");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_AAC()
{
    //Filling
    #if defined(MEDIAINFO_ADTS_YES)
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Format, "AAC");
        Handle->Fill(Stream_Audio, 0, Audio_Codec,  "AAC");
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Format, "AAC");
        Handle->Fill(Stream_Audio, 0, Audio_Codec,  "AAC");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_PCM()
{
    //Filling
    #if defined(MEDIAINFO_PCM_YES)
        File__Analyze* Handle=new File_Pcm();
        Ztring Codec;
        switch (private_stream_1_ID)
        {
            case 0x80 : Codec=_T("M2TS"); break;
            case 0x83 : Codec=_T("EVOB"); break;
            default   : Codec=_T("VOB");
        }
        ((File_Pcm*)Handle)->Codec=Codec;
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Format, "PCM");
        Handle->Fill(Stream_Audio, 0, Audio_Codec,  "PCM");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_RLE()
{
    //Filling
    #if defined(MEDIAINFO_RLE_YES)
        File__Analyze* Handle=new File_Rle();
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Text);
        Handle->Fill(Stream_Text, 0, Text_Format, "RLE");
        Handle->Fill(Stream_Text, 0, Text_Codec,  "RLE");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_PGS()
{
    //Filling
    #if defined(MEDIAINFO_PGS_YES)
        File__Analyze* Handle=new File_Pgs();
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Text);
        Handle->Fill(Stream_Text, 0, Text_Format, "PGS");
        Handle->Fill(Stream_Text, 0, Text_Codec,  "PGS");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_AES3()
{
    //Filling
    #if defined(MEDIAINFO_AES3_YES)
        File__Analyze* Handle=new File_Aes3();
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill(Stream_Audio, 0, Audio_Format, "AES3");
        Handle->Fill(Stream_Audio, 0, Audio_Codec,  "AES3");
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_PS2()
{
    //Filling
    #if defined(MEDIAINFO_PS2A_YES)
        File__Analyze* Handle=new File_Ps2Audio();
    #else
        //Filling
        File__Analyze* Handle=new File_Unknown();
        Open_Buffer_Init(Handle);
        Handle->Stream_Prepare(Stream_Audio);
    #endif
    return Handle;
}

//---------------------------------------------------------------------------
File__Analyze* File_MpegPs::ChooseParser_NULL()
{
    //Filling
    File__Analyze* Handle=new File__Analyze();
    return Handle;
}

//***************************************************************************
// Output_Buffer
//***************************************************************************

//---------------------------------------------------------------------------
size_t File_MpegPs::Output_Buffer_Get (const String &Code)
{
    //Parsing Parsers
    for (size_t Pos=0; Pos<Streams.size(); Pos++)
    {
        if (Streams[Pos].Parser)
            if (size_t Size=Streams[Pos].Parser->Output_Buffer_Get(Code))
                return Size;
        if (Streams[Pos].Parser2)
            if (size_t Size=Streams[Pos].Parser2->Output_Buffer_Get(Code))
                return Size;
        if (Streams[Pos].Parser3)
            if (size_t Size=Streams[Pos].Parser3->Output_Buffer_Get(Code))
                return Size;
        if (Streams[Pos].Parser4)
            if (size_t Size=Streams[Pos].Parser4->Output_Buffer_Get(Code))
                return Size;
    }

    return 0;
}

//---------------------------------------------------------------------------
size_t File_MpegPs::Output_Buffer_Get (size_t Pos_)
{
    //Parsing Parsers
    for (size_t Pos=0; Pos<Streams.size(); Pos++)
    {
        if (Streams[Pos].Parser)
            if (size_t Size=Streams[Pos].Parser->Output_Buffer_Get(Pos_))
                return Size;
        if (Streams[Pos].Parser2)
            if (size_t Size=Streams[Pos].Parser2->Output_Buffer_Get(Pos_))
                return Size;
        if (Streams[Pos].Parser3)
            if (size_t Size=Streams[Pos].Parser3->Output_Buffer_Get(Pos_))
                return Size;
        if (Streams[Pos].Parser4)
            if (size_t Size=Streams[Pos].Parser4->Output_Buffer_Get(Pos_))
                return Size;
    }

    return 0;
}

} //Namespace

#endif //MEDIAINFO_MPEGPS_YES

