// File_Mpeg4 - Info for MPEG-4 files
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
//
// Descriptors part
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
#ifdef MEDIAINFO_MPEG4_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Mpeg4_Descriptors.h"
#if defined(MEDIAINFO_OGG_YES)
    #include "MediaInfo/Multiple/File_Ogg.h"
#endif
#if defined(MEDIAINFO_MPEG4_YES)
    #include "MediaInfo/Audio/File_Mpeg4_AudioSpecificConfig.h"
#endif
#if defined(MEDIAINFO_AVC_YES)
    #include "MediaInfo/Video/File_Avc.h"
#endif
#if defined(MEDIAINFO_VC1_YES)
    #include "MediaInfo/Video/File_Vc1.h"
#endif
#if defined(MEDIAINFO_DIRAC_YES)
    #include "MediaInfo/Video/File_Dirac.h"
#endif
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_MPEGV_YES)
    #include "MediaInfo/Video/File_Mpegv.h"
#endif
#if defined(MEDIAINFO_JPEG_YES)
    #include "MediaInfo/Image/File_Jpeg.h"
#endif
#if defined(MEDIAINFO_PNG_YES)
    #include "MediaInfo/Image/File_Png.h"
#endif
#if defined(MEDIAINFO_AC3_YES)
    #include "MediaInfo/Audio/File_Ac3.h"
#endif
#if defined(MEDIAINFO_DTS_YES)
    #include "MediaInfo/Audio/File_Dts.h"
#endif
#if defined(MEDIAINFO_MPEGA_YES)
    #include "MediaInfo/Audio/File_Mpega.h"
#endif
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_MPEG4V_YES
    const char* Mpeg4v_Profile_Level(int32u Profile_Level);
