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
#include <MediaInfo/Setup.h>
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
#include <memory>
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
extern const char*  Mpeg_Psi_stream_Codec(int8u ID, int32u format_identifier);
extern stream_t     Mpeg_Psi_stream_Kind(int32u ID, int32u format_identifier);
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
    /*Config.File_Filter_Set(83);
    File__Duplicate_Set(_T("83;file"));
    Config.File_Filter_Set(451);
    File__Duplicate_Set(_T("451;file"));
    Config.File_Filter_Set(452);
    File__Duplicate_Set(_T("452;file"));
    Config.File_Filter_Set(453);
    File__Duplicate_Set(_T("453;file"));
    Config.File_Filter_Set(454);
    File__Duplicate_Set(_T("454;file"));
    Config.File_Filter_Set(455);
    File__Duplicate_Set(_T("455;file"));
    Config.File_Filter_Set(456);
    File__Duplicate_Set(_T("456;file"));
    Config.File_Filter_Set(457);
    File__Duplicate_Set(_T("457;file"));
    Config.File_Filter_Set(459);
    File__Duplicate_Set(_T("459;file"));
    Config.File_Filter_Set(460);
    File__Duplicate_Set(_T("460;file"));
    Config.File_Filter_Set(461);
    File__Duplicate_Set(_T("461;file"));
    Config.File_Filter_Set(463);
    File__Duplicate_Set(_T("463;file"));
    Config.File_Filter_Set(464);
    File__Duplicate_Set(_T("464;file"));*/
    //Config.File_Filter_Set(1);
    //File__Duplicate_Set(_T("1;file"));
    //Config.File_Filter_Set(451);
    //File__Duplicate_Set(_T("451;file"));
    /*Config.File_Filter_Set(451);
    File__Duplicate_Set(_T("451;file"));
    Config.File_Filter_Set(452);
    File__Duplicate_Set(_T("452;file")); */
    //File__Duplicate_Set(_T("program_number=1;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\1.ts"));
    //File__Duplicate_Set(_T("program_map_PID=1000;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\1.ts"));
    //File__Duplicate_Set(_T("elementary_PID=1001;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\1.ts"));
    //File__Duplicate_Set(_T("elementary_PID=1002;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\1.ts"));
    //File__Duplicate_Set(_T("program_number=1;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\2.ts"));
    //File__Duplicate_Set(_T("elementary_PID=1001;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\2.ts"));
    //File__Duplicate_Set(_T("elementary_PID=1003;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\2.ts"));
    //File__Duplicate_Set(_T("program_number=1;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\3.ts"));
    //File__Duplicate_Set(_T("elementary_PID=1001;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\3.ts"));
    //File__Duplicate_Set(_T("elementary_PID=2063;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\3.ts"));
    //File__Duplicate_Set(_T("2;file://D:\\test 2008-02-06 - TS\\multitrack audio streams\\2.ts"));
    //File__Duplicate_Set(_T("452;file://e:\\test\\xxx.ts"));

    //Default values
    Streams[0x00].TS_Kind=File_Mpeg_Psi::program_association_table;
    Streams[0x01].TS_Kind=File_Mpeg_Psi::conditional_access_table;
    Streams[0x02].TS_Kind=File_Mpeg_Psi::transport_stream_description_table;
    for (int32u Pos=0x03; Pos<0x10; Pos++)
        Streams[Pos].TS_Kind=File_Mpeg_Psi::reserved;
    for (int32u Pos=0x00; Pos<0x10; Pos++)
        Streams[Pos].Searching_Payload_Start=true;

    //File__Duplicate
    Streams[0x00].ShouldDuplicate=true;

    //Temp
    format_identifier=0x00000000;
    BDAV_Size=0; //No BDAV header
    MpegTs_JumpTo_Begin=16*1024*1024;
    MpegTs_JumpTo_End=8*1024*1024;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::Read_Buffer_Continue()
{
    /*
    //Tests
    static int Step=0;
    if (Step==1 && File_Offset>30000000)
        Step++;
    if (Step==3 && File_Offset>60000000)
        Step++;
    if (Step==0)
    {
        Config.File_Filter_Set(452);
        File__Duplicate_Set(_T("452;file://e:\\test\\xxx.ts"));
        Step++;
    }
    else if (Step==2)
    {
        Config.File_Filter_Set(453);
        File__Duplicate_Set(_T("452"));
        File__Duplicate_Set(_T("453;file://e:\\test\\xxx.ts"));
        Step++;
    }
    else if (Step==4)
    {
        Config.File_Filter_Set(452);
        File__Duplicate_Set(_T("453"));
        File__Duplicate_Set(_T("452;file://e:\\test\\xxx.ts"));
        Step++;
    }
    */
    
    //We can accept a lost of synchronisation loss
    Trusted*=2;

    //File_Filter configuration
    if (Config.File_Filter_HasChanged())
    {
        bool Searching_Payload_Start=!Config.File_Filter_Get();
        for (int32u Pos=0x01; Pos<0x10; Pos++)
            Streams[Pos].Searching_Payload_Start=Searching_Payload_Start; //base PID depends of File_Filter configuration
        Streams[0x00].Searching_Payload_Start=true; //program_map
    }

    //File__Duplicate configuration
    if (File__Duplicate_HasChanged())
        Streams[0x00].Searching_Payload_Start=true; //Re-enabling program_map_table
}

