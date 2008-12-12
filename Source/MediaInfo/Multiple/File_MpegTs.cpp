// File_Mpegts - Info for MPEG Transport Stream files
// Copyright (C) 2006-2008 Jerome Martinez, Zen@MediaArea.net
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

//---------------------------------------------------------------------------
//From Mpeg_Psi
extern const char*  Mpeg_Psi_stream_type(int8u ID, int32u format_identifier);
extern const char*  Mpeg_Psi_stream_Format(int8u ID, int32u format_identifier);
extern const char*  Mpeg_Psi_stream_Codec(int8u ID, int32u format_identifier);
extern stream_t     Mpeg_Psi_stream_Kind(int32u ID, int32u format_identifier);
extern const char*  Mpeg_Descriptors_stream_Format(int8u descriptor_tag, int32u format_identifier);
extern const char*  Mpeg_Descriptors_stream_Codec(int8u descriptor_tag, int32u format_identifier);
extern stream_t     Mpeg_Descriptors_stream_Kind(int8u descriptor_tag, int32u format_identifier);

//---------------------------------------------------------------------------
const char* Mpeg_Psi_kind(File_Mpeg_Psi::ts_kind ID)
{
    switch (ID)
    {
        case File_Mpeg_Psi::program_association_table          : return "Program Association Table";
        case File_Mpeg_Psi::program_map_table                  : return "Program Map Table";
        case File_Mpeg_Psi::network_information_table          : return "Network Information Table";
        case File_Mpeg_Psi::conditional_access_table           : return "Conditional Access Table";
        case File_Mpeg_Psi::transport_stream_description_table : return "Transport Stream Description Table";
        case File_Mpeg_Psi::reserved                           : return "reserved";
        case File_Mpeg_Psi::pes                                : return "PES";
        case File_Mpeg_Psi::null                               : return "Null";
        case File_Mpeg_Psi::dvb_nit_st                         : return "DVB - NIT, ST";
        case File_Mpeg_Psi::dvb_sdt_bat_st                     : return "DVB - SDT, BAT, ST";
        case File_Mpeg_Psi::dvb_eit                            : return "DVB - EIT";
        case File_Mpeg_Psi::dvb_rst_st                         : return "DVB - RST, ST";
        case File_Mpeg_Psi::dvb_tdt_tot_st                     : return "DVB - TDT, TOT, ST";
        case File_Mpeg_Psi::dvb_mip                            : return "DVB - MIP (no table_id)";
        case File_Mpeg_Psi::dvb_reserved                       : return "DVB - reserved";
        case File_Mpeg_Psi::dvb_inband                         : return "DVB - Inband Signalling";
        case File_Mpeg_Psi::dvb_measurement                    : return "DVB - Measurement";
        case File_Mpeg_Psi::dvb_dit                            : return "DVB - DIT";
        case File_Mpeg_Psi::dvb_sit                            : return "DVB - SIT";
        case File_Mpeg_Psi::arib                               : return "ARIB";
        case File_Mpeg_Psi::cea_osd                            : return "CEA OSD";
        case File_Mpeg_Psi::atsc_pate                          : return "ATSC - PAT-E";
        case File_Mpeg_Psi::atsc_stt_pide                      : return "ATSC - STT, PID-E";
        case File_Mpeg_Psi::atsc_op                            : return "ATSC - operational and management packets";
        case File_Mpeg_Psi::atsc_psip                          : return "ATSC - PSIP";
        case File_Mpeg_Psi::atsc_scte                          : return "ATSC - SCTE Network/System Information Base";
        case File_Mpeg_Psi::atsc_reserved                      : return "ATSC - reserved";
        case File_Mpeg_Psi::docsis                             : return "DOCSIS";
        default : return "";
    }
}

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
    Trusted_Multiplier=2;

    //Internal config
    TS_Size=188;
    BDAV_Size=0; //No BDAV header

    //Data
    format_identifier=0xFFFFFFFF;
    MpegTs_JumpTo_Begin=MediaInfoLib::Config.MpegTs_MaximumOffset_Get();
    MpegTs_JumpTo_End=8*1024*1024;

    //Count
    program_Count=0;
    elementary_PID_Count=0;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegTs::FileHeader_Begin()
{
    //Integrity
    if (File_Offset==0 && Detect_NonMPEGTS())
        return false;

    //Configuration
    Option_Manage();

    return true;
}

