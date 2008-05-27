// File_Wm - Info for Windows Media files
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
// Main part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_WM_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Wm.h"
#if defined(MEDIAINFO_MPEGPS_YES)
    #include "MediaInfo/Multiple/File_MpegPs.h"
#endif
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Wm::File_Wm()
:File__Analyze()
{
    //Configuration
    DataMustAlwaysBeComplete=false;

    //Stream
    Codec_Description_Count=0;
    Stream_Number=0;
    Data_Parse_Padding=0;
    MaximumDataPacketSize=(int32u)-1;
    NumberPayloads=1;
    NumberPayloads_Pos=0;
    Data_Parse_Begin=true;
    IsDvrMs=false;
}

//---------------------------------------------------------------------------
void File_Wm::Read_Buffer_Finalize()
{
    //Encryption management
    /*const Ztring& Encryption=Retrieve(Stream_General, 0, General_Encryption);
    if (!Encryption.empty())
    {
        for (size_t StreamKind=Stream_General+1; StreamKind<Stream_Max; StreamKind++)
            for (size_t Pos=0; Pos<(*Stream[StreamKind]).size(); Pos++)
                Fill ((stream_t)StreamKind, 0, "Encryption", Encryption);
    }

        Fill("BitRate", CurrentBitRate[StreamNumber]);
    */

    std::map<int16u, stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
    {
        std::map<std::string, ZenLib::Ztring>::iterator Info_Temp=Temp->second.Info.begin();
        while (Info_Temp!=Temp->second.Info.end())
        {
            if (Codec_Description_Count==Stream.size() || Info_Temp->first!="CodecID_Description") //With some files, There are only x Codec desecription and !x streams, no coherancy
                Fill(Temp->second.StreamKind, Temp->second.StreamPos, Info_Temp->first.c_str(), Info_Temp->second);
            Info_Temp++;
        }

        if (Temp->second.StreamKind==Stream_Video && Temp->second.AverageTimePerFrame>0)
            Fill(Temp->second.StreamKind, Temp->second.StreamPos, "FrameRate", ((float)10000000)/Temp->second.AverageTimePerFrame, 3, true);
        if (Temp->second.AverageBitRate>0)
            Fill(Temp->second.StreamKind, Temp->second.StreamPos, "BitRate", Temp->second.AverageBitRate, 10, true);
        if (Temp->second.LanguageID!=(int16u)-1 && Temp->second.LanguageID<(int16u)Languages.size())
            Fill(Temp->second.StreamKind, Temp->second.StreamPos, "Language", Languages[Temp->second.LanguageID]);
        else if (!Language_ForAll.empty())
            Fill(Temp->second.StreamKind, Temp->second.StreamPos, "Language", Language_ForAll);
        if (Temp->second.Parser)
        {
            if (Temp->second.StreamKind==Stream_Max)
                if (Temp->second.Parser->Count_Get(Stream_Audio))
                {
                    Stream_Prepare(Stream_Audio);
                    Temp->second.StreamKind=StreamKind_Last;
                    Temp->second.StreamPos=StreamPos_Last;
                }
            Temp->second.Parser->Open_Buffer_Finalize();
            Merge(*Temp->second.Parser, Temp->second.StreamKind, 0, Temp->second.StreamPos);
        }
        Temp++;
    }

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
        Stream.clear();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wm::Header_Parse()
{
    if (!MustUseAlternativeParser)
    {
        //Parsing
        int128u Name;
        int64u Size;
        Get_UUID(Name,                                              "Name");
        Get_L8 (Size,                                               "Size");

        //Filling
        #ifndef __BORLANDC__
            Header_Fill_Code(Name.hi, Ztring().From_UUID(Name));
        #else //__BORLANDC__
            Header_Fill_Code(Name.hi&0xFFFFFFFF, Ztring().From_UUID(Name)); //Borland does not like int64u for const?
        #endif //__BORLANDC__
        Header_Fill_Size(Size);
    }
    else
        //Data
        Header_Parse_Data();
}

//---------------------------------------------------------------------------
void File_Wm::Header_Parse_Data()
{
    //Only Payload
    if (Data_Parse_Begin)
        Header_Parse_Data_Begin();

    //Parsing
    Header_Parse_Data_Payload();
}

//---------------------------------------------------------------------------
void File_Wm::Header_Parse_Data_Begin()
{
    //Parsing
    PacketLength=0; SizeOfMediaObject=0;
    int8u  Flags, ErrorCorrectionData_Length, ErrorCorrectionLengthType, SequenceType, PaddingLengthType, PacketLengthType;
    bool   ErrorCorrectionPresent;
    Element_Begin("Error Correction");
        Get_L1 (Flags,                                          "Flags");
            Get_Flags (Flags&0x0F, ErrorCorrectionData_Length,  "Error Correction Data Length"); //4 lowest bits
            Skip_Flags(Flags, 4,                                "Opaque Data Present");
            Get_Flags ((Flags>>5)&0x03, ErrorCorrectionLengthType, "Error Correction Length Type"); //bits 6 and 7
            Get_Flags (Flags, 7, ErrorCorrectionPresent,        "Error Correction Present");
        if (ErrorCorrectionPresent && ErrorCorrectionLengthType==0 && ErrorCorrectionData_Length==2)
        {
            int8u  TypeNumber;
            Get_L1 (TypeNumber,                                 "Type/Number");
                Skip_Flags((TypeNumber>>4)&0x0F, "Type");
                Skip_Flags( TypeNumber    &0x0F, "Number");
            Skip_L1(                                            "Cycle");
        }
    Element_End();
    Element_Begin("Payload Parsing Information");
        Get_L1 (Flags,                                          "Length Type Flags");
            Get_Flags (Flags, 0, MultiplePayloadsPresent,       "Multiple Payloads Present");
            Get_Flags ((Flags>>1)&0x3, SequenceType,            "Sequence Type");
            Get_Flags ((Flags>>3)&0x3, PaddingLengthType,       "Padding Length Type");
            Get_Flags ((Flags>>5)&0x3, PacketLengthType,        "Packet Length Type");
            Skip_Flags(Flags, 7,                                "Error Correction Present");
        Get_L1 (Flags,                                          "Property Flags");
            Get_Flags ( Flags    &0x3, ReplicatedDataLengthType, "Replicated Data Length Type");
            Get_Flags ((Flags>>2)&0x3, OffsetIntoMediaObjectLengthType, "Offset Into Media Object Length Type");
            Get_Flags ((Flags>>4)&0x3, MediaObjectNumberLengthType, "Media Object Number Length Type");
            Get_Flags ((Flags>>6)&0x3, StreamNumberLengthType,  "Stream Number Length Type");
        switch (PacketLengthType)
        {
            case 1 : {int8u  Data; Get_L1(Data,                 "Packet Length"); PacketLength=Data;} break;
            case 2 : {int16u Data; Get_L2(Data,                 "Packet Length"); PacketLength=Data;} break;
            case 3 :               Get_L4(PacketLength,         "Packet Length");                     break;
            default: ;
        }
        switch (SequenceType)
        {
            case 1 : Skip_L1(                                   "Sequence"); break;
            case 2 : Skip_L2(                                   "Sequence"); break;
            case 3 : Skip_L4(                                   "Sequence"); break;
            default: ;
        }
        switch (PaddingLengthType)
        {
            case 1 : {int8u  Data; Get_L1(Data,                 "Padding Length"); Data_Parse_Padding=Data;} break;
            case 2 : {int16u Data; Get_L2(Data,                 "Padding Length"); Data_Parse_Padding=Data;} break;
            case 3 :               Get_L4(Data_Parse_Padding,   "Padding Length");                           break;
            default: Data_Parse_Padding=0;
        }
        Skip_L4(                                                "Send Time");
        Skip_L2(                                                "Duration");
    Element_End();

    if (MultiplePayloadsPresent)
        Header_Parse_Data_MultiplePayloads();
}

//---------------------------------------------------------------------------
void File_Wm::Header_Parse_Data_MultiplePayloads()
{
    //Parsing
    Element_Begin("Multiple Payloads");
        int8u Flags;
        Get_L1 (Flags,                                          "Flags");
            Get_Flags ( Flags    &0x3F, NumberPayloads,         "Number of Payloads"); //6 bits
            Get_Flags ((Flags>>6)&0x03, PayloadLengthType,      "Payload Length Type"); //bits 6 and 7
    Element_End();

    //Filling
    NumberPayloads_Pos=0;
}

//---------------------------------------------------------------------------
void File_Wm::Header_Parse_Data_Payload()
{
    Element_Begin("Payload");
        int32u ReplicatedDataLength;
        int8u  StreamNumber;
        Get_L1 (StreamNumber,                                   "Stream Number");
        StreamNumber&=0x7F; //For KeyFrame
        Element_Info(StreamNumber);
        switch (MediaObjectNumberLengthType)
        {
            case 1 : Skip_L1(                                   "Media Object Number"); break;
            case 2 : Skip_L2(                                   "Media Object Number"); break;
            case 3 : Skip_L4(                                   "Media Object Number"); break;
            default: ;
        }
        switch (OffsetIntoMediaObjectLengthType)
        {
            case 1 : Skip_L1(                                   "Offset Into Media Object"); break;
            case 2 : Skip_L2(                                   "Offset Into Media Object"); break;
            case 3 : Skip_L4(                                   "Offset Into Media Object"); break;
            default: ;
        }
        switch (ReplicatedDataLengthType)
        {
            case 1 : {int8u  Data; Get_L1(Data,                 "Replicated Data Length"); ReplicatedDataLength=Data;} break;
            case 2 : {int16u Data; Get_L2(Data,                 "Replicated Data Length"); ReplicatedDataLength=Data;} break;
            case 3 :               Get_L4(ReplicatedDataLength, "Replicated Data Length");                             break;
            default: ;
        }
        if (ReplicatedDataLengthType!=0 && ReplicatedDataLength>0)
        {
            if (ReplicatedDataLength>=8)
            {
                Get_L4 (SizeOfMediaObject,                      "Size Of Media Object");
                Skip_L4(                                        "Presentation Time");
                if (ReplicatedDataLength>8)
                {
                    Skip_XX(ReplicatedDataLength-8,             "Payload Extension");
                }
            }
            else if (ReplicatedDataLength==1)
            {
                Skip_L1(                                        "Presentation Time Delta");
                //TODO
            }
            else
                Skip_XX(ReplicatedDataLength,                   "Replicated Data");
        }
    Element_End();

    if (MultiplePayloadsPresent)
    {
        switch (PayloadLengthType)
        {
            case 1 : {int8u  Data; Get_L1(Data,                 "Payload Length"); SizeOfMediaObject=Data;} break;
            case 2 : {int16u Data; Get_L2(Data,                 "Payload Length"); SizeOfMediaObject=Data;} break;
            case 3 :               Get_L4(SizeOfMediaObject,    "Payload Length");                             break;
            default: SizeOfMediaObject=0; //Problem
        }
    }

    if (PacketLength==0 && SizeOfMediaObject>0)
        PacketLength=(int32u)(Element_Offset+SizeOfMediaObject+Data_Parse_Padding);

    if (NumberPayloads_Pos+1<NumberPayloads)
        PacketLength-=Data_Parse_Padding; //Padding is only for the last packet
    if (PacketLength>MaximumDataPacketSize)
        PacketLength=MaximumDataPacketSize; //Some files seem to have a wrong SizeOfMediaObject, trying to parse them and trusting the header better than SizeOfMediaObject

    //Filling
    Header_Fill_Code(StreamNumber, "Packet");
    Header_Fill_Size(PacketLength);

    PacketLength=0; //Used
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wm::HowTo(stream_t UNUSED(StreamKind))
{
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_WM_YES