//---------------------------------------------------------------------------
void File_MpegTs::Read_Buffer_Finalize()
{
    for (std::map<int64u, stream>::iterator Stream=Streams.begin(); Stream!=Streams.end(); Stream++)
    {
        if (Stream->second.TS_Kind==File_Mpeg_Psi::pes)
        {
            //By the parser
            StreamKind_Last=Stream_Max;
            if (Stream->second.Parser)
            {
                Stream->second.Parser->Open_Buffer_Finalize();
                Merge (*Stream->second.Parser);
            }
            //By the descriptors
            if (StreamKind_Last==Stream_Max && Stream->second.StreamIsRegistred
             && Mpeg_Descriptors_stream_Kind(Stream->second.descriptor_tag, format_identifier)!=Stream_Max)
            {
                StreamKind_Last=Mpeg_Descriptors_stream_Kind(Stream->second.descriptor_tag, format_identifier);
                Stream_Prepare(StreamKind_Last);
            }
            //By the stream_type
            if (StreamKind_Last==Stream_Max)
            {
                //The parser failed
                if (Stream->second.StreamIsRegistred && Mpeg_Psi_stream_Kind(Stream->second.stream_type, format_identifier)!=Stream_Max)
                {
                    StreamKind_Last=Mpeg_Psi_stream_Kind(Stream->second.stream_type, format_identifier);
                    Stream_Prepare(StreamKind_Last);
                }
            }

            //More info
            if (StreamKind_Last!=Stream_Max) //Found by the Parser or stream_type
            {
                //Codec
                if (Get(StreamKind_Last, StreamPos_Last, _T("Codec")).empty())
                    Fill("Codec", Mpeg_Psi_stream_Codec(Stream->second.stream_type, format_identifier));
                if (Get(StreamKind_Last, StreamPos_Last, _T("Codec")).empty())
                    Fill("Codec", Mpeg_Descriptors_stream_Codec(Stream->second.descriptor_tag, format_identifier));

                //TimeStamp
                if (Stream->second.TimeStamp_End!=(int64u)-1)
                {
                    if (Stream->second.TimeStamp_End<Stream->second.TimeStamp_Start)
                        Stream->second.TimeStamp_End+=0x200000000LL; //33 bits, cyclic
                    int64u PlayTime=Stream->second.TimeStamp_End-Stream->second.TimeStamp_Start;
                    if (PlayTime!=0 && PlayTime!=(int64u)-1)
                        Fill("PlayTime", PlayTime/90, 10, true);
                    else
                        Fill("PlayTime", "", 0, true, true); //Clear it
                }

                //Encryption
                if (Stream->second.Scrambled)
                    Fill("Encryption", "Encrypted");

                //TS info
                for (std::map<ZenLib::Ztring, ZenLib::Ztring>::iterator Info=Stream->second.Infos.begin(); Info!=Stream->second.Infos.end(); Info++)
                {
                    if (Get(StreamKind_Last, StreamPos_Last, Info->first).empty())
                        Fill(Info->first.To_Local().c_str(), Info->second);
                }

                //Common
                Fill("ID", Stream->first);
                Fill("ID/String", Decimal_Hexa(Stream->first));
                Fill("MenuID", Stream->second.program_number);
                Fill("MenuID/String", Decimal_Hexa(Stream->second.program_number));

                //Menu
                Programs[Stream->second.program_number].List.push_back(Ztring::ToZtring(Stream->first));
                Ztring Codec=Get(StreamKind_Last, StreamPos_Last, _T("Codec"));
                Programs[Stream->second.program_number].Codec.push_back(Codec);
                Ztring Language=Get(StreamKind_Last, StreamPos_Last, _T("Language"));
                Programs[Stream->second.program_number].Language.push_back(Language);
                Ztring Menu_Temp=Decimal_Hexa(Stream->first);
                Menu_Temp+=_T(" (");
                Menu_Temp+=Codec;
                if (!Language.empty())
                {
                    Menu_Temp+=_T(", ");
                    Menu_Temp+=Language;
                }
                Menu_Temp+=_T(")");
                Programs[Stream->second.program_number].Text.push_back(Menu_Temp);
            }
        }
        delete Stream->second.Parser; Stream->second.Parser=NULL;
    }
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Streams.clear();

    //Fill General
         if (!Video.empty() && Video[0](_T("Codec"))==_T("MPEG-2V"))
        Fill(Stream_General, 0, "Format", "MPEG-2TS");
    else if (!Video.empty() && Video[0](_T("Codec"))==_T("MPEG-4V"))
        Fill(Stream_General, 0, "Format", "MPEG-4TS");
    else if (!Video.empty() && Video[0](_T("Codec"))==_T("AVC"))
        Fill(Stream_General, 0, "Format", "MPEG-4TS");
    else
        Fill(Stream_General, 0, "Format", "MPEG-1TS");

    //Fill Menu
    if (Programs.size()>1)
    {
        for (std::map<int16u, program>::iterator Program=Programs.begin(); Program!=Programs.end(); Program++)
        {
            Stream_Prepare(Stream_Menu);
            Fill("MenuID", Program->first);
            Fill("MenuID/String", Decimal_Hexa(Program->first));
            Fill("ID", Programs[Program->first].pid);
            Fill("ID/String", Decimal_Hexa(Programs[Program->first].pid));
            Program->second.List.Separator_Set(0, _T(" / "));
            Fill("List", Program->second.List.Read());
            Program->second.Text.Separator_Set(0, _T(" / "));
            Fill("List/String", Program->second.Text.Read());
            Program->second.Language.Separator_Set(0, _T(" / "));
            Fill("Language", Program->second.Language.Read());
            Program->second.Codec.Separator_Set(0, _T(" / "));
            Fill("Codec", Program->second.Codec.Read());
        }
    }
    Programs.clear();

    File__Duplicate::Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegTs::Header_Begin()
{
    //Must have enough buffer for having header
    if (Buffer_Offset+4+BDAV_Size>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && CC1(Buffer+Buffer_Offset+BDAV_Size)!=0x47)
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
    Header_Fill_Size(188+BDAV_Size);

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
    if (Streams[pid].program_number>0)
        Data_Info(Ztring::ToZtring_From_CC2(Streams[pid].program_number));
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
        {
            //Encrypted
            Streams[pid].Searching_Payload_Start=false;
            Streams[pid].Scrambled=true;
            Skip_XX(188+BDAV_Size-Element_Offset,                  "Scrambled data");
        }
    }
    else if (Element_Offset<188+BDAV_Size)
        Skip_XX(188+BDAV_Size-Element_Offset,                      "Junk");
}

