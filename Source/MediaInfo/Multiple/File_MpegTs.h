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
//
// Information about MPEG Transport Stream files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_MpegTsH
#define MediaInfo_MpegTsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Duplicate.h"
#include "MediaInfo/Multiple/File_Mpeg_Psi.h"
#include <map>
#include <vector>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_MpegTs
//***************************************************************************

class File_MpegTs : public File__Duplicate
{
public :
    File_MpegTs();

protected :
    //Formats
    bool FileHeader_Begin ();
    virtual void Read_Buffer_Finalize (); //virtual for BDAV

    //Information
    void Option_Manage ();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    void Header_Parse_AdaptationField();
    void Data_Parse();

    //Data
    struct stream
    {
        File__Analyze*                              Parser;
        File__Analyze*                              ES_Parser;
        std::map<std::string, ZenLib::Ztring>       Infos;
        int8u                                       stream_type;
        int8u                                       descriptor_tag;
        int32u                                      format_identifier;
        File_Mpeg_Psi::ts_kind                      TS_Kind;
        std::vector<int16u>                         program_numbers;
        int64u                                      TimeStamp_Start;
        int64u                                      TimeStamp_End;
        bool                                        StreamIsRegistred;
        bool                                        Scrambled;
        bool                                        Searching;
        bool                                        Searching_Payload_Start;
        bool                                        Searching_Payload_Continue;
        bool                                        Searching_TimeStamp_Start;
        bool                                        Searching_TimeStamp_End;
        bool                                        ShouldDuplicate;

        stream()
        {
            Parser=NULL;
            ES_Parser=NULL;
            stream_type=0x00;
            descriptor_tag=0x00;
            TS_Kind=File_Mpeg_Psi::unknown;
            TimeStamp_Start=(int64u)-1;
            TimeStamp_End=(int64u)-1;
            StreamIsRegistred=false;
            Scrambled=false;
            Searching=false;
            Searching_Payload_Start=false;
            Searching_Payload_Continue=false;
            Searching_TimeStamp_Start=false;
            Searching_TimeStamp_End=false;
            ShouldDuplicate=false;
        }

        ~stream()
        {
            delete Parser; //Parser=NULL;
            delete ES_Parser; //ES_Parser=NULL;
        }

        void Searching_Payload_Start_Set(bool ToSet)
        {
            Searching_Payload_Start=ToSet;
            Searching_Test();
        }
        void Searching_Payload_Continue_Set(bool ToSet)
        {
            Searching_Payload_Continue=ToSet;
            Searching_Test();
        }
        void Searching_TimeStamp_Start_Set(bool ToSet)
        {
            Searching_TimeStamp_Start=ToSet;
            Searching_Test();
        }
        void Searching_TimeStamp_End_Set(bool ToSet)
        {
            Searching_TimeStamp_End=ToSet;
            Searching_Test();
        }
        void Searching_Test()
        {
            if (Searching_Payload_Start
             || Searching_Payload_Continue
             || Searching_TimeStamp_Start
             || Searching_TimeStamp_End)
                Searching=true;
             else
                Searching=false;
        }
    };
    std::vector<stream>         Streams;
    int32u                      format_identifier;
    int16u                      pid;
    bool                        payload_unit_start_indicator;

    struct program
    {
        int16u                                      pid;
        std::map<std::string, ZenLib::Ztring>       Infos;
        ZtringList                                  List_ID;
        ZtringList                                  List_StreamKind;
        ZtringList                                  List_StreamPos;
        ZtringList                                  Language;
        ZtringList                                  Format;
        ZtringList                                  Codec;
        ZtringList                                  Text_ID;

        program()
        {
            pid=0;
        }

        ~program()
        {
        }
    };
    std::map<int16u, program>   Programs;

    struct pid_pmts
    {
        std::vector<int16u>                         List;

        pid_pmts()
        {
        }

        ~pid_pmts()
        {
        }
    };
    std::map<int16u, pid_pmts> PID_PMTs;

    //Elements
    void PSI();
    void PSI_program_association_table();
    void PSI_program_map_table();
    void PSI_network_information_table();
    void PSI_atsc_psip();
    void PSI_dvb_sdt_bat_st();
    void Reserved();
    void PES();
    void Null();

    //Helpers
    bool Synchronize();
    bool Header_Parser_QuickSearch();
    void Detect_EOF();
    bool Detect_NonMPEGTS();

    //Count
    size_t program_Count;
    size_t elementary_PID_Count;

    //Temp
    bool Parsing_End;
    size_t BDAV_Size;
    size_t TS_Size;
    int64u MpegTs_JumpTo_Begin;
    int64u MpegTs_JumpTo_End;

    //Friends
    friend class File_Bdav;
};

} //NameSpace

#endif
