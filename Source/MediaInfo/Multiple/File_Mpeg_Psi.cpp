// File_Mpeg_Psi - Info for MPEG Stream files
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
#if defined(MEDIAINFO_MPEGPS_YES) || defined(MEDIAINFO_MPEGTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Mpeg_Psi.h"
#include "MediaInfo/Multiple/File_Mpeg_Descriptors.h"
#include <memory>
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const char* Mpeg_Psi_ATSC_table_type(int16u ID)
{
    switch (ID)
    {
        case 0x0000 : return "Terrestrial VCT with current_next_indicator=1";
        case 0x0001 : return "Terrestrial VCT with current_next_indicator=0";
        case 0x0002 : return "Cable VCT with current_next_indicator=1";
        case 0x0003 : return "Cable VCT with current_next_indicator==0";
        case 0x0004 : return "Channel ETT";
        case 0x0005 : return "DCCSCT";
        case 0x0010 : return "Short-form Virtual Channel Table-VCM Subtyp";
        case 0x0011 : return "Short-form Virtual Channel Table-DCM Subtyp";
        case 0x0012 : return "Short-form Virtual Channel Table-ICM Subtyp";
        case 0x0020 : return "Network Information Table-CDS Table Subtype";
        case 0x0021 : return "Network Information Table-MMS Table Subtype";
        case 0x0030 : return "Network Text Tabl e-SNS Subtype";
        default :
            if (ID>=0x0100
             && ID<=0x017F) return "Event Information Table (EIT)";
            if (ID>=0x0200
             && ID<=0x027F) return "Event Extended Text Table (EETT)";
            if (ID>=0x301
             && ID<=0x03FF) return "Rating Region Table (RRT)";
            if (ID>=0x0400
             && ID<=0x0FFF) return "User private";
            if (ID>=0x1000
             && ID<=0x10FF) return "Aggregate Event Information Table (AEIT)";
            if (ID>=0x1100
             && ID<=0x11FF) return "Aggregate Extended Text Table (AETT)";
            if (ID>=0x1400
             && ID<=0x14FF) return "DCCT";
            return "Reserved";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg_Psi_stream_type(int8u ID, int32u format_identifier)
{
    switch (ID)
    {
        case 0x00 : return "ITU-T | ISO/IEC Reserved";
        case 0x01 : return "ISO/IEC 11172 Video";
        case 0x02 : return "ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream";
        case 0x03 : return "ISO/IEC 11172 Audio";
        case 0x04 : return "ISO/IEC 13818-3 Audio";
        case 0x05 : return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections";
        case 0x06 : return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data";
        case 0x07 : return "ISO/IEC 13522 MHEG";
        case 0x08 : return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC";
        case 0x09 : return "ITU-T Rec. H.222.1";
        case 0x0A : return "ISO/IEC 13818-6 type A";
        case 0x0B : return "ISO/IEC 13818-6 type B";
        case 0x0C : return "ISO/IEC 13818-6 type C";
        case 0x0D : return "ISO/IEC 13818-6 type D";
        case 0x0E : return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary";
        case 0x0F : return "ISO/IEC 13818-7 Audio with ADTS transport syntax";
        case 0x10 : return "ISO/IEC 14496-2 Visual";
        case 0x11 : return "ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1";
        case 0x12 : return "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets";
        case 0x13 : return "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.";
        case 0x14 : return "ISO/IEC 13818-6 Synchronized Download Protocol";
        case 0x15 : return "Metadata carried in PES packets";
        case 0x16 : return "Metadata carried in metadata_sections";
        case 0x17 : return "Metadata carried in ISO/IEC 13818-6 Data Carousel";
        case 0x18 : return "Metadata carried in ISO/IEC 13818-6 Object Carousel";
        case 0x19 : return "Metadata carried in ISO/IEC 13818-6 Synchronized Download Protocol";
        case 0x1A : return "IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP)";
        case 0x1B : return "AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 Video";
        case 0x1C : return "ISO/IEC 14496-3 Audio, without using any additional transport syntax";
        case 0x1D : return "ISO/IEC 14496-17 Text";
        case 0x1E : return "Auxiliary video data stream as defined in ISO/IEC 23002-3";
        case 0x7F : return "IPMP stream";
        default :
            if (ID<=0x7F) return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 reserved";
            switch (format_identifier)
            {
                case Mpeg_Descriptors::CUEI :
                case Mpeg_Descriptors::SCTE : //SCTE
                case Mpeg_Descriptors::GA94 :
                case Mpeg_Descriptors::S14A : //ATSC
                        switch (ID)
                        {
                            case 0x81 : return "ATSC - AC-3";
                            case 0x82 : return "SCTE - Standard Subtitle";
                            case 0x83 : return "SCTE - Isochronous Data";
                            case 0x84 : return "ATSC - Reserved";
                            case 0x85 : return "ATSC - Program Identifier";
                            case 0x87 : return "ATSC - E-AC-3";
                            case 0x90 : return "DVB  - stream_type value for Time Slicing / MPE-FEC";
                            case 0x95 : return "ATSC - Data Service Table, Network Resources Table";
                            default   : return "ATSC/SCTE - Unknown";
                        }
                case Mpeg_Descriptors::HDMV : //Bluray
                        switch (ID)
                        {
                            case 0x80 : return "BluRay - PCM";
                            case 0x81 : return "BluRay - AC-3";
                            case 0x82 : return "BluRay - DTS";
                            case 0x83 : return "BluRay - AC-3 (TrueHD)";
                            case 0x84 : return "BluRay - E-AC-3";
                            case 0x85 : return "BluRay - DTS (HD-HRA)";
                            case 0x86 : return "BluRay - DTS (HD-MA)";
                            case 0x90 : return "BluRay - PGS";
                            case 0xA1 : return "BluRay - AC-3";
                            case 0xA2 : return "BluRay - DTS";
                            case 0xEA : return "BluRay - VC-1";
                            default   : return "Bluray - Unknown";
                        }
                case 0xFFFFFFFF : //Unknown
                        return "";
                default                     :
                        switch (ID)
                        {
                            case 0x80 : return "DigiCipher II video";
                            case 0x81 : return "AC-3";
                            case 0x88 : return "VC-1";
                            case 0x87 : return "E-AC-3";
                            case 0xD1 : return "Dirac";
                            default   : return "User Private";
                        }
            }
    }
}

//---------------------------------------------------------------------------
const char* Mpeg_Psi_stream_Format(int8u ID, int32u format_identifier)
{
    switch (ID)
    {
        case 0x01 : return "MPEG Video"; //Version 1
        case 0x02 : return "MPEG Video"; //Version 2
        case 0x03 : return "MPEG Audio"; //Version 1
        case 0x04 : return "MPEG Audio";    //Version 2
        case 0x0F : return "AAC";
        case 0x10 : return "MPEG-4 Visual";
        case 0x11 : return "AAC";
        case 0x1B : return "AVC";
        case 0x1C : return "AAC";
        case 0x1D : return "Timed Text";
        default :
            switch (format_identifier)
            {
                case Mpeg_Descriptors::CUEI :
                case Mpeg_Descriptors::SCTE : //SCTE
                case Mpeg_Descriptors::GA94 :
                case Mpeg_Descriptors::S14A : //ATSC
                        switch (ID)
                        {
                            case 0x81 : return "AC-3";
                            case 0x82 : return "Text";
                            case 0x87 : return "E-AC-3";
                            default   : return "";
                        }
                case Mpeg_Descriptors::HDMV : //Bluray
                        switch (ID)
                        {
                            case 0x80 : return "PCM";
                            case 0x81 : return "AC-3";
                            case 0x82 : return "DTS";
                            case 0x83 : return "AC-3"; // (TrueHD)"
                            case 0x84 : return "E-AC-3";
                            case 0x85 : return "DTS"; //" (HD-HRA)"
                            case 0x86 : return "DTS"; //" (HD-MA)"
                            case 0x90 : return "PGS";
                            case 0xA1 : return "AC-3";
                            case 0xA2 : return "DTS";
                            case 0xEA : return "VC-1";
                            default   : return "";
                        }
                case 0xFFFFFFFF : //Unknown
                        return "";
                default                     :
                        switch (ID)
                        {
                            case 0x80 : return "MPEG Video";
                            case 0x81 : return "AC-3";
                            case 0x87 : return "E-AC-3";
                            case 0x88 : return "VC-1";
                            case 0xD1 : return "Dirac";
                            default   : return "";
                        }
            }
    }
}

//---------------------------------------------------------------------------
const char* Mpeg_Psi_stream_Codec(int8u ID, int32u format_identifier)
{
    switch (ID)
    {
        case 0x01 : return "MPEG-1V";
        case 0x02 : return "MPEG-2V";
        case 0x03 : return "MPEG-1A";
        case 0x04 : return "MPEG-2A";
        case 0x0F : return "AAC";
        case 0x10 : return "MPEG-4V";
        case 0x11 : return "AAC";
        case 0x1B : return "AVC";
        case 0x1C : return "AAC";
        case 0x1D : return "Text";
        default :
            switch (format_identifier)
            {
                case Mpeg_Descriptors::CUEI :
                case Mpeg_Descriptors::SCTE : //SCTE
                case Mpeg_Descriptors::GA94 :
                case Mpeg_Descriptors::S14A : //ATSC
                        switch (ID)
                        {
                            case 0x81 : return "AC3";
                            case 0x82 : return "Text";
                            case 0x87 : return "AC3+";
                            default   : return "";
                        }
                case Mpeg_Descriptors::HDMV : //Bluray
                        switch (ID)
                        {
                            case 0x80 : return "PCM";
                            case 0x81 : return "AC3";
                            case 0x82 : return "DTS";
                            case 0x83 : return "AC3+";
                            case 0x86 : return "DTS";
                            case 0x90 : return "PGS";
                            case 0xEA : return "VC1";
                            default   : return "";
                        }
                case 0xFFFFFFFF : //Unknown
                        return "";
                default                     :
                        switch (ID)
                        {
                            case 0x80 : return "MPEG-2V";
                            case 0x81 : return "AC3";
                            case 0x87 : return "AC3+";
                            case 0x88 : return "VC-1";
                            case 0xD1 : return "Dirac";
                            default   : return "";
                        }
            }
    }
}

//---------------------------------------------------------------------------
stream_t Mpeg_Psi_stream_Kind(int32u ID, int32u format_identifier)
{
    switch (ID)
    {
        case 0x01 : return Stream_Video;
        case 0x02 : return Stream_Video;
        case 0x03 : return Stream_Audio;
        case 0x04 : return Stream_Audio;
        case 0x0F : return Stream_Audio;
        case 0x10 : return Stream_Video;
        case 0x11 : return Stream_Audio;
        case 0x1B : return Stream_Video;
        case 0x1C : return Stream_Audio;
        case 0x1D : return Stream_Text;
        default :
            switch (format_identifier)
            {
                case Mpeg_Descriptors::CUEI :
                case Mpeg_Descriptors::SCTE : //SCTE
                case Mpeg_Descriptors::GA94 :
                case Mpeg_Descriptors::S14A : //ATSC
                        switch (ID)
                        {
                            case 0x81 : return Stream_Audio;
                            case 0x82 : return Stream_Text;
                            case 0x87 : return Stream_Audio;
                            default   : return Stream_Max;
                        }
                case Mpeg_Descriptors::HDMV : //Bluray
                        switch (ID)
                        {
                            case 0x80 : return Stream_Audio;
                            case 0x81 : return Stream_Audio;
                            case 0x82 : return Stream_Audio;
                            case 0x83 : return Stream_Audio;
                            case 0x84 : return Stream_Audio;
                            case 0x85 : return Stream_Audio;
                            case 0x86 : return Stream_Audio;
                            case 0x90 : return Stream_Text;
                            case 0xA1 : return Stream_Audio;
                            case 0xA2 : return Stream_Audio;
                            case 0xEA : return Stream_Video;
                            default   : return Stream_Max;
                        }
                case 0xFFFFFFFF : //Unknown
                        return Stream_Max;
                default                     :
                        switch (ID)
                        {
                            case 0x80 : return Stream_Video;
                            case 0x81 : return Stream_Audio;
                            case 0x87 : return Stream_Audio;
                            case 0x88 : return Stream_Video;
                            case 0xD1 : return Stream_Video;
                            default   : return Stream_Max;
                        }
            }
    }
}

//---------------------------------------------------------------------------
const char* File_Mpeg_Psi::Mpeg_Psi_Element_Name() //Part of File_Mpeg_Psi for acceding to Element_Code
{
    switch (Element_Code)
    {
        case 0x00 : return "program_association_section";
        case 0x01 : return "conditional_access_section";
        case 0x02 : return "TS_program_map_section";
        case 0x03 : return "TS_description_section";
        case 0x04 : return "ISO_IEC_14496_scene_description_section";
        case 0x05 : return "ISO_IEC_14496_object_descriptor_section";
        case 0x06 : return "Metadata?";
        case 0x38 : return "ISO/IEC 13818-6 reserved";
        case 0x39 : return "DSM-CC addressable section";
        case 0x3A : return "DSM-CC : return MPE";
        case 0x3B : return "DSM-CC : return U-N messages : return except DDM";
        case 0x3C : return "DSM-CC : return DDM";
        case 0x3D : return "DSM-CC : return stream descriptors";
        case 0x3E : return "DSM-CC : return private data : return IP-Datagram";
        case 0x3F : return "DSM-CC addressable section";
        case 0x40 : return "DVB - network_information_section - actual_network";
        case 0x41 : return "DVB - network_information_section - other_network";
        case 0x42 : return "DVB - service_description_section - actual_transport_stream";
        case 0x46 : return "DVB - service_description_section - other_transport_stream";
        case 0x4A : return "DVB - bouquet_association_section";
        case 0x4E : return "DVB - event_information_section - actual_transport_stream : return present/following";
        case 0x4F : return "DVB - event_information_section - other_transport_stream : return present/following";
        case 0x50 :
        case 0x51 :
        case 0x52 :
        case 0x53 :
        case 0x54 :
        case 0x55 :
        case 0x56 :
        case 0x57 :
        case 0x58 :
        case 0x59 :
        case 0x5A :
        case 0x5B :
        case 0x5C :
        case 0x5E :
        case 0x5F : return "DVB - event_information_section - actual_transport_stream : return schedule";
        case 0x60 :
        case 0x61 :
        case 0x62 :
        case 0x63 :
        case 0x64 :
        case 0x65 :
        case 0x66 :
        case 0x67 :
        case 0x68 :
        case 0x69 :
        case 0x6A :
        case 0x6B :
        case 0x6C :
        case 0x6D :
        case 0x6E :
        case 0x6F : return "DVB - event_information_section - other_transport_stream : return schedule";
        case 0x70 : return "DVB - time_date_section";
        case 0x71 : return "DVB - running_status_section";
        case 0x72 : return "DVB - stuffing_section";
        case 0x73 : return "DVB - time_offset_section";
        case 0x74 : return "DVB - application information section";
        case 0x75 : return "DVB - container section";
        case 0x76 : return "DVB - related content section";
        case 0x77 : return "DVB - content identifier section";
        case 0x78 : return "DVB - MPE-FEC section";
        case 0x79 : return "DVB - resolution notification section";
        case 0x7E : return "DVB - discontinuity_information_section";
        case 0x7F : return "DVB - selection_information_section";
        case 0xC0 : return "ATSC - Program Information Message";
        case 0xC1 : return "ATSC - Program Name Message";
        case 0xC2 : return "ATSC/SCTE - Network Information Message";
        case 0xC3 : return "ATSC/SCTE - Network Text Table (NTT)";
        case 0xC4 : return "ATSC/SCTE - Short Form Virtual Channel Table (S-VCT)";
        case 0xC5 : return "ATSC/SCTE - System Time Table (STT)";
        case 0xC6 : return "ATSC/SCTE - Subtitle Message";
        case 0xC7 : return "ATSC - Master Guide Table (MGT)";
        case 0xC8 : return "ATSC - Terrestrial Virtual Channel Table (TVCT)";
        case 0xC9 : return "ATSC - Cable Virtual Channel Table (CVCT) / Long-form Virtual Channel Table (L-VCT)";
        case 0xCA : return "ATSC - Rating Region Table (RRT)";
        case 0xCB : return "ATSC - Event Information Table (EIT)";
        case 0xCC : return "ATSC - Extended Text Table (ETT)";
        case 0xCD : return "ATSC - System Time Table (STT)";
        case 0xCE : return "ATSC - Data Event Table (DET)";
        case 0xCF : return "ATSC - Data Service Table (DST)";
        case 0xD0 : return "ATSC - Program Identifier Table (PIT)";
        case 0xD1 : return "ATSC - Network Resource Table (NRT)";
        case 0xD2 : return "ATSC - Long-term Service Table (L-TST)";
        case 0xD3 : return "ATSC - Directed Channel Change Table (DCCT)";
        case 0xD4 : return "ATSC - DCC Selection Code Table (DCCSCT)";
        case 0xD5 : return "ATSC - Selection Information Table (SIT)";
        case 0xD6 : return "ATSC - Aggregate Event Information Table (AEIT)";
        case 0xD7 : return "ATSC - Aggregate Extended Text Table (AETT)";
        case 0xD8 : return "ATSC - Cable Emergency Alert";
        case 0xD9 : return "ATSC - Aggregate Data Event Table";
        case 0xDA : return "ATSC - Satellite VCT";
        default :
            if (table_id>=0x06
             && table_id<=0x37) return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 reserved";
            if (table_id>=0x40
             && table_id<=0x7F) return "DVB - reserved";
            if (table_id>=0x80
             && table_id<=0x8F) return "CA message : return EMM : return ECM";
            if (table_id>=0xC0
             && table_id<=0xDF) return "ATSC/SCTE - reserved";
            if (table_id<=0xFE) return "User Private";
            return "unknown";
    }
}

const char* Mpeg_Psi_atsc_service_type(int8u service_type)
{
    switch (service_type)
    {
        case 0x00 : return "reserved";
        case 0x01 : return "Analog television";
        case 0x02 : return "Digital television";
        case 0x03 : return "Digital radio";
        case 0x04 : return "Data";
        default   : return "reserved for future use";
    }
}
//---------------------------------------------------------------------------
// CRC_32_Table
// A CRC is computed like this:
// Init: int32u CRC_32 = 0xFFFFFFFF;
// for each data byte do
//     CRC_32=(CRC_32<<8) ^ CRC_32_Table[(CRC_32>>24)^(data_byte)];
int32u Psi_CRC_32_Table[256] =
{
  0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9,
  0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,
  0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61,
  0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,
  0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9,
  0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,
  0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011,
  0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,
  0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039,
  0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,
  0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81,
  0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,
  0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49,
  0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,
  0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1,
  0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,
  0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE,
  0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,
  0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16,
  0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,
  0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE,
  0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,
  0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066,
  0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,
  0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E,
  0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,
  0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6,
  0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,
  0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E,
  0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,
  0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686,
  0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,
  0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637,
  0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,
  0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F,
  0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,
  0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47,
  0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,
  0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF,
  0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,
  0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7,
  0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,
  0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F,
  0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,
  0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7,
  0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,
  0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F,
  0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,
  0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640,
  0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,
  0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8,
  0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,
  0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30,
  0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,
  0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088,
  0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,
  0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0,
  0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,
  0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18,
  0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,
  0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0,
  0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,
  0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668,
  0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpeg_Psi::File_Mpeg_Psi()
:File__Analyze()
{
    //In
    From_TS=true; //Default is from TS

    //Out
    WantItAgain=false;

    //Temp
    Stream_Current=0x0000;
    transport_stream_id=0xFFFF; //Impossible
    PCR_PID=0x1FFF; //Default value
    CRC_32=0;
    program_number=0x0000; //Reserved value
}

//---------------------------------------------------------------------------
File_Mpeg_Psi::~File_Mpeg_Psi()
{
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Header_Parse()
{
    //From Program stream
    if (!From_TS)
    {
        //Filling
        table_id=0xFF; //Make it invalid
        Header_Fill_Code((int64u)-1, "program_stream_map"); //(int64u)-1 for precising "out of scope"
        Header_Fill_Size(Element_Size-4);
        return;
    }

    //Payload
    Get_B1 (pointer_field,                                      "pointer_field");
    if (pointer_field)
        Skip_XX(pointer_field,                                  "payload");

    //Parsing
    int16u section_length;
    bool section_syntax_indicator;
    Get_B1 (table_id,                                           "table_id");
    BS_Begin();
    Get_SB (    section_syntax_indicator,                       "section_syntax_indicator");
    Skip_SB(                                                    "private_indicator");
    Skip_S1( 2,                                                 "reserved");
    Get_S2 (12, section_length,                                 "section_length");
    BS_End();

    //Size
    if ((size_t)(pointer_field+section_length)<Element_Offset+4) //We must have 4 more byte for CRC
    {
        Element_WaitForMoreData();
        Finished(); //Error, we exit
        return;
    }
    if (Element_Size<Element_Offset+section_length)
    {
        Element_WaitForMoreData();
        return;
    }
    //Element[Element_Level-1].IsComplete=true;

    //CRC32
    if (section_syntax_indicator)
    {
        int32u CRC_32=0xffffffff;
        const int8u* CRC_32_Buffer=Buffer+Buffer_Offset+(size_t)Element_Offset-3; //table_id position

        while(CRC_32_Buffer<Buffer+Buffer_Offset+(size_t)Element_Offset+section_length) //from table_id to the end, CRC_32 included
        {
            CRC_32=(CRC_32<<8) ^ Psi_CRC_32_Table[(CRC_32>>24)^(*CRC_32_Buffer)];
            CRC_32_Buffer++;
        }
        CRC_32=0;
    }

    //Filling
    Header_Fill_Code(table_id, Ztring().From_Number(table_id, 16));
    Header_Fill_Size(Element_Offset+section_length-4);
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Data_Parse()
{
    //Check if OK
    if(CRC_32!=0)
    {
        Skip_XX(Element_Size,                                   "Data (CRC failed)");
        Finished();
        return;
    }

    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case 0x##_NAME : Table_##_NAME(); break;

    Element_Name(Mpeg_Psi_Element_Name());

    switch (table_id)
    {
        ELEMENT_CASE(00, "program_association_section");
        ELEMENT_CASE(01, "conditional_access_section");
        ELEMENT_CASE(02, "TS_program_map_section");
        ELEMENT_CASE(03, "TS_description_section");
        ELEMENT_CASE(04, "ISO_IEC_14496_scene_description_section");
        ELEMENT_CASE(05, "ISO_IEC_14496_object_descriptor_section");
        ELEMENT_CASE(06, "Metadata?");
        ELEMENT_CASE(38, "ISO/IEC 13818-6 reserved");
        ELEMENT_CASE(39, "DSM-CC addressable section");
        ELEMENT_CASE(3A, "DSM-CC, MPE");
        ELEMENT_CASE(3B, "DSM-CC, U-N messages, except DDM");
        ELEMENT_CASE(3C, "DSM-CC, DDM");
        ELEMENT_CASE(3D, "DSM-CC, stream descriptors");
        ELEMENT_CASE(3E, "DSM-CC, private data, IP-Datagram");
        ELEMENT_CASE(3F, "DSM-CC addressable section");
        ELEMENT_CASE(40, "DVB - network_information_section - actual_network");
        ELEMENT_CASE(41, "DVB - network_information_section - other_network");
        ELEMENT_CASE(42, "DVB - service_description_section - actual_transport_stream");
        ELEMENT_CASE(46, "DVB - service_description_section - other_transport_stream");
        ELEMENT_CASE(4A, "DVB - bouquet_association_section");
        ELEMENT_CASE(4E, "DVB - event_information_section - actual_transport_stream, present/following");
        ELEMENT_CASE(4F, "DVB - event_information_section - other_transport_stream, present/following");
        case 0x50 :
        case 0x51 :
        case 0x52 :
        case 0x53 :
        case 0x54 :
        case 0x55 :
        case 0x56 :
        case 0x57 :
        case 0x58 :
        case 0x59 :
        case 0x5A :
        case 0x5B :
        case 0x5C :
        case 0x5E :
        ELEMENT_CASE(5F, "DVB - event_information_section - actual_transport_stream, schedule");
        case 0x60 :
        case 0x61 :
        case 0x62 :
        case 0x63 :
        case 0x64 :
        case 0x65 :
        case 0x66 :
        case 0x67 :
        case 0x68 :
        case 0x69 :
        case 0x6A :
        case 0x6B :
        case 0x6C :
        case 0x6D :
        case 0x6E :
        ELEMENT_CASE(6F, "DVB - event_information_section - other_transport_stream, schedule");
        ELEMENT_CASE(70, "DVB - time_date_section");
        ELEMENT_CASE(71, "DVB - running_status_section");
        ELEMENT_CASE(72, "DVB - stuffing_section");
        ELEMENT_CASE(73, "DVB - time_offset_section");
        ELEMENT_CASE(74, "DVB - application information section");
        ELEMENT_CASE(75, "DVB - container section");
        ELEMENT_CASE(76, "DVB - related content section");
        ELEMENT_CASE(77, "DVB - content identifier section");
        ELEMENT_CASE(78, "DVB - MPE-FEC section");
        ELEMENT_CASE(79, "DVB - resolution notification section");
        ELEMENT_CASE(7E, "DVB - discontinuity_information_section");
        ELEMENT_CASE(7F, "DVB - selection_information_section");
        ELEMENT_CASE(C0, "ATSC - Program Information Message");
        ELEMENT_CASE(C1, "ATSC - Program Name Message");
        ELEMENT_CASE(C2, "ATSC/SCTE - Network Information Message");
        ELEMENT_CASE(C3, "ATSC/SCTE - Network Text Table (NTT)");
        ELEMENT_CASE(C4, "ATSC/SCTE - Short Form Virtual Channel Table (S-VCT)");
        ELEMENT_CASE(C5, "ATSC/SCTE - System Time Table (STT)");
        ELEMENT_CASE(C6, "ATSC/SCTE - Subtitle Message");
        ELEMENT_CASE(C7, "ATSC - Master Guide Table (MGT)");
        ELEMENT_CASE(C8, "ATSC - Terrestrial Virtual Channel Table (TVCT)");
        ELEMENT_CASE(C9, "ATSC - Cable Virtual Channel Table (CVCT) / Long-form Virtual Channel Table (L-VCT)");
        ELEMENT_CASE(CA, "ATSC - Rating Region Table (RRT)");
        ELEMENT_CASE(CB, "ATSC - Event Information Table (RRT)");
        ELEMENT_CASE(CC, "ATSC - Extended Text Table (ETT)");
        ELEMENT_CASE(CD, "ATSC - System Time Table (STT)");
        ELEMENT_CASE(CE, "ATSC - Data Event Table (DET)");
        ELEMENT_CASE(CF, "ATSC - Data Service Table (DST)");
        ELEMENT_CASE(D0, "ATSC - Program Identifier Table (PIT)");
        ELEMENT_CASE(D1, "ATSC - Network Resource Table (NRT)");
        ELEMENT_CASE(D2, "ATSC - Long-term Service Table (L-TST)");
        ELEMENT_CASE(D3, "ATSC - Directed Channel Change Table (DCCT)");
        ELEMENT_CASE(D4, "ATSC - DCC Selection Code Table (DCCSCT)");
        ELEMENT_CASE(D5, "ATSC - Selection Information Table (SIT)");
        ELEMENT_CASE(D6, "ATSC - Aggregate Event Information Table (AEIT)");
        ELEMENT_CASE(D7, "ATSC - Aggregate Extended Text Table (AETT)");
        ELEMENT_CASE(D8, "ATSC - Cable Emergency Alert");
        ELEMENT_CASE(D9, "ATSC - Aggregate Data Event Table");
        ELEMENT_CASE(DA, "ATSC - Satellite VCT");
        default :
            if (table_id>=0x06
             && table_id<=0x37) {Element_Name("ITU-T Rec. H.222.0 | ISO/IEC 13818-1 reserved"); Skip_XX(Element_Size, "Unknown"); break;}
            if (table_id>=0x40
             && table_id<=0x7F) {Element_Name("DVB - reserved"); Skip_XX(Element_Size, "Unknown"); break;}
            if (table_id>=0x80
             && table_id<=0x8F) {Element_Name("CA message, EMM, ECM"); Skip_XX(Element_Size, "Unknown"); break;}
            if (table_id>=0xC0
             && table_id<=0xDF) {Element_Name("ATSC/SCTE - reserved");Skip_XX(Element_Size, "Unknown");  break;}
            if (table_id<=0xFE) {Element_Name("User Private"); Skip_XX(Element_Size, "Unknown"); break;}
            if (Element_Code==(int64u)-1) {program_stream_map(); break;} //Specific to MPEG-PS
                                {Element_Name("forbidden"); Skip_XX(Element_Size, "Unknown"); break;}
    }

    Element_Size+=4;
    Skip_B4(                                                    "CRC32");

    switch(table_id)
    {
        case 0x40 :
        case 0x41 :
        case 0x42 :
        case 0x46 :
        case 0x4E :
        case 0x4F :
                    WantItAgain=true;
                    break;
        default   : ;
    }

    Finished();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_reserved()
{

}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_iso13818_6()
{
    Element_Info("Defined in ISO/IEC 13818-6");
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_user_private()
{
    Element_Info("user_private");
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_forbidden()
{
    Element_Info("forbidden");
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::program_stream_map()
{
    Element_Name("program_stream_map");

    //Parsing
    int16u program_stream_info_length, elementary_stream_map_length;
    bool single_extension_stream_flag;
    BS_Begin();
    Skip_SB(                                                    "current_next_indicator");
    Get_SB (single_extension_stream_flag,                       "single_extension_stream_flag");
    Skip_SB(                                                    "reserved");
    Skip_S1( 5,                                                 "program_stream_map_version");
    Skip_S1( 7,                                                 "reserved");
    Mark_1 ();
    BS_End();
    Get_B2 (program_stream_info_length,                         "program_stream_info_length");

    //Descriptors
    if (Element_Offset+program_stream_info_length>Element_Size)
    {
        Trusted_IsNot("Integrity error");
        return;
    }
    Descriptors_Size=program_stream_info_length;
    Descriptors();

    Get_B2 (elementary_stream_map_length,                       "elementary_stream_map_length");
    if ((int64u)(4+program_stream_info_length+2+elementary_stream_map_length)>Element_Size)
    {
        Trusted_IsNot("Integrity error");
        return;
    }
    while (Element_Offset<(int32u)(4+program_stream_info_length+2+elementary_stream_map_length))
    {
        Element_Begin();
        int16u ES_info_length;
        int8u  stream_type, elementary_stream_id;
        Get_B1 (stream_type,                                    "stream_type"); Param_Info(Mpeg_Psi_stream_type(stream_type, 0x00000000));
        Get_B1 (elementary_stream_id,                           "elementary_stream_id");
        Get_B2 (ES_info_length,                                 "ES_info_length");
        Element_Name(Ztring::ToZtring(elementary_stream_id, 16));
        if (elementary_stream_id==0xFD && !single_extension_stream_flag)
        {
            Skip_S1(8,                                          "pseudo_descriptor_tag");
            Skip_S1(8,                                          "pseudo_descriptor_length");
            Mark_1();
            Skip_S1(7,                                          "elementary_stream_id_extension");
            if (ES_info_length>=3)
                ES_info_length-=3;
        }
        //Descriptors
        Descriptors_Size=ES_info_length;
        Descriptors();

        Element_End(4+ES_info_length);

        //Filling
        Streams[elementary_stream_id].stream_type=stream_type;

        if (Element_Offset==Element_Size)
            break; //there is a problem
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_00()
{
    //Parsing
    int8u version_number;
    Get_B2(transport_stream_id,                                 "transport_stream_id");
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Get_S1 ( 5, version_number,                                 "version_number");
    Skip_SB(                                                    "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");

    //For each program_number
    int16u program_number, xxx_PID;
    while (Element_Offset<Element_Size)
    {
        Element_Begin(4);
        Get_B2 (    program_number,                             "program_number");
        BS_Begin();
        Skip_S1( 3,                                             "reserved");
        if (program_number==0)
        {
            Get_S2 ( 13, xxx_PID,                               "network_PID"); Element_Info(Ztring::ToZtring_From_CC2(xxx_PID));
            Streams[xxx_PID].Kind=network_information_table;
        }
        else
        {
            Get_S2 ( 13, xxx_PID,                               "program_map_PID"); Element_Info(Ztring::ToZtring_From_CC2(xxx_PID));
            Streams[xxx_PID].Kind=program_map_table;
        }
        BS_End();

        //Filling
        Programs[program_number].pid=xxx_PID;
        Element_End(Ztring::ToZtring_From_CC2(program_number));
    }
    BS_End();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_01()
{
    //Parsing
    BS_Begin();
    Skip_S3(18,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_SB(                                                    "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");

    //Descriptors
    if (Element_Offset<Element_Size)
    {
        Descriptors_Size=(int16u)(Element_Size-Element_Offset);
        Descriptors();
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_02()
{
    //Parsing
    Get_B2 (    program_number,                                 "program_number");
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_SB(                                                    "current_next_indicator");
    Skip_S1( 8,                                                 "section_number");
    Skip_S1( 8,                                                 "last_section_number");
    Skip_S1( 3,                                                 "reserved");
    Get_S2 (13, PCR_PID,                                        "PCR_PID");
    Skip_S1( 4,                                                 "reserved");
    Get_S2 (12, Descriptors_Size,                               "program_info_length");
    BS_End();

    //Descriptors
    if (Descriptors_Size>0)
        Descriptors();

    //Parsing
    while (Element_Offset<Element_Size)
    {
        Element_Begin();
        int8u stream_type;
        BS_Begin();
        Get_S1 ( 8, stream_type,                                "stream_type"); Element_Info(Mpeg_Psi_stream_type(stream_type, Programs[program_number].format_identifier)); Param_Info(Mpeg_Psi_stream_type(stream_type, Streams[Stream_Current].format_identifier));
        Skip_S1( 3,                                             "reserved");
        Get_S2 (13, Stream_Current,                             "elementary_PID");
        Skip_S1( 4,                                             "reserved");
        Get_S2 (12, Descriptors_Size,                           "ES_info_length");
        BS_End();

        //Filling
        Streams[Stream_Current].stream_type=stream_type;
        Streams[Stream_Current].program_number=program_number;
        Streams[Stream_Current].format_identifier=Programs[program_number].format_identifier;

        //Descriptors
        if (Descriptors_Size>0)
            Descriptors();

        Element_End(Ztring::ToZtring_From_CC2(Stream_Current), 5+Descriptors_Size);

        //Merging ES if present
        std::map<int16u, File_Mpeg_Descriptors::es_element>::iterator ES_Element=ES_Elements.find(Streams[Stream_Current].ES_ID);
        if (ES_Element!=ES_Elements.end())
        {
            //Transfering the Parser
            Streams[Stream_Current].ES_Parser=ES_Element->second.Parser; ES_Element->second.Parser=NULL;
        }
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_03()
{
    //Parsing
    BS_Begin();
    Skip_S3(18,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_SB(                                                    "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");

    //Descriptors
    if (Element_Offset<Element_Size)
    {
        Descriptors_Size=(int16u)(Element_Size-Element_Offset);
        Descriptors();
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_42()
{
    //Parsing
    Skip_B2(                                                    "transport_stream_id");
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_S1( 1,                                                 "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");
    Skip_B2(                                                    "original_network_id");
    Skip_B1(                                                    "reserved_future_use");
    while (Element_Offset<Element_Size)
    {
        Element_Begin();
        int64u Test;
        Peek_B5(Test);
        if (Test!=0xFFFFFFFFFFULL)
        {
            Get_B2 (    program_number,                             "service_id");
            BS_Begin();
            Skip_S1( 6,                                             "reserved_future_use");
            Skip_SB(                                                "EIT_schedule_flag");
            Skip_SB(                                                "EIT_present_following_flag");
            Skip_S1( 3,                                             "running_status");
            Skip_SB(                                                "free_CA_mode");
            Get_S2 (12, Descriptors_Size,                           "ES_info_length");
            BS_End();

            //Descriptors
            if (Descriptors_Size>0)
                Descriptors();

            Element_End(Ztring::ToZtring_From_CC2(program_number), 5+Descriptors_Size);
        }
        else
        {
            Skip_XX(Element_Size-Element_Offset,                    "Junk");
            Element_End("Junk");
        }
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_46()
{
    Table_42();

    //Clear info, this is another stream
    Streams.clear();
    Programs.clear();
    ES_Elements.clear();
    Infos.clear();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_4E()
{
    //Parsing
    Skip_B2(                                                    "service_id");
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_SB(                                                    "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");
    Skip_B2(                                                    "transport_stream_id");
    Skip_B2(                                                    "original_network_id");
    Skip_B1(                                                    "segment_last_section_number");
    Skip_B1(                                                    "last_table_id");
    while (Element_Offset<Element_Size)
    {
        Element_Begin();
        int32u time, duration;
        int16u event_id, date;
        Get_B2 (event_id,                                       "event_id");
        Get_B2 (date,                                           "start_time (date)"); Param_Info(Date_MJD(date));
        Get_B3 (time,                                           "start_time (time)"); Param_Info(Time_BCD(time));
        Get_B3 (duration,                                       "duration"); Param_Info(Time_BCD(duration));
        BS_Begin();
        Skip_S1( 3,                                             "running_status");
        Skip_SB(                                                "free_CA_mode");
        Get_S2 (12, Descriptors_Size,                           "descriptors_loop_length");
        BS_End();

        //Descriptors
        if (Descriptors_Size>0)
            Descriptors();

        Element_End(Ztring::ToZtring_From_CC2(event_id), 11+Descriptors_Size);
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_4F()
{
    Table_4E();

    //Clear info, this is another stream
    Streams.clear();
    Programs.clear();
    ES_Elements.clear();
    Infos.clear();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_5F()
{
    Table_4E();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_6F()
{
    Table_4E();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_7F()
{
    //Parsing
    Skip_B2(                                                    "DVB_reserved_future_use");
    BS_Begin();
    Skip_S1( 2,                                                 "ISO_reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_S1( 1,                                                 "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");
    BS_Begin();
    Skip_S1( 4,                                                 "DVB_reserved_for_future_use");
    Get_S2 (12, Descriptors_Size,                               "transmission_info_loop_length");
    BS_End();

    //Descriptors
    if (Descriptors_Size>0)
        Descriptors();

    while (Element_Offset<Element_Size)
    {
        Element_Begin();
        int16u service_id;
        Get_B2 (    service_id,                                 "service_id");
        BS_Begin();
        Skip_SB(                                                "DVB_reserved_future_use");
        Skip_S1( 3,                                             "running_status");
        Get_S2 (12, Descriptors_Size,                           "service_loop_length");
        BS_End();

        //Descriptors
        if (Descriptors_Size>0)
            Descriptors();

        Element_End(Ztring::ToZtring_From_CC2(service_id), 5+Descriptors_Size);
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_C7()
{
    //Parsing
    int16u tables_defined;
    Skip_B2(                                                    "transport_stream_id");
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_S1( 1,                                                 "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");
    Skip_B1(                                                    "protocol_version");
    Get_B2 (    tables_defined,                                 "tables_defined");
    for (int16u Pos=0; Pos<tables_defined; Pos++)
    {
        int16u table_type, table_type_PID;
        Element_Begin();
        Get_B2 (    table_type,                                 "table_type"); Param_Info(Mpeg_Psi_ATSC_table_type(table_type));
        BS_Begin();
        Skip_S1( 3,                                             "reserved");
        Get_S2 (13, table_type_PID,                             "table_type_PID");
        Skip_S1( 3,                                             "reserved");
        Skip_S1( 5,                                             "table_type_version_number");
        BS_End();
        Skip_B4(                                                "number_bytes");
        BS_Begin();
        Skip_S1( 4,                                             "reserved");
        Get_S2 (12, Descriptors_Size,                           "table_type_descriptors_length");
        BS_End();

        //Descriptors
        if (Descriptors_Size>0)
            Descriptors();

        Element_Info(Mpeg_Psi_ATSC_table_type(table_type));
        Element_End(Ztring::ToZtring_From_CC2(table_type_PID), 11+Descriptors_Size);

        //Filling
        Streams[table_type_PID].Kind=atsc_psip;
    }
    BS_Begin();
    Skip_S1( 4,                                                 "reserved");
    Get_S2 (12, Descriptors_Size,                               "descriptors_length");
    BS_End();

    //Descriptors
    if (Descriptors_Size>0)
        Descriptors();

    //CC_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_C8()
{
    //Parsing
    Ztring short_name;
    int8u num_channels_in_section;
    BS_Begin();
    Skip_B2(                                                    "transport_stream_id");
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_S1( 1,                                                 "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");
    Skip_B1(                                                    "protocol_version");
    Get_B1 (    num_channels_in_section,                        "num_channels_in_section");
    BS_End();
    for (int8u Pos=0; Pos<num_channels_in_section; Pos++)
    {
        int16u major_channel_number, minor_channel_number;
        int8u service_type;
        Element_Begin();
        Get_UTF16B(14, short_name,                              "short_name");
        BS_Begin();
        Skip_S1( 4,                                             "reserved");
        Get_S2 (10, major_channel_number,                       "major_channel_number");
        Get_S2 (10, minor_channel_number,                       "minor_channel_number");
        BS_End();
        Skip_B1(                                                "modulation_mode");
        Skip_B4(                                                "carrier_frequency");
        Skip_B2(                                                "channel_TSID");
        Get_B2 (    program_number,                             "program_number");
        BS_Begin();
        Skip_S1( 2,                                             "ETM_location");
        Skip_SB(                                                "access_controlled");
        Skip_SB(                                                "hidden");
        Skip_S1( 2,                                             "reserved");
        Skip_SB(                                                "hide_guide");
        Skip_S1( 3,                                             "reserved");
        Get_S1 ( 6, service_type,                               "service_type");
        BS_End();
        Skip_B2(                                                "source_id");
        BS_Begin();
        Skip_S1( 6,                                             "reserved");
        Get_S2 (10, Descriptors_Size,                           "descriptors_length");
        BS_End();

        //Descriptors
        if (Descriptors_Size>0)
            Descriptors();

        Element_End(Ztring::ToZtring_From_CC2(program_number), 18+Descriptors_Size);

        //Filling
        Ztring Channel=Ztring::ToZtring(major_channel_number);
        if (minor_channel_number)
            Channel+=_T("-")+Ztring::ToZtring(minor_channel_number);
        if (program_number>0 && program_number<2000)
        {
            Programs[program_number].Infos["ServiceName"]=short_name;
            Programs[program_number].Infos["ServiceChannel"]=Channel;
            Programs[program_number].Infos["ServiceType"]=Mpeg_Psi_atsc_service_type(service_type);
        }
        else if (program_number==0xFFFF)
        {
            Streams[0x0000].Infos["ServiceName"]=short_name;
            Streams[0x0000].Infos["ServiceChannel"]=Channel;
            Streams[0x0000].Infos["ServiceType"]=Mpeg_Psi_atsc_service_type(service_type);
        }
    }
    //CC_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_CB()
{
    //Parsing
    int8u num_events_in_section;
    Skip_B2(                                                    "source_id");
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_S1( 1,                                                 "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");
    Skip_B1(                                                    "protocol_version");
    Get_B1 (    num_events_in_section,                          "num_events_in_section");
    BS_End();
    for (int8u Pos=0; Pos<num_events_in_section; Pos++)
    {
        Ztring title;
        int32u length_in_seconds;
        int16u event_id;
        Element_Begin();
        BS_Begin();
        Skip_S1( 2,                                             "reserved");
        Get_S2 (14, event_id,                                   "event_id");
        BS_End();
        Skip_B4(                                                "start_time");
        BS_Begin();
        Skip_S1( 2,                                             "reserved");
        Skip_S1( 2,                                             "ETM_location");
        Get_S3 (20, length_in_seconds,                          "length_in_seconds");
        BS_End();
        Skip_B1 (                                               "title_length"); //We don't use it for verification
        ATSC_multiple_string_structure(title);
        BS_Begin();
        Skip_S1( 6,                                             "reserved");
        Get_S2 (10, Descriptors_Size,                           "descriptors_length");
        BS_End();

        //Descriptors
        if (Descriptors_Size>0)
            Descriptors();
        Streams[Stream_Current].Infos.clear(); //Info for future programs is not supported

        Element_End(Ztring::ToZtring_From_CC2(event_id));
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_CC()
{
    //Parsing
    Ztring extended_text_message;
    Skip_B2(                                                    "ETT_table_id_extension");
    BS_Begin();
    Skip_S1( 2,                                                 "reserved");
    Skip_S1( 5,                                                 "version_number");
    Skip_S1( 1,                                                 "current_next_indicator");
    BS_End();
    Skip_B1(                                                    "section_number");
    Skip_B1(                                                    "last_section_number");
    Skip_B1(                                                    "protocol_version");
    Element_Begin("ETM_id", 2);
        Skip_B2(                                                "source_id");
        BS_Begin();
        Skip_S1(14,                                             "event_id");
        Skip_S1( 2,                                             "lsb");
        BS_End();
    Element_End();
    ATSC_multiple_string_structure(extended_text_message);
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Descriptors()
{
    if (Descriptors_Size==0)
        return;

    Element_Begin("Descriptors", Descriptors_Size);

    //Parsing
    File_Mpeg_Descriptors Descriptors;
    Buffer_Offset+=(size_t)Element_Offset; //Positionning
    Open_Buffer_Init(&Descriptors, File_Size, File_Offset+Buffer_Offset);
    Descriptors.format_identifier=Streams[Stream_Current].format_identifier==0x00000000?Programs[program_number].format_identifier:Streams[Stream_Current].format_identifier; //format_identifier of the stream if exist, else general format_identifier
    Descriptors.StreamKind=Stream_Current?Stream_Max:Stream_General; //Saying if it is General or not
    Descriptors.table_id=table_id;
    Open_Buffer_Continue(&Descriptors, Buffer+Buffer_Offset, Descriptors_Size);
    Buffer_Offset-=(size_t)Element_Offset; //Positionning
    Element_Offset+=Descriptors_Size;
    Element_End();

    //Filling
    //About a PID
    if (Stream_Current) //Only if an ID is registered
    {
        Streams[Stream_Current].Infos=Descriptors.Infos;
        Streams[Stream_Current].format_identifier=Descriptors.registration_format_identifier; //General
        if (Streams[Stream_Current].descriptor_tag==0x00)
            Streams[Stream_Current].descriptor_tag=Descriptors.descriptor_tag;
        if (Streams[Stream_Current].CA_PID==0x0000)
            Streams[Stream_Current].CA_PID=Descriptors.CA_PID;
        if (Streams[Stream_Current].ES_ID==0x0000)
            Streams[Stream_Current].ES_ID=Descriptors.ES_ID;
    }
    else if (program_number) //This is about a program, not a stream
    {
        Programs[program_number].format_identifier=Descriptors.registration_format_identifier;
        for (std::map<std::string, ZenLib::Ztring>::iterator Info=Descriptors.Infos.begin(); Info!=Descriptors.Infos.end(); Info++)
            Programs[program_number].Infos[Info->first]=Info->second;
    }
    else
        for (std::map<std::string, ZenLib::Ztring>::iterator Info=Descriptors.Infos.begin(); Info!=Descriptors.Infos.end(); Info++)
            Infos[Info->first]=Info->second;

    //About program
    for (std::map<std::string, ZenLib::Ztring>::iterator Info=Descriptors.Program.Infos.begin(); Info!=Descriptors.Program.Infos.end(); Info++)
        Programs[program_number].Infos[Info->first]=Info->second;

    //About es_element
    for (std::map<int16u, File_Mpeg_Descriptors::es_element>::iterator ES_Element=Descriptors.ES_Elements.begin(); ES_Element!=Descriptors.ES_Elements.end(); ES_Element++)
    {
        //Transfering the Parser
        ES_Elements[ES_Element->first].Parser=ES_Element->second.Parser; ES_Element->second.Parser=NULL;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::ATSC_multiple_string_structure(Ztring &Value)
{
    //Parsing
    Ztring string;
    int8u number_strings, number_segments;
    Get_B1(number_strings,                                      "number_strings");
    for (int8u Pos=0; Pos<number_strings; Pos++)
    {
        Element_Begin("String");
        Skip_Local(3,                                           "ISO_639_language_code");
        Get_B1(number_segments,                                 "number_segments");
        for (int8u Pos=0; Pos<number_segments; Pos++)
        {
            Element_Begin("Segment");
            Ztring String;
            int8u compression_type, mode, number_bytes;
            Get_B1 (compression_type,                           "compression_type");
            Get_B1 (mode,                                       "mode");
            Get_B1 (number_bytes,                               "number_bytes");
            switch (compression_type)
            {
                case 0x00 :
                            switch (mode)
                            {
                                case 0x00 : Get_Local(number_bytes, string, "string"); break;
                                default   : Skip_XX(number_bytes, "Unknown");
                            }
                            break;
                default   : Skip_XX(number_bytes,               "Unknown");
            }
            Element_End(3+number_bytes);
        }
        Element_End("String");
    }

    //Filling
    Value=string;
}

//---------------------------------------------------------------------------
//Modified Julian Date
Ztring File_Mpeg_Psi::Date_MJD(int16u Date_)
{
    //Calculating
    float64 Date=Date_;
    int Y2=(int)((Date-15078.2)/365.25);
    int M2=(int)(((Date-14956.1) - ((int)(Y2*365.25))) /30.6001);
    int D =(int)(Date-14956 - ((int)(Y2*365.25)) - ((int)(M2*30.6001)));
    int K=0;
    if (M2==14 || M2==15)
        K=1;
    int Y =Y2+K;
    int M =M2-1-K*12;

    //Formating
    return Ztring::ToZtring(1900+Y)+_T("-")
         + Ztring::ToZtring(     M)+_T("-")
         + Ztring::ToZtring(     D);
}

//---------------------------------------------------------------------------
//Form: HHMMSS, BCD
Ztring File_Mpeg_Psi::Time_BCD(int32u Time)
{
    return Ztring::ToZtring((Time>>16)&0xFF, 16)+_T(":") //BCD
         + Ztring::ToZtring((Time>> 8)&0xFF, 16)+_T(":") //BCD
         + Ztring::ToZtring((Time    )&0xFF, 16);        //BCD
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGTS_YES
