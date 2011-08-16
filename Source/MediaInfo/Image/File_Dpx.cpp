// File_Dpx - Info for DPX (SMPTE 268M) files
// Copyright (C) 2010-2011 MediaArea.net SARL, Info@MediaArea.net
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
#if defined(MEDIAINFO_DPX_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Image/File_Dpx.h"
#if MEDIAINFO_DEMUX
    #include "MediaInfo/MediaInfo_Config_MediaInfo.h"
#endif //MEDIAINFO_DEMUX
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
const char* DPX_Orientation[]=
{
    "Left to right, Top to bottom",
    "Right to left, Top to bottom",
    "Left to right, Bottom to top",
    "Right to left, Bottom to top",
    "Top to bottom, Left to right",
    "Top to bottom, Right to left",
    "Bottom to top, Left to right",
    "Bottom to top, Right to left",
    "Reserved for future use"
};

//---------------------------------------------------------------------------
const char* DPX_Descriptors0[]=
{
    "User defined (or unspecified single component)",
    "Red (R)",
    "Green (G)",
    "Blue (B)",
    "Alpha (matte)",
    "Luma (Y)",
    "Color Difference (CB, CR, subsampled by two)",
    "Depth (Z)",
    "Composite video"
};

const char* DPX_Descriptors50[]=
{
    "R,G,B",
    "R,G,B, Alpha (A)",
    "A, B, G, R"
};

const char* DPX_Descriptors100[]=
{
    "CB, Y, CR, Y (4:2:2) ---- based on SMPTE 125M",
    "CB, Y, A, CR, Y, A (4:2:2:4)",
    "CB, Y, CR (4:4:4)",
    "CB, Y, CR, A (4:4:4:4)"
};

const char* DPX_Descriptors150[]=
{
    "User-defined 2-component element",
    "User-defined 3-component element",
    "User-defined 4-component element",
    "User-defined 5-component element",
    "User-defined 6-component element",
    "User-defined 7-component element",
    "User-defined 8-component element"
};

const char* DPX_Descriptors(int8u i)
{
    if(i<10)
        return DPX_Descriptors0[i];
    if(i<50)
        return "Reserved for future single components";
    if(i<53)
        return DPX_Descriptors50[i-50];
    if(i<100)
        return "Reserved for future RGB ++ formats";
    if(i<104)
        return DPX_Descriptors100[i-100];
    if(i<150)
        return "Reserved for future CBYCR ++ formats";
    if(i<157)
        return "Reserved for future single components";
    return "Reserved for future formats";
}

//---------------------------------------------------------------------------
const char* DPX_TransferCharacteristic[]=
{
    "User defined",
    "Printing density",
    "Linear",
    "Logarithmic",
    "Unspecified video",
    "SMPTE 274M",
    "ITU-R 709-4",
    "ITU-R 601-5 system B or G (625)",
    "ITU-R 601-5 system M (525)",
    "Composite video (NTSC); see SMPTE 170M",
    "Composite video (PAL); see ITU-R 624-4",
    "Z (depth) - linear",
    "Z (depth) - homogeneous", //(distance to screen and angle of view must also be specified in user-defined section)
    "Reserved for future use"
};

//---------------------------------------------------------------------------
const char* DPX_ColorimetricSpecification[]=
{
    "User defined",
    "Printing density",
    "Not applicable",
    "Not applicable",
    "Unspecified video",
    "SMPTE 274M",
    "ITU-R 709-4",
    "ITU-R 601-5 system B or G (625)",
    "ITU-R 601-5 system M (525)",
    "Composite video (NTSC); see SMPTE 170M",
    "Composite video (PAL); see ITU-R 624-4",
    "Not applicable",
    "Not applicable",
    "Reserved for future use"
};
 
//---------------------------------------------------------------------------
const char* DPX_ValidBitDephs(int8u i)
{
    switch(i)
    {
        case 1  :
        case 8  :
        case 10 :
        case 12 :
        case 16 :
                    return "integer";
        case 32 :
                    return "IEEE floating point (R32)";
        case 64 :
                    return "IEEE floating point (R64)";
        default :
                    return "invalid";
    }
}

