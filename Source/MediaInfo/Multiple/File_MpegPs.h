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
    int32u stream_type_FromTS;      //ID from TS
    int8u  MPEG_Version;            //MPEG_Version from TS

protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_MpegPs();
    ~File_MpegPs();

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

    //Packet
    void MPEG_program_end();    //0xB9
    void pack_start();          //0xBA
    void system_header_start(); //0xBB
    void program_stream_map();  //0xBC
    void private_stream_1();    //0xBD
    void padding_stream();      //0xBE
    void private_stream_2();    //0xBF
    void audio_stream();        //0xC0 --> 0xDF
    void video_stream();        //0xE0 --> 0xEF, 0xFD (if !FromTS)
    void LATM();                //0xFA

    //private_stream_1 specific
    void           private_stream_1_ChooseID();
    void           private_stream_1_ChooseID_VOB();
    File__Analyze* private_stream_1_ChooseParser();
    File__Analyze* private_stream_1_ChooseParser_RLE();
    File__Analyze* private_stream_1_ChooseParser_AC3();
    File__Analyze* private_stream_1_ChooseParser_DTS();
    File__Analyze* private_stream_1_ChooseParser_SDDS();
    File__Analyze* private_stream_1_ChooseParser_PCM();
    File__Analyze* private_stream_1_ChooseParser_NULL();
    ZenLib::Char*  private_stream_1_ChooseExtension();
    void           private_stream_1_Element_Info();
    int8u          private_stream_1_ID;
    size_t         private_stream_1_Offset;
    bool           private_stream_1_IsDvdVideo;

    //Count
    int8u video_stream_Count;
    int8u audio_stream_Count;
    int8u private_stream_1_Count;
    bool  private_stream_2_Count;

    //From packets
    int32u program_mux_rate;

    //PS
    struct ps_stream
    {
        struct Mpeg_TimeStamp
        {
            int64u PTS;
            int64u DTS;
            bool PTS_Is_Valid;
            bool DTS_Is_Valid;

            Mpeg_TimeStamp ()
            {
                PTS=0;
                DTS=0;
                PTS_Is_Valid=false;
                DTS_Is_Valid=false;
            }
        };

        int8u          stream_type;
        File__Analyze* Parser;
        File__Analyze* Parser2; //Sometimes, we need to do parallel tests
        File__Analyze* Parser3; //Sometimes, we need to do parallel tests
        Mpeg_TimeStamp TimeStamp_Start;
        Mpeg_TimeStamp TimeStamp_End;
        bool           StreamIsRegistred;
        bool           Searching_Payload;
        bool           Searching_TimeStamp_Start;
        bool           Searching_TimeStamp_End;

        ps_stream()
        {
            stream_type=0;
            Parser=NULL;
            Parser2=NULL;
            Parser3=NULL;
            StreamIsRegistred=false;
            Searching_Payload=false;
            Searching_TimeStamp_Start=false;
            Searching_TimeStamp_End=false;
        }

        ~ps_stream()
        {
            delete Parser; //Parser=NULL;
            delete Parser2; //Parser2=NULL;
            delete Parser3; //Parser3=NULL;
        }
    };
    std::map<int64u, ps_stream> Stream;
    std::map<int64u, ps_stream> Stream_Private1; //There can have multiple streams in one private stream

    //Temp
    int64u SizeToAnalyze; //Total size of a chunk to analyse, it may be changed by the parser
    bool   video_stream_Unlimited;
    int8u  video_stream_Unlimited_start_code;

    //Helpers
    bool Synchronize();
    bool Header_Parser_QuickSearch();
    bool Detect_NonMPEGPS();

    //File__Analyze helpers
    void Read_Buffer_Finalize_PerStream(std::map<int64u, ps_stream>::iterator &Temp);
};

} //NameSpace

#endif