//---------------------------------------------------------------------------
//
void File_MpegTs::Header_Parse_AdaptationField()
{
    size_t Element_Pos_Save=Element_Offset;
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
            Get_S8 (33, program_clock_reference_base,           "program_clock_reference_base"); Param_Info(Ztring().Duration_From_Milliseconds(program_clock_reference_base/90));
            if (Streams[pid].Searching_TimeStamp_Start)
            {
                //This is the first PCR
                Streams[pid].TimeStamp_Start=program_clock_reference_base;
                Streams[pid].Searching_TimeStamp_Start=false;
            }
            Streams[pid].TimeStamp_End=program_clock_reference_base;
            Data_Info(Ztring().Duration_From_Milliseconds(program_clock_reference_base/90));
            Skip_S1( 6,                                         "reserved");
            Skip_S2( 9,                                         "program_clock_reference_extension");
            BS_End();

            //Test if we can find the TS bitrate
            if (program_clock_reference_base<Streams[pid].TimeStamp_Start)
                program_clock_reference_base+=0x200000000LL; //33 bits, cyclic
            if ((program_clock_reference_base-Streams[pid].TimeStamp_Start)/90>4000)
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
    if (!Streams[pid].Searching_Payload_Start && !Streams[pid].Searching_Payload_Continue)
    {
        //File__Duplicate
        if (Streams[pid].ShouldDuplicate)
            File__Duplicate_Write();

        return;
    }

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

    //File__Duplicate
    if (Streams[pid].ShouldDuplicate)
        File__Duplicate_Write();
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
        Open_Buffer_Init(Streams[pid].Parser, File_Size, File_Offset+Buffer_Offset);
    }

    //Parsing
    Open_Buffer_Continue(Streams[pid].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);

    //Filling
    if (Streams[pid].Parser->File_Offset==Streams[pid].Parser->File_Size)
    {
        //Finnished, we can fill data
        switch (Streams[pid].TS_Kind)
        {
            case File_Mpeg_Psi::program_association_table : PSI_program_association_table(); break;
            case File_Mpeg_Psi::program_map_table :         PSI_program_map_table(); break;
            default : ;
        }

        //Disabling this PID
        delete Streams[pid].Parser; Streams[pid].Parser=NULL;
        if (Streams[pid].TS_Kind==File_Mpeg_Psi::program_association_table
         || Streams[pid].TS_Kind==File_Mpeg_Psi::program_map_table)
        {
            Streams[pid].Searching_Payload_Start=false;
            Streams[pid].Searching_Payload_Continue=false;
        }
        else
        {
            Streams[pid].Searching_Payload_Start=true;
            Streams[pid].Searching_Payload_Continue=true;
        }
    }
    else
        //Waiting for more data
        Streams[pid].Searching_Payload_Continue=true;
}

