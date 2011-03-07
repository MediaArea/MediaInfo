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
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_DPX_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Image/File_Dpx.h"
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
void File_Dpx::Streams_Finish()
{
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Dpx::FileHeader_Begin()
{
    //Element_Size
    if (Buffer_Size<4)
        return false; //Must wait for more data

    if (CC4(Buffer)!=0x53445058) //"SPDX"
    {
        Reject();
        return false;
    }

    //All should be OK...
    Accept();
    Fill(Stream_General, 0, General_Format, "DPX");
    return true;
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dpx::Header_Parse()
{
    //There is no exact header, but a block order
    if (Sizes.empty())
    {
        Sizes.push_back(768); //Fixed size
        Sizes_Pos=Pos_FileInformation;
    }
    else
        Sizes_Pos++; //We go automaticly to the next block
        
    //Filling
    Header_Fill_Code(Sizes_Pos); //We use Sizes_Pos as the unique key
    Header_Fill_Size(Sizes[Sizes_Pos]);
}

//---------------------------------------------------------------------------
void File_Dpx::Data_Parse()
{
    switch (Element_Code)
    {
        case Pos_FileInformation :  FileInformationHeader();
                                    if (Sizes.size()<6) //Validity test of this block
                                        Reject();    
                                    break;
        case Pos_GenericSection   : GenericSectionHeader(); break;
        case Pos_IndustrySpecific : IndustrySpecificHeader(); break;
        case Pos_UserDefined      : UserDefinedHeader(); break;
        default                   : ;
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
        if (Sizes[Pos_Padding])
            Skip_XX(Sizes[Pos_Padding],                         "Padding");
        Skip_XX(Sizes[Pos_ImageData],                           "Image data");

        Finish();
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dpx::FileInformationHeader()
{
    Element_Name("File information");

    //Parsing
    std::string CreationDate, Creator, Project, Copyright; 
    int32u Size_Header, Size_Total, Size_Generic, Size_Industry, Size_User;
    Skip_String(4,                                              "Magic number");
    Get_B4 (Size_Header,                                        "Offset to image data");
    Skip_String(8,                                              "Version number of header format");
    Get_B4 (Size_Total,                                         "Total image file size");
    Skip_B4(                                                    "Ditto Key");
    Get_B4 (Size_Generic,                                       "Generic section header length");
    Get_B4 (Size_Industry,                                      "Industry specific header length");
    Get_B4 (Size_User,                                          "User-defined header length");
    Skip_String(100,                                            "FileName");
    Get_String (24,  CreationDate,                              "Creation Date");
    Get_String (100, Creator,                                   "Creator");
    Get_String (200, Project,                                   "Project");
    Get_String (200, Copyright,                                 "Right to use or copyright statement");
    Skip_B4(                                                    "Encryption key");
    Skip_XX(104,                                                "Reserved for future use");

    FILLING_BEGIN();
        //Coherency tests
        if (Size_Generic+Size_Industry+Size_User>Size_Header || Size_Header>Size_Total)
        {
            Reject();
            return;
        }

        //Filling sizes
        Sizes.push_back(Size_Generic);
        Sizes.push_back(Size_Industry);
        Sizes.push_back(Size_User);
        Sizes.push_back(Size_Header-(Size_Generic+Size_Industry+Size_User)); //Size of padding
        Sizes.push_back(Size_Total-Size_Header); //Size of image

        //Filling meta
        Fill(Stream_General, 0, General_Encoded_Date, CreationDate); //ToDo: transform it in UTC
        Fill(Stream_General, 0, General_Encoded_Library, Creator);
        Fill(Stream_General, 0, "Project", Project); //ToDo: map to a MediaInfo field (which one?)
        Fill(Stream_General, 0, General_Copyright, Copyright);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dpx::GenericSectionHeader()
{
    Element_Name("Generic section header");

    //Parsing
    Element_Begin("Image information");
    int32u Width, Height, PAR_H, PAR_V;
    int16u ImageElements;
    Info_B2(ImageOrientation,                                   "Image orientation");Param_Info(DPX_Orientation[ImageOrientation]);
    Get_B2 (ImageElements,                                      "Number of image elements");
    if (ImageElements>8)
        ImageElements=8;
    Get_B4 (Width,                                              "Pixels per line");
    Get_B4 (Height,                                             "Lines per image element");
    for(int16u ImageElement=0; ImageElement<ImageElements; ImageElement++)
        GenericSectionHeader_ImageElement();
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
    Skip_String(100,                                            "Source image filename");
    Skip_String(24,                                             "Source image date/time");
    Skip_String(32,                                             "Input device name");
    Skip_String(32,                                             "Input device serial number");
    Element_Begin("Border validity");
    Skip_B2(                                                    "XL border");
    Skip_B2(                                                    "XR border");
    Skip_B2(                                                    "YT border");
    Skip_B2(                                                    "YB border");
    Element_End();
    Get_B4 (PAR_H,                                              "Pixel ratio : horizontal");
    Get_B4 (PAR_V,                                              "Pixel ratio : vertical");
    
    Element_Begin("Additional source image information");
    Skip_BFP4(9,                                                "X scanned size");
    Skip_BFP4(9,                                                "Y scanned size");
    Skip_XX(20,                                                 "Reserved for future use");
    Element_End();

    FILLING_BEGIN();
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
void File_Dpx::GenericSectionHeader_ImageElement()
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
    Skip_String(32,                                             "Description of image element");
    Element_End();

    FILLING_BEGIN();
        Stream_Prepare(Stream_Image);
        Fill(Stream_Image, StreamPos_Last, Image_Format, "DPX");
        Fill(Stream_Image, StreamPos_Last, Image_ID, StreamPos_Last); //No specific ID
        Fill(Stream_Image, StreamPos_Last, Image_BitDepth, BitDephs);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dpx::IndustrySpecificHeader()
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
    Skip_String(32,                                             "Frame identification - e.g. keyframe");
    Skip_String(100,                                            "Slate information");
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
void File_Dpx::UserDefinedHeader()
{
    Element_Name("User defined header");

    //Parsing
    if (Sizes[Pos_UserDefined]<32)
    {
        //Not in spec
        Skip_XX(Sizes[Pos_UserDefined],                         "Unknown");
        return;            
    }
    Skip_String(32,                                             "User identification");
    Skip_XX(Sizes[Pos_UserDefined],                             "User defined");
}

} //NameSpace

#endif //MEDIAINFO_DPX_YES