//---------------------------------------------------------------------------
const char* DPX_ComponentDataPackingMethod[]=
{
    "Packed into 32-bit words",
    "Filled to 32-bit words, method A",
    "Filled to 32-bit words, method B",
    "Reserved for future use",
    "Reserved for future use",
    "Reserved for future use",
    "Reserved for future use",
    "Reserved for future use"
};

//---------------------------------------------------------------------------
const char* DPX_ComponentDataEncodingMethod[]=
{
    "No encoding applied",
    "Run-length encoded",
    "Reserved for future use",
    "Reserved for future use",
    "Reserved for future use",
    "Reserved for future use",
    "Reserved for future use",
    "Reserved for future use"
};

//---------------------------------------------------------------------------
const char* DPX_VideoSignalStandard0[]=
{
    "Undefined",
    "NTSC",
    "PAL",
    "PAL-M",
    "SECAM"
};

const char* DPX_VideoSignalStandard50[]=
{
    "YCBCR ITU-R 601-5 525-line, 2:1 interlace, 4:3 aspect ratio",
    "YCBCR ITU-R 601-5 625-line, 2:1 interlace, 4:3 aspect ratio"
};

const char* DPX_VideoSignalStandard100[]=
{
    "YCBCR ITU-R 601-5 525-line, 2:1 interlace, 16:9 aspect ratio",
    "YCBCR ITU-R 601-5 625-line, 2:1 interlace, 16:9 aspect ratio"
};

const char* DPX_VideoSignalStandard150[]=
{
    "YCBCR 1050-line, 2:1 interlace, 16:9 aspect ratio",
    "YCBCR 1125-line, 2:1 interlace, 16:9 aspect ratio (SMPTE 274M)",
    "YCBCR 1250-line, 2:1 interlace, 16:9 aspect ratio",
    "YCBCR 1125-line, 2:1 interlace, 16:9 aspect ratio (SMPTE 240M)"
};

const char* DPX_VideoSignalStandard200[]=
{
    "YCBCR 525-line, 1:1 progressive, 16:9 aspect ratio",
    "YCBCR 625-line, 1:1 progressive, 16:9 aspect ratio",
    "YCBCR 750-line, 1:1 progressive, 16:9 aspect ratio (SMPTE 296M)",
    "YCBCR 1125-line, 1:1 progressive, 16:9 aspect ratio (SMPTE 274M)"
};

const char* DPX_VideoSignalStandard(int8u i)
{
    if(i<5)
        return DPX_VideoSignalStandard0[i];
    if(i<50)
        return "Reserved for other composite video";
    if(i<52)
        return DPX_VideoSignalStandard50[i-50];
    if(i<100)
        return "Reserved for future component video";
    if(i<102)
        return DPX_VideoSignalStandard100[i-100];
    if(i<150)
        return "Reserved for future widescreen";
    if(i<154)
        return DPX_VideoSignalStandard150[i-150];
    if(i<200)
        return "Reserved for future high-definition interlace";
    if(i<204)
        return DPX_VideoSignalStandard200[i-200];
    return "Reserved for future high-definition progressive";
}

//***************************************************************************
// Const
//***************************************************************************