//---------------------------------------------------------------------------
void File_MpegTs::PSI_program_association_table()
{
    File_Mpeg_Psi* Parser=(File_Mpeg_Psi*)Streams[pid].Parser;
    for (std::map<int16u, File_Mpeg_Psi::stream>::iterator Program=Parser->Streams.begin(); Program!=Parser->Streams.end(); Program++)
    {
        int16u PID=Program->first;

        //Program
        if (Program->second.program_number!=0x0000)
            Programs[Program->second.program_number].pid=PID;

        //Enabling what we know parsing
        Streams[PID].TS_Kind=Program->second.Kind;
        Streams[PID].program_number=Program->second.program_number;
        Streams[PID].Searching_Payload_Start=true;

        //File_Filter
        if (!Config.File_Filter_Get(Streams[Program->first].program_number))
        {
            Streams[PID].Searching_Payload_Start=false;
            Streams[PID].Searching_Payload_Continue=false;
        }
        else
        {
            Streams[PID].Searching_Payload_Start=true;
            Streams[PID].Searching_Payload_Continue=true;
        }

        //File__Duplicate
        if (File__Duplicate_Get_From_program_number(Program->second.program_number))
            Streams[PID].ShouldDuplicate=true;
        else
            Streams[PID].ShouldDuplicate=false;

        //Menus
        //ProgramNumber2StreamNumber[Parser->program_association_section_ProgramNumber[Pos]]=elementary_PID;
    }

    //Filling
    Fill(Stream_General, 0, "ID", Parser->transport_stream_id, 16, true);
}

