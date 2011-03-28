// File_Mk - Info for Ibi files
// Copyright (C) 2011-2011 MediaArea.net SARL, Info@MediaArea.net
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
 
//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_IBI_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Ibi.h"
#include <zlib.h>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Ibi::File_Ibi()
:File__Analyze()
{
    //Configuration
    #if MEDIAINFO_DEMUX
        Demux_Level=2; //Container
    #endif //MEDIAINFO_DEMUX
    DataMustAlwaysBeComplete=false;

    //In
    Ibi=NULL;
}

//---------------------------------------------------------------------------
File_Ibi::~File_Ibi()
{
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ibi::Header_Parse()
{
    //Test of zero padding
    int8u Null;
    Peek_B1(Null);
    if (Null==0x00)
    {
        Skip_B1(                                                "Zero byte");
        Header_Fill_Code((int32u)-1); //Should be (int64u)-1 but Borland C++ does not like this
        Header_Fill_Size(1);
        return;
    }

    //Parsing
    int64u Name, Size;
    Get_EB (Name,                                               "Name");
    Get_EB (Size,                                               "Size");

    //Filling
    Header_Fill_Code(Name, Ztring().From_Number(Name, 16));
    Header_Fill_Size(Element_Offset+Size);
}

//---------------------------------------------------------------------------
namespace Elements
{
    //Common
    const int64u Zero=(int32u)-1; //Should be (int64u)-1 but Borland C++ does not like this
    const int64u CRC32=0x3F;
    const int64u Void=0x6C;

    //EBML
    const int64u Ebml=0xA45DFA3;
    const int64u Ebml_Version=0x286;
    const int64u Ebml_ReadVersion=0x2F7;
    const int64u Ebml_MaxIDLength=0x2F2;
    const int64u Ebml_MaxSizeLength=0x2F3;
    const int64u Ebml_DocType=0x282;
    const int64u Ebml_DocTypeVersion=0x287;
    const int64u Ebml_DocTypeReadVersion=0x285;

    //Segment
    const int64u Stream=0x01;
    const int64u Stream_Header=0x01;
    const int64u Stream_ByteOffset=0x02;
    const int64u Stream_FrameNumber=0x03;
    const int64u Stream_Dts=0x04;
    const int64u CompressedIndex=0x02;
}

//---------------------------------------------------------------------------
void File_Ibi::Data_Parse()
{
    #define LIS2(_ATOM, _NAME) \
        case Elements::_ATOM : \
                if (Level==Element_Level) \
                { \
                    Element_Name(_NAME); \
                    _ATOM(); \
                    Element_ThisIsAList(); \
                } \

    #define ATO2(_ATOM, _NAME) \
                case Elements::_ATOM : \
                        if (Level==Element_Level) \
                        { \
                            if (Element_IsComplete_Get()) \
                            { \
                                Element_Name(_NAME); \
                                _ATOM(); \
                            } \
                            else \
                            { \
                                Element_WaitForMoreData(); \
                                return; \
                            } \
                        } \
                        break; \

    #define ATOM_END_MK \
        ATOM(Zero) \
        ATOM(CRC32) \
        ATOM(Void) \
        ATOM_END

    //Parsing
    DATA_BEGIN
    LIST(Ebml)
        ATOM_BEGIN
        ATOM(Ebml_Version)
        ATOM(Ebml_ReadVersion)
        ATOM(Ebml_MaxIDLength)
        ATOM(Ebml_MaxSizeLength)
        ATOM(Ebml_DocType)
        ATOM(Ebml_DocTypeVersion)
        ATOM(Ebml_DocTypeReadVersion)
        ATOM_END_MK
    LIST(Stream)
        ATOM_BEGIN
            ATOM(Stream_Header)
            ATOM(Stream_ByteOffset)
            ATOM(Stream_FrameNumber)
            ATOM(Stream_Dts)
        ATOM_END_MK
    ATOM(CompressedIndex)
    DATA_DEFAULT
        Finish("Ibi");
    DATA_END_DEFAULT
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ibi::Zero()
{
    Element_Name("ZeroPadding");
}

//---------------------------------------------------------------------------
void File_Ibi::CRC32()
{
    Element_Name("CRC32");

    //Parsing
    UInteger_Info();
}

//---------------------------------------------------------------------------
void File_Ibi::Void()
{
    Element_Name("Void");

    //Parsing
    UInteger_Info();
}

//---------------------------------------------------------------------------
void File_Ibi::Ebml()
{
    Element_Name("Ebml");
}

//---------------------------------------------------------------------------
void File_Ibi::Ebml_Version()
{
    Element_Name("Version");

    //Parsing
    UInteger_Info();
}

//---------------------------------------------------------------------------
void File_Ibi::Ebml_ReadVersion()
{
    Element_Name("ReadVersion");

    //Parsing
    UInteger_Info();
}

//---------------------------------------------------------------------------
void File_Ibi::Ebml_MaxIDLength()
{
    Element_Name("MaxIDLength");

    //Parsing
    UInteger_Info();
}

//---------------------------------------------------------------------------
void File_Ibi::Ebml_MaxSizeLength()
{
    Element_Name("MaxSizeLength");

    //Parsing
    UInteger_Info();
}

//---------------------------------------------------------------------------
void File_Ibi::Ebml_DocType()
{
    Element_Name("DocType");

    //Parsing
    Ztring Data;
    Get_Local(Element_Size, Data,                               "Data"); Element_Info(Data);

    //Filling
    FILLING_BEGIN();

        if (Data==_T("MediaInfo Index"))
        {
            Accept("Ibi");
            Fill(Stream_General, 0, General_Format, "Ibi");
        }
        else
        {
            Reject("Ibi");
            return;
        }

        Buffer_MaximumSize=8*1024*1024;
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Ibi::Ebml_DocTypeVersion()
{
    Element_Name("DocTypeVersion");

    //Parsing
    UInteger_Info();
}

//---------------------------------------------------------------------------
void File_Ibi::Ebml_DocTypeReadVersion()
{
    Element_Name("DocTypeReadVersion");

    //Parsing
    UInteger_Info();
}

//---------------------------------------------------------------------------
void File_Ibi::Stream()
{
    Element_Name("Stream");
}

void File_Ibi::Stream_Header()
{
    Element_Name("Stream Header");
    int64u ID;
    Get_EB (ID,                                                 "ID");

    FILLING_BEGIN();
        if (Ibi)
        {
            //Filling information for ID after data
            bool Found=false;
            for (size_t Pos=0; Pos<Ibi->Streams.size(); Pos++)
                if (Ibi->Streams[Pos].ID==(int64u)-1)
                {
                    Ibi->Streams[Pos].ID=ID;
                    Found=true;
                    break;
                }
            if (!Found)
            {
                ibi::stream Stream;
                Stream.ID=ID;
                Ibi->Streams.push_back(Stream);
            }
        }
    FILLING_END();
}

void File_Ibi::Stream_ByteOffset()
{
    Element_Name("Byte Offset");

    //Init
    if (Ibi && Ibi->Streams.empty())
        Ibi->Streams.push_back(ibi::stream());
    
    //Parsing
    size_t Pos=0;
    while (Element_Offset<Element_Size)
    {
        int64u Item;
        Get_EB (Item,                                           "Item");

        FILLING_BEGIN();
            if (Ibi)
            {
                //Filling information for ID after data
                if (Pos>=Ibi->Streams[Ibi->Streams.size()-1].Infos.size())
                    Ibi->Streams[Ibi->Streams.size()-1].Infos.push_back(ibi::stream::info());
                Ibi->Streams[Ibi->Streams.size()-1].Infos[Pos].StreamOffset=(Pos?Ibi->Streams[Ibi->Streams.size()-1].Infos[Pos-1].StreamOffset:0)+Item;
                Pos++;
            }
        FILLING_END();
    }
}

void File_Ibi::Stream_FrameNumber()
{
    Element_Name("Frame Number");

    //Init
    if (Ibi && Ibi->Streams.empty())
        Ibi->Streams.push_back(ibi::stream());
    
    //Parsing
    size_t Pos=0;
    while (Element_Offset<Element_Size)
    {
        int64u Item;
        Get_EB (Item,                                           "Item");

        FILLING_BEGIN();
            if (Ibi)
            {
                //Filling information for ID after data
                if (Pos>=Ibi->Streams[Ibi->Streams.size()-1].Infos.size())
                    Ibi->Streams[Ibi->Streams.size()-1].Infos.push_back(ibi::stream::info());
                Ibi->Streams[Ibi->Streams.size()-1].Infos[Pos].FrameNumber=(Pos?Ibi->Streams[Ibi->Streams.size()-1].Infos[Pos-1].FrameNumber:0)+Item;
                Pos++;
            }
        FILLING_END();
    }
}

void File_Ibi::Stream_Dts()
{
    Element_Name("DTS");

    //Init
    if (Ibi && Ibi->Streams.empty())
        Ibi->Streams.push_back(ibi::stream());
    
    //Parsing
    size_t Pos=0;
    int64u Item;
    Get_EB (Item,                                               "DtsFrequencyNumerator");
    FILLING_BEGIN();
        if (Ibi)
            Ibi->Streams[Ibi->Streams.size()-1].DtsFrequencyNumerator=Item;
    FILLING_END();
    Get_EB (Item,                                               "DtsFrequencyDenominator");
    FILLING_BEGIN();
        if (Ibi)
            Ibi->Streams[Ibi->Streams.size()-1].DtsFrequencyDenominator=Item;
    FILLING_END();

    while (Element_Offset<Element_Size)
    {
        Get_EB (Item,                                           "Item");

        FILLING_BEGIN();
            if (Ibi)
            {
                //Filling information for ID after data
                if (Pos>=Ibi->Streams[Ibi->Streams.size()-1].Infos.size())
                    Ibi->Streams[Ibi->Streams.size()-1].Infos.push_back(ibi::stream::info());
                Ibi->Streams[Ibi->Streams.size()-1].Infos[Pos].Dts=(Pos?Ibi->Streams[Ibi->Streams.size()-1].Infos[Pos-1].Dts:0)+Item;
                Pos++;
            }
        FILLING_END();
    }
}

void File_Ibi::CompressedIndex()
{
    Element_Name("Compressed Index");
    int64u UncompressedSize;
    Get_EB (UncompressedSize,                                   "Uncompressed size");

    //Sizes
    unsigned long Source_Size=(unsigned long)(Element_Size-Element_Offset);
    unsigned long Dest_Size=(unsigned long)UncompressedSize;

    //Uncompressing
    int8u* Dest=new int8u[Dest_Size];
    if (uncompress((Bytef*)Dest, &Dest_Size, (const Bytef*)Buffer+Buffer_Offset+(size_t)Element_Offset, Source_Size)<0)
    {
        Skip_XX(Element_Size-Element_Offset,                    "Problem during the decompression");
        delete[] Dest; //Dest=NULL;
        return;
    }

    //Exiting this element
    Skip_XX(Element_Size,                                   "Will be parsed");

    //Configuring buffer
    const int8u* Buffer_Sav=Buffer;
    size_t Buffer_Size_Sav=Buffer_Size;
    int8u* Buffer_Temp_Sav=Buffer_Temp;
    size_t Buffer_Temp_Size_Sav=Buffer_Temp_Size;
    size_t Buffer_Offset_Sav=Buffer_Offset;
    size_t Buffer_Offset_Temp_Sav=Buffer_Offset_Temp;
    Buffer=NULL;
    Buffer_Size=0;
    Buffer_Temp=NULL;
    Buffer_Temp_Size=0;
    Buffer_Offset=0;
    Buffer_Offset_Temp=0;

    //Configuring level
    std::vector<int64u> Element_Sizes_Sav;
    size_t Element_Level_Sav=Element_Level;
    while(Element_Level)
    {
        Element_Sizes_Sav.push_back(Element_TotalSize_Get());
        Element_End();
    }

    //Configuring file size
    int64u File_Size_Sav=File_Size;
    if (File_Size<File_Offset+Buffer_Offset+Element_Offset+Dest_Size)
        File_Size=File_Offset+Buffer_Offset+Element_Offset+Dest_Size;
    Element_Level++;
    Header_Fill_Size(File_Size);
    Element_Level--;

    //Parsing
    Open_Buffer_Continue(Dest, Dest_Size);
    delete[] Dest; //Dest=NULL;

    //Resetting file size
    File_Size=File_Size_Sav;
    while(Element_Level)
        Element_End();
    Element_Level++;
    Header_Fill_Size(File_Size);
    Element_Level--;

    //Configuring level
    while(Element_Level<Element_Level_Sav)
    {
        Element_Begin();
        Element_Begin();
        Header_Fill_Size(Element_Sizes_Sav[0]);
        Element_End();
    }

    //Resetting buffer
    Buffer=Buffer_Sav;
    Buffer_Size=Buffer_Size_Sav;
    Buffer_Temp=Buffer_Temp_Sav;
    Buffer_Temp_Size=Buffer_Temp_Size_Sav;
    Buffer_Offset=Buffer_Offset_Sav;
    Buffer_Offset_Temp=Buffer_Offset_Temp_Sav;
}

//***************************************************************************
// Data
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ibi::UInteger_Info()
{
    switch (Element_Size)
    {
        case 1 :
                {
                    Info_B1(Data,                               "Data"); Element_Info(Data);
                    return;
                }
        case 2 :
                {
                    Info_B2(Data,                               "Data"); Element_Info(Data);
                    return;
                }
        case 3 :
                {
                    Info_B3(Data,                               "Data"); Element_Info(Data);
                    return;
                }
        case 4 :
                {
                    Info_B4(Data,                               "Data"); Element_Info(Data);
                    return;
                }
        case 5 :
                {
                    Info_B5(Data,                               "Data"); Element_Info(Data);
                    return;
                }
        case 6 :
                {
                    Info_B6(Data,                               "Data"); Element_Info(Data);
                    return;
                }
        case 7 :
                {
                    Info_B7(Data,                               "Data"); Element_Info(Data);
                    return;
                }
        case 8 :
                {
                    Info_B8(Data,                               "Data"); Element_Info(Data);
                    return;
                }
        case 16:
                {
                    Info_B16(Data,                              "Data"); Element_Info(Data);
                    return;
                }
        default : Skip_XX(Element_Size,                         "Data");
    }
}

//---------------------------------------------------------------------------
int64u File_Ibi::UInteger_Get()
{
    switch (Element_Size)
    {
        case 1 :
                {
                    int8u Data;
                    Get_B1 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 2 :
                {
                    int16u Data;
                    Get_B2 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 3 :
                {
                    int32u Data;
                    Get_B3 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 4 :
                {
                    int32u Data;
                    Get_B4 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 5 :
                {
                    int64u Data;
                    Get_B5 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 6 :
                {
                    int64u Data;
                    Get_B6 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 7 :
                {
                    int64u Data;
                    Get_B7 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 8 :
                {
                    int64u Data;
                    Get_B8 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        default :   Skip_XX(Element_Size,                       "Data");
                    return 0;
    }
}

//---------------------------------------------------------------------------
int128u File_Ibi::UInteger16_Get()
{
    switch (Element_Size)
    {
        case 1 :
                {
                    int8u Data;
                    Get_B1 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 2 :
                {
                    int16u Data;
                    Get_B2 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 3 :
                {
                    int32u Data;
                    Get_B3 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 4 :
                {
                    int32u Data;
                    Get_B4 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 5 :
                {
                    int64u Data;
                    Get_B5 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 6 :
                {
                    int64u Data;
                    Get_B6 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 7 :
                {
                    int64u Data;
                    Get_B7 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 8 :
                {
                    int64u Data;
                    Get_B8 (Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        case 16:
                {
                    int128u Data;
                    Get_B16(Data,                               "Data"); Element_Info(Data);
                    return Data;
                }
        default :   Skip_XX(Element_Size,                       "Data");
                    return 0;
    }
}

} //NameSpace

#endif //MEDIAINFO_IBI_YES