enum Elements
{
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
File_Dpx::File_Dpx()
:File__Analyze()
{
    //Configuration
    ParserName=_T("DPX");
}

//***************************************************************************
// Streams management
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dpx::Streams_Accept()
{
    Fill(Stream_General, 0, General_Format, "DPX");
    Fill(Stream_General, 0, General_Format_Version, Version==1?"Version 1":"Version 2");

    //Configuration
    Buffer_MaximumSize=64*1024*1024;
    Frame_Count_NotParsedIncluded=0;
}

//---------------------------------------------------------------------------
void File_Dpx::Streams_Finish()
{
}

//***************************************************************************
// Buffer - Demux
//***************************************************************************

//---------------------------------------------------------------------------
#if MEDIAINFO_DEMUX
bool File_Dpx::Demux_UnpacketizeContainer_Test()
{
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
bool File_Dpx::FileHeader_Begin()
{
    //Element_Size
    if (Buffer_Size<4)
        return false; //Must wait for more data

    int32u Magic=CC4(Buffer);
    switch (Magic)
    {
        case 0x802A5FD7 :   //       (v1 Big)
        case 0xD75F2A80 :   //       (v1 Little)
        case 0x53445058 :   //"SPDX" (v2 Big)
        case 0x58504453 :   //"XDPS" (v2 Little)
                            break;
        default         :
                            Reject();
                            return false;
    }

    //Generic Section size
    if (Buffer_Size<28)
        return false; //Must wait for more data
    Sizes_Pos=Pos_GenericSection;
    switch (Magic)
    {
        case 0x802A5FD7 :   //       (v1 Big)
        case 0xD75F2A80 :   //       (v1 Little)
                            Version=1;
                            break;
        case 0x58504453 :   //"XDPS" (v2 Little)
        case 0x53445058 :   //"SPDX" (v2 Big)
                            Version=2;
                            break;
        default         :   ;
    }
    switch (Magic)
    {
        case 0xD75F2A80 :   //       (v1 Little)
        case 0x58504453 :   //"XDPS" (v2 Little)
                            LittleEndian=true;
                            Sizes.push_back(LittleEndian2int32u(Buffer+24));
                            break;
        case 0x802A5FD7 :   //       (v1 Big)
        case 0x53445058 :   //"SPDX" (v2 Big)
                            LittleEndian=false;
                            Sizes.push_back(BigEndian2int32u(Buffer+24));
                            break;
        default         :   ;
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

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dpx::Header_Parse()
{
    //Filling
    Header_Fill_Code(Sizes_Pos); //We use Sizes_Pos as the unique key
    Header_Fill_Size(Sizes[Sizes_Pos]);
}

//---------------------------------------------------------------------------
void File_Dpx::Data_Parse()
{
    Sizes_Pos++; //We go automaticly to the next block

    if (Version==1)
    {
        switch (Element_Code)
        {
            case Pos_GenericSection   : GenericSectionHeader_v1(); break;
            case Pos_IndustrySpecific : IndustrySpecificHeader_v1(); break;
            case Pos_UserDefined      : UserDefinedHeader_v1(); break;
            default                   : ;
        }
    }
    else
    {
        switch (Element_Code)
        {
            case Pos_GenericSection   : GenericSectionHeader_v2(); break;
            case Pos_IndustrySpecific : IndustrySpecificHeader_v2(); break;
            case Pos_UserDefined      : UserDefinedHeader_v2(); break;
            default                   : ;
        }
    }

    //Special cases
    bool ShouldParseMore=false;
    for (size_t Pos=Pos_Padding-1; Pos>(size_t)Element_Code; Pos--) //Start from padding position (excluded), finish at current element
        if (Sizes[Pos])
            ShouldParseMore=true; //More blocks are interesting
    if (!ShouldParseMore)
    {
        //Not related to this block, but we have not more intersting stuff, simply skip the rest
        Element_End(); //This is not in this block, but the next one
        //if (Sizes[Pos_Padding])
        //    Skip_XX(Sizes[Pos_Padding],                         "Padding");
        //Skip_XX(Sizes[Pos_ImageData],                           "Image data");

        if (Frame_Count_NotParsedIncluded!=(int64u)-1)
            Frame_Count_NotParsedIncluded++;
        GoToFromEnd(0);
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dpx::GenericSectionHeader_v1()
{
    Element_Name("Generic section header");

    //Parsing
    Element_Begin("File information");
    Ztring CreationDate, CreationTime;
    int32u Size_Header, Size_Total, Size_Generic, Size_Industry, Size_User;
    Skip_B4(                                                    "Magic number");
    Get_X4 (Size_Header,                                        "Offset to image data");
    Get_X4 (Size_Generic,                                       "Generic section header length");
    Get_X4 (Size_Industry,                                      "Industry specific header length");
    Get_X4 (Size_User,                                          "User-defined header length");
    Get_X4 (Size_Total,                                         "Total image file size");
    Skip_String(8,                                              "Version number of header format");
    Skip_UTF8  (100,                                            "FileName");
    Get_UTF8   (12,  CreationDate,                              "Creation Date");
    Get_UTF8   (12,  CreationTime,                              "Creation Time");
    Skip_XX(36,                                                 "Reserved for future use");
    Element_End();

    Element_Begin("Image information");
    int8u ImageElements;
    Info_B1(ImageOrientation,                                   "Image orientation"); Param_Info(DPX_Orientation[ImageOrientation>8?8:ImageOrientation]);
    Get_B1 (ImageElements,                                      "Number of image elements");
    Skip_B2(                                                    "Unused");
    if (ImageElements>8)
        ImageElements=8;
    for(int8u ImageElement=0; ImageElement<ImageElements; ImageElement++)
        GenericSectionHeader_v1_ImageElement();
    if (ImageElements!=8)
        Skip_XX((8-ImageElements)*28,                           "Padding");
    Skip_BFP4(9,                                                "White point - x");
    Skip_BFP4(9,                                                "White point - y");
    Skip_BFP4(9,                                                "Red primary chromaticity - x");
    Skip_BFP4(9,                                                "Red primary chromaticity - u");
    Skip_BFP4(9,                                                "Green primary chromaticity - x");
    Skip_BFP4(9,                                                "Green primary chromaticity - y");
    Skip_BFP4(9,                                                "Blue primary chromaticity - x");
    Skip_BFP4(9,                                                "Blue primary chromaticity - y");
    Skip_UTF8(200,                                              "Label text");
    Skip_XX(28,                                                 "Reserved for future use");
    Element_End();

    Element_Begin("Image Data Format Information");
    Skip_B1(                                                    "Data interleave");
    Skip_B1(                                                    "Packing");
    Skip_B1(                                                    "Data signed or unsigned");
    Skip_B1(                                                    "Image sense");
    Skip_B4(                                                    "End of line padding");
    Skip_B4(                                                    "End of channel padding");
    Skip_XX(20,                                                 "Reserved for future use");

    Element_Begin("Image Origination Information");
    Skip_B4(                                                    "X offset");
    Skip_B4(                                                    "Y offset");
    Skip_UTF8  (100,                                            "FileName");
    Get_UTF8   (12,  CreationDate,                              "Creation Date");
    Get_UTF8   (12,  CreationTime,                              "Creation Time");
    Skip_UTF8(64,                                               "Input device");
    Skip_UTF8(32,                                               "Input device model number");
    Skip_UTF8(32,                                               "Input device serial number");
    Skip_BFP4(9,                                                "X input device pitch");
    Skip_BFP4(9,                                                "Y input device pitch");
    Skip_BFP4(9,                                                "Image gamma of capture device");
    Skip_XX(40,                                                 "Reserved for future use");
    Element_End();

    FILLING_BEGIN();
        //Coherency tests
        if (Size_Generic+Size_Industry+Size_User>Size_Header || Size_Header>Size_Total)
        {
            Reject();
            return;
        }

        //Filling sizes
        Sizes.push_back(Size_Industry);
        Sizes.push_back(Size_User);
        Sizes.push_back(Size_Header-(Size_Generic+Size_Industry+Size_User)); //Size of padding
        Sizes.push_back(Size_Total-Size_Header); //Size of image

        //Filling meta
        Fill(Stream_General, 0, General_Encoded_Date, CreationDate+_T(' ')+CreationTime); //ToDo: transform it in UTC
        Fill(Stream_Image, 0, Image_Encoded_Date, CreationDate+_T(' ')+CreationTime); //ToDo: transform it in UTC
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dpx::GenericSectionHeader_v1_ImageElement()
{
    Element_Begin("image element");
    int32u Width, Height;
    Skip_B1(                                                    "Designator - Byte 0");
    Skip_B1(                                                    "Designator - Byte 1");
    Skip_B1(                                                    "Bits per pixel");
    Skip_B1(                                                    "Unused");
    Get_X4 (Width,                                              "Pixels per line");
    Get_X4 (Height,                                             "Lines per image element");
    Skip_BFP4(9,                                                "Minimum data value");
    Skip_BFP4(9,                                                "Minimum quantity represented");
    Skip_BFP4(9,                                                "Maximum data value");
    Skip_BFP4(9,                                                "Maximum quantity represented");
    Element_End();

    FILLING_BEGIN();
        if (Count_Get(Stream_Image)==0)
        {
            Stream_Prepare(Stream_Image);
            Fill(Stream_Image, StreamPos_Last, Image_Format, "DPX");
            Fill(Stream_Image, StreamPos_Last, Image_Format_Version, "Version 1");
            Fill(Stream_Image, StreamPos_Last, Image_Width, Width);
            Fill(Stream_Image, StreamPos_Last, Image_Height, Height);
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dpx::GenericSectionHeader_v2()
{
    Element_Name("Generic section header");

    //Parsing
    Element_Begin("File information");
    std::string CreationDate, Creator, Project, Copyright; 
    int32u Size_Header, Size_Total, Size_Generic, Size_Industry, Size_User;
    Skip_String(4,                                              "Magic number");
    Get_X4 (Size_Header,                                        "Offset to image data");
    Skip_String(8,                                              "Version number of header format");
    Get_X4 (Size_Total,                                         "Total image file size");
    Skip_B4(                                                    "Ditto Key");
    Get_X4 (Size_Generic,                                       "Generic section header length");
    Get_X4 (Size_Industry,                                      "Industry specific header length");
    Get_X4 (Size_User,                                          "User-defined header length");
    Skip_UTF8  (100,                                            "FileName");
    Get_String (24,  CreationDate,                              "Creation Date");
    Get_String (100, Creator,                                   "Creator");
    Get_String (200, Project,                                   "Project");
    Get_String (200, Copyright,                                 "Right to use or copyright statement");
    Skip_B4(                                                    "Encryption key");
    Skip_XX(104,                                                "Reserved for future use");
    Element_End();

    Element_Begin("Image information");
    int32u Width, Height, PAR_H, PAR_V;
    int16u ImageElements;
    Info_B2(ImageOrientation,                                   "Image orientation");Param_Info(DPX_Orientation[ImageOrientation]);
    Get_X2 (ImageElements,                                      "Number of image elements");
    if (ImageElements>8)
        ImageElements=8;
    Get_X4 (Width,                                              "Pixels per line");
    Get_X4 (Height,                                             "Lines per image element");
    for(int16u ImageElement=0; ImageElement<ImageElements; ImageElement++)
        GenericSectionHeader_v2_ImageElement();
    if (ImageElements!=8)
        Skip_XX((8-ImageElements)*72,                           "Padding");
    Skip_XX(52,                                                 "Reserved for future use");
    Element_End();

    Element_Begin("Image source information");
    Skip_B4(                                                    "X Offset");
    Skip_B4(                                                    "Y Offset");
    Skip_BFP4(9,                                                "X center");
    Skip_BFP4(9,                                                "Y center");
    Skip_B4(                                                    "X original size");
    Skip_B4(                                                    "Y original size");
    Skip_UTF8(100,                                              "Source image filename");
    Skip_UTF8(24,                                               "Source image date/time");
    Skip_UTF8(32,                                               "Input device name");
    Skip_UTF8(32,                                               "Input device serial number");
    Element_Begin("Border validity");
    Skip_B2(                                                    "XL border");
    Skip_B2(                                                    "XR border");
    Skip_B2(                                                    "YT border");
    Skip_B2(                                                    "YB border");
    Element_End();
    Get_X4 (PAR_H,                                              "Pixel ratio : horizontal");
    Get_X4 (PAR_V,                                              "Pixel ratio : vertical");
    
    Element_Begin("Additional source image information");
    Skip_BFP4(9,                                                "X scanned size");
    Skip_BFP4(9,                                                "Y scanned size");
    Skip_XX(20,                                                 "Reserved for future use");
    Element_End();

    FILLING_BEGIN();
        //Coherency tests
        if (Size_Generic+Size_Industry+Size_User>Size_Header || Size_Header>Size_Total)
        {
            Reject();
            return;
        }

        //Filling sizes
        Sizes.push_back(Size_Industry);
        Sizes.push_back(Size_User);
        Sizes.push_back(Size_Header-(Size_Generic+Size_Industry+Size_User)); //Size of padding
        Sizes.push_back(Size_Total-Size_Header); //Size of image

        //Filling meta
        Fill(Stream_General, 0, General_Encoded_Date, CreationDate); //ToDo: transform it in UTC
        Fill(Stream_Image, 0, Image_Encoded_Date, CreationDate); //ToDo: transform it in UTC
        Fill(Stream_General, 0, General_Encoded_Library, Creator);
        Fill(Stream_Image, 0, Image_Encoded_Library, Creator);
        Fill(Stream_General, 0, "Project", Project); //ToDo: map to a MediaInfo field (which one?)
        Fill(Stream_General, 0, General_Copyright, Copyright);

        for (size_t StreamPos=0; StreamPos<Count_Get(Stream_Image); StreamPos++) //This is for all images
        {
            Fill(Stream_Image, StreamPos, Image_Width, Width);
            Fill(Stream_Image, StreamPos, Image_Height, Height);
            //if (PAR_V) //ToDo: add aspect ratio field for images
                //Fill(Stream_Image, StreamPos, Image_PixelAspectRatio, ((float)PAR_H)/PAR_V);
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dpx::GenericSectionHeader_v2_ImageElement()
{
    Element_Begin("image element");
    Info_B4(DataSign,                                           "Data sign");Param_Info((DataSign==0?"unsigned":"signed"));
    Skip_B4(                                                    "Reference low data code value");
    Skip_BFP4(9,                                                "Reference low quantity represented");
    Skip_B4(                                                    "Reference high data code value");
    Skip_BFP4(9,                                                "Reference high quantity represented");
    Info_B1(Descriptor,                                         "Descriptor");Param_Info(DPX_Descriptors(Descriptor));
    Info_B1(TransferCharacteristic,                             "Transfer characteristic");Param_Info(DPX_TransferCharacteristic[TransferCharacteristic]);
    Info_B1(ColorimetricSpecification,                          "Colorimetric specification");Param_Info(DPX_ColorimetricSpecification[ColorimetricSpecification]);
    Info_B1(BitDephs,                                           "Bit depth");Param_Info(DPX_ValidBitDephs(BitDephs));
    Info_B2(ComponentDataPackingMethod,                         "Packing");Param_Info((ComponentDataPackingMethod<8?DPX_ComponentDataPackingMethod[ComponentDataPackingMethod]:"invalid"));
    Info_B2(ComponentDataEncodingMethod,                        "Encoding");Param_Info((ComponentDataEncodingMethod<8?DPX_ComponentDataEncodingMethod[ComponentDataEncodingMethod]:"invalid"));
    Skip_B4(                                                    "Offset to data");
    Skip_B4(                                                    "End-of-line padding");
    Skip_B4(                                                    "End-of-image padding");
    Skip_UTF8(32,                                               "Description of image element");
    Element_End();

    FILLING_BEGIN();
        Stream_Prepare(Stream_Image);
        Fill(Stream_Image, StreamPos_Last, Image_Format, "DPX");
        Fill(Stream_Image, StreamPos_Last, Image_Format_Version, "Version 2");
        Fill(Stream_Image, StreamPos_Last, Image_BitDepth, BitDephs);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dpx::IndustrySpecificHeader_v1()
{
    Element_Name("Motion picture industry specific header");

    //Parsing
    Element_Begin("Motion-picture film information");
    Skip_B1(                                                    "?");
    Skip_B1(                                                    "?");
    Skip_B1(                                                    "?");
    Skip_B1(                                                    "?");
    Skip_B4(                                                    "?");
    Skip_B4(                                                    "?");
    Skip_UTF8(32,                                               "?");
    Skip_B4(                                                    "?");
    Skip_B4(                                                    "?");
    Skip_UTF8(32,                                               "?");
    Skip_UTF8(200,                                              "?");
    Skip_XX(740,                                                "Reserved for future use");
    Element_End();
}

//---------------------------------------------------------------------------
void File_Dpx::IndustrySpecificHeader_v2()
{
    Element_Name("Industry specific header");

    //Parsing
    Element_Begin("Motion-picture film information");
    Skip_String(2,                                              "Film mfg. ID code");
    Skip_String(2,                                              "Film type");
    Skip_String(2,                                              "Offset in perfs");
    Skip_String(6,                                              "Prefix");
    Skip_String(4,                                              "Count");
    Skip_String(32,                                             "Format - e.g. Academy");
    Skip_B4(                                                    "Frame position in sequence");
    Skip_B4(                                                    "Sequence length (frames)");
    Skip_B4(                                                    "Held count (1 = default)");
    Skip_BFP4(9,                                                "Frame rate of original (frames/s)");
    Skip_BFP4(9,                                                "Shutter angle of camera in degrees");
    Skip_UTF8(32,                                               "Frame identification - e.g. keyframe");
    Skip_UTF8(100,                                              "Slate information");
    Skip_XX(56,                                                 "Reserved for future use");
    Element_End();
    
    Element_Begin("Television information");
    Skip_B4(                                                    "SMPTE time code");
    Skip_B4(                                                    "SMPTE user bits");
    Info_B1(Interlace,                                          "Interlace");Param_Info((Interlace==0?"noninterlaced":"2:1 interlace"));
    Skip_B1(                                                    "Field number");
    Info_B1(VideoSignalStandard,                                "Video signal standard");Param_Info(DPX_VideoSignalStandard(VideoSignalStandard));
    Skip_B1(                                                    "Zero");
    Skip_BFP4(9,                                                "Horizontal sampling rate (Hz)");
    Skip_BFP4(9,                                                "Vertical sampling rate (Hz)");
    Skip_BFP4(9,                                                "Temporal sampling rate or frame rate (Hz)");
    Skip_BFP4(9,                                                "Time offset from sync to first pixel (ms)");
    Skip_BFP4(9,                                                "Gamma");
    Skip_BFP4(9,                                                "Black level code value");
    Skip_BFP4(9,                                                "Black gain");
    Skip_BFP4(9,                                                "Breakpoint");
    Skip_BFP4(9,                                                "Reference white level code value");
    Skip_BFP4(9,                                                "Integration time (s)");
    Skip_XX(76,                                                 "Reserved for future use");
    Element_End();
}

//---------------------------------------------------------------------------
void File_Dpx::UserDefinedHeader_v1()
{
    Element_Name("User defined header");

    //Parsing
    Skip_XX(Sizes[Pos_UserDefined],                             "Unknown");
}

//---------------------------------------------------------------------------
void File_Dpx::UserDefinedHeader_v2()
{
    Element_Name("User defined header");

    //Parsing
    if (Sizes[Pos_UserDefined]<32)
    {
        //Not in spec
        Skip_XX(Sizes[Pos_UserDefined],                         "Unknown");
        return;            
    }
    Skip_UTF8(32,                                               "User identification");
    Skip_XX(Sizes[Pos_UserDefined],                             "User defined");
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dpx::Get_X2(int16u &Info, const char* Name)
{
    if (LittleEndian)
        Get_L2 (Info,                                           Name);
    else
        Get_B2 (Info,                                           Name);
}

//---------------------------------------------------------------------------
void File_Dpx::Get_X4(int32u &Info, const char* Name)
{
    if (LittleEndian)
        Get_L4 (Info,                                           Name);
    else
        Get_B4 (Info,                                           Name);
}

} //NameSpace

#endif //MEDIAINFO_DPX_YES
