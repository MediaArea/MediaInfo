// File_Mpegts - Info for MPEG Transport Stream files
// Copyright (C) 2006-2009 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_MPEGTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_MpegTs.h"
#include "MediaInfo/Multiple/File_MpegPs.h"
#include "MediaInfo/Multiple/File_Mpeg_Descriptors.h"
#include "MediaInfo/MediaInfo_Config_MediaInfo.h"
#include <memory>
#include <algorithm>
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

namespace Elements
{
    const int32u HDMV=0x48444D56; //BluRay
}

//***************************************************************************
// Depends of configuration
//***************************************************************************

#if !defined(MEDIAINFO_BDAV_YES)
    const size_t BDAV_Size=0;
#endif
#if !defined(MEDIAINFO_TSP_YES)
    const size_t TSP_Size=0;
#endif
#if !defined(MEDIAINFO_BDAV_YES) && !defined(MEDIAINFO_TSP_YES)
    const size_t TS_Size=188;
#endif

//***************************************************************************
// Info
//***************************************************************************

//---------------------------------------------------------------------------
//From Mpeg_Psi
extern const char* Mpeg_Descriptors_registration_format_identifier_Format(int32u format_identifier);
extern stream_t    Mpeg_Descriptors_registration_format_identifier_StreamKind(int32u format_identifier);

extern const char* Mpeg_Psi_stream_type_Format(int8u stream_type, int32u format_identifier);
extern const char* Mpeg_Psi_stream_type_Codec(int8u stream_type, int32u format_identifier);
extern stream_t    Mpeg_Psi_stream_type_StreamKind(int32u stream_type, int32u format_identifier);
extern const char* Mpeg_Psi_stream_type_Info(int8u stream_type, int32u format_identifier);

extern const char* Mpeg_Psi_table_id(int8u table_id);
extern const char* Mpeg_Descriptors_stream_Format(int8u descriptor_tag, int32u format_identifier);
extern const char* Mpeg_Descriptors_stream_Codec(int8u descriptor_tag, int32u format_identifier);
extern stream_t    Mpeg_Descriptors_stream_Kind(int8u descriptor_tag, int32u format_identifier);