//---------------------------------------------------------------------------
void File_MpegTs::PSI_program_map_table()
{
    File_Mpeg_Psi* Parser=(File_Mpeg_Psi*)Streams[pid].Parser;
    format_identifier=Parser->format_identifier;
    for (std::map<int16u, File_Mpeg_Psi::stream>::iterator Stream=Parser->Streams.begin(); Stream!=Parser->Streams.end(); Stream++)
    {
        //Retrieving info
        int32u elementary_PID=Stream->first;

        //Enabling what we know parsing
        //Streams[elementary_PID].stream_type=program_map_Values_Temp->second.stream_type;
        //Streams[elementary_PID].KindOfStream=program_map_Values_Temp->second.KindOfStream;
        //Streams[elementary_PID].Info=program_map_Values_Temp->second.Info;
        /*
        if (Mpeg_Psi_stream_Kind(Streams[elementary_PID].stream_type)!=Stream_Max  //Known
         || Streams[elementary_PID].KindOfStream!=Stream_Max
         || (Streams[elementary_PID].stream_type>=0x80 && Streams[elementary_PID].stream_type<=0xFF) //Private
           )
        {
            Streams[elementary_PID].TS_Kind=pes;
            Streams[elementary_PID].TS_Needed=true;
            Streams[elementary_PID].PES_Needed=true;
        }
        */
        if (Streams[elementary_PID].Parser==NULL) //Not yet parsed
            Streams[elementary_PID].TS_Kind=File_Mpeg_Psi::pes;
        if (elementary_PID!=0x0000 && Stream->second.program_number!=0x0000)
            Streams[elementary_PID].Infos=Stream->second.Infos;
        Streams[elementary_PID].program_number=Stream->second.program_number;
        Streams[elementary_PID].stream_type=Stream->second.stream_type;
        Streams[elementary_PID].descriptor_tag=Stream->second.descriptor_tag;
        Streams[elementary_PID].Searching_Payload_Start=true;
        Streams[elementary_PID].Searching_TimeStamp_Start=true;
        if (MpegTs_JumpTo_Begin+MpegTs_JumpTo_End>=File_Size)
            Streams[elementary_PID].Searching_TimeStamp_End=true;
        if (File__Duplicate_Get_From_program_number(Streams[elementary_PID].program_number))
            Streams[elementary_PID].ShouldDuplicate=true;

        //Not precised PID handling
        /*
        for (size_t Pos=0x11; Pos<0x1FFF; Pos++)
        {
            if (Stream.find(Pos)==Stream.end() || Streams[Pos].TS_Kind==File_Mpeg_Psi::unknown)
            {
                //File_Filter
                if (!Config.File_Filter_Get())
                    Streams[Pos].Searching_Payload_Start=true;

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
            }

            if (Pos==0x002F)
                Pos=0x1ABB; //Skipping normal data
            if (Pos==0x01ABC)
                Pos=0x1FF6; //Skipping normal data
        }
        */
    }
}

