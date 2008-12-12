// File_Mpeg - Info for MPEG files
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
// Information about MPEG files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_MpegPsH
#define MediaInfo_MpegPsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Mpeg
//***************************************************************************

class File_MpegPs : public File__Analyze
{
public :
    //In
    bool   FromTS;                  //Indicate if stream comes from TS
    int8u  stream_type_FromTS;      //ID from TS
    int8u  descriptor_tag_FromTS;   //Descriptor from TS
    int32u format_identifier_FromTS;//Registration from TS
    int8u  MPEG_Version;            //MPEG_Version from TS

protected :
    //Format
    bool FileHeader_Begin ();
    void Read_Buffer_Finalize ();

public :
    File_MpegPs();

private :
    //Buffer
    bool Header_Begin();
    void Header_Parse();
    bool Header_Parse_Fill_Size();
    void Header_Parse_PES_packet(int8u start_code);
    void Header_Parse_PES_packet_MPEG1(int8u start_code);
    void Header_Parse_PES_packet_MPEG2(int8u start_code);
    void Data_Parse();
    void Detect_EOF();
    bool BookMark_Needed();

    //Packet
    void MPEG_program_end();    //0xB9
    void pack_start();          //0xBA
    void system_header_start(); //0xBB
    void program_stream_map();  //0xBC
    void private_stream_1();    //0xBD
    void padding_stream();      //0xBE
    void private_stream_2();    //0xBF
    void audio_stream();        //0xC0 --> 0xDF
    void video_stream();        //0xE0 --> 0xEF
    void LATM();                //0xFA
    void extension_stream();    //0xFD

    //private_stream_1 specific
    void           private_stream_1_Choose_DVD_ID();
    File__Analyze* private_stream_1_ChooseParser();
    ZenLib::Char*  private_stream_1_ChooseExtension();
    void           private_stream_1_Element_Info();
    int8u          private_stream_1_ID;
    size_t         private_stream_1_Offset;
    bool           private_stream_1_IsDvdVideo;

    //extension_stream specific
    ZenLib::Char*  extension_stream_ChooseExtension();

    //Count
    int8u video_stream_Count;
    int8u audio_stream_Count;
    int8u private_stream_1_Count;
    bool  private_stream_2_Count;
    int8u extension_stream_Count;

    //From packets
    int32u program_mux_rate;

    //PS
    struct ps_stream
    {
        struct Mpeg_TimeStamp
        {
            struct Mpeg_TimeStamp_TS
            {
                int64u File_Pos;
                int64u TimeStamp;
                int64u Is_Valid;

                Mpeg_TimeStamp_TS()
                {
                    File_Pos=(int64u)-1;
                    TimeStamp=(int64u)-1;
                    Is_Valid=false;
                }
            };

            Mpeg_TimeStamp_TS PTS;
            Mpeg_TimeStamp_TS DTS;
        };

        int8u          stream_type;
        int8u          DVD_Identifier;
        File__Analyze* Parser;
        File__Analyze* Parser2; //Sometimes, we need to do parallel tests
        File__Analyze* Parser3; //Sometimes, we need to do parallel tests
        File__Analyze* Parser4; //Sometimes, we need to do parallel tests
        Mpeg_TimeStamp TimeStamp_Start;
        Mpeg_TimeStamp TimeStamp_End;
        bool           StreamIsRegistred;
        bool           Searching_Payload;
        bool           Searching_TimeStamp_Start;
        bool           Searching_TimeStamp_End;
        size_t         FrameCount_AfterLast_TimeStamp_End;

        ps_stream()
        {
            stream_type=0;
            DVD_Identifier=0;
            Parser=NULL;
            Parser2=NULL;
            Parser3=NULL;
            Parser4=NULL;
            StreamIsRegistred=false;
            Searching_Payload=false;
            Searching_TimeStamp_Start=false;
            Searching_TimeStamp_End=false;
            FrameCount_AfterLast_TimeStamp_End=0;
        }

        ~ps_stream()
        {
            delete Parser; //Parser=NULL;
            delete Parser2; //Parser2=NULL;
            delete Parser3; //Parser3=NULL;
            delete Parser4; //Parser4=NULL;
        }
    };
    std::vector<ps_stream> Streams;
    std::vector<ps_stream> Streams_Private1; //There can have multiple streams in one private stream
    std::vector<ps_stream> Streams_Extension; //There can have multiple streams in one private stream
    int8u start_code;

    //Temp
    int64u SizeToAnalyze; //Total size of a chunk to analyse, it may be changed by the parser
    int8u  stream_id_extension;
    bool   video_stream_Unlimited;
    int8u  video_stream_Unlimited_start_code;
    int64u PTS;
    int64u DTS;
    bool   Parsing_End_ForDTS;

    //Helpers
    bool Synchronize();
    bool Header_Parser_QuickSearch();
    bool Detect_NonMPEGPS();

    //Parsers
    File__Analyze* ChooseParser_Mpegv();
    File__Analyze* ChooseParser_Mpeg4v();
    File__Analyze* ChooseParser_Avc();
    File__Analyze* ChooseParser_VC1();
    File__Analyze* ChooseParser_Dirac();
    File__Analyze* ChooseParser_Mpega();
    File__Analyze* ChooseParser_Adts();
    File__Analyze* ChooseParser_AC3();
    File__Analyze* ChooseParser_DTS();
    File__Analyze* ChooseParser_SDDS();
    File__Analyze* ChooseParser_AAC();
    File__Analyze* ChooseParser_PCM();
    File__Analyze* ChooseParser_AES3();
    File__Analyze* ChooseParser_RLE();
    File__Analyze* ChooseParser_PGS();
    File__Analyze* ChooseParser_NULL();

    //File__Analyze helpers
    void Read_Buffer_Finalize_PerStream(size_t StreamID, ps_stream &Temp);

    //Output buffer
    size_t Output_Buffer_Get (const String &Value);
    size_t Output_Buffer_Get (size_t Pos);
};

} //NameSpace

#endif
