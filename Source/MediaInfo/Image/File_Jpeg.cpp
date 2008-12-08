// File_Jpeg - Info for NewFormat files
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
// Links
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// http://www.fileformat.info/format/jpeg/
// http://park2.wakwak.com/~tsuruzoh/Computer/Digicams/exif-e.html
// http://www.w3.org/Graphics/JPEG/jfif3.pdf
// http://www.sentex.net/~mwandel/jhead/
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
#if defined(MEDIAINFO_JPEG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Image/File_Jpeg.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Const
//***************************************************************************

namespace Elements
{
    const int16u TEM =0xFF01;
    const int16u S0F0=0xFFC0;
    const int16u S0F1=0xFFC1;
    const int16u S0F2=0xFFC2;
    const int16u S0F3=0xFFC3;
    const int16u DHT =0xFFC4;
    const int16u S0F5=0xFFC5;
    const int16u S0F6=0xFFC6;
    const int16u S0F7=0xFFC7;
    const int16u JPG =0xFFC8;
    const int16u S0F9=0xFFC9;
    const int16u S0FA=0xFFCA;
    const int16u S0FB=0xFFCB;
    const int16u DAC =0xFFCC;
    const int16u S0FD=0xFFCD;
    const int16u S0FE=0xFFCE;
    const int16u S0FF=0xFFCF;
    const int16u RST0=0xFFD0;
    const int16u RST1=0xFFD1;
    const int16u RST2=0xFFD2;
    const int16u RST3=0xFFD3;
    const int16u RST4=0xFFD4;
    const int16u RST5=0xFFD5;
    const int16u RST6=0xFFD6;
    const int16u RST7=0xFFD7;
    const int16u SOI =0xFFD8;
    const int16u EOI =0xFFD9;
    const int16u SOS =0xFFDA;
    const int16u DQT =0xFFDB;
    const int16u DNL =0xFFDC;
    const int16u DRI =0xFFDD;
    const int16u DHP =0xFFDE;
    const int16u EXP =0xFFDF;
    const int16u APP0=0xFFE0;
    const int16u APP1=0xFFE1;
    const int16u APP2=0xFFE2;
    const int16u APP3=0xFFE3;
    const int16u APP4=0xFFE4;
    const int16u APP5=0xFFE5;
    const int16u APP6=0xFFE6;
    const int16u APP7=0xFFE7;
    const int16u APP8=0xFFE8;
    const int16u APP9=0xFFE9;
    const int16u APPA=0xFFEA;
    const int16u APPB=0xFFEB;
    const int16u APPC=0xFFEC;
    const int16u APPD=0xFFED;
    const int16u APPE=0xFFEE;
    const int16u APPF=0xFFEF;
    const int16u JPG0=0xFFF0;
    const int16u JPG1=0xFFF1;
    const int16u JPG2=0xFFF2;
    const int16u JPG3=0xFFF3;
    const int16u JPG4=0xFFF4;
    const int16u JPG5=0xFFF5;
    const int16u JPG6=0xFFF6;
    const int16u JPG7=0xFFF7;
    const int16u JPG8=0xFFF8;
    const int16u JPG9=0xFFF9;
    const int16u JPGA=0xFFFA;
    const int16u JPGB=0xFFFB;
    const int16u JPGC=0xFFFC;
    const int16u JPGD=0xFFFD;
    const int16u COM =0xFFFE;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Jpeg::FileHeader_Parse()
{
    //Parsing
    Element_Begin("FFD8 - Start Of Image", 10);
    int16u identifier;
    Get_B2 (identifier,                                         "identifier");
    Element_End();

    FILLING_BEGIN();
        if (identifier!=0xFFD8)
        {
            Finished();
            return;
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
// An Element
// Size                             2 bytes, Pos=4
// Code                             5 bytes, Pos=6
//
void File_Jpeg::Header_Parse()
{
    //Parsing
    int16u code, size=0;
    Get_B2 (code,                                               "code");
    switch (code)
    {
        case Elements::TEM :
        case Elements::RST0 :
        case Elements::RST1 :
        case Elements::RST2 :
        case Elements::RST3 :
        case Elements::RST4 :
        case Elements::RST5 :
        case Elements::RST6 :
        case Elements::RST7 :
        case Elements::SOI  :
        case Elements::EOI  :
                    break; //Size=0
        default   : Get_B2 (size,                                  "size");
    }

    //Filling
    Header_Fill_Code(code, Ztring().From_CC2(code));
    Header_Fill_Size(2+size);
}

//---------------------------------------------------------------------------
// Element parse
//
void File_Jpeg::Data_Parse()
{
    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case Elements::_NAME : Element_Info(_DETAIL); _NAME(); break;

    //Parsing
    switch (Element_Code)
    {
        ELEMENT_CASE(TEM , "TEM");
        ELEMENT_CASE(S0F0, "Baseline DCT (Huffman)");
        ELEMENT_CASE(S0F1, "Extended sequential DCT (Huffman)");
        ELEMENT_CASE(S0F2, "Progressive DCT (Huffman)");
        ELEMENT_CASE(S0F3, "Lossless (sequential) (Huffman)");
        ELEMENT_CASE(DHT , "Define Huffman Tables");
        ELEMENT_CASE(S0F5, "Differential sequential DCT (Huffman)");
        ELEMENT_CASE(S0F6, "Differential progressive DCT (Huffman)");
        ELEMENT_CASE(S0F7, "Differential lossless (sequential) (Huffman)");
        ELEMENT_CASE(JPG , "Reserved for JPEG extensions");
        ELEMENT_CASE(S0F9, "Extended sequential DCT (Arithmetic)");
        ELEMENT_CASE(S0FA, "Progressive DCT (Arithmetic)");
        ELEMENT_CASE(S0FB, "Lossless (sequential) (Arithmetic)");
        ELEMENT_CASE(DAC , "Define Arithmetic Coding");
        ELEMENT_CASE(S0FD, "Differential sequential DCT (Arithmetic)");
        ELEMENT_CASE(S0FE, "Differential progressive DCT (Arithmetic)");
        ELEMENT_CASE(S0FF, "Differential lossless (sequential) (Arithmetic)");
        ELEMENT_CASE(RST0, "Restart Interval Termination 0");
        ELEMENT_CASE(RST1, "Restart Interval Termination 1");
        ELEMENT_CASE(RST2, "Restart Interval Termination 2");
        ELEMENT_CASE(RST3, "Restart Interval Termination 3");
        ELEMENT_CASE(RST4, "Restart Interval Termination 4");
        ELEMENT_CASE(RST5, "Restart Interval Termination 5");
        ELEMENT_CASE(RST6, "Restart Interval Termination 6");
        ELEMENT_CASE(RST7, "Restart Interval Termination 7");
        ELEMENT_CASE(SOI , "Start Of Image");
        ELEMENT_CASE(EOI , "End Of Image");
        ELEMENT_CASE(SOS , "Start Of Scan");
        ELEMENT_CASE(DQT , "Define Quantization Tables");
        ELEMENT_CASE(DNL , "Define Number of Lines");
        ELEMENT_CASE(DRI , "Define Restart Interval");
        ELEMENT_CASE(DHP , "Define Hierarchical Progression");
        ELEMENT_CASE(EXP , "Expand Reference Components");
        ELEMENT_CASE(APP0, "Application-specific marker 0");
        ELEMENT_CASE(APP1, "Application-specific marker 1");
        ELEMENT_CASE(APP2, "Application-specific marker 2");
        ELEMENT_CASE(APP3, "Application-specific marker 3");
        ELEMENT_CASE(APP4, "Application-specific marker 4");
        ELEMENT_CASE(APP5, "Application-specific marker 5");
        ELEMENT_CASE(APP6, "Application-specific marker 6");
        ELEMENT_CASE(APP7, "Application-specific marker 7");
        ELEMENT_CASE(APP8, "Application-specific marker 8");
        ELEMENT_CASE(APP9, "Application-specific marker 9");
        ELEMENT_CASE(APPA, "Application-specific marker 10");
        ELEMENT_CASE(APPB, "Application-specific marker 11");
        ELEMENT_CASE(APPC, "Application-specific marker 12");
        ELEMENT_CASE(APPD, "Application-specific marker 13");
        ELEMENT_CASE(APPE, "Application-specific marker 14");
        ELEMENT_CASE(APPF, "Application-specific marker 15");
        ELEMENT_CASE(JPG0, "JPG");
        ELEMENT_CASE(JPG1, "JPG");
        ELEMENT_CASE(JPG2, "JPG");
        ELEMENT_CASE(JPG3, "JPG");
        ELEMENT_CASE(JPG4, "JPG");
        ELEMENT_CASE(JPG5, "JPG");
        ELEMENT_CASE(JPG6, "JPG");
        ELEMENT_CASE(JPG7, "JPG");
        ELEMENT_CASE(JPG8, "JPG");
        ELEMENT_CASE(JPG9, "JPG");
        ELEMENT_CASE(JPGA, "JPG");
        ELEMENT_CASE(JPGB, "JPG");
        ELEMENT_CASE(JPGC, "JPG");
        ELEMENT_CASE(JPGD, "JPG");
        ELEMENT_CASE(COM , "Comment");
        default : Element_Info("Reserved");
                  Skip_XX(Element_Size,                         "Data");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Jpeg::SOF_()
{
    //Parsing
    int16u Height, Width;
    int8u  Resolution, Count;
    Get_B1 (Resolution,                                         "Resolution");
    Get_B2 (Height,                                             "Height");
    Get_B2 (Width,                                              "Width");
    Get_B1 (Count,                                              "Number of image components in frame");
    for (int8u Pos=0; Pos<Count; Pos++)
    {
        Skip_B1(                                                "Identifier");
        Skip_B1(                                                "sampling factor");
        Skip_B1(                                                "Quantization table destination selector");
    }

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "JPEG");
        Stream_Prepare(Stream_Image);
        Fill(Stream_Image, 0, Image_Format, "JPEG");
        Fill(Stream_Image, 0, Image_Codec, "JPEG");
        Fill(Stream_Image, 0, Image_Codec_String, "JPEG"); //To Avoid automatic filling
        Fill(Stream_Image, 0, Image_Resolution, Resolution);
        Fill(Stream_Image, 0, Image_Height, Height);
        Fill(Stream_Image, 0, Image_Width, Width);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Jpeg::SOS()
{
    //Parsing
    int8u Count;
    Get_B1 (Count,                                              "Number of image components in scan");
    for (int8u Pos=0; Pos<Count; Pos++)
    {
        Skip_B1(                                                "Scan component selector");
        Skip_B1(                                                "Entropy coding table destination selector");
    }
    Skip_B1(                                                    "Start of spectral or predictor selection");
    Skip_B1(                                                    "End of spectral selection");
    Skip_B1(                                                    "Successive approximation bit position");

    //Filling
    Finished(); //No need of more
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0()
{
    //Parsing
    int32u Name;
    Get_C4(Name,                                                "Name");
    if (Name==CC4("AVI1"))
        APP0_AVI1();
    if (Name==CC4("JFIF"))
        APP0_JFIF();
    if (Name==CC4("JFFF"))
        APP0_JFXX();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_AVI1()
{
    //Parsing
    int8u  FieldOrder=(int8u)-1;;
    Element_Begin("AVI1");
        if (Element_Size==16-4)
        {
            Get_B1 (FieldOrder,                                     "Field Order");
            Skip_XX(7,                                              "Zeroes");
        }
        if (Element_Size==18-4)
        {
            Get_B1 (FieldOrder,                                     "Field Order");
            Skip_B1(                                                "Zero");
            Skip_B4(                                                "Size of 1st Field");
            Skip_B4(                                                "Size of 2nd Field");
        }
    Element_End();

    FILLING_BEGIN();
        if (Count_Get(Stream_General)==0 && File_Name.empty())
        {
            Stream_Prepare(Stream_General);
            Fill(Stream_General, 0, General_Format, "JPEG");
            Stream_Prepare(Stream_Video);
            Fill(Stream_Video, 0, Video_Format, "M-JPEG");
            Fill(Stream_Video, 0, Video_Codec, "M-JPEG");
        }

        switch (FieldOrder)
        {
            case 0x00 : Fill(Stream_Video, 0, Video_Interlacement, "PPF"); Fill(Stream_Video, 0, Video_ScanType, "Progressive"); break;
            case 0x01 : Fill(Stream_Video, 0, Video_Interlacement, "TFF"); Fill(Stream_Video, 0, Video_ScanType, "Interlaced"); Fill(Stream_Video, 0, Video_ScanOrder, "TFF"); break;
            case 0x02 : Fill(Stream_Video, 0, Video_Interlacement, "BFF"); Fill(Stream_Video, 0, Video_ScanType, "Interlaced"); Fill(Stream_Video, 0, Video_ScanOrder, "BFF"); break;
            default   : ;
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFIF()
{
    //Parsing
    Skip_B1(                                                    "Zero");
    Element_Begin("JFIF");
        int16u Width, Height;
        int8u  Unit, ThumbailX, ThumbailY;
        Skip_B2(                                                "Version");
        Get_B1 (Unit,                                           "Unit"); //0=Pixels, 1=dpi, 2=dpcm
        Get_B2 (Width,                                          "Xdensity");
        Get_B2 (Height,                                         "Ydensity");
        Get_B1 (ThumbailX,                                      "Xthumbail");
        Get_B1 (ThumbailY,                                      "Ythumbail");
        Skip_XX(3*ThumbailX*ThumbailY,                          "RGB Thumbail");
    Element_End();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFXX()
{
    Skip_B1(                                                    "Zero");
    Element_Begin("Extension");
        Skip_B1(                                                "extension_code"); //0x10 Thumbnail coded using JPEG, 0x11 Thumbnail stored using 1 byte/pixel, 0x13 Thumbnail stored using 3 bytes/pixel
        if (Element_Size>Element_Offset)
            Skip_XX(Element_Size-Element_Offset,                "extension_data");
    Element_End();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFXX_JPEG()
{
    //Parsing
    Element_Begin("Thumbail JPEG");
        if (Element_Size>Element_Offset)
            Skip_XX(Element_Size-Element_Offset,                "Data");
    Element_End();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFXX_1B()
{
    //Parsing
    Element_Begin("Thumbail 1 byte per pixel");
        int8u  ThumbailX, ThumbailY;
        Get_B1 (ThumbailX,                                      "Xthumbail");
        Get_B1 (ThumbailY,                                      "Ythumbail");
        Skip_XX(768,                                            "Palette");
        Skip_XX(ThumbailX*ThumbailY,                            "Thumbail");
    Element_End();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFXX_3B()
{
    //Parsing
    Element_Begin("Thumbail 3 bytes per pixel");
        int8u  ThumbailX, ThumbailY;
        Get_B1 (ThumbailX,                                      "Xthumbail");
        Get_B1 (ThumbailY,                                      "Ythumbail");
        Skip_XX(3*ThumbailX*ThumbailY,                          "RGB Thumbail");
    Element_End();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP1()
{
    //Parsing
    int64u Name;
    Get_C6(Name,                                                "Name");

    if (Name==CC6("Exif\0\0"))
        APP1_EXIF();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP1_EXIF()
{
    //Parsing
    Element_Begin("Exif");
        int32u Alignment;
        Get_C4(Alignment,                                       "Alignment");
        if (Alignment==0x49492A00)
            Skip_B4(                                            "First_IFD");
        if (Alignment==0x4D4D2A00)
            Skip_L4(                                            "First_IFD");
    Element_End();
}

} //NameSpace

#endif