#endif //MEDIAINFO_MPEG4V_YES
//---------------------------------------------------------------------------

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const char* Mpeg4_Descriptors_Predefined(int8u ID)
{
    switch (ID)
    {
        case 0x00 : return "Custom";
        case 0x01 : return "null SL packet header";
        case 0x02 : return "Reserved for use in MP4 files";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg4_Descriptors_ObjectTypeIndication(int8u ID)
{
    switch (ID)
    {
        case 0x01 : return "Systems ISO/IEC 14496-1";
        case 0x02 : return "Systems ISO/IEC 14496-1 (v2)";
        case 0x03 : return "Interaction Stream";
        case 0x05 : return "AFX Stream";
        case 0x06 : return "Font Data Stream";
        case 0x07 : return "Synthesized Texture Stream";
        case 0x08 : return "Streaming Text Stream";
        case 0x20 : return "Visual ISO/IEC 14496-2 (MPEG-4 Visual)";
        case 0x21 : return "Visual ISO/IEC 14496-10 (AVC)";
        case 0x22 : return "Parameter Sets for Visual ISO/IEC 14496-10 (AVC)";
        case 0x40 : return "Audio ISO/IEC 14496-3 (AAC)";
        case 0x60 : return "Visual ISO/IEC 13818-2 Simple Profile (MPEG Video)";
        case 0x61 : return "Visual ISO/IEC 13818-2 Main Profile (MPEG Video)";
        case 0x62 : return "Visual ISO/IEC 13818-2 SNR Profile (MPEG Video)";
        case 0x63 : return "Visual ISO/IEC 13818-2 Spatial Profile (MPEG Video)";
        case 0x64 : return "Visual ISO/IEC 13818-2 High Profile (MPEG Video)";
        case 0x65 : return "Visual ISO/IEC 13818-2 422 Profile (MPEG Video)";
        case 0x66 : return "Audio ISO/IEC 13818-7 Main Profile (AAC)";
        case 0x67 : return "Audio ISO/IEC 13818-7 LowComplexity Profile (AAC)";
        case 0x68 : return "Audio ISO/IEC 13818-7 Scaleable Sampling Rate Profile (AAC)";
        case 0x69 : return "Audio ISO/IEC 13818-3 (MPEG Audio)";
        case 0x6A : return "Visual ISO/IEC 11172-2 (MPEG Video)";
        case 0x6B : return "Audio ISO/IEC 11172-3 (MPEG Audio)";
        case 0x6C : return "Visual ISO/IEC 10918-1 (M-JPEG)";
        case 0x6D : return "PNG";
        case 0xA0 : return "EVRC";
        case 0xA1 : return "SMV";
        case 0xA2 : return "3GPP2 Compact Multimedia Format (CMF)";
        case 0xA3 : return "VC-1";
        case 0xA4 : return "Dirac";
        case 0xA5 : return "AC-3";
        case 0xA6 : return "E-AC-3";
        case 0xD1 : return "Private - EVRC";
        case 0xD3 : return "Private - AC-3";
        case 0xD4 : return "Private - DTS";
        case 0xDD : return "Private - Ogg";
        case 0xDE : return "Private - Ogg";
        case 0xE1 : return "Private - QCELP";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg4_Descriptors_StreamType(int8u ID)
{
    switch (ID)
    {
        case 0x01 : return "ObjectDescriptorStream";
        case 0x02 : return "ClockReferenceStream";
        case 0x03 : return "SceneDescriptionStream";
        case 0x04 : return "VisualStream";
        case 0x05 : return "AudioStream";
        case 0x06 : return "MPEG7Stream";
        case 0x07 : return "IPMPStream";
        case 0x08 : return "ObjectContentInfoStream";
        case 0x09 : return "MPEGJStream";
        case 0x0A : return "Interaction Stream";
        case 0x0B : return "IPMPToolStream";
        case 0x0C : return "FontDataStream";
        case 0x0D : return "StreamingText";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg4_Descriptors_ODProfileLevelIndication(int8u ID)
{
    switch (ID)
    {
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg4_Descriptors_SceneProfileLevelIndication(int8u ID)
{
    switch (ID)
    {
        case    1 : return "Simple2D@L1";
        case    2 : return "Simple2D@L2";
        case   11 : return "Basic2D@L1";
        case   12 : return "Core2D@L1";
        case   13 : return "Core2D@L2";
        case   14 : return "Advanced2D@L1";
        case   15 : return "Advanced2D@L2";
        case   16 : return "Advanced2D@L3";
        case   17 : return "Main2D@L1";
        case   18 : return "Main2D@L2";
        case   19 : return "Main2D@L3";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg4_Descriptors_AudioProfileLevelIndication(int8u ID)
{
    switch (ID)
    {
        case    1 : return "Main@L1";
        case    2 : return "Main@L2";
        case    3 : return "Main@L3";
        case    4 : return "Main@L4";
        case    5 : return "Scalable@L1";
        case    6 : return "Scalable@L2";
        case    7 : return "Scalable@L3";
        case    8 : return "Scalable@L4";
        case    9 : return "Speech@L1";
        case   10 : return "Speech@L2";
        case   11 : return "Synthesis@L1";
        case   12 : return "Synthesis@L2";
        case   13 : return "Synthesis@L3";
        case   14 : return "HighQualityAudio@L1";
        case   15 : return "HighQualityAudio@L2";
        case   16 : return "HighQualityAudio@L3";
        case   17 : return "HighQualityAudio@L4";
        case   18 : return "HighQualityAudio@L5";
        case   19 : return "HighQualityAudio@L6";
        case   20 : return "HighQualityAudio@L7";
        case   21 : return "HighQualityAudio@L8";
        case   22 : return "LowDelayAudio@L1";
        case   23 : return "LowDelayAudio@L2";
        case   24 : return "LowDelayAudio@L3";
        case   25 : return "LowDelayAudio@L4";
        case   26 : return "LowDelayAudio@L5";
        case   27 : return "LowDelayAudio@L6";
        case   28 : return "LowDelayAudio@L7";
        case   29 : return "LowDelayAudio@L8";
        case   30 : return "NaturalAudio@L1";
        case   31 : return "NaturalAudio@L2";
        case   32 : return "NaturalAudio@L3";
        case   33 : return "NaturalAudio@L4";
        case   34 : return "MobileAudioInternetworking@L1";
        case   35 : return "MobileAudioInternetworking@L2";
        case   36 : return "MobileAudioInternetworking@L3";
        case   37 : return "MobileAudioInternetworking@L4";
        case   38 : return "MobileAudioInternetworking@L5";
        case   39 : return "MobileAudioInternetworking@L6";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg4_Descriptors_VisualProfileLevelIndication(int8u ID)
{
    switch (ID)
    {
        case    1 : return "Simple@L3";
        case    2 : return "Simple@L2";
        case    3 : return "Simple@L1";
        case    4 : return "SimpleScalable@L1";
        case    5 : return "SimpleScalable@L2";
        case    6 : return "Core@L2";
        case    7 : return "Core@L1";
        case    8 : return "Main@L4";
        case    9 : return "Main@L3";
        case   10 : return "Main@L2";
        case   11 : return "N-Bit@L2";
        case   12 : return "Hybrid@L2";
        case   13 : return "Hybrid@L1";
        case   14 : return "BasicAnimatedTexture@L2";
        case   15 : return "BasicAnimatedTexture@L1";
        case   16 : return "ScalableTexture@L3";
        case   17 : return "ScalableTexture@L2";
        case   18 : return "ScalableTexture@L1";
        case   19 : return "SimpleFaceAnimation@L2";
        case   20 : return "SimpleFaceAnimation@L1";
        case   21 : return "SimpleFBA@L2";
        case   22 : return "SimpleFBA@L1";
        case   23 : return "AdvancedRealTimeSimple@L4";
        case   24 : return "AdvancedRealTimeSimple@L3";
        case   25 : return "AdvancedRealTimeSimple@L2";
        case   26 : return "AdvancedRealTimeSimple@L1";
        case   27 : return "CoreScalable@L3";
        case   28 : return "CoreScalable@L2";
        case   29 : return "CoreScalable@L1";
        case   30 : return "AdvancedCodingEfficiency@L4";
        case   31 : return "AdvancedCodingEfficiency@L3";
        case   32 : return "AdvancedCodingEfficiency@L2";
        case   33 : return "AdvancedCoding@EfficiencyL1";
        case   34 : return "AdvanceCore@L2";
        case   35 : return "AdvanceCore@L1";
        case   36 : return "AdvancedScalableTexture@L3";
        case   37 : return "AdvancedScalableTexture@L2";
        case   38 : return "AdvancedScalableTexture@L1";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg4_Descriptors_GraphicsProfileLevelIndication(int8u ID)
{
    switch (ID)
    {
        case    1 : return "Simple2D@L1";
        case    2 : return "Simple2D+Text@L1";
        case    3 : return "Simple2D+Text@L2";
        case    4 : return "Core2D@L1";
        case    5 : return "Core2D@L2";
        case    6 : return "Advanced2D@L1";
        case    7 : return "Advanced2D@L2";
        default   : return "";
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpeg4_Descriptors::File_Mpeg4_Descriptors()
:File__Analyze()
{
    //In
    KindOfStream=Stream_Max;
    Parser_DoNotFreeIt=false;

    //Out
    Parser=NULL;
    ES_ID=0x0000;

    //Temp
    ObjectTypeId=0x00;
}

//---------------------------------------------------------------------------
File_Mpeg4_Descriptors::~File_Mpeg4_Descriptors()
{
    if (!Parser_DoNotFreeIt)
        delete Parser;// Parser=NULL;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Header_Parse()
{
    //Parsing
    size_t Size=0;
    int8u type, Size_ToAdd;
    Get_B1(type,                                            "type");
    do
    {
        Get_B1(Size_ToAdd,                                  "size");
        Size=(Size<<7) | (Size_ToAdd&0x7F);
    }
    while (Size_ToAdd&0x80);

    //Filling
    Header_Fill_Code(type, Ztring().From_CC1(type));
    Header_Fill_Size(Element_Offset+Size);
}


//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Data_Parse()
{
    //Preparing
    if (Count_Get(KindOfStream)==0)
        Stream_Prepare(KindOfStream);

    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case 0x##_NAME : Element_Name(_DETAIL); Descriptor_##_NAME(); break;

    //Parsing
    switch (Element_Code)
    {
        ELEMENT_CASE(00, "Forbidden");
        ELEMENT_CASE(01, "ObjectDescrTag");
        ELEMENT_CASE(02, "InitialObjectDescrTag");
        ELEMENT_CASE(03, "ES_DescrTag");
        ELEMENT_CASE(04, "DecoderConfigDescrTag");
        ELEMENT_CASE(05, "DecSpecificInfoTag");
        ELEMENT_CASE(06, "SLConfigDescrTag");
        ELEMENT_CASE(07, "ContentIdentDescrTag");
        ELEMENT_CASE(08, "SupplContentIdentDescrTag");
        ELEMENT_CASE(09, "IPI_DescrPointerTag");
        ELEMENT_CASE(0A, "IPMP_DescrPointerTag");
        ELEMENT_CASE(0B, "IPMP_DescrTag");
        ELEMENT_CASE(0C, "QoS_DescrTag");
        ELEMENT_CASE(0D, "RegistrationDescrTag");
        ELEMENT_CASE(0E, "ES_ID_IncTag");
        ELEMENT_CASE(0F, "ES_ID_RefTag");
        ELEMENT_CASE(10, "MP4_IOD_Tag");
        ELEMENT_CASE(11, "MP4_OD_Tag");
        ELEMENT_CASE(12, "IPL_DescrPointerRefTag");
        ELEMENT_CASE(13, "ExtendedProfileLevelDescrTag");
        ELEMENT_CASE(14, "profileLevelIndicationIndexDescrTag");
        ELEMENT_CASE(40, "ContentClassificationDescrTag");
        ELEMENT_CASE(41, "KeyWordDescrTag");
        ELEMENT_CASE(42, "RatingDescrTag");
        ELEMENT_CASE(43, "LanguageDescrTag");
        ELEMENT_CASE(44, "ShortTextualDescrTag");
        ELEMENT_CASE(45, "ExpandedTextualDescrTag");
        ELEMENT_CASE(46, "ContentCreatorNameDescrTag");
        ELEMENT_CASE(47, "ContentCreationDateDescrTag");
        ELEMENT_CASE(48, "OCICreatorNameDescrTag");
        ELEMENT_CASE(49, "OCICreationDateDescrTag");
        ELEMENT_CASE(4A, "SmpteCameraPositionDescrTag");
        ELEMENT_CASE(4B, "SegmentDescrTag");
        ELEMENT_CASE(4C, "MediaTimeDescrTag");
        ELEMENT_CASE(60, "IPMP_ToolsListDescrTag");
        ELEMENT_CASE(61, "IPMP_ToolTag");
        ELEMENT_CASE(62, "FLEXmuxTimingDescrTag");
        ELEMENT_CASE(63, "FLEXmuxCodeTableDescrTag");
        ELEMENT_CASE(64, "ExtSLConfigDescrTag");
        ELEMENT_CASE(65, "FLEXmuxBufferSizeDescrTag");
        ELEMENT_CASE(66, "FLEXmuxIdentDescrTag");
        ELEMENT_CASE(67, "DependencyPointerTag");
        ELEMENT_CASE(68, "DependencyMarkerTag");
        ELEMENT_CASE(69, "FLEXmuxChannelDescrTag");
        default: if (Element_Code>=0xC0)
                    Element_Info("user private");
                 else
                    Element_Info("unknown");
                 Skip_XX(Element_Size,                          "Data");
                 break;
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_02()
{
    //Parsing
    bool URL_Flag;
    BS_Begin();
    Skip_S2(10,                                                 "ObjectDescriptorID");
    Get_SB (    URL_Flag,                                       "URL_Flag");
    Skip_SB(                                                    "includeInlineProfileLevelFlag");
    Skip_S1( 4,                                                 "reserved");
    BS_End();
    if (URL_Flag)
    {
        int8u URLlength;
        Get_B1 (URLlength,                                      "URLlength");
        Skip_UTF8(URLlength,                                    "URLstring");
    }
    Info_B1(ODProfileLevel,                                     "ODProfileLevelIndication"); Param_Info(Mpeg4_Descriptors_ODProfileLevelIndication(ODProfileLevel));
    Info_B1(SceneProfileLevel,                                  "sceneProfileLevelIndication"); Param_Info(Mpeg4_Descriptors_SceneProfileLevelIndication(SceneProfileLevel));
    Info_B1(AudioProfileLevel,                                  "audioProfileLevelIndication"); Param_Info(Mpeg4_Descriptors_AudioProfileLevelIndication(AudioProfileLevel));
    Info_B1(VisualProfileLevel,                                 "visualProfileLevelIndication"); Param_Info(Mpeg4_Descriptors_VisualProfileLevelIndication(VisualProfileLevel));
    Info_B1(GraphicsProfileLevel,                               "graphicsProfileLevelIndication"); Param_Info(Mpeg4_Descriptors_GraphicsProfileLevelIndication(GraphicsProfileLevel));

    FILLING_BEGIN();
        Element_ThisIsAList();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_03()
{
    //Parsing
    bool streamDependenceFlag, URL_Flag, OCRstreamFlag;
    Get_B2 (ES_ID,                                              "ES_ID");
    BS_Begin();
    Get_SB (   streamDependenceFlag,                            "streamDependenceFlag");
    Get_SB (   URL_Flag,                                        "URL_Flag");
    Get_SB (   OCRstreamFlag,                                   "OCRstreamFlag");
    Skip_S1(5,                                                  "streamPriority");
    BS_End();
    if (streamDependenceFlag)
        Skip_B2(                                                "dependsOn_ES_ID");
    if (URL_Flag)
    {
        int8u URLlength;
        Get_B1 (URLlength,                                      "URLlength");
        Skip_UTF8(URLlength,                                    "URLstring");
    }
    if (OCRstreamFlag)
        Skip_B2(                                                "OCR_ES_Id");

    FILLING_BEGIN();
        Element_ThisIsAList();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_04()
{
    //Parsing
    int32u bufferSizeDB, MaxBitrate, AvgBitrate;
    Get_B1 (ObjectTypeId,                                       "objectTypeIndication"); Param_Info(Mpeg4_Descriptors_ObjectTypeIndication(ObjectTypeId));
    BS_Begin();
    Info_S1(6, streamType,                                      "streamType"); Param_Info(Mpeg4_Descriptors_StreamType(streamType));
    Skip_SB(                                                    "upStream");
    Skip_SB(                                                    "reserved");
    BS_End();
    Get_B3 (bufferSizeDB,                                       "bufferSizeDB");
    Get_B4 (MaxBitrate,                                         "maxBitrate");
    Get_B4 (AvgBitrate,                                         "avgBitrate");

    FILLING_BEGIN();
        switch (ObjectTypeId)
        {
            case 0x01 : Fill(StreamKind_Last, StreamPos_Last, "Format", "System", Error, false, true); break;
            case 0x02 : Fill(StreamKind_Last, StreamPos_Last, "Format", "System Core", Error, false, true); break;
            //case 0x03 Interaction Stream
            //case 0x05 AFX
            //case 0x06 Font Data
            //case 0x07 Synthesized Texture Stream
            case 0x08 : Fill(StreamKind_Last, StreamPos_Last, "Format", "Streaming Text", Error, false, true); break;
            case 0x20 : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG-4 Visual", Error, false, true); break;
            case 0x21 : Fill(StreamKind_Last, StreamPos_Last, "Format", "AVC", Error, false, true); break;
            //case 0x22 Parameter Sets for AVC
            case 0x40 : Fill(StreamKind_Last, StreamPos_Last, "Format", "AAC", Error, false, true); break; //MPEG-4 AAC
            case 0x60 : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Video", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "Simple" , Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 2", Error, false, true); break; //MPEG-2V Simple
            case 0x61 : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Video", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "Main"   , Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 2", Error, false, true); break; //MPEG-2V Main
            case 0x62 : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Video", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "SNR"    , Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 2", Error, false, true); break; //MPEG-2V SNR
            case 0x63 : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Video", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "Spatial", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 2", Error, false, true); break; //MPEG-2V Spatial
            case 0x64 : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Video", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "High"   , Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 2", Error, false, true); break; //MPEG-2V High
            case 0x65 : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Video", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "4:2:2"  , Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 2", Error, false, true); break; //MPEG-2V 4:2:2
            case 0x66 : Fill(StreamKind_Last, StreamPos_Last, "Format", "AAC", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "Main", Error, false, true); break; //MPEG-2 AAC Main
            case 0x67 : Fill(StreamKind_Last, StreamPos_Last, "Format", "AAC", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "LC", Error, false, true); break; //MPEG-2 AAC LC
            case 0x68 : Fill(StreamKind_Last, StreamPos_Last, "Format", "AAC", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "SSR", Error, false, true); break; //MPEG-2 AAC SSR
            case 0x69 : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Audio", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Profile", "Layer 3", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 2", Error, false, true); break;
            case 0x6A : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Video", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 1", Error, false, true); break;
            case 0x6B : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Audio", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "Format_Version", "Version 1", Error, false, true); break;
            case 0x6C : Fill(StreamKind_Last, StreamPos_Last, "Format", "M-JPEG", Error, false, true); break;
            case 0x6D : Fill(StreamKind_Last, StreamPos_Last, "Format", "PNG", Error, false, true); break;
            case 0x6E : Fill(StreamKind_Last, StreamPos_Last, "Format", "MPEG Video", Error, false, true); break;  
            case 0xA0 : Fill(StreamKind_Last, StreamPos_Last, "Format", "EVRC", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "SamplingRate", "8000"); Fill(StreamKind_Last, StreamPos_Last, "Channel(s)", "1", 10, true); break;
            case 0xA1 : Fill(StreamKind_Last, StreamPos_Last, "Format", "SMV", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "SamplingRate", "8000"); Fill(StreamKind_Last, StreamPos_Last, "Channel(s)", "1", 10, true);  break;
            case 0xA2 : Fill(StreamKind_Last, StreamPos_Last, "Format", "3GPP2", Error, false, true); break;
            case 0xA3 : Fill(StreamKind_Last, StreamPos_Last, "Format", "VC-1", Error, false, true); break;
            case 0xA4 : Fill(StreamKind_Last, StreamPos_Last, "Format", "Dirac", Error, false, true); break;
            case 0xA5 : Fill(StreamKind_Last, StreamPos_Last, "Format", "AC-3", Error, false, true); break;
            case 0xA6 : Fill(StreamKind_Last, StreamPos_Last, "Format", "E-AC-3", Error, false, true); break; 
            case 0xD1 : Fill(StreamKind_Last, StreamPos_Last, "Format", "EVRC", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "SamplingRate", "8000"); Fill(StreamKind_Last, StreamPos_Last, "Channel(s)", "1", 10, true);  break;
            case 0xD3 : Fill(StreamKind_Last, StreamPos_Last, "Format", "AC-3", Error, false, true); break;
            case 0xD4 : Fill(StreamKind_Last, StreamPos_Last, "Format", "DTS", Error, false, true); break;
            case 0xDD : Fill(StreamKind_Last, StreamPos_Last, "Format", "Ogg", Error, false, true); break;
            case 0xDE : Fill(StreamKind_Last, StreamPos_Last, "Format", "Ogg", Error, false, true); break;
            case 0xE1 : Fill(StreamKind_Last, StreamPos_Last, "Format", "QCELP", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "SamplingRate", "8000"); Fill(StreamKind_Last, StreamPos_Last, "Channel(s)", "1", 10, true);  break;
            default: ;
        }
        switch (ObjectTypeId)
        {
            case 0x01 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "System", Error, false, true); break;
            case 0x02 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "System Core", Error, false, true); break;
            case 0x20 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-4V", Error, false, true); break;
            case 0x21 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "H264", Error, false, true); break;
            case 0x40 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "AAC", Error, false, true); break; //MPEG-4 AAC
            case 0x60 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Simple
            case 0x61 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Main
            case 0x62 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V SNR
            case 0x63 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Spatial
            case 0x64 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V High
            case 0x65 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V 4:2:2
            case 0x66 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "AAC", Error, false, true); break; //MPEG-2 AAC Main
            case 0x67 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "AAC", Error, false, true); break; //MPEG-2 AAC LC
            case 0x68 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "AAC", Error, false, true); break; //MPEG-2 AAC SSR
            case 0x69 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-2A L3", Error, false, true); break;
            case 0x6A : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-1V", Error, false, true); break;
            case 0x6B : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-1A", Error, false, true); break;
            case 0x6C : Fill(StreamKind_Last, StreamPos_Last, "Codec", "M-JPEG", Error, false, true); break;
            case 0x6D : Fill(StreamKind_Last, StreamPos_Last, "Codec", "PNG", Error, false, true); break;
            case 0x6E : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-4V", Error, false, true); break; 
            case 0xA0 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "EVRC", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "SamplingRate", "8000"); Fill(StreamKind_Last, StreamPos_Last, "Channel(s)", "1", 10, true); break;
            case 0xA1 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "SMV", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "SamplingRate", "8000"); Fill(StreamKind_Last, StreamPos_Last, "Channel(s)", "1", 10, true);  break;
            case 0xA2 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "MPEG-4V", Error, false, true); break;
            case 0xA3 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "VC-1", Error, false, true); break;
            case 0xA4 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "Dirac", Error, false, true); break;
            case 0xA5 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "AC3", Error, false, true); break;
            case 0xA6 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "AC3+", Error, false, true); break;
            case 0xD1 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "EVRC", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "SamplingRate", "8000"); Fill(StreamKind_Last, StreamPos_Last, "Channel(s)", "1", 10, true);  break;
            case 0xD3 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "AC3", Error, false, true); break;
            case 0xD4 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "DTS", Error, false, true); break;
            case 0xDD : Fill(StreamKind_Last, StreamPos_Last, "Codec", "Ogg", Error, false, true); break;
            case 0xDE : Fill(StreamKind_Last, StreamPos_Last, "Codec", "Ogg", Error, false, true); break;
            case 0xE1 : Fill(StreamKind_Last, StreamPos_Last, "Codec", "QCELP", Error, false, true); Fill(StreamKind_Last, StreamPos_Last, "SamplingRate", "8000"); Fill(StreamKind_Last, StreamPos_Last, "Channel(s)", "1", 10, true);  break;
            default: ;
        }
        Fill(StreamKind_Last, StreamPos_Last, "CodecID", ObjectTypeId, 16, true);
        Fill(StreamKind_Last, StreamPos_Last, "Codec/CC", ObjectTypeId, 16, true);

        //Bitrate mode
        if (AvgBitrate>0
         && !(bufferSizeDB==AvgBitrate && bufferSizeDB==MaxBitrate && bufferSizeDB==0x1000)) //Some buggy data were found
        {
            Fill(StreamKind_Last, StreamPos_Last, "BitRate_Nominal", AvgBitrate);
            if (MaxBitrate<=AvgBitrate*1.005)
                Fill(StreamKind_Last, StreamPos_Last, "BitRate_Mode", "CBR");
            else
            {
                Fill(StreamKind_Last, StreamPos_Last, "BitRate_Mode", "VBR");
                Fill(StreamKind_Last, StreamPos_Last, "BitRate_Maximum", MaxBitrate);
            }
        }

        //Creating parser
        switch (ObjectTypeId)
        {
            case 0x20 : //MPEG-4 Visual
                        #if defined(MEDIAINFO_MPEG4V_YES)
                            Parser=new File_Mpeg4v;
                            ((File_Mpeg4v*)Parser)->Frame_Count_Valid=1;
                            ((File_Mpeg4v*)Parser)->FrameIsAlwaysComplete=true;
                        #endif
                        break;
            case 0x21 : //AVC
                        #if defined(MEDIAINFO_AVC_YES)
                            Parser=new File_Avc;
                            ((File_Avc*)Parser)->MustParse_SPS_PPS=true;
                            ((File_Avc*)Parser)->SizedBlocks=true;
                        #endif
                        break;
            case 0x40 :
                        #if defined(MEDIAINFO_MPEG4_YES)
                            Parser=new File_Mpeg4_AudioSpecificConfig;
                        #endif
                        break;
            case 0x60 :
            case 0x61 :
            case 0x62 :
            case 0x63 :
            case 0x64 :
            case 0x65 :
            case 0x6A : //MPEG Video
                        #if defined(MEDIAINFO_MPEGV_YES)
                            Parser=new File_Mpegv;
                            ((File_Mpegv*)Parser)->Frame_Count_Valid=30; //For searching Pulldown
                            ((File_Mpegv*)Parser)->FrameIsAlwaysComplete=true;
                        #endif
                        break;
            case 0x66 :
            case 0x67 :
            case 0x68 : //MPEG-2 AAC
                        #if defined(MEDIAINFO_MPEG4_YES)
                            Parser=new File_Mpeg4_AudioSpecificConfig; //Should be ADIF, but the only sample I have is AudioSpecificConfig 
                        #endif
                        break;
            case 0x69 :
            case 0x6B : //MPEG Audio
                        #if defined(MEDIAINFO_MPEGA_YES)
                            Parser=new File_Mpega;
                        #endif
                        break;
            case 0x6C : //M-JPEG
                        #if defined(MEDIAINFO_JPEG_YES)
                            Parser=new File_Jpeg;
                        #endif
                        break;
            case 0x6D : //PNG
                        #if defined(MEDIAINFO_PNG_YES)
                            Parser=new File_Png;
                        #endif
                        break;
            case 0xA3 : //VC-1
                        #if defined(MEDIAINFO_VC1_YES)
                            Parser=new File_Vc1; 
                        #endif
                        break;
            case 0xA4 : //Dirac
                        #if defined(MEDIAINFO_DIRAC_YES)
                            Parser=new File_Dirac;
                        #endif
                        break;
            case 0xA5 : //AC-3
            case 0xA6 : //E-AC-3
            case 0xD3 : //AC-3
                        #if defined(MEDIAINFO_AC3_YES)
                            Parser=new File_Ac3;
                        #endif
                        break;
            case 0xD4 : //DTS
                        #if defined(MEDIAINFO_DTS_YES)
                            Parser=new File_Dts;
                        #endif
                        break;
            case 0xDD :
            case 0xDE : //OGG
                        #if defined(MEDIAINFO_OGG_YES)
                            Parser=new File_Ogg;
                            ((File_Ogg*)Parser)->SizedBlocks=true;
                        #endif
                        break;
            default: ;
        }
        if (Parser)
            Open_Buffer_Init(Parser);

        Element_ThisIsAList();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_05()
{
    if (ObjectTypeId==0x00 && Parser==NULL) //If no ObjectTypeId detected
    {
        switch (KindOfStream)
        {
            case Stream_Video :
                                #if defined(MEDIAINFO_MPEG4V_YES)
                                    Parser=new File_Mpeg4v;
                                    ((File_Mpeg4v*)Parser)->Frame_Count_Valid=1;
                                    ((File_Mpeg4v*)Parser)->FrameIsAlwaysComplete=true;
                                #endif
                                break;
            case Stream_Audio :
                                #if defined(MEDIAINFO_MPEG4_YES)
                                    Parser=new File_Mpeg4_AudioSpecificConfig;
                                #endif
                                break;
            default: ;
        }
    }

    if (Parser==NULL)
    {
        Skip_XX(Element_Size,                                   "Unknown");
        return;
    }

    //Parsing
    Open_Buffer_Init(Parser, File_Offset+Buffer_Offset+Element_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(Parser, Buffer+Buffer_Offset, (size_t)Element_Size);
    if (!Parser_DoNotFreeIt
     || StreamKind_Last==Stream_Audio && Retrieve(Stream_Audio, StreamPos_Last, Audio_Format)==_T("AAC")) //File_Mpeg4_AudioSpecificConfig is only for DecConfig
    {
        Open_Buffer_Finalize(Parser);

        //Filling
        Merge(*Parser, StreamKind_Last, 0, StreamPos_Last);

        delete Parser; Parser=NULL;
    }

    //Positionning
    Element_Offset=Element_Size;
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_06()
{
    //Parsing
    int8u predefined, timeStampLength;
    bool durationFlag, useTimeStampsFlag;
    Get_B1 (predefined,                                         "predefined"); Param_Info(Mpeg4_Descriptors_Predefined(predefined));
    switch (predefined)
    {
        case 0x00 :
            {
                    BS_Begin();
                    Skip_SB(                                    "useAccessUnitStartFlag");
                    Skip_SB(                                    "useAccessUnitEndFlag");
                    Skip_SB(                                    "useRandomAccessPointFlag");
                    Skip_SB(                                    "hasRandomAccessUnitsOnlyFlag");
                    Skip_SB(                                    "usePaddingFlag");
                    Get_SB (useTimeStampsFlag,                  "useTimeStampsFlag");
                    Skip_SB(                                    "useIdleFlag");
                    Get_SB (durationFlag,                       "durationFlag");
                    BS_End();
                    Skip_B4(                                    "timeStampResolution");
                    Skip_B4(                                    "OCRResolution");
                    Get_B1 (timeStampLength,                    "timeStampLength");
                    Skip_B1(                                    "OCRLength");
                    Skip_B1(                                    "AU_Length");
                    Skip_B1(                                    "instantBitrateLength");
                    BS_Begin();
                    Skip_S1(4,                                  "degradationPriorityLength");
                    Skip_S1(5,                                  "AU_seqNumLength");
                    Skip_S1(5,                                  "packetSeqNumLength");
                    Skip_S1(2,                                  "reserved");
                    BS_End();
            }
            break;
        case 0x01 :
                    useTimeStampsFlag=false;
                    durationFlag=false;
                    timeStampLength=32;
                    break;
        case 0x02 :
                    useTimeStampsFlag=true;
                    durationFlag=false;
                    timeStampLength=32;
                    break;
        default   :
                    useTimeStampsFlag=false;
                    durationFlag=false;
                    timeStampLength=32;
    }
    if (durationFlag)
    {
        Skip_B4(                                                "timeScale");
        Skip_B2(                                                "accessUnitDuration");
        Skip_B2(                                                "compositionUnitDuration");
    }
    if (!useTimeStampsFlag)
    {
        BS_Begin();
        Skip_S8(timeStampLength,                                "startDecodingTimeStamp");
        Skip_S8(timeStampLength,                                "startCompositionTimeStamp");
        BS_End();
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_0E()
{
    //Parsing
    Skip_B4(                                                    "Track_ID"); //ID of the track to use
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_0F()
{
    //Parsing
    Skip_B2(                                                    "ref_index");  //track ref. index of the track to use
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_10()
{
    Descriptor_02();
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_11()
{
    Descriptor_01();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEG4_YES