//---------------------------------------------------------------------------
void File_MpegTs::Read_Buffer_Finalize()
{
    if (Streams.empty())
        return; //Not initialized

    for (size_t StreamID=0; StreamID<0x2000; StreamID++)//std::map<int64u, stream>::iterator Stream=Streams.begin(); Stream!=Streams.end(); Stream++)
    {
        //PES
        if (Streams[StreamID].TS_Kind==File_Mpeg_Psi::pes)
        {
            //By the parser
            StreamKind_Last=Stream_Max;
            if (Streams[StreamID].Parser)
            {
                Streams[StreamID].Parser->Open_Buffer_Finalize();
                Merge (*Streams[StreamID].Parser);

                //Info from the ES parser
                if (Streams[StreamID].ES_Parser)
                {
                    Streams[StreamID].ES_Parser->Open_Buffer_Finalize();
                    Merge (*Streams[StreamID].ES_Parser, StreamKind_Last, 0, StreamPos_Last);
                }
            }
            //By the descriptors
            if (StreamKind_Last==Stream_Max && Streams[StreamID].StreamIsRegistred
             && Mpeg_Descriptors_stream_Kind(Streams[StreamID].descriptor_tag, format_identifier)!=Stream_Max)
            {
                StreamKind_Last=Mpeg_Descriptors_stream_Kind(Streams[StreamID].descriptor_tag, format_identifier);
                Stream_Prepare(StreamKind_Last);
            }
            //By the stream_type
            if (StreamKind_Last==Stream_Max)
            {
                //The parser failed
                if ((Streams[StreamID].StreamIsRegistred || format_identifier==Mpeg_Descriptors::HDMV) && Mpeg_Psi_stream_Kind(Streams[StreamID].stream_type, format_identifier)!=Stream_Max)
                {
                    StreamKind_Last=Mpeg_Psi_stream_Kind(Streams[StreamID].stream_type, format_identifier);
                    Stream_Prepare(StreamKind_Last);
                }
            }

            //More info
            if (StreamKind_Last!=Stream_Max) //Found by the Parser or stream_type
            {
                //Codec
                if (Retrieve(StreamKind_Last, StreamPos_Last, "Format").empty())
                    Fill(StreamKind_Last, StreamPos_Last, "Format", Mpeg_Descriptors_stream_Format(Streams[StreamID].descriptor_tag, format_identifier));
                if (Retrieve(StreamKind_Last, StreamPos_Last, "Format").empty())
                    Fill(StreamKind_Last, StreamPos_Last, "Format", Mpeg_Psi_stream_Format(Streams[StreamID].stream_type, format_identifier));
                if (Retrieve(StreamKind_Last, StreamPos_Last, "Codec").empty())
                    Fill(StreamKind_Last, StreamPos_Last, "Codec", Mpeg_Descriptors_stream_Codec(Streams[StreamID].descriptor_tag, format_identifier));
                if (Retrieve(StreamKind_Last, StreamPos_Last, "Codec").empty())
                    Fill(StreamKind_Last, StreamPos_Last, "Codec", Mpeg_Psi_stream_Codec(Streams[StreamID].stream_type, format_identifier));

                //TimeStamp
                if (Streams[StreamID].TimeStamp_End!=(int64u)-1)
                {
                    if (Streams[StreamID].TimeStamp_End<Streams[StreamID].TimeStamp_Start)
                        Streams[StreamID].TimeStamp_End+=0x200000000LL; //33 bits, cyclic
                    int64u Duration=Streams[StreamID].TimeStamp_End-Streams[StreamID].TimeStamp_Start;
                    if (Duration!=0 && Duration!=(int64u)-1)
                        Fill(StreamKind_Last, StreamPos_Last, "Duration", Duration/90, 10, true);
                    else
                        Fill(StreamKind_Last, StreamPos_Last, "Duration", "", 0, true, true); //Clear it
                }

                //Encryption
                if (Streams[StreamID].Scrambled)
                    Fill(StreamKind_Last, StreamPos_Last, "Encryption", "Encrypted");

                //TS info
                for (std::map<std::string, ZenLib::Ztring>::iterator Info=Streams[StreamID].Infos.begin(); Info!=Streams[StreamID].Infos.end(); Info++)
                {
                    if (Retrieve(StreamKind_Last, StreamPos_Last, Info->first.c_str()).empty())
                        Fill(StreamKind_Last, StreamPos_Last, Info->first.c_str(), Info->second);
                }

                //Common
                Fill(StreamKind_Last, StreamPos_Last, "ID", StreamID, 10, true);
                Fill(StreamKind_Last, StreamPos_Last, "ID/String", Decimal_Hexa(StreamID), true);

                for (size_t Pos=0; Pos<Streams[StreamID].program_numbers.size(); Pos++)
                {
                    //Common
                    Fill(StreamKind_Last, StreamPos_Last, "MenuID", Streams[StreamID].program_numbers[Pos]);
                    Fill(StreamKind_Last, StreamPos_Last, "MenuID/String", Decimal_Hexa(Streams[StreamID].program_numbers[Pos]));

                    //Menu
                    Programs[Streams[StreamID].program_numbers[Pos]].List_ID.push_back(Ztring::ToZtring(StreamID));
                    Programs[Streams[StreamID].program_numbers[Pos]].List_StreamKind.push_back(Ztring::ToZtring(StreamKind_Last));
                    Programs[Streams[StreamID].program_numbers[Pos]].List_StreamPos.push_back(Ztring::ToZtring(StreamPos_Last));
                    Ztring Format=Retrieve(StreamKind_Last, StreamPos_Last, "Format");
                    Programs[Streams[StreamID].program_numbers[Pos]].Format.push_back(Format);
                    Ztring Codec=Retrieve(StreamKind_Last, StreamPos_Last, "Codec");
                    Programs[Streams[StreamID].program_numbers[Pos]].Codec.push_back(Codec);
                    Ztring Language=Retrieve(StreamKind_Last, StreamPos_Last, "Language");
                    Programs[Streams[StreamID].program_numbers[Pos]].Language.push_back(Language);
                    Ztring Menu_Temp=Decimal_Hexa(StreamID);
                    Menu_Temp+=_T(" (");
                    Menu_Temp+=Format;
                    if (!Language.empty())
                    {
                        Menu_Temp+=_T(", ");
                        Menu_Temp+=Language;
                    }
                    Menu_Temp+=_T(")");
                    Programs[Streams[StreamID].program_numbers[Pos]].Text_ID.push_back(Menu_Temp);
                }
            }
        }

        //PCR
        if (Streams[StreamID].TS_Kind==File_Mpeg_Psi::PCR)
        {
            if (Streams[StreamID].TimeStamp_End!=(int64u)-1)
            {
                if (Streams[StreamID].TimeStamp_End<Streams[StreamID].TimeStamp_Start)
                    Streams[StreamID].TimeStamp_End+=0x200000000LL; //33 bits, cyclic
                int64u Duration=Streams[StreamID].TimeStamp_End-Streams[StreamID].TimeStamp_Start;
                if (Duration!=0 && Duration!=(int64u)-1)
                    Fill(Stream_General, 0, General_Duration, Duration/90, 10, true);
                else
                    Fill(Stream_General, 0, General_Duration, "", 0, true, true); //Clear it
            }
        }

        delete Streams[StreamID].Parser; Streams[StreamID].Parser=NULL;
    }

    //TS info for General
    for (std::map<std::string, ZenLib::Ztring>::iterator Info=Streams[0x0000].Infos.begin(); Info!=Streams[0x0000].Infos.end(); Info++)
    {
        if (Retrieve(Stream_General, 0, Info->first.c_str()).empty())
            Fill(Stream_General, 0, Info->first.c_str(), Info->second);
    }

    //Fill General
    Fill(Stream_General, 0, General_Format, "MPEG-TS");

    //Fill Menu
    if (Programs.size()>1 || Config->File_MpegTs_ForceMenu_Get())
    {
        for (std::map<int16u, program>::iterator Program=Programs.begin(); Program!=Programs.end(); Program++)
        {
            Stream_Prepare(Stream_Menu);
            Fill(StreamKind_Last, StreamPos_Last, "MenuID", Program->first);
            Fill(StreamKind_Last, StreamPos_Last, "MenuID/String", Decimal_Hexa(Program->first));
            Fill(StreamKind_Last, StreamPos_Last, "ID", Programs[Program->first].pid);
            Fill(StreamKind_Last, StreamPos_Last, "ID/String", Decimal_Hexa(Programs[Program->first].pid));
            Program->second.List_ID.Separator_Set(0, _T(" / "));
            Fill(StreamKind_Last, StreamPos_Last, "List", Program->second.List_ID.Read());
            Program->second.List_StreamKind.Separator_Set(0, _T(" / "));
            Fill(StreamKind_Last, StreamPos_Last, "List_StreamKind", Program->second.List_StreamKind.Read());
            Program->second.List_StreamPos.Separator_Set(0, _T(" / "));
            Fill(StreamKind_Last, StreamPos_Last, "List_StreamPos", Program->second.List_StreamPos.Read());
            Program->second.Text_ID.Separator_Set(0, _T(" / "));
            Fill(StreamKind_Last, StreamPos_Last, "List/String", Program->second.Text_ID.Read());
            Program->second.Language.Separator_Set(0, _T(" / "));
            Fill(StreamKind_Last, StreamPos_Last, "Language", Program->second.Language.Read());
            Program->second.Codec.Separator_Set(0, _T(" / "));
            Fill(StreamKind_Last, StreamPos_Last, "Codec", Program->second.Codec.Read());
            Program->second.Format.Separator_Set(0, _T(" / "));
            Fill(StreamKind_Last, StreamPos_Last, "Format", Program->second.Format.Read());

            for (std::map<std::string, ZenLib::Ztring>::iterator Info=Program->second.Infos.begin(); Info!=Program->second.Infos.end(); Info++)
                Fill(Stream_Menu, StreamPos_Last, Info->first.c_str(), Info->second);
        }
    }

    File__Duplicate::Read_Buffer_Finalize();

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Streams.clear();
    Programs.clear();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegTs::Header_Begin()
{
    //Must have enough buffer for having header
    if (Buffer_Offset+BDAV_Size+4>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && Buffer[Buffer_Offset+BDAV_Size]!=0x47)
    {
        Trusted_IsNot("MPEG-TS, Synchronisation lost");
        if (File__Duplicate_Get())
            Trusted++; //We don't want to stop parsing if duplication is requested, TS is not a lot stable, normal...
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //Quick search
    if (!Header_Parser_QuickSearch())
        return false;

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_MpegTs::Header_Parse()
{
    //Filling
    Header_Fill_Size(TS_Size);

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
    if (!Streams[pid].program_numbers.empty())
    {
        for (size_t Pos=0; Pos<Streams[pid].program_numbers.size(); Pos++)
            Data_Info(Ztring::ToZtring_From_CC2(Streams[pid].program_numbers[Pos]));
    }
    else
        Data_Info("    ");
    Data_Info(Mpeg_Psi_kind(Streams[pid].TS_Kind));

    //Adaptation
    if (Adaptation)
        Header_Parse_AdaptationField();

    //Filling
    Header_Fill_Code(pid, Ztring().From_CC2(pid));

    //Data
    if (Data)
    {
        //Encryption
        if (transport_scrambling_control>0)
            Streams[pid].Scrambled=true;
    }
    else if (Element_Offset<TS_Size)
        Skip_XX(TS_Size-Element_Offset,                      "Junk");
}

//---------------------------------------------------------------------------
//
void File_MpegTs::Header_Parse_AdaptationField()
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
            BS_Begin();
            int64u program_clock_reference_base;
            Get_S8 (33, program_clock_reference_base,           "program_clock_reference_base"); Param_Info_From_Milliseconds(program_clock_reference_base/90);
            if (Streams[pid].Searching_TimeStamp_Start)
            {
                //This is the first PCR
                Streams[pid].TimeStamp_Start=program_clock_reference_base;
                Streams[pid].Searching_TimeStamp_Start_Set(false);
            }
            Streams[pid].TimeStamp_End=program_clock_reference_base;
            Data_Info_From_Milliseconds(program_clock_reference_base/90);
            Skip_S1( 6,                                         "reserved");
            Skip_S2( 9,                                         "program_clock_reference_extension");
            BS_End();

            //Test if we can find the TS bitrate
            if (program_clock_reference_base<Streams[pid].TimeStamp_Start)
                program_clock_reference_base+=0x200000000LL; //33 bits, cyclic
            if (File_Size!=(int64u)-1 && (program_clock_reference_base-Streams[pid].TimeStamp_Start)/90>4000) //Only if not unlimited
            {
                //We are already parsing 4 seconds, we don't hope to have more info
                MpegTs_JumpTo_Begin=File_Offset+Buffer_Offset;
                MpegTs_JumpTo_End=MpegTs_JumpTo_Begin/2;
            }
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

//---------------------------------------------------------------------------
void File_MpegTs::Data_Parse()
{
    //File__Duplicate
    if (Streams[pid].ShouldDuplicate)
        File__Duplicate_Write(pid);

    if (!Streams[pid].Searching_Payload_Start && !Streams[pid].Searching_Payload_Continue)
        return;

    //PIDs
    switch (Streams[pid].TS_Kind)
    {
        case File_Mpeg_Psi::unknown :
        case File_Mpeg_Psi::reserved :
        case File_Mpeg_Psi::null :
            break;

        case File_Mpeg_Psi::pes :
            PES();
            break;

        case File_Mpeg_Psi::program_association_table :
        case File_Mpeg_Psi::program_map_table :
        case File_Mpeg_Psi::network_information_table :
        case File_Mpeg_Psi::conditional_access_table :
        case File_Mpeg_Psi::transport_stream_description_table :
        case File_Mpeg_Psi::dvb_nit_st :
        case File_Mpeg_Psi::dvb_sdt_bat_st :
        case File_Mpeg_Psi::dvb_eit :
        case File_Mpeg_Psi::dvb_rst_st :
        case File_Mpeg_Psi::dvb_tdt_tot_st :
        case File_Mpeg_Psi::dvb_mip :
        case File_Mpeg_Psi::dvb_reserved :
        case File_Mpeg_Psi::dvb_inband :
        case File_Mpeg_Psi::dvb_measurement :
        case File_Mpeg_Psi::dvb_dit :
        case File_Mpeg_Psi::dvb_sit :
        case File_Mpeg_Psi::arib :
        case File_Mpeg_Psi::cea_osd :
        case File_Mpeg_Psi::atsc_pate :
        case File_Mpeg_Psi::atsc_stt_pide :
        case File_Mpeg_Psi::atsc_op :
        case File_Mpeg_Psi::atsc_psip :
        case File_Mpeg_Psi::atsc_scte :
        case File_Mpeg_Psi::atsc_reserved :
        case File_Mpeg_Psi::docsis :
            PSI();
            break;

        default: ;
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::PSI()
{
    //Initializing
    if (Streams[pid].Parser!=NULL && payload_unit_start_indicator)
    {
        //There was a false start before, start from beginning
        delete ((File_Mpeg_Psi*)Streams[pid].Parser); Streams[pid].Parser=NULL;
    }
    if (Streams[pid].Parser==NULL)
    {
        if (!payload_unit_start_indicator)
            return; //This is not the start of the PSI
        Streams[pid].Parser=new File_Mpeg_Psi;
    }

    //Parsing
    Open_Buffer_Init(Streams[pid].Parser, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(Streams[pid].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);

    //Filling
    if (Streams[pid].Parser->File_Offset==Streams[pid].Parser->File_Size)
    {
        //Finished, we can fill data
        switch (Streams[pid].TS_Kind)
        {
            case File_Mpeg_Psi::program_association_table : PSI_program_association_table(); break;
            case File_Mpeg_Psi::program_map_table         : PSI_program_map_table(); break;
            case File_Mpeg_Psi::network_information_table : PSI_network_information_table(); break;
            case File_Mpeg_Psi::dvb_sdt_bat_st            : PSI_dvb_sdt_bat_st(); break;
            case File_Mpeg_Psi::atsc_psip                 : PSI_atsc_psip(); break;
            default : ;
        }

        //Disabling this PID
        if (!((File_Mpeg_Psi*)Streams[pid].Parser)->WantItAgain)
        {
            delete Streams[pid].Parser; Streams[pid].Parser=NULL;
            Streams[pid].Searching_Payload_Start_Set(pid==0x0000 && File_Offset<0x8000);
            Streams[pid].Searching_Payload_Continue_Set(false);
        }
    }
    else
        //Waiting for more data
        Streams[pid].Searching_Payload_Continue_Set(true);
}

//---------------------------------------------------------------------------
void File_MpegTs::PSI_program_association_table()
{
    program_Count=0;
    File_Mpeg_Psi* Parser=(File_Mpeg_Psi*)Streams[pid].Parser;
    for (std::map<int16u, File_Mpeg_Descriptors::program>::iterator Program=Parser->Programs.begin(); Program!=Parser->Programs.end(); Program++)
    {
        int16u PID=Program->second.pid;

        //Program
        if (Program->first!=0x0000)
        {
            Programs[Program->first].pid=PID;
            if (Config->File_Filter_Get(Program->first))
                program_Count++;
        }

        //Enabling what we know parsing
        Streams[PID].TS_Kind=Program->first!=0x0000?File_Mpeg_Psi::program_map_table:File_Mpeg_Psi::network_information_table;
        if (Config->File_Filter_Get(Program->first))
        {
            std::vector<int16u>::iterator Pos=find(Streams[PID].program_numbers.begin(), Streams[PID].program_numbers.end(), Program->first);
            if (Pos==Streams[PID].program_numbers.end())
                Streams[PID].program_numbers.push_back(Program->first);
        }

        //File__Duplicate
        if (File__Duplicate_Get_From_PID(PID))
            Streams[PID].ShouldDuplicate=true;
        else
            Streams[PID].ShouldDuplicate=false;

        //Case of multiple program in one PID
        PID_PMTs[PID].List.push_back(Program->first);

        //Menus
        //ProgramNumber2StreamNumber[Parser->program_association_section_ProgramNumber[Pos]]=elementary_PID;
    }

    //File_Filter (taking care for multiple programs in one PID)
    for(std::map<int16u, pid_pmts>::iterator PID_PMT=PID_PMTs.begin(); PID_PMT!=PID_PMTs.end(); PID_PMT++)
    {
        int16u PID=PID_PMT->first;

        //File_Filter
        bool ShouldFilter=false;
        for (size_t Pos=0; Pos<PID_PMT->second.List.size(); Pos++)
            if (Config->File_Filter_Get(PID_PMT->second.List[Pos]))
                ShouldFilter=true;
            else
            {
                PID_PMT->second.List.erase(PID_PMT->second.List.begin()+Pos);
                Pos--;
            }
        Streams[PID].Searching_Payload_Start_Set(ShouldFilter);
        Streams[PID].Searching_Payload_Continue_Set(ShouldFilter);
    }

    //Filling
    Fill(Stream_General, 0, General_ID, Parser->transport_stream_id, 16, true);
}

//---------------------------------------------------------------------------
void File_MpegTs::PSI_program_map_table()
{
    File_Mpeg_Psi* Parser=(File_Mpeg_Psi*)Streams[pid].Parser;

    //Format identifier
    format_identifier=Parser->Programs[Parser->program_number].format_identifier;

    //PCR
    int16u PCR_PID=Parser->PCR_PID;
    if (PCR_PID!=0x1FFF)
    {
        if (Streams[PCR_PID].TS_Kind==File_Mpeg_Psi::unknown) //Only if not already registered
        {
            Streams[PCR_PID].TS_Kind=File_Mpeg_Psi::PCR;
            Streams[PCR_PID].Searching_TimeStamp_Start_Set(File_Size!=(int64u)-1); //Only if not unlimited
            if (MpegTs_JumpTo_Begin+MpegTs_JumpTo_End>=File_Size)
                Streams[PCR_PID].Searching_TimeStamp_End_Set(File_Size!=(int64u)-1); //Only if not unlimited
        }
    }

    int16u program_number=0; //We want to remember the program_number, it is always the same
    for (std::map<int16u, File_Mpeg_Psi::stream>::iterator Stream=Parser->Streams.begin(); Stream!=Parser->Streams.end(); Stream++)
    {
        //Retrieving info
        int16u elementary_PID=Stream->first;

        //Scrambling
        if (Stream->second.CA_PID)
        {
            if (Streams[Stream->second.CA_PID].TS_Kind==File_Mpeg_Psi::unknown
             || Streams[Stream->second.CA_PID].TS_Kind>=File_Mpeg_Psi::ts_outofspec) //Only if not already registered
            {
                elementary_PID_Count++;
                Streams[Stream->second.CA_PID].program_numbers.push_back(Stream->second.program_number);
                Streams[Stream->second.CA_PID].TS_Kind=File_Mpeg_Psi::conditional_access_table;
                Streams[Stream->second.CA_PID].Searching_Payload_Start_Set(true);
            }
        }

        if (elementary_PID==0x0000)
        {
            //About the program
        }
        else if (Config->File_Filter_Get(Stream->second.program_number))
        {
            std::vector<int16u>::iterator Pos=find(Streams[elementary_PID].program_numbers.begin(), Streams[elementary_PID].program_numbers.end(), Stream->second.program_number);
            if (Pos==Streams[elementary_PID].program_numbers.end())
                Streams[elementary_PID].program_numbers.push_back(Stream->second.program_number);
            if (Streams[elementary_PID].TS_Kind==File_Mpeg_Psi::unknown
             || Streams[elementary_PID].TS_Kind==File_Mpeg_Psi::PCR
             || Streams[elementary_PID].TS_Kind>=File_Mpeg_Psi::ts_outofspec) //Only if not already registered
            {
                elementary_PID_Count++;
                Streams[elementary_PID].TS_Kind=File_Mpeg_Psi::pes;
                if (Stream->second.program_number!=0x0000)
                    Streams[elementary_PID].Infos=Stream->second.Infos;
                Streams[elementary_PID].stream_type=Stream->second.stream_type;
                Streams[elementary_PID].descriptor_tag=Stream->second.descriptor_tag;
                Streams[elementary_PID].format_identifier=Stream->second.format_identifier;
                Streams[elementary_PID].ES_Parser=Stream->second.ES_Parser; Stream->second.ES_Parser=NULL;
                Streams[elementary_PID].Searching_Payload_Start_Set(true);
                Streams[elementary_PID].Searching_TimeStamp_Start_Set(File_Size!=(int64u)-1); //Only if not unlimited
                if (MpegTs_JumpTo_Begin+MpegTs_JumpTo_End>=File_Size)
                    Streams[elementary_PID].Searching_TimeStamp_End_Set(File_Size!=(int64u)-1); //Only if not unlimited
            }

            //File_Duplicate
            Streams[elementary_PID].ShouldDuplicate=File__Duplicate_Get_From_PID(elementary_PID);
        }

        program_number=Stream->second.program_number; //One of them, they should have all the same
    }
    if (program_Count && Config->File_Filter_Get(program_number))
        program_Count--;

    //DVB/ATSC handling
    if (program_Count==0 && Streams[0x1FFB].TS_Kind!=File_Mpeg_Psi::atsc_psip) //One test to know if we have already did it
        for (size_t Pos=0x10; Pos<0x1FFF; Pos++)
         {
            if (Streams[Pos].TS_Kind==File_Mpeg_Psi::unknown)
             {
                File_Mpeg_Psi::ts_kind Kind;
                switch (Pos)
                {
                    case 0x0010 : Kind=File_Mpeg_Psi::dvb_nit_st; break;
                    case 0x0011 : Kind=File_Mpeg_Psi::dvb_sdt_bat_st; break;
                    case 0x0012 : Kind=File_Mpeg_Psi::dvb_eit; break;
                    case 0x0013 : Kind=File_Mpeg_Psi::dvb_rst_st; break;
                    case 0x0014 : Kind=File_Mpeg_Psi::dvb_tdt_tot_st; break;
                    case 0x0015 : Kind=File_Mpeg_Psi::dvb_mip; break;
                    case 0x0016 :
                    case 0x0017 :
                    case 0x0018 :
                    case 0x0019 :
                    case 0x001A :
                    case 0x001B : Kind=File_Mpeg_Psi::dvb_reserved; break;
                    case 0x001C : Kind=File_Mpeg_Psi::dvb_inband; break;
                    case 0x001D : Kind=File_Mpeg_Psi::dvb_measurement; break;
                    case 0x001E : Kind=File_Mpeg_Psi::dvb_dit; break;
                    case 0x001F : Kind=File_Mpeg_Psi::dvb_sit; break;
                    case 0x0020 :
                    case 0x0021 :
                    case 0x0022 :
                    case 0x0023 :
                    case 0x0024 :
                    case 0x0025 :
                    case 0x0026 :
                    case 0x0027 :
                    case 0x0028 :
                    case 0x0029 :
                    case 0x002A :
                    case 0x002B :
                    case 0x002C :
                    case 0x002D :
                    case 0x002E :
                    case 0x002F : Kind=File_Mpeg_Psi::arib; break;
                    case 0x1ABC : Kind=File_Mpeg_Psi::cea_osd; break;
                    case 0x1FF7 : Kind=File_Mpeg_Psi::atsc_pate; break;
                    case 0x1FF8 : Kind=File_Mpeg_Psi::atsc_stt_pide; break;
                    case 0x1FF9 : Kind=File_Mpeg_Psi::atsc_reserved; break;
                    case 0x1FFA : Kind=File_Mpeg_Psi::atsc_op; break;
                    case 0x1FFB : Kind=File_Mpeg_Psi::atsc_psip; break;
                    case 0x1FFC : Kind=File_Mpeg_Psi::atsc_scte; break;
                    case 0x1FFD : Kind=File_Mpeg_Psi::atsc_reserved; break;
                    case 0x1FFE : Kind=File_Mpeg_Psi::docsis; break;
                    default     : Kind=File_Mpeg_Psi::reserved;
                }

                Streams[Pos].TS_Kind=Kind;
                Streams[Pos].Searching_Payload_Start_Set(true);
            }

            if (Pos==0x002F)
                Pos=0x1ABB; //Skipping normal data
            if (Pos==0x01ABC)
                Pos=0x1FF6; //Skipping normal data
        }

    //Case of multiple program in one PID
    if (program_number)
    {
        std::vector<int16u>::iterator Pos=find(PID_PMTs[pid].List.begin(), PID_PMTs[pid].List.end(), program_number);
        if (Pos!=PID_PMTs[pid].List.end())
            PID_PMTs[pid].List.erase(Pos);
        if (!PID_PMTs[pid].List.empty())
            ((File_Mpeg_Psi*)Streams[pid].Parser)->WantItAgain=true; //Disabling the tag "Finished", we want it again!
    }
}

//---------------------------------------------------------------------------
void File_MpegTs::PSI_network_information_table()
{
    File_Mpeg_Psi* Parser=(File_Mpeg_Psi*)Streams[pid].Parser;

    for (std::map<int16u, File_Mpeg_Psi::stream>::iterator Stream=Parser->Streams.begin(); Stream!=Parser->Streams.end(); Stream++)
    {
        //Enabling what we know parsing
        Streams[0x0000].Infos=Stream->second.Infos;
    }

    //Global infos
    for (std::map<std::string, ZenLib::Ztring>::iterator Info=Parser->Infos.begin(); Info!=Parser->Infos.end(); Info++)
        Streams[0x0000].Infos[Info->first]=Info->second;
}

//---------------------------------------------------------------------------
void File_MpegTs::PSI_atsc_psip()
{
    File_Mpeg_Psi* Parser=(File_Mpeg_Psi*)Streams[pid].Parser;

    Element_Info(Parser->Mpeg_Psi_Element_Name());

    for (std::map<int16u, File_Mpeg_Psi::stream>::iterator Stream=Parser->Streams.begin(); Stream!=Parser->Streams.end(); Stream++)
    {
        if (Stream->first>0x10) //Protection again erasing standard PID
        {
            Streams[Stream->first].TS_Kind=Stream->second.Kind;
            Streams[Stream->first].Searching_Payload_Start_Set(true);
        }
        if (Streams[Stream->first].Parser)
            ((File_Mpeg_Psi*)Streams[pid].Parser)->WantItAgain=true; //Disabling the tag "Finished", we want it again!
    }

    if (Streams[0x1FFB].Parser)
        ((File_Mpeg_Psi*)Streams[0x1FFB].Parser)->WantItAgain=true; //Disabling the tag "Finished", we want it again!

    for (std::map<int16u, File_Mpeg_Descriptors::program>::iterator Psi_Program=Parser->Programs.begin(); Psi_Program!=Parser->Programs.end(); Psi_Program++)
    {
        //Transfering the strings
        std::map<int16u, program>::iterator Program=Programs.find(Psi_Program->first);
        if (Program!=Programs.end())
            for (std::map<std::string, ZenLib::Ztring>::iterator Info=Psi_Program->second.Infos.begin(); Info!=Psi_Program->second.Infos.end(); Info++)
                Program->second.Infos[Info->first]=Info->second;
    }

    //Transfering the strings
    std::map<int16u, File_Mpeg_Psi::stream>::iterator Psi_Stream=Parser->Streams.find(0x0000);
    if (Psi_Stream!=Parser->Streams.end())
    {
        for (std::map<std::string, ZenLib::Ztring>::iterator Info=Psi_Stream->second.Infos.begin(); Info!=Psi_Stream->second.Infos.end(); Info++)
            Streams[0x0000].Infos[Info->first]=Info->second;
    }
}

//---------------------------------------------------------------------------
void File_MpegTs::PSI_dvb_sdt_bat_st()
{
    File_Mpeg_Psi* Parser=(File_Mpeg_Psi*)Streams[pid].Parser;

    for (std::map<int16u, File_Mpeg_Descriptors::program>::iterator Psi_Program=Parser->Programs.begin(); Psi_Program!=Parser->Programs.end(); Psi_Program++)
    {
        //Transfering the strings
        std::map<int16u, program>::iterator Program=Programs.find(Psi_Program->first);
        if (Program!=Programs.end())
            for (std::map<std::string, ZenLib::Ztring>::iterator Info=Psi_Program->second.Infos.begin(); Info!=Psi_Program->second.Infos.end(); Info++)
                Program->second.Infos[Info->first]=Info->second;
    }
}

//---------------------------------------------------------------------------
void File_MpegTs::PES()
{
    //Info
    Element_Info(Mpeg_Psi_stream_type(Streams[pid].stream_type, format_identifier));

    //Exists
    Streams[pid].StreamIsRegistred=true;

    //Case of encrypted streams
    if (Streams[pid].Scrambled)
    {
        //Don't need anymore
        Streams[pid].Searching_Payload_Start_Set(false);
        Streams[pid].Searching_Payload_Continue_Set(false);
        Skip_XX(Element_Size-Element_Offset,                    "Scrambled data");
        elementary_PID_Count--;

        //Demux
        Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(pid, 16)+_T(".mpg"));

        return;
    }

    //If unknown stream_type
    if (Streams[pid].Parser==NULL)
    {
        if (Mpeg_Psi_stream_Kind(Streams[pid].stream_type, format_identifier)==Stream_Max
         && Streams[pid].stream_type!=0x06 //Exception for private data
         && Streams[pid].stream_type<=0x7F //Exception for private data
         && Mpeg_Descriptors_stream_Kind(Streams[pid].descriptor_tag, format_identifier)==Stream_Max //From Descriptor
         && format_identifier!=0xFFFFFFFF) //No info
        {
            Streams[pid].Searching_Payload_Start_Set(false);
            Streams[pid].Searching_Payload_Continue_Set(false);
            elementary_PID_Count--;
            return;
        }
    }

    //Waiting for first payload_unit_start_indicator
    if (Streams[pid].Parser==NULL && !payload_unit_start_indicator)
    {
        Element_DoNotShow(); //We don't want to show this item because there is no interessant info
        return; //This is not the start of the PES
    }

    //Allocating an handle if needed
    if (Streams[pid].Parser==NULL)
    {
            #if defined(MEDIAINFO_MPEGPS_YES)
                Streams[pid].Parser=new File_MpegPs;
                Open_Buffer_Init(Streams[pid].Parser);
                ((File_MpegPs*)Streams[pid].Parser)->FromTS=true;
                ((File_MpegPs*)Streams[pid].Parser)->stream_type_FromTS=Streams[pid].stream_type;
                ((File_MpegPs*)Streams[pid].Parser)->descriptor_tag_FromTS=Streams[pid].descriptor_tag;
                ((File_MpegPs*)Streams[pid].Parser)->format_identifier_FromTS=Streams[pid].format_identifier;
                ((File_MpegPs*)Streams[pid].Parser)->MPEG_Version=2;
                Streams[pid].Searching_Payload_Continue_Set(true);
            #else
                //Filling
                Streams[pid].Parser=new File__Analyze();
                Open_Buffer_Init();
                //Streams_Count--;
            #endif

    }

    //Open MPEG-PS (PES)
    if (Streams[pid].Parser && (Streams[pid].Parser->File_GoTo==(int64u)-1 || Streams[pid].Parser->File_GoTo<File_Offset+Buffer_Offset) && Streams[pid].Parser->File_Offset!=Streams[pid].Parser->File_Size)
    {
        //Parsing
        Open_Buffer_Init(Streams[pid].Parser, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Streams[pid].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);

        //if (Element[Element_Level].ToShow.NoShow && payload_unit_start_indicator)
        //    Element[Element_Level].ToShow.NoShow=false; //Show it because this is the first interesssant payload

        //Need anymore?
        if (Streams[pid].Parser->File_GoTo!=(int64u)-1 || Streams[pid].Parser->File_Offset==Streams[pid].Parser->File_Size)
        {
            Streams[pid].Searching_Payload_Start_Set(false);
            Streams[pid].Searching_Payload_Continue_Set(false);
            elementary_PID_Count--;
        }
    }
    else
    {
        Streams[pid].Searching_Payload_Start_Set(false);
        Streams[pid].Searching_Payload_Continue_Set(false);
    }

    //Demux
    Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring::ToZtring(pid, 16)+_T(".mpg"));
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegTs::Synchronize()
{
    //Synchronizing
    while (           Buffer_Offset+188*3+BDAV_Size*4+1<=Buffer_Size
      && !(Buffer[Buffer_Offset+188*0+BDAV_Size*1]==0x47
        && Buffer[Buffer_Offset+188*1+BDAV_Size*2]==0x47
        && Buffer[Buffer_Offset+188*2+BDAV_Size*3]==0x47
        && Buffer[Buffer_Offset+188*3+BDAV_Size*4]==0x47))
        Buffer_Offset++;
    if (Buffer_Offset+188*3+BDAV_Size*4>=Buffer_Size)
    {
        //No synchro found
        if (Synched)
        {
            //Sync lost
            Info("MPEG-TS, Synchronisation lost");
            Synched=false;
        }
        //Managing first Synch attempt
        else if (!File_Name.empty() && File_Offset+Buffer_Size>=188*4+BDAV_Size*5 && Count_Get(Stream_General)==0)
            Finished(); //This is not a TS file, ending

        return false;
    }

    //Synched is OK
    if (Streams.empty())
    {
        //Default values
        Streams.resize(0x2000);
        Streams[0x00].TS_Kind=File_Mpeg_Psi::program_association_table;
        Streams[0x01].TS_Kind=File_Mpeg_Psi::conditional_access_table;
        Streams[0x02].TS_Kind=File_Mpeg_Psi::transport_stream_description_table;
        for (int32u Pos=0x03; Pos<0x10; Pos++)
            Streams[Pos].TS_Kind=File_Mpeg_Psi::reserved;
        for (int32u Pos=0x00; Pos<0x10; Pos++)
            Streams[Pos].Searching_Payload_Start_Set(true);

        //Count
        program_Count=(size_t)-1;
        elementary_PID_Count=0;

        //Temp
        format_identifier=0x00000000;
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
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_MpegTs::Header_Parser_QuickSearch()
{
    while (Buffer_Offset+TS_Size<=Buffer_Size)
    {
        if (Buffer[Buffer_Offset+BDAV_Size]!=0x47)
            break;

        //Getting PID
        int16u PID=CC2(Buffer+Buffer_Offset+BDAV_Size+1)&0x1FFF;

        if (Streams[PID].Searching)
        {
            //Searching start
            if (Streams[PID].Searching_Payload_Start)
            {
                int8u Info=Buffer[Buffer_Offset+BDAV_Size+1];
                if (Info&0x40) //payload_unit_start_indicator
                    return true;
            }

            //Searching continue
            if (Streams[PID].Searching_Payload_Continue)
                return true;

            //Adaptation layer
            if (Streams[PID].Searching_TimeStamp_Start || Streams[PID].Searching_TimeStamp_End)
            {
                if ((Buffer[Buffer_Offset+BDAV_Size+3]&0x20)==0x20) //Adaptation is present
                {
                    int8u PID_Adaptation_Info=Buffer[Buffer_Offset+BDAV_Size+5];
                    if (PID_Adaptation_Info&0x10) //PCR is present
                        return true;
                }
            }
        }

        //File__Duplicate
        if (Streams[PID].ShouldDuplicate)
        {
            Element_Size=TS_Size;
            File__Duplicate_Write(PID);
        }

        Buffer_Offset+=TS_Size;
    }

    if (Buffer_Offset==Buffer_Size)
        return false;
    if (Buffer_Offset+TS_Size<=Buffer_Size)
        Trusted_IsNot("MPEG-TS, Synchronisation lost");
    Synched=false;
    return Synchronize();
}

//---------------------------------------------------------------------------
void File_MpegTs::Detect_EOF()
{
    if (File_Offset+Buffer_Size>=MpegTs_JumpTo_Begin && Programs.empty() && format_identifier!=0xFFFFFFFF)
    {
        //Activating all streams as PES, in case of PAT/PMT are missing (ofen in .trp files)
        if (!Streams.empty())
        {
            for (size_t StreamID=0x20; StreamID<0x1FFF; StreamID++)
            {
                Streams[StreamID].Searching_Payload_Start_Set(true);
                Streams[StreamID].Searching_TimeStamp_Start_Set(true);
                Streams[StreamID].TS_Kind=File_Mpeg_Psi::pes;
            }
        }
        format_identifier=0xFFFFFFFF;
        File_GoTo=0;
        Fill(Stream_General, 0, General_Format_Profile, "No PAT/PMT");
    }


    //Jump to the end of the file
    if (File_Offset+Buffer_Offset>0x8000 && File_Offset+Buffer_Offset+MpegTs_JumpTo_End<File_Size && (
       (File_Offset+Buffer_Offset>=MpegTs_JumpTo_Begin)
    || (program_Count==0 && elementary_PID_Count==0)
    ))
    {
        //Details
        Info("MPEG-TS, Jumping to end of file");

        if (File_Size!=(int64u)-1) //Only if not unlimited
        {
            //Reactivating interessant TS streams
            if (!Streams.empty())
            {
                //End timestamp is out of date
                for (size_t StreamID=0; StreamID<0x2000; StreamID++)//std::map<int64u, stream>::iterator Stream=Streams.begin(); Stream!=Streams.end(); Stream++)
                {
                    //End timestamp is out of date
                    if (Streams[StreamID].TS_Kind==File_Mpeg_Psi::pes)
                        Streams[StreamID].TimeStamp_End=(int64u)-1;
                    if (Streams[StreamID].TimeStamp_Start!=(int64u)-1)
                        Streams[StreamID].Searching_TimeStamp_End_Set(!Streams[StreamID].Searching_TimeStamp_Start); //Searching only for a start found

                    //Specific
                    if (Streams[StreamID].TS_Kind>=File_Mpeg_Psi::ts_outofspec)
                        Streams[StreamID].Searching_Payload_Start_Set(false); //Does not search for DVB/ATSC anymore
                }
            }
        }

        File_GoTo=File_Size-MpegTs_JumpTo_End;
    }
}

//---------------------------------------------------------------------------
bool File_MpegTs::Detect_NonMPEGTS ()
{
    //File_Size
    if (File_Size<=8)
        return false; //We can't do detection

    //Element_Size
    if (Buffer_Size<=8)
        return true; //Must wait for more data

    //Detect mainly DAT files, and the parser is not enough precise to detect them later
    if (CC4(Buffer)==CC4("RIFF"))
    {
        Finished();
        return true;
    }

    //Detect MPEG-4 files, and the parser is not enough precise to detect them later if there is a lot of 0x47 in the size chunks
    if (CC4(Buffer+4)==CC4("ftyp") || CC4(Buffer+4)==CC4("moov") || CC4(Buffer+4)==CC4("mdat"))
    {
        Finished();
        return true;
    }

    //Seems OK
    return false;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::Option_Manage()
{
    if (!Streams.empty())
    {
        //File_Filter configuration
        if (Config->File_Filter_HasChanged())
        {
            bool Searching_Payload_Start=!Config->File_Filter_Get();
            for (int32u Pos=0x01; Pos<0x10; Pos++)
                Streams[Pos].Searching_Payload_Start_Set(Searching_Payload_Start); //base PID depends of File_Filter configuration
            Streams[0x00].Searching_Payload_Start_Set(true); //program_map
        }

        //File__Duplicate configuration
        if (File__Duplicate_HasChanged())
        {
            Streams[0x00].ShouldDuplicate=true;
            Streams[0x00].Searching_Payload_Start_Set(true); //Re-enabling program_map_table
        }
    }
}

//***************************************************************************
// Output_Buffer
//***************************************************************************

//---------------------------------------------------------------------------
size_t File_MpegTs::Output_Buffer_Get (const String &Code)
{
    if (size_t Size=File__Duplicate::Output_Buffer_Get(Code))
        return Size;

    //Parsing Parsers
    for (size_t Pos=0; Pos<Streams.size(); Pos++)
        if (Streams[Pos].Parser)
            if (size_t Size=Streams[Pos].Parser->Output_Buffer_Get(Code))
                return Size;

    return 0;
}

//---------------------------------------------------------------------------
size_t File_MpegTs::Output_Buffer_Get (size_t Pos_)
{
    //Optimization
    //if (Output_Buffer_Get_Pos.size()>Pos_ && Output_Buffer_Get_Pos[Pos_]!=(int16u)-1)
    //    return Streams[Output_Buffer_Get_Pos[Pos_]].Parser->Output_Buffer_Get(Pos_);

    if (size_t Size=File__Duplicate::Output_Buffer_Get(Pos_))
        return Size;

    //Parsing Parsers
    for (size_t Pos=0; Pos<Streams.size(); Pos++)
        if (Streams[Pos].Parser)
            if (size_t Size=Streams[Pos].Parser->Output_Buffer_Get(Pos_))
            {
                //Optimization
                //if (Output_Buffer_Get_Pos.size()<=Pos_)
                //    Output_Buffer_Get_Pos.resize(Pos_+1, (int16u)-1);
                //Output_Buffer_Get_Pos[Pos_]=Pos;

                return Size;
            }

    return 0;
}

} //NameSpace

#endif //MEDIAINFO_MPEGTS_YES