//---------------------------------------------------------------------------
void File_MpegTs::PES()
{
    //Info
    Element_Info(Mpeg_Psi_stream_type(Streams[Element_Code].stream_type, format_identifier));

    //Exists
    Streams[Element_Code].StreamIsRegistred=true;

    //If unknown stream_type
    if (Streams[Element_Code].Parser==NULL)
    {
        if (Mpeg_Psi_stream_Kind(Streams[Element_Code].stream_type, format_identifier)==Stream_Max
         && Streams[Element_Code].stream_type!=0x06) //Exception for private data
        {
            Streams[Element_Code].Searching_Payload_Start=false;
            Streams[Element_Code].Searching_Payload_Continue=false;
            return;
        }
    }

    //Waiting for first payload_unit_start_indicator
    if (Streams[Element_Code].Parser==NULL && !payload_unit_start_indicator)
    {
        Element_DoNotShow(); //We don't want to show this item because there is no interessant info
        return; //This is not the start of the PES
    }

    //Allocating an handle if needed
    if (Streams[Element_Code].Parser==NULL)
    {
            #if defined(MEDIAINFO_MPEGPS_YES)
                Streams[Element_Code].Parser=new File_MpegPs;
                ((File_MpegPs*)Streams[Element_Code].Parser)->FromTS=true;
                ((File_MpegPs*)Streams[Element_Code].Parser)->stream_type_FromTS=Streams[Element_Code].stream_type;
                ((File_MpegPs*)Streams[Element_Code].Parser)->descriptor_tag_FromTS=Streams[Element_Code].descriptor_tag;
                ((File_MpegPs*)Streams[Element_Code].Parser)->MPEG_Version=2;
                Streams[Element_Code].Searching_Payload_Continue=true;
            #else
                //Filling
                Streams[Element_Code].Parser=new File__Analyze();
                //Streams_Count--;
            #endif

    }

    //Open MPEG-PS (PES)
    if (Streams[Element_Code].Parser && (Streams[Element_Code].Parser->File_GoTo==(int64u)-1 || Streams[Element_Code].Parser->File_GoTo<File_Offset+Buffer_Offset) && Streams[Element_Code].Parser->File_Offset!=Streams[Element_Code].Parser->File_Size)
    {
        //Parsing
        Open_Buffer_Init(Streams[Element_Code].Parser, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Streams[Element_Code].Parser, Buffer+Buffer_Offset, (size_t)Element_Size);

        //if (Element[Element_Level].ToShow.NoShow && payload_unit_start_indicator)
        //    Element[Element_Level].ToShow.NoShow=false; //Show it because this is the first interesssant payload

        //Need anymore?
        if (Streams[Element_Code].Parser->File_GoTo!=(int64u)-1 || Streams[Element_Code].Parser->File_Offset==Streams[Element_Code].Parser->File_Size)
        {
            Streams[Element_Code].Searching_Payload_Start=false;
            Streams[Element_Code].Searching_Payload_Continue=false;
            //Streams_Count--;
        }
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
      && !(CC1(Buffer+Buffer_Offset+188*0+BDAV_Size*1)==0x47
        && CC1(Buffer+Buffer_Offset+188*1+BDAV_Size*2)==0x47
        && CC1(Buffer+Buffer_Offset+188*2+BDAV_Size*3)==0x47
        && CC1(Buffer+Buffer_Offset+188*3+BDAV_Size*4)==0x47))
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
        else if (File_Offset+Buffer_Size>=188*4+BDAV_Size*5 && Count_Get(Stream_General)==0)
            Finnished(); //This is not a TS file, ending

        return false;
    }

    //There is no start code, so Stream_General is filled here
    if (Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);

    //Synched is OK
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_MpegTs::Header_Parser_QuickSearch()
{
    while (Buffer_Offset+188+BDAV_Size<=Buffer_Size)
    {
        if (CC1(Buffer+Buffer_Offset+BDAV_Size)!=0x47)
            break;

        //Getting PID
        int16u PID=CC2(Buffer+Buffer_Offset+BDAV_Size+1)&0x1FFF;

        //Searching start
        if (Streams[PID].Searching_Payload_Start)
        {
            int8u Info=CC1(Buffer+Buffer_Offset+BDAV_Size+1);
            if (Info&0x40) //payload_unit_start_indicator
                return true;
        }

        //Searching continue
        if (Streams[PID].Searching_Payload_Continue)
            return true;

        //Adaptation layer
        if (Streams[PID].Searching_TimeStamp_Start || Streams[PID].Searching_TimeStamp_End)
        {
            if ((CC1(Buffer+Buffer_Offset+BDAV_Size+3)&0x20)==0x20) //Adaptation is present
            {
                int8u PID_Adaptation_Info=CC1(Buffer+Buffer_Offset+BDAV_Size+5);
                if (PID_Adaptation_Info&0x10) //PCR is present
                    return true;
            }
        }

        //File__Duplicate
        if (Streams[PID].ShouldDuplicate)
        {
            Element_Size=188+BDAV_Size;
            File__Duplicate_Write();
        }

        Buffer_Offset+=188+BDAV_Size;
    }

    if (Buffer_Offset+188+BDAV_Size>Buffer_Size)
    {
        Synched=false;
        Synchronize();
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
void File_MpegTs::Detect_EOF()
{
    //Jump to the end of the file
    if (File_Offset+Buffer_Offset+MpegTs_JumpTo_End<File_Size && (
       (File_Offset+Buffer_Offset>=MpegTs_JumpTo_Begin)
    //|| (Streams_Count==0)
    ))
    {
        //Details
        Info("MPEG-TS, Jumping to end of file");

        //Reactivating interessant TS streams
        std::map<int64u, stream>::iterator Temp=Streams.begin();
        while (Temp!=Streams.end())
        {
            //End timestamp is out of date
            if (Temp->second.TS_Kind==File_Mpeg_Psi::pes)
                Temp->second.TimeStamp_End=(int64u)-1;
            Temp->second.Searching_TimeStamp_End=!Temp->second.Searching_TimeStamp_Start; //Searching only for a start found

            Temp++;
        }

        File_GoTo=File_Size-MpegTs_JumpTo_End;
    }
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
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

#endif //MEDIAINFO_MPEGTS_YES
