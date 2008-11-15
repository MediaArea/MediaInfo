// File_Ogg - Info for ogg files
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
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_OGG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Ogg.h"
#include "MediaInfo/Multiple/File_Ogg_SubElement.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Ogg::File_Ogg()
:File__Analyze()
{
    //Temp - Global
    StreamsToDo=0;
    Parsing_End=false;
    
    //Temp - Stream
    Chunk_Sizes_Finnished=true;
    packet_type=0;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ogg::Read_Buffer_Finalize()
{
    std::map<int64u, stream>::iterator Stream_Temp=Stream.begin();
    while (Stream_Temp!=Stream.end())
    {
        //Filling
        if (Stream_Temp->second.absolute_granule_position_Resolution)
            Fill(Stream_Temp->second.StreamKind, Stream_Temp->second.StreamPos, "Duration", float64_int64s(((float64)(Stream_Temp->second.absolute_granule_position))*1000/Stream_Temp->second.absolute_granule_position_Resolution));
        Stream_Temp++;
    }

    Fill(Stream_General, 0, General_Format, "OGG");

    //No more need
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Stream.clear();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ogg::Header_Begin()
{
    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //Quick test of synchro
    if (CC4(Buffer+Buffer_Offset)!=Ogg::OggS && !Synchronize())
        return false;

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_Ogg::Header_Parse()
{
    //Parsing
    int64u absolute_granule_position;
    int32u stream_serial_number, page_sequence_no;
    int16u total_page_size;
    int8u  stream_structure_version, flags, page_segments, packet_lacing_value;
    Skip_C4(                                                    "capture_pattern");
    Get_L1 (stream_structure_version,                           "stream_structure_version");
    Get_L1 (flags,                                              "header_type_flag");
        Get_Flags (flags, 0, continued,                         "continued packet");
        Skip_Flags(flags, 1,                                    "first page of logical bitstream (bos)");
        Skip_Flags(flags, 2,                                    "last page of logical bitstream (eos)");
    Get_L8 (absolute_granule_position,                          "absolute granule position");
    Get_L4 (stream_serial_number,                               "stream serial number");
    Get_L4 (page_sequence_no,                                   "page sequence no");
    Skip_L4(                                                    "page checksum");
    Get_L1 (page_segments,                                      "page_segments");
    total_page_size=0;
    Chunk_Sizes.clear();
    Chunk_Sizes.push_back(0);
    for (int8u Pos=0; Pos<page_segments; Pos++)
    {
        Get_L1 (packet_lacing_value,                            "packet lacing value");
        total_page_size+=packet_lacing_value;
        Chunk_Sizes[Chunk_Sizes.size()-1]+=packet_lacing_value;
        if (packet_lacing_value!=0xFF)
        {
            Chunk_Sizes.push_back(0);
            Chunk_Sizes_Finnished=true;
        }
        else
            Chunk_Sizes_Finnished=false;
    }
    if (Chunk_Sizes_Finnished)
        Chunk_Sizes.resize(Chunk_Sizes.size()-1); //Keep out the last value

    //Filling
    Header_Fill_Size(27+page_segments+total_page_size);
    Header_Fill_Code(stream_serial_number, Ztring::ToZtring(stream_serial_number, 16));
    Stream[stream_serial_number].absolute_granule_position=absolute_granule_position;
}

//---------------------------------------------------------------------------
void File_Ogg::Data_Parse()
{
    //If first chunk of a stream
    if (Stream[Element_Code].Parser==NULL)
    {
        Stream[Element_Code].Parser=new File_Ogg_SubElement;
        ((File_Ogg_SubElement*)Stream[Element_Code].Parser)->IsStandAlone=Stream.size()==1;
        Open_Buffer_Init(Stream[Element_Code].Parser);
        StreamsToDo++;
    }

    //Parsing
    File_Ogg_SubElement* Parser=(File_Ogg_SubElement*)Stream[Element_Code].Parser;
    if (Parser->File_GoTo==(int64u)-1)
        //For each chunk
        for (size_t Chunk_Sizes_Pos=0; Chunk_Sizes_Pos<Chunk_Sizes.size(); Chunk_Sizes_Pos++)
        {
            //Info
            if (!continued)
                Peek_L1(packet_type); //Only for information
            Element_Info(Ztring::ToZtring(packet_type, 16));
            if (continued)
                Element_Info("Continue");

            //Parsing
            Open_Buffer_Init(Parser, File_Size, File_Offset+Buffer_Offset);
            if (continued || Parser->File_Offset!=Parser->File_Size)
                Open_Buffer_Continue(Parser, Buffer+Buffer_Offset+(size_t)Element_Offset, Chunk_Sizes[Chunk_Sizes_Pos]);
            if (Chunk_Sizes_Pos<Chunk_Sizes.size()-1
             || (Chunk_Sizes_Pos==Chunk_Sizes.size()-1 && Chunk_Sizes_Finnished))
                Open_Buffer_Continue(Parser, Buffer+Buffer_Offset, 0); //Purge old datas

            if (Parser->File_GoTo!=(int64u)-1)
            {
                Open_Buffer_Finalize(Parser);
                Merge(*Parser);
                Merge(*Parser, Stream_General, 0, 0);
                Stream[Element_Code].StreamKind=((File_Ogg_SubElement*)Parser)->StreamKind;
                Stream[Element_Code].StreamPos=Count_Get(Stream[Element_Code].StreamKind)-1;
                Stream[Element_Code].absolute_granule_position_Resolution=((File_Ogg_SubElement*)Stream[Element_Code].Parser)->absolute_granule_position_Resolution;
                StreamsToDo--;
            }

            Element_Offset+=Chunk_Sizes[Chunk_Sizes_Pos];
            continued=false; //If there is another chunk, this can not be a continued chunk
            if (Parser->File_GoTo!=(int64u)-1)
                Chunk_Sizes_Pos=Chunk_Sizes.size();
        }

    //End of stream
    if (!Parsing_End && File_Size>2*256*1024 &&
        (StreamsToDo==0 || File_Offset+Buffer_Offset+Element_Offset>256*1024))
    {
        Info("OGG, Jumping to end of file");
        File_GoTo=File_Size-(IsSub?0:256*1024);
        std::map<int64u, stream>::iterator Stream_Temp=Stream.begin();
        while (Stream_Temp!=Stream.end())
        {
            Stream_Temp->second.absolute_granule_position=0;
            Stream_Temp++;
        }
        Parsing_End=true;
    }
}


/*
void File_Ogg::Read_Buffer_Continue()
{
    ShouldStop=false;

    //Integrity test
    if (Buffer_Size<4 || CC4(Buffer)!=CC4("OggS"))
    {
        Finnished();
        return;
    }

    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "Ogg");

    //Buffer
    while (!ShouldStop)
    {
        ChunkHeader_Analyse();
        if (!ShouldStop)
        {
            ChunkData_Analyse();
        }
    }

    //Some headers my have not comments, but Analysing start withs comments... We analyse this chunks now
    while (!ChunkHeader_ID.empty())
        Identification_Analyse(ChunkHeader_ID[0]);


    //Finding last frame time
    Buffer_Offset=End_Size-14;
    while (Buffer_Offset>0 && CC4(End+Buffer_Offset)!=CC4("OggS"))
        Buffer_Offset--;
    if (Buffer_Offset==0)
        return 1; //Time not found, but begin is OK...
    int64u Size=LittleEndian2int64u(End+Buffer_Offset+6);
    Ztring ID; ID.From_Number(LittleEndian2int32u(End+Buffer_Offset+14));
    size_t Duration=0;
    for (size_t Pos=0; Pos<Audio.size(); Pos++)
        if (Retrieve(Stream_Audio, Pos, _T("ID"))==ID && Retrieve(Stream_Audio, Pos, _T("SamplingRate")).To_int32u()!=0)
            Duration=Size*1000/Retrieve(Stream_Audio, Pos, _T("SamplingRate")).To_int32u();
    for (size_t Pos=0; Pos<Video.size(); Pos++)
        if (Retrieve(Stream_Video, Pos, _T("ID"))==ID && Retrieve(Stream_Video, Pos, _T("FrameRate")). To_float32()!=0)
            Duration=Size*1000/Retrieve(Stream_Video, Pos, _T("FrameRate")).To_int32u();
    if (Duration)
        Fill(Stream_General, 0, General_Duration, Duration);


    //No need of more
    Finnished();
}
*/


//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ogg::Synchronize()
{
    //Look for first Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if Synchronize() has already parsed first bytes
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+4<=Buffer_Size
        && CC4(Buffer+Buffer_Offset_Temp)!=Ogg::OggS)
        Buffer_Offset_Temp++;

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (Buffer_Offset_Temp+3==Buffer_Size)
        {
            if (CC3(Buffer+Buffer_Offset_Temp)!=CC3("Ogg"))
            {
                Buffer_Offset_Temp++;
                if (CC2(Buffer+Buffer_Offset_Temp)!=CC2("Og"))
                {
                    Buffer_Offset_Temp++;
                    if (CC1(Buffer+Buffer_Offset_Temp)!=CC1("O"))
                        Buffer_Offset_Temp++;
                }
            }
        }

        Buffer_Offset=Buffer_Offset_Temp;
        Buffer_Offset_Temp=0;
        return false;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+4>Buffer_Size)
        return false;

    //Error in stream?
    if (Buffer_Offset_Temp-Buffer_Offset>0)
    {
        if (Synched)
            Trusted_IsNot("Sync error");
        else
            Info("Synchronization");
    }

    //OK, we continue
    Buffer_Offset=Buffer_Offset_Temp;
    Buffer_Offset_Temp=0;
    Synched=true;
    return true;
}

} //NameSpace

#endif //MEDIAINFO_OGG_YES