//---------------------------------------------------------------------------
Ztring Decimal_Hexa(int64u Number)
{
    Ztring Temp;
    Temp.From_Number(Number);
    Temp+=_T(" (0x");
    Temp+=Ztring::ToZtring(Number, 16);
    Temp+=_T(")");
    return Temp;
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_MpegTs::File_MpegTs()
:File__Duplicate()
{
    //Config
    MustSynchronize=true;
    Buffer_TotalBytes_FirstSynched_Max=64*1024;
    Trusted_Multiplier=2;

    //Internal config
    #if defined(MEDIAINFO_BDAV_YES)
        BDAV_Size=0; //No BDAV header
    #endif
    #if defined(MEDIAINFO_TSP_YES)
        TSP_Size=0; //No TSP footer
    #endif
    
    //Data
    MpegTs_JumpTo_Begin=MediaInfoLib::Config.MpegTs_MaximumOffset_Get();
    MpegTs_JumpTo_End=8*1024*1024;
    Complete_Stream=NULL;
}

File_MpegTs::~File_MpegTs ()
{
    delete Complete_Stream; Complete_Stream=NULL;

    File__Duplicate_Delete();
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::Streams_Fill()
{
    if (Complete_Stream==NULL || Complete_Stream->Streams.empty())
        return; //Not initialized
    if (!IsAccepted)
        Accept("MPEG-TS");

    //Per stream
    for (size_t StreamID=0; StreamID<0x2000; StreamID++)//std::map<int64u, stream>::iterator Stream=Streams.begin(); Stream!=Streams.end(); Stream++)
    {
        //PES
        if (Complete_Stream->Streams[StreamID].Kind==complete_stream::stream::pes)
        {
            //By the parser
            StreamKind_Last=Stream_Max;
            if (Complete_Stream->Streams[StreamID].Parser && Complete_Stream->Streams[StreamID].Parser->IsAccepted)
            {
                Complete_Stream->Streams[StreamID].Parser->Open_Buffer_Finalize();
                Merge (*Complete_Stream->Streams[StreamID].Parser);

                //More from the FMC parser
                if (Complete_Stream->Streams[StreamID].FMC_ES_ID_IsValid)
                {
                    complete_stream::transport_stream::iod_ess::iterator IOD_ES=Complete_Stream->Transport_Streams[Complete_Stream->transport_stream_id].IOD_ESs.find(Complete_Stream->Streams[StreamID].FMC_ES_ID);
                    if (IOD_ES!=Complete_Stream->Transport_Streams[Complete_Stream->transport_stream_id].IOD_ESs.end())
                    {
                        IOD_ES->second.Parser->Open_Buffer_Finalize();
                        Merge (*IOD_ES->second.Parser, StreamKind_Last, StreamPos_Last, 0);
                    }
                }
            }

            //By the descriptors
            if (StreamKind_Last==Stream_Max && Complete_Stream->transport_stream_id_IsValid)
            {
                int32u format_identifier=Complete_Stream->Transport_Streams[Complete_Stream->transport_stream_id].Programs[Complete_Stream->Streams[StreamID].program_numbers[0]].registration_format_identifier;
                if (Complete_Stream->Streams[StreamID].IsRegistered
                 && Mpeg_Descriptors_registration_format_identifier_StreamKind(format_identifier)!=Stream_Max)
                {
                    StreamKind_Last=Mpeg_Descriptors_registration_format_identifier_StreamKind(format_identifier);
                    Stream_Prepare(StreamKind_Last);
                    Fill(StreamKind_Last, StreamPos_Last, "Format", Mpeg_Descriptors_registration_format_identifier_Format(format_identifier));
                    Fill(StreamKind_Last, StreamPos_Last, "Codec", Mpeg_Descriptors_registration_format_identifier_Format(format_identifier));
                }
            }

            //By the stream_type
            if (StreamKind_Last==Stream_Max && Complete_Stream->transport_stream_id_IsValid)
            {
                int32u format_identifier=Complete_Stream->Transport_Streams[Complete_Stream->transport_stream_id].Programs[Complete_Stream->Streams[StreamID].program_numbers[0]].registration_format_identifier;
                if ((Complete_Stream->Streams[StreamID].IsRegistered ||  format_identifier==Elements::HDMV) && Mpeg_Psi_stream_type_StreamKind(Complete_Stream->Streams[StreamID].stream_type, format_identifier)!=Stream_Max)
                {
                    StreamKind_Last=Mpeg_Psi_stream_type_StreamKind(Complete_Stream->Streams[StreamID].stream_type, format_identifier);
                    if (StreamKind_Last==Stream_General) //Only information, no streams
                    {
                        Merge (*Complete_Stream->Streams[StreamID].Parser, Stream_General, 0, 0);
                        StreamKind_Last=Stream_Max;
                    }
                    Stream_Prepare(StreamKind_Last);
                    Fill(StreamKind_Last, StreamPos_Last, "Format", Mpeg_Psi_stream_type_Format(Complete_Stream->Streams[StreamID].stream_type, format_identifier));
                    Fill(StreamKind_Last, StreamPos_Last, "Codec", Mpeg_Psi_stream_type_Codec(Complete_Stream->Streams[StreamID].stream_type, format_identifier));
                }
            }

            //By the StreamKind
            if (StreamKind_Last==Stream_Max && Complete_Stream->Streams[StreamID].StreamKind!=Stream_Max && Complete_Stream->Streams[StreamID].IsRegistered)
            {
                Stream_Prepare(Complete_Stream->Streams[StreamID].StreamKind);
            }

            //More info
            if (StreamKind_Last!=Stream_Max) //Found by the Parser or stream_type
            {
                Complete_Stream->Streams[StreamID].StreamKind=StreamKind_Last;
                Complete_Stream->Streams[StreamID].StreamPos=StreamPos_Last;

                //Encryption
                if (Complete_Stream->Streams[StreamID].IsScrambled)
                    Fill(StreamKind_Last, StreamPos_Last, "Encryption", "Encrypted");

                //TS info
                for (std::map<std::string, ZenLib::Ztring>::iterator Info=Complete_Stream->Streams[StreamID].Infos.begin(); Info!=Complete_Stream->Streams[StreamID].Infos.end(); Info++)
                {
                    if (Retrieve(StreamKind_Last, StreamPos_Last, Info->first.c_str()).empty())
                        Fill(StreamKind_Last, StreamPos_Last, Info->first.c_str(), Info->second);
                }

                //Common
                Fill(StreamKind_Last, StreamPos_Last, "ID", StreamID, 10, true);
                Fill(StreamKind_Last, StreamPos_Last, "ID/String", Decimal_Hexa(StreamID), true);
                for (size_t Pos=0; Pos<Complete_Stream->Streams[StreamID].program_numbers.size(); Pos++)
                {
                    Fill(StreamKind_Last, StreamPos_Last, "MenuID", Complete_Stream->Streams[StreamID].program_numbers[Pos], 10, Pos==0);
                    Fill(StreamKind_Last, StreamPos_Last, "MenuID/String", Decimal_Hexa(Complete_Stream->Streams[StreamID].program_numbers[Pos]), Pos==0);
                }
            }

            //Desactivating the stream (except for timestamp)
            Complete_Stream->Streams[StreamID].Searching_Payload_Start_Set(false);
            Complete_Stream->Streams[StreamID].Searching_Payload_Continue_Set(false);
        }
    }

    //Fill General
    Fill(Stream_General, 0, General_Format, BDAV_Size?"BDAV":(TSP_Size?"MPEG-TS 188+16":"MPEG-TS"), Unlimited, true, true);
    if (Complete_Stream->transport_stream_id_IsValid)
    {
        Fill(Stream_General, 0, General_ID, Complete_Stream->transport_stream_id, 16);
        Fill(Stream_General, 0, General_ID_String, /*Decimal_Hexa(*/Complete_Stream->transport_stream_id/*)*/, 16);
    }
    if (!Complete_Stream->network_name.empty())
        Fill(Stream_General, 0, General_NetworkName, Complete_Stream->network_name);
    if (!Complete_Stream->original_network_name.empty())
        Fill(Stream_General, 0, General_OriginalNetworkName, Complete_Stream->original_network_name);
    Ztring Countries;
    Ztring TimeZones;
    for (std::map<Ztring, Ztring>::iterator TimeZone=Complete_Stream->TimeZones.begin(); TimeZone!=Complete_Stream->TimeZones.end(); TimeZone++)
    {
        Countries+=TimeZone->first+_T(" / ");
        TimeZones+=TimeZone->second+_T(" / ");
    }
    if (!Countries.empty())
    {
        Countries.resize(Countries.size()-3);
        Fill(Stream_General, 0, General_Country, Countries);
    }
    if (!TimeZones.empty())
    {
        TimeZones.resize(TimeZones.size()-3);
        Fill(Stream_General, 0, General_TimeZone, TimeZones);
    }
    if (!Complete_Stream->Start_Time.empty())
        Fill(Stream_General, 0, General_Duration_Start, Complete_Stream->Start_Time);
    complete_stream::transport_streams::iterator Transport_Stream=Complete_Stream->transport_stream_id_IsValid?Complete_Stream->Transport_Streams.find(Complete_Stream->transport_stream_id):Complete_Stream->Transport_Streams.end();
    if (Transport_Stream!=Complete_Stream->Transport_Streams.end())
    {
        //TS info
        for (std::map<std::string, ZenLib::Ztring>::iterator Info=Transport_Stream->second.Infos.begin(); Info!=Transport_Stream->second.Infos.end(); Info++)
            Fill(Stream_General, 0, Info->first.c_str(), Info->second);

        //Per source (ATSC)
        std::map<Ztring, Ztring> EPGs;
        complete_stream::sources::iterator Source=Complete_Stream->Sources.find(Transport_Stream->second.source_id);
        if (Source!=Complete_Stream->Sources.end())
        {
            if (!Source->second.texts.empty())
            {
                Ztring Texts;
                for (std::map<int16u, Ztring>::iterator text=Source->second.texts.begin(); text!=Source->second.texts.end(); text++)
                    Texts+=text->second+_T(" - ");
                if (!Texts.empty())
                    Texts.resize(Texts.size()-3);
                Fill(Stream_General, 0, General_ServiceProvider, Texts);
            }
        }

        //Per program
        if (!Transport_Stream->second.Programs.empty()
         && (Transport_Stream->second.Programs.size()>1
          || !Transport_Stream->second.Programs.begin()->second.Infos.empty()
          || !Transport_Stream->second.Programs.begin()->second.DVB_EPG_Blocks.empty()
          || Complete_Stream->Sources.find(Transport_Stream->second.Programs.begin()->second.source_id)!=Complete_Stream->Sources.end()
          || Config->File_MpegTs_ForceMenu_Get()))
            for (complete_stream::transport_stream::programs::iterator Program=Transport_Stream->second.Programs.begin(); Program!=Transport_Stream->second.Programs.end(); Program++)
            {
                if (Program->second.IsParsed)
                {
                    Stream_Prepare(Stream_Menu);
                    Program->second.StreamPos=StreamPos_Last;
                    Fill(Stream_Menu, StreamPos_Last, Menu_ID, Program->second.pid);
                    Fill(Stream_Menu, StreamPos_Last, Menu_ID_String, Decimal_Hexa(Program->second.pid));
                    Fill(Stream_Menu, StreamPos_Last, Menu_MenuID, Program->first);
                    Fill(Stream_Menu, StreamPos_Last, Menu_MenuID_String, Decimal_Hexa(Program->first));
                    for (std::map<std::string, ZenLib::Ztring>::iterator Info=Program->second.Infos.begin(); Info!=Program->second.Infos.end(); Info++)
                        Fill(Stream_Menu, StreamPos_Last, Info->first.c_str(), Info->second);

                    //Per PID
                    Ztring Languages;
                    for (size_t Pos=0; Pos<Program->second.elementary_PIDs.size(); Pos++)
                    {
                        int16u elementary_PID=Program->second.elementary_PIDs[Pos];
                        if (Complete_Stream->Streams[elementary_PID].IsRegistered)
                        {
                            Ztring Format=Retrieve(Complete_Stream->Streams[elementary_PID].StreamKind, Complete_Stream->Streams[elementary_PID].StreamPos, "Format");
                            Fill(Stream_Menu, StreamPos_Last, Menu_Format, Format);
                            Fill(Stream_Menu, StreamPos_Last, Menu_Codec, Retrieve(Complete_Stream->Streams[elementary_PID].StreamKind, Complete_Stream->Streams[elementary_PID].StreamPos, "Codec"));
                            Fill(Stream_Menu, StreamPos_Last, Menu_List_StreamKind, Complete_Stream->Streams[elementary_PID].StreamKind);
                            Fill(Stream_Menu, StreamPos_Last, Menu_List_StreamPos, Complete_Stream->Streams[elementary_PID].StreamPos);
                            Fill(Stream_Menu, StreamPos_Last, Menu_List, elementary_PID);
                            Ztring Language=Retrieve(Complete_Stream->Streams[elementary_PID].StreamKind, Complete_Stream->Streams[elementary_PID].StreamPos, "Language");
                            Languages+=Language+_T(" / ");
                            Ztring List_String=Decimal_Hexa(elementary_PID);
                            List_String+=_T(" (");
                            List_String+=Format;
                            if (!Language.empty())
                            {
                                List_String+=_T(", ");
                                List_String+=Language;
                            }
                            List_String+=_T(")");
                            Fill(Stream_Menu, StreamPos_Last, Menu_List_String, List_String);
                        }
                    }
                    if (!Languages.empty())
                        Languages.resize(Languages.size()-3);
                    Fill(Stream_Menu, StreamPos_Last, Menu_Language, Languages);
                }
            }
    }

    File__Duplicate_Read_Buffer_Finalize();
}

//---------------------------------------------------------------------------
void File_MpegTs::Streams_Update()
{

    if (Complete_Stream==NULL || Complete_Stream->Streams.empty())
        return; //Not initialized

    //General
    if (Complete_Stream->End_Time_IsUpdated)
    {
        Fill(Stream_General, 0, General_Duration_End, Complete_Stream->End_Time, true);
        Complete_Stream->End_Time_IsUpdated=false;
    }

    //Per stream
    bool PCR_Found=false;
    int64u Duration_Max=0;
    for (size_t StreamID=0; StreamID<0x2000; StreamID++)//std::map<int64u, stream>::iterator Stream=Streams.begin(); Stream!=Streams.end(); Stream++)
    {
        //PES
        if (Complete_Stream->Streams[StreamID].Kind==complete_stream::stream::pes)
        {
            StreamKind_Last=Complete_Stream->Streams[StreamID].StreamKind;
            StreamPos_Last=Complete_Stream->Streams[StreamID].StreamPos;

            //More info
            if (StreamKind_Last!=Stream_Max)
            {
                #ifdef MEDIAINFO_MPEGTS_PCR_YES
                    //TimeStamp (PCR)
                    if (/*Retrieve(StreamKind_Last, StreamPos_Last, "Duration").empty()
                     && */Complete_Stream->Streams[StreamID].TimeStamp_Start!=(int64u)-1
                     && Complete_Stream->Streams[StreamID].TimeStamp_End!=(int64u)-1)
                    {
                        if (Complete_Stream->Streams[StreamID].TimeStamp_End<Complete_Stream->Streams[StreamID].TimeStamp_Start)
                            Complete_Stream->Streams[StreamID].TimeStamp_End+=0x200000000LL; //33 bits, cyclic
                        int64u Duration=Complete_Stream->Streams[StreamID].TimeStamp_End-Complete_Stream->Streams[StreamID].TimeStamp_Start;
                        if (Duration!=0 && Duration!=(int64u)-1)
                        {
                            Fill(StreamKind_Last, StreamPos_Last, "Duration", Duration/90, 10, true);
                            if (Duration_Max<Duration)
                                Duration_Max=Duration;
                        }
                        else
                            Clear(StreamKind_Last, StreamPos_Last, "Duration");
                    }
                #endif //MEDIAINFO_MPEGTS_PCR_YES
            }
        }

        //PCR
        #ifdef MEDIAINFO_MPEGTS_PCR_YES
            if (Complete_Stream->Streams[StreamID].Kind==complete_stream::stream::pcr)
            {
                if (Complete_Stream->Streams[StreamID].TimeStamp_Start!=(int64u)-1
                 && Complete_Stream->Streams[StreamID].TimeStamp_End!=(int64u)-1)
                {
                    if (Complete_Stream->Streams[StreamID].TimeStamp_End<Complete_Stream->Streams[StreamID].TimeStamp_Start)
                        Complete_Stream->Streams[StreamID].TimeStamp_End+=0x200000000LL; //33 bits, cyclic
                    int64u Duration=Complete_Stream->Streams[StreamID].TimeStamp_End-Complete_Stream->Streams[StreamID].TimeStamp_Start;
                    if (Duration!=0 && Duration!=(int64u)-1)
                    {
                        Fill(Stream_General, 0, General_Duration, Duration/90, 10, true);
                        PCR_Found=true;
                    }
                }
            }
        #endif //MEDIAINFO_MPEGTS_PCR_YES
    }
    if (!PCR_Found && Duration_Max)
        Fill(Stream_General, 0, General_Duration, Duration_Max/90, 10, true);

    //EPG
    complete_stream::transport_streams::iterator Transport_Stream=Complete_Stream->transport_stream_id_IsValid?Complete_Stream->Transport_Streams.find(Complete_Stream->transport_stream_id):Complete_Stream->Transport_Streams.end();
    if (Transport_Stream!=Complete_Stream->Transport_Streams.end())
    {
        //Per source (ATSC)
        std::map<Ztring, Ztring> EPGs;
        complete_stream::sources::iterator Source=Complete_Stream->Sources.find(Transport_Stream->second.source_id);
        if (Source!=Complete_Stream->Sources.end())
        {
            //EPG
            for (complete_stream::source::atsc_epg_blocks::iterator ATSC_EPG_Block=Source->second.ATSC_EPG_Blocks.begin(); ATSC_EPG_Block!=Source->second.ATSC_EPG_Blocks.end(); ATSC_EPG_Block++)
                for (complete_stream::source::atsc_epg_block::events::iterator Event=ATSC_EPG_Block->second.Events.begin(); Event!=ATSC_EPG_Block->second.Events.end(); Event++)
                {
                    Ztring Texts;
                    for (std::map<int16u, Ztring>::iterator text=Event->second.texts.begin(); text!=Event->second.texts.end(); text++)
                        Texts+=text->second+_T(" - ");
                    if (!Texts.empty())
                        Texts.resize(Texts.size()-3);
                    EPGs[Ztring().Date_From_Seconds_1970(Event->second.start_time+315964800-Complete_Stream->GPS_UTC_offset)]=Event->second.title+_T(" / ")+Texts+_T(" /  /  / ")+Event->second.duration+_T(" / ");
                }
            if (!EPGs.empty())
            {
                Fill(Stream_General, 0, General_EPG_Positions_Begin, Count_Get(Stream_General, 0), 10, true);
                for (std::map<Ztring, Ztring>::iterator EPG=EPGs.begin(); EPG!=EPGs.end(); EPG++)
                    Fill(Stream_General, 0, EPG->first.To_Local().c_str(), EPG->second, true);
                Fill(Stream_General, 0, General_EPG_Positions_End, Count_Get(Stream_General, 0), 10, true);
            }
        }

        //Per program
        if (!Transport_Stream->second.Programs.empty()
         && (Transport_Stream->second.Programs.size()>1
          || !Transport_Stream->second.Programs.begin()->second.Infos.empty()
          || !Transport_Stream->second.Programs.begin()->second.DVB_EPG_Blocks.empty()
          || Complete_Stream->Sources.find(Transport_Stream->second.Programs.begin()->second.source_id)!=Complete_Stream->Sources.end()
          || Config->File_MpegTs_ForceMenu_Get()))
            for (complete_stream::transport_stream::programs::iterator Program=Transport_Stream->second.Programs.begin(); Program!=Transport_Stream->second.Programs.end(); Program++)
            {
                if (Program->second.IsParsed)
                {
                    bool EPGs_IsUpdated=false;

                    //EPG - DVB
                    if (Program->second.DVB_EPG_Blocks_IsUpdated)
                    {
                        for (complete_stream::transport_stream::program::dvb_epg_blocks::iterator DVB_EPG_Block=Program->second.DVB_EPG_Blocks.begin(); DVB_EPG_Block!=Program->second.DVB_EPG_Blocks.end(); DVB_EPG_Block++)
                            for (complete_stream::transport_stream::program::dvb_epg_block::events::iterator Event=DVB_EPG_Block->second.Events.begin(); Event!=DVB_EPG_Block->second.Events.end(); Event++)
                                EPGs[Event->second.start_time]=Event->second.short_event.event_name+_T(" / ")+Event->second.short_event.text+_T(" / ")+Event->second.content+_T(" /  / ")+Event->second.duration+_T(" / ")+Event->second.running_status;
                        Program->second.DVB_EPG_Blocks_IsUpdated=false;
                        EPGs_IsUpdated=true;
                    }

                    //EPG - ATSC
                    complete_stream::sources::iterator Source=Complete_Stream->Sources.find(Program->second.source_id);
                    if (Source!=Complete_Stream->Sources.end())
                    {
                        if (!Source->second.texts.empty())
                        {
                            Ztring Texts;
                            for (std::map<int16u, Ztring>::iterator text=Source->second.texts.begin(); text!=Source->second.texts.end(); text++)
                                Texts+=text->second+_T(" - ");
                            if (!Texts.empty())
                                Texts.resize(Texts.size()-3);
                            if (Program->second.StreamPos==(size_t)-1)
                            {
                                Stream_Prepare(Stream_Menu);
                                Program->second.StreamPos=StreamPos_Last;
                            }
                            Fill(Stream_Menu, Program->second.StreamPos, Menu_ServiceProvider, Texts, true);
                        }
                        if (Source->second.ATSC_EPG_Blocks_IsUpdated)
                        {
                            for (complete_stream::source::atsc_epg_blocks::iterator ATSC_EPG_Block=Source->second.ATSC_EPG_Blocks.begin(); ATSC_EPG_Block!=Source->second.ATSC_EPG_Blocks.end(); ATSC_EPG_Block++)
                                for (complete_stream::source::atsc_epg_block::events::iterator Event=ATSC_EPG_Block->second.Events.begin(); Event!=ATSC_EPG_Block->second.Events.end(); Event++)
                                {
                                    Ztring Texts;
                                    for (std::map<int16u, Ztring>::iterator text=Event->second.texts.begin(); text!=Event->second.texts.end(); text++)
                                        Texts+=text->second+_T(" - ");
                                    if (!Texts.empty())
                                        Texts.resize(Texts.size()-3);
                                    EPGs[Ztring().Date_From_Seconds_1970(Event->second.start_time+315964800-Complete_Stream->GPS_UTC_offset)]=Event->second.title+_T(" / ")+Texts+_T(" /  /  / ")+Event->second.duration+_T(" / ");
                                }
                            Source->second.ATSC_EPG_Blocks_IsUpdated=false;
                            EPGs_IsUpdated=true;
                        }
                    }

                    //EPG - Filling
                    if (EPGs_IsUpdated)
                    {
                        if (Program->second.StreamPos==(size_t)-1)
                        {
                            Stream_Prepare(Stream_Menu);
                            Program->second.StreamPos=StreamPos_Last;
                        }
                        size_t Chapters_Pos_Begin=Retrieve(Stream_Menu, Program->second.StreamPos, Menu_Chapters_Pos_Begin).To_int32u();
                        size_t Chapters_Pos_End=Retrieve(Stream_Menu, Program->second.StreamPos, Menu_Chapters_Pos_End).To_int32u();
                        if (Chapters_Pos_Begin && Chapters_Pos_End)
                        {
                            for (size_t Pos=Chapters_Pos_End-1; Pos>=Chapters_Pos_Begin; Pos--)
                                Clear(Stream_Menu, Program->second.StreamPos, Pos);
                            Clear(Stream_Menu, Program->second.StreamPos, Menu_Chapters_Pos_Begin);
                            Clear(Stream_Menu, Program->second.StreamPos, Menu_Chapters_Pos_End);
                        }
                        Fill(Stream_Menu, Program->second.StreamPos, Menu_Chapters_Pos_Begin, Count_Get(Stream_Menu, Program->second.StreamPos), 10, true);
                        for (std::map<Ztring, Ztring>::iterator EPG=EPGs.begin(); EPG!=EPGs.end(); EPG++)
                            Fill(Stream_Menu, Program->second.StreamPos, EPG->first.To_UTF8().c_str(), EPG->second, true);
                        Fill(Stream_Menu, Program->second.StreamPos, Menu_Chapters_Pos_End, Count_Get(Stream_Menu, Program->second.StreamPos), 10, true);
                        EPGs.clear();
                    }
                }
            }
    }
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegTs::Synchronize()
{
    //Synchronizing
    while (       Buffer_Offset+188*7+BDAV_Size*8+TSP_Size*7+1<=Buffer_Size
      && !(Buffer[Buffer_Offset+188*0+BDAV_Size*1+TSP_Size*0]==0x47
        && Buffer[Buffer_Offset+188*1+BDAV_Size*2+TSP_Size*1]==0x47
        && Buffer[Buffer_Offset+188*2+BDAV_Size*3+TSP_Size*2]==0x47
        && Buffer[Buffer_Offset+188*3+BDAV_Size*4+TSP_Size*3]==0x47
        && Buffer[Buffer_Offset+188*4+BDAV_Size*5+TSP_Size*4]==0x47
        && Buffer[Buffer_Offset+188*5+BDAV_Size*6+TSP_Size*5]==0x47
        && Buffer[Buffer_Offset+188*6+BDAV_Size*7+TSP_Size*6]==0x47
        && Buffer[Buffer_Offset+188*7+BDAV_Size*8+TSP_Size*7]==0x47))
    {
        Buffer_Offset++;
        while (       Buffer_Offset+BDAV_Size+1<=Buffer_Size
            && Buffer[Buffer_Offset+BDAV_Size]!=0x47)
            Buffer_Offset++;
    }

    if (Buffer_Offset+188*7+BDAV_Size*8+TSP_Size*7>=Buffer_Size)
        return false;

    //Synched is OK
    return true;
}

//---------------------------------------------------------------------------
bool File_MpegTs::Synched_Test()
{
    while (Buffer_Offset+TS_Size<=Buffer_Size)
    {
        //Synchro testing
        if (Buffer[Buffer_Offset+BDAV_Size]!=0x47)
        {
             Synched=false;
             if (File__Duplicate_Get())
                 Trusted++; //We don't want to stop parsing if duplication is requested, TS is not a lot stable, normal...
             return false;
        }

        //Getting PID
        pid=(Buffer[Buffer_Offset+BDAV_Size+1]&0x1F)<<8
          |  Buffer[Buffer_Offset+BDAV_Size+2];

        complete_stream::streams::iterator Stream=Complete_Stream->Streams.begin()+pid;
        if (Stream->Searching)
        {
            payload_unit_start_indicator=(Buffer[Buffer_Offset+BDAV_Size+1]&0x40)!=0;
            if (payload_unit_start_indicator)
            {
                //Searching start
                if (Stream->Searching_Payload_Start) //payload_unit_start_indicator
                {
                    if (Stream->Kind==complete_stream::stream::psi)
                    {
                        //Searching table_id
                        size_t Version_Pos=BDAV_Size
                                          +4 //standart header
                                          +((Buffer[Buffer_Offset+BDAV_Size+3]&0x20)?(1+Buffer[Buffer_Offset+BDAV_Size+4]):0); //adaptation_field_control (adaptation) --> adaptation_field_length
                        Version_Pos+=1+Buffer[Buffer_Offset+Version_Pos]; //pointer_field
                        int8u table_id=Buffer[Buffer_Offset+Version_Pos]; //table_id
                        if (table_id==0xCD) //specifc case for ATSC STT
                            return true; //Version has no meaning
                        complete_stream::stream::table_ids::iterator Table_ID=Stream->Table_IDs.begin()+table_id;
                        if (*Table_ID)
                        {
                            //Searching table_id_extension, version_number, section_number
                            #ifndef MEDIAINFO_MINIMIZESIZE
                                Stream->Element_Info=Mpeg_Psi_table_id(table_id);
                            #endif //MEDIAINFO_MINIMIZESIZE
                            if (!(Buffer[Buffer_Offset+Version_Pos+1]&0x80)) //section_syntax_indicator
                                return true; //No version
                            Version_Pos+=3; //Header size
                            if (Version_Pos+5>=BDAV_Size+188)
                                return true; //Not able to detect version (too far)
                            int16u table_id_extension=(Buffer[Buffer_Offset+Version_Pos]<<8)|Buffer[Buffer_Offset+Version_Pos+1];
                            int8u  version_number=(Buffer[Buffer_Offset+Version_Pos+2]&0x3F)>>1;
                            int8u  section_number=Buffer[Buffer_Offset+Version_Pos+3];
                            complete_stream::stream::table_id::table_id_extensions::iterator Table_ID_Extension=(*Table_ID)->Table_ID_Extensions.find(table_id_extension);
                            if (Table_ID_Extension==(*Table_ID)->Table_ID_Extensions.end())
                            {
                                if ((*Table_ID)->Table_ID_Extensions_CanAdd)
                                {
                                    (*Table_ID)->Table_ID_Extensions[table_id_extension].version_number=version_number;
                                    (*Table_ID)->Table_ID_Extensions[table_id_extension].Section_Numbers.resize(0x100);
                                    (*Table_ID)->Table_ID_Extensions[table_id_extension].Section_Numbers[section_number]=true;
                                    return true; //table_id_extension is not yet parsed
                                }
                            }
                            else if (Table_ID_Extension->second.version_number!=version_number)
                            {
                                Table_ID_Extension->second.version_number=version_number;
                                Table_ID_Extension->second.Section_Numbers.clear();
                                Table_ID_Extension->second.Section_Numbers.resize(0x100);
                                Table_ID_Extension->second.Section_Numbers[section_number]=true;
                                return true; //tversion is different
                            }
                            else if (!Table_ID_Extension->second.Section_Numbers[section_number])
                            {
                                Table_ID_Extension->second.Section_Numbers[section_number]=true;
                                return true; //section is not yet parsed
                            }
                        }
                    }
                    else
                        return true; //No version in this pid
                }
                else
                {
                    //Adaptation layer
                    #ifdef MEDIAINFO_MPEGTS_PCR_YES
                        if (Stream->Searching_TimeStamp_Start
                         || Stream->Searching_TimeStamp_End)
                        {
                            if ((Buffer[Buffer_Offset+BDAV_Size+3]&0x20)==0x20) //Adaptation is present
                            {
                                int8u pid_Adaptation_Info=Buffer[Buffer_Offset+BDAV_Size+5];
                                if (pid_Adaptation_Info&0x10) //PCR is present
                                {
                                    int64u program_clock_reference_base=(  (((int64u)Buffer[Buffer_Offset+BDAV_Size+6])<<25)
                                                                         | (((int64u)Buffer[Buffer_Offset+BDAV_Size+7])<<17)
                                                                         | (((int64u)Buffer[Buffer_Offset+BDAV_Size+8])<< 9)
                                                                         | (((int64u)Buffer[Buffer_Offset+BDAV_Size+9])<< 1)
                                                                         | (((int64u)Buffer[Buffer_Offset+BDAV_Size+10])>>7));
                                    if (Complete_Stream->Streams[pid].Searching_TimeStamp_End)
                                        Complete_Stream->Streams[pid].TimeStamp_End=program_clock_reference_base;
                                    if (Complete_Stream->Streams[pid].Searching_TimeStamp_Start)
                                    {
                                        //This is the first PCR
                                        Complete_Stream->Streams[pid].TimeStamp_Start=program_clock_reference_base;
                                        Complete_Stream->Streams[pid].Searching_TimeStamp_Start_Set(false);
                                        Complete_Stream->Streams[pid].Searching_TimeStamp_End_Set(true);
                                        Complete_Stream->Streams_With_StartTimeStampCount++;
                                    }

                                    //Test if we can find the TS bitrate
                                    if (!Complete_Stream->Streams[pid].EndTimeStampMoreThanxSeconds && Complete_Stream->Streams[pid].TimeStamp_Start!=(int64u)-1)
                                    {
                                        if (program_clock_reference_base<Complete_Stream->Streams[pid].TimeStamp_Start)
                                            program_clock_reference_base+=0x200000000LL; //33 bits, cyclic
                                        if ((program_clock_reference_base-Complete_Stream->Streams[pid].TimeStamp_Start)/90>8000)
                                        {
                                            Complete_Stream->Streams[pid].EndTimeStampMoreThanxSeconds=true;
                                            Complete_Stream->Streams_With_EndTimeStampMoreThanxSecondsCount++;
                                            if (Complete_Stream->Streams_With_StartTimeStampCount>0
                                             && Complete_Stream->Streams_With_StartTimeStampCount==Complete_Stream->Streams_With_EndTimeStampMoreThanxSecondsCount)
                                            {
                                                //We are already parsing 4 seconds (for all PCRs), we don't hope to have more info
                                                MpegTs_JumpTo_Begin=File_Offset+Buffer_Offset;
                                                MpegTs_JumpTo_End=MpegTs_JumpTo_Begin/2;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    #endif //MEDIAINFO_MPEGTS_PCR_YES

                    //Searching continue and parser timestamp
                    #ifdef MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
                        if (Stream->Searching_ParserTimeStamp_Start
                         || Stream->Searching_ParserTimeStamp_End)
                            return true;
                    #endif //MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
                }
            }
            else
                //Searching continue and parser timestamp
                if (Stream->Searching_Payload_Continue
                #ifdef MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
                     || Stream->Searching_ParserTimeStamp_Start
                     || Stream->Searching_ParserTimeStamp_End
                #endif //MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
                 )
                    return true;
        }

        //File__Duplicate
        if (Stream->ShouldDuplicate)
        {
            Element_Size=TS_Size;
            File__Duplicate_Write(pid);
        }

        Buffer_Offset+=TS_Size;
    }

    return false; //Not enough data
}

//---------------------------------------------------------------------------
void File_MpegTs::Synched_Init()
{
    //Config->File_Filter_Set(462);
    //Default values
    Complete_Stream=new complete_stream;
    Complete_Stream->Streams.resize(0x2000);
    Complete_Stream->Streams[0x0000].Searching_Payload_Start_Set(true);
    Complete_Stream->Streams[0x0000].Kind=complete_stream::stream::psi;
    Complete_Stream->Streams[0x0000].Table_IDs.resize(0x100);
    Complete_Stream->Streams[0x0000].Table_IDs[0x00]=new complete_stream::stream::table_id; //program_association_section
    Complete_Stream->Streams[0x0001].Searching_Payload_Start_Set(true);
    Complete_Stream->Streams[0x0001].Kind=complete_stream::stream::psi;
    Complete_Stream->Streams[0x0001].Table_IDs.resize(0x100);
    Complete_Stream->Streams[0x0001].Table_IDs[0x01]=new complete_stream::stream::table_id; //conditional_access_section

    //Temp
    MpegTs_JumpTo_Begin=(File_Offset_FirstSynched==(int64u)-1?0:File_Offset_FirstSynched)+MediaInfoLib::Config.MpegTs_MaximumOffset_Get();
    MpegTs_JumpTo_End=8*1024*1024;
    if (MpegTs_JumpTo_Begin+MpegTs_JumpTo_End>=File_Size)
    {
        MpegTs_JumpTo_Begin=File_Size;
        MpegTs_JumpTo_End=File_Size;
    }

    //There is no start code, so Stream_General is filled here
    Stream_Prepare(Stream_General);

    //Continue, again, for Duplicate and Filter
    Option_Manage();
}

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegTs::FileHeader_Begin()
{
    //Configuring
    #if defined(MEDIAINFO_BDAV_YES) || defined(MEDIAINFO_TSP_YES)
        TS_Size=188
        #if defined(MEDIAINFO_BDAV_YES)
            +BDAV_Size
        #endif
        #if defined(MEDIAINFO_TSP_YES)
            +TSP_Size
        #endif
        ;
    #endif

    //Configuration
    Option_Manage();

    return true;
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::Header_Parse()
#ifndef MEDIAINFO_MINIMIZESIZE
{
    //Parsing
    int8u transport_scrambling_control;
    bool  Adaptation, Data;
    if (BDAV_Size)
        Skip_B4(                                                "BDAV"); //BDAV supplement
    Skip_B1(                                                    "sync_byte");
    BS_Begin();
    Skip_SB(                                                    "transport_error_indicator");
    Get_SB (    payload_unit_start_indicator,                   "payload_unit_start_indicator");
    Skip_SB(                                                    "transport_priority");
    Get_S2 (13, pid,                                            "pid");
    Get_S1 ( 2, transport_scrambling_control,                   "transport_scrambling_control");
    Get_SB (    Adaptation,                                     "adaptation_field_control (adaptation)");
    Get_SB (    Data,                                           "adaptation_field_control (data)");
    Skip_S1( 4,                                                 "continuity_counter");
    BS_End();

    //Info
    if (!Complete_Stream->Streams[pid].program_numbers.empty())
    {
        Ztring Program_Numbers;
        for (size_t Pos=0; Pos<Complete_Stream->Streams[pid].program_numbers.size(); Pos++)
            Program_Numbers+=Ztring::ToZtring_From_CC2(Complete_Stream->Streams[pid].program_numbers[Pos])+_T('/');
        if (!Program_Numbers.empty())
            Program_Numbers.resize(Program_Numbers.size()-1);
        Data_Info(Program_Numbers);
    }
    else
        Data_Info("    ");
    Data_Info(Complete_Stream->Streams[pid].Element_Info);

    //Adaptation
    if (Adaptation)
        Header_Parse_AdaptationField();

    //Data
    if (Data)
    {
        //Encryption
        if (transport_scrambling_control>0)
            Complete_Stream->Streams[pid].IsScrambled=true;
    }
    else if (Element_Offset<TS_Size)
        Skip_XX(TS_Size-Element_Offset,                         "Junk");

    //Filling
    Header_Fill_Code(pid, Ztring().From_CC2(pid));
    Header_Fill_Size(TS_Size);
}
#else //MEDIAINFO_MINIMIZESIZE
{
    //Parsing
           payload_unit_start_indicator=(Buffer[Buffer_Offset+BDAV_Size+1]&0x40)!=0;
    int8u  transport_scrambling_control= Buffer[Buffer_Offset+BDAV_Size+3]&0xC0;
    bool   Adaptation=                  (Buffer[Buffer_Offset+BDAV_Size+3]&0x20)!=0;
    bool   Data=                        (Buffer[Buffer_Offset+BDAV_Size+3]&0x10)!=0;
    Element_Offset+=BDAV_Size+4;

    //Adaptation
    if (Adaptation)
        Header_Parse_AdaptationField();

    //Data
    if (Data)
    {
        //Encryption
        if (transport_scrambling_control>0)
            Complete_Stream->Streams[pid].IsScrambled=true;
    }

    //Filling
    Element[1].Next=File_Offset+Buffer_Offset+TS_Size;  //Header_Fill_Size(TS_Size);
    Element[1].IsComplete=true;                         //Header_Fill_Size(TS_Size);
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
void File_MpegTs::Header_Parse_AdaptationField()
#ifndef MEDIAINFO_MINIMIZESIZE
{
    int64u Element_Pos_Save=Element_Offset;
    Element_Begin("adaptation_field");
    int8u Adaptation_Size;
    Get_B1 (Adaptation_Size,                                    "adaptation_field_length");
    if (Adaptation_Size>0)
    {
        bool PCR_flag, OPCR_flag, splicing_point_flag, transport_private_data_flag, adaptation_field_extension_flag;
        BS_Begin();
        Skip_SB(                                                "discontinuity_indicator");
        Skip_SB(                                                "random_access_indicator");
        Skip_SB(                                                "elementary_stream_priority_indicator");
        Get_SB (    PCR_flag,                                   "PCR_flag");
        Get_SB (    OPCR_flag,                                  "OPCR_flag");
        Get_SB (    splicing_point_flag,                        "splicing_point_flag");
        Get_SB (    transport_private_data_flag,                "transport_private_data_flag");
        Get_SB (    adaptation_field_extension_flag,            "adaptation_field_extension_flag");
        BS_End();
        if (PCR_flag)
        {
            #ifdef MEDIAINFO_MPEGTS_PCR_YES
                BS_Begin();
                int64u program_clock_reference_base;
                Get_S8 (33, program_clock_reference_base,           "program_clock_reference_base"); Param_Info_From_Milliseconds(program_clock_reference_base/90);
                if (Complete_Stream->Streams[pid].Searching_TimeStamp_End)
                    Complete_Stream->Streams[pid].TimeStamp_End=program_clock_reference_base;
                if (Complete_Stream->Streams[pid].Searching_TimeStamp_Start)
                {
                    //This is the first PCR
                    Complete_Stream->Streams[pid].TimeStamp_Start=program_clock_reference_base;
                    Complete_Stream->Streams[pid].Searching_TimeStamp_Start_Set(false);
                    Complete_Stream->Streams[pid].Searching_TimeStamp_End_Set(true);
                    Complete_Stream->Streams_With_StartTimeStampCount++;
                }
                Data_Info_From_Milliseconds(program_clock_reference_base/90);
                Skip_S1( 6,                                         "reserved");
                Skip_S2( 9,                                         "program_clock_reference_extension");
                BS_End();

                //Test if we can find the TS bitrate
                if (!Complete_Stream->Streams[pid].EndTimeStampMoreThanxSeconds && Complete_Stream->Streams[pid].TimeStamp_Start!=(int64u)-1)
                {
                    if (program_clock_reference_base<Complete_Stream->Streams[pid].TimeStamp_Start)
                        program_clock_reference_base+=0x200000000LL; //33 bits, cyclic
                    if ((program_clock_reference_base-Complete_Stream->Streams[pid].TimeStamp_Start)/90>8000)
                    {
                        Complete_Stream->Streams[pid].EndTimeStampMoreThanxSeconds=true;
                        Complete_Stream->Streams_With_EndTimeStampMoreThanxSecondsCount++;
                        if (Complete_Stream->Streams_With_StartTimeStampCount>0
                         && Complete_Stream->Streams_With_StartTimeStampCount==Complete_Stream->Streams_With_EndTimeStampMoreThanxSecondsCount)
                        {
                            //We are already parsing 4 seconds (for all PCRs), we don't hope to have more info
                            MpegTs_JumpTo_Begin=File_Offset+Buffer_Offset;
                            MpegTs_JumpTo_End=MpegTs_JumpTo_Begin/2;
                        }
                    }
                }
            #else //MEDIAINFO_MPEGTS_PCR_YES
                Skip_B6(                                        "program_clock_reference");
            #endif //MEDIAINFO_MPEGTS_PCR_YES
        }
        if (OPCR_flag)
        {
            BS_Begin();
            Skip_S8(33,                                         "original_program_clock_reference_base");
            Skip_S1( 6,                                         "reserved");
            Skip_S2( 9,                                         "original_program_clock_reference_extension");
            BS_End();
        }
        if (splicing_point_flag)
        {
            Skip_B1(                                            "splice_countdown");
        }
        if (transport_private_data_flag)
        {
            int8u transport_private_data_length;
            Get_B1 (transport_private_data_length,              "transport_private_data_length");
            Skip_XX(transport_private_data_length,              "transport_private_data");
        }
        if (adaptation_field_extension_flag)
        {
            int8u adaptation_field_extension_length;
            Get_B1 (adaptation_field_extension_length,          "adaptation_field_extension_length");
            Skip_XX(adaptation_field_extension_length,          "adaptation_field_extension");
        }
    }
    if (Element_Offset<Element_Pos_Save+1+Adaptation_Size)
        Skip_XX(Element_Pos_Save+1+Adaptation_Size-Element_Offset, "stuffing_bytes");
    Element_End(1+Adaptation_Size);
}
#else //MEDIAINFO_MINIMIZESIZE
{
    int8u Adaptation_Size=Buffer[Buffer_Offset+BDAV_Size+4];
    #ifdef MEDIAINFO_MPEGTS_PCR_YES
        if (Adaptation_Size)
        {
            bool PCR_flag=(Buffer[Buffer_Offset+BDAV_Size+5]&0x10)!=0;
            if (PCR_flag)
            {
                int64u program_clock_reference_base=(  (((int64u)Buffer[Buffer_Offset+BDAV_Size+6])<<25)
                                                     | (((int64u)Buffer[Buffer_Offset+BDAV_Size+7])<<17)
                                                     | (((int64u)Buffer[Buffer_Offset+BDAV_Size+8])<< 9)
                                                     | (((int64u)Buffer[Buffer_Offset+BDAV_Size+9])<< 1)
                                                     | (((int64u)Buffer[Buffer_Offset+BDAV_Size+10])>>7));
                if (Complete_Stream->Streams[pid].Searching_TimeStamp_End)
                    Complete_Stream->Streams[pid].TimeStamp_End=program_clock_reference_base;
                if (Complete_Stream->Streams[pid].Searching_TimeStamp_Start)
                {
                    //This is the first PCR
                    Complete_Stream->Streams[pid].TimeStamp_Start=program_clock_reference_base;
                    Complete_Stream->Streams[pid].Searching_TimeStamp_Start_Set(false);
                    Complete_Stream->Streams[pid].Searching_TimeStamp_End_Set(true);
                    Complete_Stream->Streams_With_StartTimeStampCount++;
                }

                //Test if we can find the TS bitrate
                if (!Complete_Stream->Streams[pid].EndTimeStampMoreThanxSeconds && Complete_Stream->Streams[pid].TimeStamp_Start!=(int64u)-1)
                {
                    if (program_clock_reference_base<Complete_Stream->Streams[pid].TimeStamp_Start)
                        program_clock_reference_base+=0x200000000LL; //33 bits, cyclic
                    if ((program_clock_reference_base-Complete_Stream->Streams[pid].TimeStamp_Start)/90>8000)
                    {
                        Complete_Stream->Streams[pid].EndTimeStampMoreThanxSeconds=true;
                        Complete_Stream->Streams_With_EndTimeStampMoreThanxSecondsCount++;
                        if (Complete_Stream->Streams_With_StartTimeStampCount>0
                         && Complete_Stream->Streams_With_StartTimeStampCount==Complete_Stream->Streams_With_EndTimeStampMoreThanxSecondsCount)
                        {
                            //We are already parsing 4 seconds (for all PCRs), we don't hope to have more info
                            MpegTs_JumpTo_Begin=File_Offset+Buffer_Offset;
                            MpegTs_JumpTo_End=MpegTs_JumpTo_Begin/2;
                        }
                    }
                }
            }
        }
    #endif //MEDIAINFO_MPEGTS_PCR_YES
    Element_Offset+=1+Adaptation_Size;
}
#endif //MEDIAINFO_MINIMIZESIZE

//---------------------------------------------------------------------------
void File_MpegTs::Data_Parse()
{
    //TSP specific
    if (TSP_Size)
        Element_Size-=TSP_Size;

    //File__Duplicate
    if (Complete_Stream->Streams[pid].ShouldDuplicate)
        File__Duplicate_Write(pid);

    //Parsing
    if (!Complete_Stream->Streams[pid].Searching_Payload_Start
     && !Complete_Stream->Streams[pid].Searching_Payload_Continue
     #ifdef MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
         && !Complete_Stream->Streams[pid].Searching_ParserTimeStamp_Start
         && !Complete_Stream->Streams[pid].Searching_ParserTimeStamp_End
     #endif //MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
     )
        Skip_XX(Element_Size,                                   "data");
    else
        switch (Complete_Stream->Streams[pid].Kind)
        {
            case complete_stream::stream::pes : PES(); break;
            case complete_stream::stream::psi : PSI(); break;
            default: ;
        }

    //TSP specific
    if (TSP_Size)
    {
        Element_Size+=TSP_Size;
        Skip_B4(                                                "TSP"); //TSP supplement
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::PES()
{
    //Info
    if (Complete_Stream->transport_stream_id_IsValid)
        Element_Info(Mpeg_Psi_stream_type_Info(Complete_Stream->Streams[pid].stream_type, Complete_Stream->Transport_Streams[Complete_Stream->transport_stream_id].Programs[Complete_Stream->Streams[pid].program_numbers[0]].registration_format_identifier));

    //Demux
    Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(pid, 16)+_T(".mpg"));

    //Exists
    if (!Complete_Stream->Streams[pid].IsRegistered)
    {
        Complete_Stream->Streams[pid].IsRegistered=true;
        for (size_t Pos=0; Pos<Complete_Stream->Streams[pid].program_numbers.size(); Pos++)
            Complete_Stream->Transport_Streams[Complete_Stream->transport_stream_id].Programs[Complete_Stream->Streams[pid].program_numbers[Pos]].IsRegistered=true;
    }

    //Case of encrypted streams
    if (Complete_Stream->Streams[pid].IsScrambled)
    {
        //Don't need anymore
        Complete_Stream->Streams[pid].Searching_Payload_Start_Set(false);
        Complete_Stream->Streams[pid].Searching_Payload_Continue_Set(false);
        #ifdef MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
            Complete_Stream->Streams[pid].Searching_ParserTimeStamp_Start_Set(false);
        #endif //MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
        Skip_XX(Element_Size-Element_Offset,                    "Scrambled data");

        //Demux
        Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(pid, 16)+_T(".mpg"));

        return;
    }

    //Parser creation
    if (Complete_Stream->Streams[pid].Parser==NULL)
    {
        //Waiting for first payload_unit_start_indicator
        if (!payload_unit_start_indicator)
        {
            Element_DoNotShow(); //We don't want to show this item because there is no interessant info
            return; //This is not the start of the PES
        }

        //If unknown stream_type
        if (Complete_Stream->transport_stream_id_IsValid
         && Mpeg_Psi_stream_type_StreamKind(Complete_Stream->Streams[pid].stream_type, Complete_Stream->Transport_Streams[Complete_Stream->transport_stream_id].Programs[Complete_Stream->Streams[pid].program_numbers[0]].registration_format_identifier)==Stream_Max
         && Complete_Stream->Streams[pid].stream_type!=0x06 //Exception for private data
         && Complete_Stream->Streams[pid].stream_type<=0x7F //Exception for private data
         && Mpeg_Descriptors_registration_format_identifier_StreamKind(Complete_Stream->Transport_Streams[Complete_Stream->transport_stream_id].Programs[Complete_Stream->Streams[pid].program_numbers[0]].registration_format_identifier)==Stream_Max) //From Descriptor
        {
            Complete_Stream->Streams[pid].Searching_Payload_Start_Set(false);
            Complete_Stream->Streams[pid].Searching_Payload_Continue_Set(false);
            #ifdef MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
                Complete_Stream->Streams[pid].Searching_ParserTimeStamp_Start_Set(false);
            #endif //MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
            Complete_Stream->Streams_NotParsedCount--;
            return;
        }

        //Allocating an handle if needed
        #if defined(MEDIAINFO_MPEGPS_YES)
            Complete_Stream->Streams[pid].Parser=new File_MpegPs;
            ((File_MpegPs*)Complete_Stream->Streams[pid].Parser)->FromTS=true;
            ((File_MpegPs*)Complete_Stream->Streams[pid].Parser)->stream_type_FromTS=Complete_Stream->Streams[pid].stream_type;
            ((File_MpegPs*)Complete_Stream->Streams[pid].Parser)->format_identifier_FromTS=Complete_Stream->Streams[pid].registration_format_identifier;
            ((File_MpegPs*)Complete_Stream->Streams[pid].Parser)->MPEG_Version=2;
            Complete_Stream->Streams[pid].Parser->ShouldContinueParsing=true;
            Complete_Stream->Streams[pid].Searching_Payload_Continue_Set(true);
        #else
            //Filling
            Streams[pid].Parser=new File_Unknown();
        #endif
        Open_Buffer_Init(Complete_Stream->Streams[pid].Parser);
    }

    //Parsing
    Open_Buffer_Continue(Complete_Stream->Streams[pid].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);
    #if defined(MEDIAINFO_MPEGPS_YES) && defined(MEDIAINFO_MPEGTS_PESTIMESTAMP_YES)
        if (!Complete_Stream->Streams[pid].Searching_ParserTimeStamp_End
         && ((File_MpegPs*)Complete_Stream->Streams[pid].Parser)->HasTimeStamps)
        {
            Complete_Stream->Streams[pid].Searching_ParserTimeStamp_Start_Set(false);
            Complete_Stream->Streams[pid].Searching_ParserTimeStamp_End_Set(true);
        }
    #endif //MEDIAINFO_MPEGTS_PESTIMESTAMP_YES

    //Need anymore?
    if ((Complete_Stream->Streams[pid].Parser->IsFilled
     #ifdef MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
        && !Complete_Stream->Streams[pid].Searching_ParserTimeStamp_End
     #endif //MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
     )
     || Complete_Stream->Streams[pid].Parser->IsFinished)
    {
        Complete_Stream->Streams[pid].Searching_Payload_Start_Set(false);
        Complete_Stream->Streams[pid].Searching_Payload_Continue_Set(false);
        if (Complete_Stream->Streams_NotParsedCount)
            Complete_Stream->Streams_NotParsedCount--;
    }
}

//---------------------------------------------------------------------------
void File_MpegTs::PSI()
{
    //Initializing
    if (payload_unit_start_indicator)
    {
        delete ((File_Mpeg_Psi*)Complete_Stream->Streams[pid].Parser); Complete_Stream->Streams[pid].Parser=new File_Mpeg_Psi;
        Open_Buffer_Init(Complete_Stream->Streams[pid].Parser);
        ((File_Mpeg_Psi*)Complete_Stream->Streams[pid].Parser)->Complete_Stream=Complete_Stream;
        ((File_Mpeg_Psi*)Complete_Stream->Streams[pid].Parser)->pid=pid;
    }
    else if (Complete_Stream->Streams[pid].Parser==NULL)
    {
        Skip_XX(Element_Size,                                   "data");
        return; //This is not the start of the PSI
    }

    //Parsing
    Open_Buffer_Continue(Complete_Stream->Streams[pid].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);

    //Filling
    if (Complete_Stream->Streams[pid].Parser->IsFinished)
    {
        //Accept
        if (!IsAccepted && pid==0x0000 && Complete_Stream->Streams[pid].Parser->IsAccepted)
            Accept("MPEG-TS");

        //Disabling this PID
        delete Complete_Stream->Streams[pid].Parser; Complete_Stream->Streams[pid].Parser=NULL;
        Complete_Stream->Streams[pid].Searching_Payload_Start_Set(true);
        Complete_Stream->Streams[pid].Searching_Payload_Continue_Set(false);
    }
    else
        //Waiting for more data
        Complete_Stream->Streams[pid].Searching_Payload_Continue_Set(true);
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::Detect_EOF()
{
    if (File_Offset+Buffer_Size>=MpegTs_JumpTo_Begin && !Complete_Stream->transport_stream_id_IsValid && Retrieve(Stream_General, 0, General_Format_Profile)!=_T("No PAT/PMT"))
    {
        //Activating all streams as PES, in case of PAT/PMT are missing (ofen in .trp files)
        Complete_Stream->Streams.clear();
        Complete_Stream->Streams.resize(0x2000);
        for (size_t StreamID=0x20; StreamID<0x1FFF; StreamID++)
        {
            Complete_Stream->Streams_NotParsedCount=(size_t)-1;
            Complete_Stream->Streams[StreamID].Kind=complete_stream::stream::pes;
            Complete_Stream->Streams[StreamID].Searching_Payload_Start_Set(true);
            #ifndef MEDIAINFO_MINIMIZESIZE
                Complete_Stream->Streams[StreamID].Element_Info="PES";
            #endif //MEDIAINFO_MINIMIZESIZE
            #ifdef MEDIAINFO_MPEGTS_PCR_YES
                Complete_Stream->Streams[StreamID].Searching_TimeStamp_Start_Set(true);
                Complete_Stream->Streams[StreamID].Searching_TimeStamp_End_Set(false);
            #endif //MEDIAINFO_MPEGTS_PCR_YES
            #ifdef MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
                //Complete_Stream->Streams[StreamID].Searching_ParserTimeStamp_Start_Set(true);
                Complete_Stream->Streams[StreamID].Searching_ParserTimeStamp_End_Set(false);
            #endif //MEDIAINFO_MPEGTS_PESTIMESTAMP_YES
        }
        if (!IsAccepted)
            Accept("MPEG-TS");
        GoTo(0, "MPEG-TS");
        Fill(Stream_General, 0, General_Format_Profile, "No PAT/PMT");
        return;
    }

    //Jump to the end of the file
    if (File_Offset+Buffer_Offset>0x8000 && File_Offset+Buffer_Offset+MpegTs_JumpTo_End<File_Size && (
       (File_Offset+Buffer_Offset>=MpegTs_JumpTo_Begin)
    //|| (program_Count==0 && elementary_PID_Count==0)
    ))
    {
        #ifdef MEDIAINFO_MPEGTS_PCR_YES
            if (File_Size!=(int64u)-1) //Only if not unlimited
            {
                //Reactivating interessant TS streams
                if (!Complete_Stream->Streams.empty())
                {
                    //End timestamp is out of date
                    for (size_t StreamID=0; StreamID<0x2000; StreamID++)//std::map<int64u, stream>::iterator Stream=Streams.begin(); Stream!=Streams.end(); Stream++)
                    {
                        //End timestamp is out of date
                        Complete_Stream->Streams[StreamID].TimeStamp_End=(int64u)-1;
                        if (Complete_Stream->Streams[StreamID].TimeStamp_Start!=(int64u)-1)
                            Complete_Stream->Streams[StreamID].Searching_TimeStamp_End_Set(true); //Searching only for a start found
                    }
                }
                Complete_Stream->End_Time.clear();
            }
        #endif //MEDIAINFO_MPEGTS_PCR_YES

        if (!IsAccepted)
            Accept("MPEG-TS");
        #if !defined(MEDIAINFO_MPEGTS_PCR_YES) && !defined(MEDIAINFO_MPEGTS_PESTIMESTAMP_YES)
            GoToFromEnd(47, "MPEG-TS"); //TODO: Should be changed later (when Finalize stuff will be split) 
        #else //!defined(MEDIAINFO_MPEGTS_PCR_YES) && !defined(MEDIAINFO_MPEGTS_PESTIMESTAMP_YES)
            GoToFromEnd(MpegTs_JumpTo_End, "MPEG-TS");
        #endif //!defined(MEDIAINFO_MPEGTS_PCR_YES) && !defined(MEDIAINFO_MPEGTS_PESTIMESTAMP_YES)
    }
}

} //NameSpace

#endif //MEDIAINFO_MPEGTS_YES

