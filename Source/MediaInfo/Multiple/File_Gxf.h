// File_Gxf - Info for GXF (SMPTE 360M) files
// Copyright (C) 2010-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Information about GXF files
// SMPTE 360M - General Exchange Format
// SMPTE RDD 14-2007 - General Exchange Format-2
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_GxfH
#define MediaInfo_File_GxfH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Gxf
//***************************************************************************

class File_Gxf : public File__Analyze
{
public :
    //Constructor/Destructor
    File_Gxf();
    ~File_Gxf();

private :
    //Streams management
    void Streams_Finish();

    //Buffer - Synchro
    bool Synchronize();
    bool Synched_Test();

    //Buffer - Per element
    void Header_Parse();
    void Data_Parse();

    //Packets
    void map();
    void media();
    void end_of_stream();
    void field_locator_table();
    void UMF_file();

    //Temp - Global
    int32u Material_Fields_First;
    int32u Material_Fields_Last;
    int32u Material_File_Size;
    int32u Material_Fields_FieldsPerFrame;
    int8u  Parsers_Count;
    bool   Material_Fields_First_IsValid;
    bool   Material_Fields_Last_IsValid;
    bool   Material_File_Size_IsValid;

    //Temp - Stream
    struct stream
    {
        File__Analyze* Parser;
        stream_t StreamKind;
        size_t StreamPos;
        int32u MediaType;
        int32u TimeStamp_Start;
        int32u TimeStamp_End;
        bool   Searching_Payload;
        bool   Searching_TimeStamp_Start;
        bool   Searching_TimeStamp_End;
        Ztring MediaFileName;

        stream()
        {
            Parser=NULL;
            StreamKind=Stream_Max;
            StreamPos=(size_t)-1;
            MediaType=0;
            Searching_Payload=false;
            Searching_TimeStamp_Start=false;
            Searching_TimeStamp_End=false;
        }
        ~stream()
        {
            delete Parser; //Parser=NULL
        }
    };
    std::vector<stream> Streams;
    File__Analyze*      UMF_File;
    int64u              SizeToAnalyze; //Total size of a chunk to analyse, it may be changed by the parser

    //File__Analyze helpers
    void Streams_Finish_PerStream(size_t StreamID, stream &Temp);
    void Detect_EOF();
};

} //NameSpace

#endif
