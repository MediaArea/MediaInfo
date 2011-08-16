// File_Exr - Info for EXR files
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
// Pre-compilation
#include "MediaInfo/PreComp.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_EXR_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Image/File_Exr.h"
#if MEDIAINFO_DEMUX
    #include "MediaInfo/MediaInfo_Config_MediaInfo.h"
#endif //MEDIAINFO_DEMUX
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Infos
//***************************************************************************

//***************************************************************************
// Const
//***************************************************************************

enum Elements
{
    Pos_FileInformation,
    Pos_GenericSection,
    Pos_IndustrySpecific,
    Pos_UserDefined,
    Pos_Padding,
    Pos_ImageData,
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Exr::File_Exr()
:File__Analyze()
{
    //Configuration
    ParserName=_T("EXR");

    #if MEDIAINFO_DEMUX
        Buffer_Offset_Save=(size_t)-1;
    #endif //MEDIAINFO_DEMUX
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_Exr::Streams_Accept()
{
    Fill(Stream_General, 0, General_Format, "EXR");
    Stream_Prepare(Stream_Image);
    Fill(Stream_Image, 0, Image_Format, "EXR");

    //Configuration
    Buffer_MaximumSize=64*1024*1024;
    Frame_Count_NotParsedIncluded=0;
}

//---------------------------------------------------------------------------
void File_Exr::Streams_Finish()
{
}

//***************************************************************************
// Buffer - Demux
//***************************************************************************

//---------------------------------------------------------------------------
#if MEDIAINFO_DEMUX
bool File_Exr::Demux_UnpacketizeContainer_Test()
{
    if (Buffer_Offset_Save==(size_t)-1)
    {
        Buffer_Offset_Save=Buffer_Offset;
        Buffer_Offset=0; //File header must be included in the packet
    }

    if (Buffer_Size<File_Size)
    {
        size_t* File_Buffer_Size_Hint_Pointer=Config->File_Buffer_Size_Hint_Pointer_Get();
        if (File_Buffer_Size_Hint_Pointer)
            (*File_Buffer_Size_Hint_Pointer)=File_Size;
        return false;
    }

    if (Config->Demux_Rate_Get())
    {
        if (Frame_Count_NotParsedIncluded!=(int64u)-1)
            FrameInfo.DTS=float64_int64s(Frame_Count_NotParsedIncluded*1000000000/Config->Demux_Rate_Get());
        else
            FrameInfo.DTS=(int64u)-1;
        FrameInfo.PTS=FrameInfo.DTS;
        FrameInfo.DUR=float64_int64s(1000000000/Config->Demux_Rate_Get());
    }
    Demux_Offset=Buffer_Size;
    Demux_UnpacketizeContainer_Demux();

    return true;
}
#endif //MEDIAINFO_DEMUX

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Exr::FileHeader_Begin()
{
    //Element_Size
    if (Buffer_Size<4)
        return false; //Must wait for more data

    if (CC4(Buffer)!=0x762F3101) //"v/1"+1
    {
        Reject();
        return false;
    }

    //All should be OK...
    Accept();

    //Testing if parsing is needed
    if (Count_Get(Stream_Video))
    {
        //In a video stream, no need to parse all frames)
        GoToFromEnd(0);
        return true;
    }

    return true;
}

//---------------------------------------------------------------------------
void File_Exr::FileHeader_Parse()
{
    //Element_Size
    if (Buffer_Size<8)
    {
        Element_WaitForMoreData();
        return;
    }

    //Parsing
    int32u Flags;
    int8u Version;
    Skip_L4(                                                    "Magic number");
    Get_L1 (Version,                                            "Version field");
    Get_L3 (Flags,                                              "Flags");

    //Filling
    Fill(Stream_General, 0, General_Format_Version, _T("Version ")+Ztring::ToZtring(Version));
    Fill(Stream_Image, 0, Image_Format_Version, _T("Version ")+Ztring::ToZtring(Version));
    Fill(Stream_Image, 0, Image_Format_Profile, (Flags&0x02)?"Tile":"Line");
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Exr::Header_Begin()
{
    #if MEDIAINFO_DEMUX
        if (Buffer_Offset_Save!=(size_t)-1)
        {
            Buffer_Offset=Buffer_Offset_Save;
            Buffer_Offset_Save=(size_t)-1;
        }
    #endif //MEDIAINFO_DEMUX

    //Name
    name_End=0;
    while (Buffer_Offset+name_End<Buffer_Size)
    {
        if (Buffer[Buffer_Offset+name_End]=='\0')
            break;
        if (name_End==31)
            break;
        name_End++;
    }
    if (Buffer_Offset+name_End>=Buffer_Size)
        return false;
    if (name_End>=31)
    {
        Reject();
        return false;
    }
    if (name_End==0)
    {
        Frame_Count_NotParsedIncluded++;
        Finish();
        return false;
    }

    //Type
    type_End=0;
    while (Buffer_Offset+name_End+1+type_End<Buffer_Size)
    {
        if (Buffer[Buffer_Offset+name_End+1+type_End]=='\0')
            break;
        if (type_End==31)
            break;
        type_End++;
    }

    if (Buffer_Offset+name_End+1+type_End>=Buffer_Size)
        return false;
    if (type_End>=31)
    {
        Reject();
        return false;
    }

    if (Buffer_Offset+name_End+1+type_End+1+4>=Buffer_Size)
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Exr::Header_Parse()
{
    int32u size;
    Get_String(name_End, name,                                  "name");
    Element_Offset++; //Null byte
    Get_String(type_End, type,                                  "type");
    Element_Offset++; //Null byte
    Get_L4 (size,                                               "size");
        
    //Filling
    Header_Fill_Code(0, Ztring().From_Local(name.c_str()));
    Header_Fill_Size(name_End+1+type_End+1+4+size);
}

//---------------------------------------------------------------------------
void File_Exr::Data_Parse()
{
         if (name=="comments" && type=="string")
        comments();
    else if (name=="compression" && type=="compression" && Element_Size==1)
        compression();
    else if (name=="dataWindow" && type=="box2i" && Element_Size==16)
        dataWindow();
    else if (name=="displayWindow" && type=="box2i" && Element_Size==16)
        displayWindow();
    else if (name=="pixelAspectRatio" && type=="float" && Element_Size==4)
        pixelAspectRatio();
    else
        Skip_XX(Element_Size,                                   "value");
}

//---------------------------------------------------------------------------
void File_Exr::comments ()
{
    //Parsing
    Ztring value;
    Get_Local(Element_Size, value,                              "value");

    //Filling
    Fill(Stream_Image, 0, General_Comment, value);
}

//---------------------------------------------------------------------------
void File_Exr::compression ()
{
    //Parsing
    int8u value;
    Get_L1 (value,                                              "value");

    //Filling
    std::string Compression;
    switch (value)
    {
        case 0x00 : Compression="raw"; break;
        case 0x01 : Compression="RLZ"; break;
        case 0x02 : Compression="ZIPS"; break;
        case 0x03 : Compression="ZIP"; break;
        case 0x04 : Compression="PIZ"; break;
        case 0x05 : Compression="PXR24"; break;
        case 0x06 : Compression="B44"; break;
        case 0x07 : Compression="B44A"; break;
        default   : ;
    }
    Fill(Stream_Image, 0, Image_Format_Compression, Compression.c_str());
}

//---------------------------------------------------------------------------
void File_Exr::dataWindow ()
{
    //Parsing
    int32u xMin, yMin, xMax, yMax;
    Get_L4 (xMin,                                               "xMin");
    Get_L4 (yMin,                                               "yMin");
    Get_L4 (xMax,                                               "xMax");
    Get_L4 (yMax,                                               "yMax");
}

//---------------------------------------------------------------------------
void File_Exr::displayWindow ()
{
    //Parsing
    int32u xMin, yMin, xMax, yMax;
    Get_L4 (xMin,                                               "xMin");
    Get_L4 (yMin,                                               "yMin");
    Get_L4 (xMax,                                               "xMax");
    Get_L4 (yMax,                                               "yMax");

    //Filling
    Fill(Stream_Image, 0, Image_Width, xMax-xMin+1);
    Fill(Stream_Image, 0, Image_Height, yMax-yMin+1);
}

//---------------------------------------------------------------------------
void File_Exr::pixelAspectRatio ()
{
    //Parsing
    float value;
    Get_LF4(value,                                              "value"); 

    //Filling
    Fill(Stream_Image, 0, Image_PixelAspectRatio, value?value:1, 3);
}

} //NameSpace

#endif //MEDIAINFO_EXR_YES
