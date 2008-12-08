// File_Png - Info for PNG files
// Copyright (C) 2005-2008 Jerome Martinez, Zen@MediaArea.net
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PNG - Format
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// From http://www.fileformat.info/format/png/
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_PNG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Image/File_Png.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Const
//***************************************************************************

const char* Png_Colour_type(int8u Colour_type)
{
    switch (Colour_type)
    {
        case 0 : return "Greyscale";
        case 2 : return "Truecolour";
        case 3 : return "Indexed-colour";
        case 4 : return "Greyscale with alpha";
        case 6 : return "Truecolour with alpha";
    default: return "";
    }
}

namespace Elements
{
    const int32u IDAT=0x49444154;
    const int32u IEND=0x49454E44;
    const int32u IHDR=0x49484452;
    const int32u PLTE=0x506C5445;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Png::FileHeader_Parse()
{
    //Parsing
    int32u Signature, ByteOrder;
    Get_B4 (Signature,                                          "Signature");
    Get_B4 (ByteOrder,                                          "ByteOrder");

    FILLING_BEGIN();
        if (ByteOrder!=0x0D0A1A0A)
        {
            Finished();
            return;
        }

        switch (Signature)
        {
            case 0x89504E47 :
                Stream_Prepare(Stream_General);
                Fill(Stream_General, 0, General_Format, "PNG");

                Stream_Prepare(Stream_Image);
                break;
            case 0x8A4E4E47 :
                Stream_Prepare(Stream_General);
                Fill(Stream_General, 0, General_Format, "MNG");

                Stream_Prepare(Stream_Image);
                Fill(Stream_Image, 0, Image_Codec, "MNG");
                Fill(Stream_Image, 0, Image_Format, "MNG");
                Finished();
                break;
            case 0x8B4A4E47 :
                Stream_Prepare(Stream_General);
                Fill(Stream_General, 0, General_Format, "JNG");

                Stream_Prepare(Stream_Image);
                Fill(Stream_Image, 0, Image_Format, "JNG");
                Fill(Stream_Image, 0, Image_Codec, "JNG");
                Finished();
                break;
            default:
                Finished();
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Png::Header_Parse()
{
    //Parsing
    int32u Length, Chunk_Type;
    Get_B4 (Length,                                             "Length");
    Get_C4 (Chunk_Type,                                         "Chunk Type");

    //Filling
    Header_Fill_Size(12+Length); //+4 for CRC
    Header_Fill_Code(Chunk_Type, Ztring().From_CC4(Chunk_Type));
}

//---------------------------------------------------------------------------
void File_Png::Data_Parse()
{
    Element_Size-=4; //For CRC

    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case Elements::_NAME : Element_Name(_DETAIL); _NAME(); break;

    //Parsing
    switch (Element_Code)
    {
        ELEMENT_CASE(IDAT,                          "Image data");
        ELEMENT_CASE(IEND,                          "Image trailer");
        ELEMENT_CASE(IHDR,                          "Image header");
        ELEMENT_CASE(PLTE,                          "Palette table");
        default : Skip_XX(Element_Size,             "Unknown");
    }

    Element_Size+=4; //For CRC
    Skip_B4(                                                    "CRC");
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Png::IDAT()
{
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Png::IEND()
{
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Png::IHDR()
{
    int32u Width, Height;
    int8u  Bit_depth, Colour_type, Compression_method, Interlace_method;
    //Parsing
    Get_B4 (Width,                                              "Width");
    Get_B4 (Height,                                             "Height");
    Get_B1 (Bit_depth,                                          "Bit depth");
    Get_B1 (Colour_type,                                        "Colour type"); Param_Info(Png_Colour_type(Colour_type));
    Get_B1 (Compression_method,                                 "Compression method");
    Skip_B1(                                                    "Filter method");
    Get_B1 (Interlace_method,                                   "Interlace method");

    FILLING_BEGIN();
        Fill(Stream_Image, 0, Image_Width, Width);
        Fill(Stream_Image, 0, Image_Height, Height);
        int8u Resolution;
        switch (Colour_type)
        {
            case 0 : Resolution=Bit_depth; break;
            case 2 : Resolution=Bit_depth*3; break;
            case 3 : Resolution=Bit_depth; break;
            case 4 : Resolution=Bit_depth*2; break;
            case 6 : Resolution=Bit_depth*4; break;
            default: Resolution=0;
        }
        if (Resolution)
            Fill(Stream_Image, 0, Image_Resolution, Resolution);
        switch (Compression_method)
        {
            case 0 :
                Fill(Stream_Image, 0, Image_Format, "LZ77");
                Fill(Stream_Image, 0, Image_Codec, "LZ77 variant");
                break;
            default: ;
        }
        switch (Interlace_method)
        {
            case 0 :
                break;
            case 1 :
                break;
            default: ;
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Png::PLTE()
{
    Skip_XX(Element_Size,                                       "Data");
}

} //NameSpace

#endif
