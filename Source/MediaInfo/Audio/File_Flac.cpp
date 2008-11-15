// File_Flac - Info for Flac files
// Copyright (C) 2003-2008 Jerome Martinez, zen@mediaarea.net
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
#if defined(MEDIAINFO_FLAC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Flac.h"
#include "MediaInfo/Tag/File_VorbisCom.h"
#include "ZenLib/Base64/base64.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Const
//***************************************************************************

//---------------------------------------------------------------------------
extern const char* Id3v2_PictureType(int8u Type); //In Tag/File_Id3v2.cpp

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Flac::File_Flac()
:File__Analyze(), File__Tags_Helper()
{
    //File__Tags_Helper
    Base=this;

    Last_metadata_block=false;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::Read_Buffer_Continue()
{
    //Tags
    if (!File__Tags_Helper::Read_Buffer_Continue())
        return;
}

//---------------------------------------------------------------------------
void File_Flac::Read_Buffer_Finalize()
{
    //Tags
    File__Tags_Helper::Read_Buffer_Finalize();
}

//---------------------------------------------------------------------------
bool File_Flac::FileHeader_Begin()
{
    if (!File__Tags_Helper::Header_Begin())
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Flac::FileHeader_Parse()
{
    //Parsing
    int32u Signature;
    Get_C4 (Signature,                                          "Signature");

    FILLING_BEGIN();
        //Integrity
        if (Signature!=CC4("fLaC"))
        {
            Finnished();
            return;
        }

        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "FLAC");
    FILLING_END();
}

//---------------------------------------------------------------------------
bool File_Flac::Header_Begin()
{
    if (!File__Tags_Helper::Header_Begin())
        return false;

    return true;
}

//---------------------------------------------------------------------------
void File_Flac::Header_Parse()
{
    //Parsing
    int32u Length;
    int8u BLOCK_TYPE;
    BS_Begin();
    Get_SB (   Last_metadata_block,                             "Last-metadata-block");
    Get_S1 (7, BLOCK_TYPE,                                      "BLOCK_TYPE");
    BS_End();
    Get_B3 (Length,                                             "Length");

    //Filling
    Header_Fill_Code(BLOCK_TYPE, Ztring().From_CC1(BLOCK_TYPE));
    Header_Fill_Size(Element_Offset+Length);
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::Data_Parse()
{
    #define ELEMENT_CASE(_NAME) \
        case Flac::_NAME : _NAME(); break;

    //Parsing
    switch (Element_Code)
    {
        ELEMENT_CASE(STREAMINFO);
        ELEMENT_CASE(PADDING);
        ELEMENT_CASE(APPLICATION);
        ELEMENT_CASE(SEEKTABLE);
        ELEMENT_CASE(VORBIS_COMMENT);
        ELEMENT_CASE(CUESHEET);
        ELEMENT_CASE(PICTURE);
        default : Skip_XX(Element_Size,                         "Data");
    }

    if (Last_metadata_block)
    {
        Fill(Stream_Audio, 0, Audio_StreamSize, File_Size-(File_Offset+Buffer_Offset+Element_Size));
        File__Tags_Helper::Data_GoTo(File_Size, "Flac");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::STREAMINFO()
{
    Element_Info("STREAMINFO");

    //Parsing
    int64u Samples;
    int32u FrameSize_Min, FrameSize_Max, SampleRate;
    int8u  Channels, BitPerSample;
    Skip_B2(                                                    "BlockSize_Min"); //The minimum block size (in samples) used in the stream.
    Skip_B2(                                                    "BlockSize_Max"); //The maximum block size (in samples) used in the stream. (Minimum blocksize == maximum blocksize) implies a fixed-blocksize stream.
    Get_B3 (    FrameSize_Min,                                  "FrameSize_Min"); //The minimum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
    Get_B3 (    FrameSize_Max,                                  "FrameSize_Max"); //The maximum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
    BS_Begin();
    Get_S3 (20, SampleRate,                                     "SampleRate"); //Sample rate in Hz. Though 20 bits are available, the maximum sample rate is limited by the structure of frame headers to 1048570Hz. Also, a value of 0 is invalid.
    Get_S1 ( 3, Channels,                                       "Channels"); Param_Info(Channels+1, " channels"); //(number of channels)-1. FLAC supports from 1 to 8 channels
    Get_S1 ( 5, BitPerSample,                                   "BitPerSample"); Param_Info(BitPerSample+1, " bits"); //(bits per sample)-1. FLAC supports from 4 to 32 bits per sample. Currently the reference encoder and decoders only support up to 24 bits per sample.
    Get_S5 (36, Samples,                                        "Samples");
    BS_End();
    Skip_B16(                                                   "MD5 signature of the unencoded audio data");

    //Filling
    if (SampleRate==0)
        return;
    Stream_Prepare(Stream_Audio);
    Fill(Stream_Audio, 0, Audio_Format, "FLAC");
    Fill(Stream_Audio, 0, Audio_Codec, "FLAC");
    if (FrameSize_Min==FrameSize_Max && FrameSize_Min!=0 ) // 0 means it is unknown
        Fill(Stream_Audio, 0, Audio_BitRate_Mode, "CBR");
     else
        Fill(Stream_Audio, 0, Audio_BitRate_Mode, "VBR");
    Fill(Stream_Audio, 0, Audio_SamplingRate, SampleRate);
    Fill(Stream_Audio, 0, Audio_Channel_s_, Channels+1);
    Fill(Stream_Audio, 0, Audio_Resolution, BitPerSample+1);
    Fill(Stream_Audio, 0, Audio_Duration, Samples*1000/SampleRate);
}

//---------------------------------------------------------------------------
void File_Flac::PADDING()
{
    Element_Info("PADDING");

    //Parsing
    Skip_XX(Element_Size,                                       "Padding");
}

//---------------------------------------------------------------------------
void File_Flac::APPLICATION()
{
    Element_Info("APPLICATION");

    //Parsing
    Skip_C4(                                                    "Application");
    if (Element_Size>4)
        Skip_XX(Element_Size-4,                                 "(Application specific)");
}

//---------------------------------------------------------------------------
void File_Flac::SEEKTABLE()
{
    Element_Info("SEEKTABLE");

    //Parsing
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Flac::VORBIS_COMMENT()
{
    Element_Info("VORBIS_COMMENT");

    //Parsing
    File_VorbisCom VorbisCom;
    VorbisCom.StreamKind=Stream_Audio;
    Open_Buffer_Init(&VorbisCom, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(&VorbisCom, Buffer+Buffer_Offset, (size_t)Element_Size);
    Open_Buffer_Finalize(&VorbisCom);
    Merge(VorbisCom, Stream_General,  0, 0);
    Merge(VorbisCom, Stream_Audio,    0, 0);
    Merge(VorbisCom, Stream_Chapters, 0, 0);
}

//---------------------------------------------------------------------------
void File_Flac::CUESHEET()
{
    Element_Info("CUESHEET");

    //Parsing
    Skip_XX(Element_Size,                                       "Data");
}

//---------------------------------------------------------------------------
void File_Flac::PICTURE()
{
    Element_Info("PICTURE");

    //Parsing
    int32u PictureType, MimeType_Size, Description_Size, Data_Size;
    Ztring MimeType, Description;
    Get_B4 (PictureType,                                        "Picture type"); Element_Info(Id3v2_PictureType((int8u)PictureType));
    Get_B4 (MimeType_Size,                                      "MIME type size");
    Get_Local(MimeType_Size, MimeType,                          "MIME type");
    Get_B4 (Description_Size,                                   "Description size");
    Get_UTF8(Description_Size, Description,                     "Description");
    Skip_B4(                                                    "Width");
    Skip_B4(                                                    "Height");
    Skip_B4(                                                    "Color depth");
    Skip_B4(                                                    "Number of colors used");
    Get_B4 (Data_Size,                                          "Data size");
    if (Element_Offset+Data_Size>Element_Size)
        return; //There is a problem
    std::string Data_Raw((const char*)(Buffer+(size_t)(Buffer_Offset+Element_Offset)), Data_Size);
    std::string Data_Base64(Base64::encode(Data_Raw));

    //Filling
    Fill(Stream_General, 0, General_Cover, "Yes");
    Fill(Stream_General, 0, General_Cover_Description, Description);
    Fill(Stream_General, 0, General_Cover_Type, Id3v2_PictureType((int8u)PictureType));
    Fill(Stream_General, 0, General_Cover_Mime, MimeType);
    Fill(Stream_General, 0, General_Cover_Data, Data_Base64);
}

} //NameSpace

#endif //MEDIAINFO_FLAC_YES

