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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_MPEG4_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Mpeg4_Descriptors.h"
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_MPEG4_YES)
    #include "MediaInfo/Audio/File_Mpeg4_AudioSpecificConfig.h"
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
        default   : return "Reserved";
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpeg4_Descriptors::File_Mpeg4_Descriptors()
:File__Analyze()
{
    KindOfStream=Stream_Max;
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
    if (KindOfStream!=Stream_Max)
    {
        Stream_Prepare(KindOfStream);
        KindOfStream=Stream_Max; //Do it once
    }

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
    Skip_B1(                                                    "ODProfileLevelIndication");
    Skip_B1(                                                    "sceneProfileLevelIndication");
    Skip_B1(                                                    "audioProfileLevelIndication");
    Info_B1(VideoProfileLevel,                                  "visualProfileLevelIndication");
        #ifdef MEDIAInfo_MPEG4V_YES
            INFO(Mpeg4v_Profile_Level(VideoProfileLevel));
        #endif //MEDIAInfo_MPEG4V_YES
    Skip_B1(                                                    "graphicsProfileLevelIndication");

    FILLING_BEGIN();
        Element_ThisIsAList();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_03()
{
    //Parsing
    bool streamDependenceFlag, URL_Flag, OCRstreamFlag;
    Skip_B2(                                                    "ES_ID");
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
    int8u ObjectTypeId;
    Get_B1 (ObjectTypeId,                                       "objectTypeIndication");
    BS_Begin();
    Skip_S1(6,                                                  "streamType");
    Skip_SB(                                                    "upStream");
    Skip_SB(                                                    "reserved");
    BS_End();
    Get_B3 (bufferSizeDB,                                       "bufferSizeDB");
    Get_B4 (MaxBitrate,                                         "maxBitrate");
    Get_B4 (AvgBitrate,                                         "avgBitrate");

    FILLING_BEGIN();
        switch (ObjectTypeId)
        {
            case 0x01 : Fill("Codec", "System", Error, false, true); break;
            case 0x02 : Fill("Codec", "System Core", Error, false, true); break;
            case 0x20 : Fill("Codec", "MPEG-4V", Error, false, true); break;
            case 0x21 : Fill("Codec", "H264", Error, false, true); break;
            case 0x40 : Fill("Codec", "AAC", Error, false, true); break; //MPEG-4 AAC
            case 0x60 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Simple
            case 0x61 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Main
            case 0x62 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V SNR
            case 0x63 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Spatial
            case 0x64 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V High
            case 0x65 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V 4:2:2
            case 0x66 : Fill("Codec", "AAC", Error, false, true); break; //MPEG-2 AAC Main
            case 0x67 : Fill("Codec", "AAC", Error, false, true); break; //MPEG-2 AAC LC
            case 0x68 : Fill("Codec", "AAC", Error, false, true); break; //MPEG-2 AAC SSR
            case 0x69 : Fill("Codec", "MPEG-2A L3", Error, false, true); break;
            case 0x6A : Fill("Codec", "MPEG-1V", Error, false, true); break;
            case 0x6B : Fill("Codec", "MPEG-1A", Error, false, true); break;
            case 0x6C : Fill("Codec", "JPEG", Error, false, true); break;
            case 0xA0 : Fill("Codec", "EVRC", Error, false, true); Fill("SamplingRate", "8000"); Fill("Channel(s)", "1", 10, true); break;
            case 0xA1 : Fill("Codec", "SMV", Error, false, true); Fill("SamplingRate", "8000"); Fill("Channel(s)", "1", 10, true);  break;
            case 0xD1 : Fill("Codec", "EVRC", Error, false, true); Fill("SamplingRate", "8000"); Fill("Channel(s)", "1", 10, true);  break;
            case 0xD3 : Fill("Codec", "AC3", Error, false, true); break;
            case 0xD4 : Fill("Codec", "DTS", Error, false, true); break;
            case 0xDD : Fill("Codec", "Ogg", Error, false, true); break;
            case 0xDE : Fill("Codec", "Ogg", Error, false, true); break;
            case 0xE1 : Fill("Codec", "QCELP", Error, false, true); Fill("SamplingRate", "8000"); Fill("Channel(s)", "1", 10, true);  break;
            default: ;
        }
        Fill("Codec/CC", ObjectTypeId, 16, true);

        //Exception, TODO: find a better way to detect ALS
        if ((AvgBitrate>640000) && (ObjectTypeId==0x40))
            Fill("Codec", "ALS", Error, false, true);

        //Bitrate mode
        if (AvgBitrate>0
         && !(bufferSizeDB==AvgBitrate && bufferSizeDB==MaxBitrate && bufferSizeDB==0x1000)) //Some buggy data were found
        {
            Fill("BitRate_Nominal", AvgBitrate);
            if (MaxBitrate<=AvgBitrate*1.005)
                Fill("BitRate_Mode", "CBR");
            else
            {
                Fill("BitRate_Mode", "VBR");
                Fill("BitRate_Maximum", MaxBitrate);
            }
        }

        Element_ThisIsAList();
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4_Descriptors::Descriptor_05()
{
    //Creating the parser
    File__Analyze* MI=NULL;
         if (0);
    #if defined(MEDIAINFO_MPEG4V_YES)
    else if (Get(StreamKind_Last, StreamPos_Last, _T("Codec"))==_T("MPEG-4V"))
        MI=new File_Mpeg4v;
    #endif
    #if defined(MEDIAINFO_MPEG4_YES)
    else if (Get(StreamKind_Last, StreamPos_Last, _T("Codec"))==_T("AAC"))
        MI=new File_Mpeg4_AudioSpecificConfig;
    #endif
    #if defined(MEDIAINFO_MPEG4V_YES)
    else if (Get(StreamKind_Last, StreamPos_Last, _T("Codec")).empty() && Codec==_T("mp4v"))
        MI=new File_Mpeg4v;
    #endif
    else
        MI=new File__Analyze();

    //Parsing
    Open_Buffer_Init(MI, File_Offset+Buffer_Offset+Element_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(MI, Buffer+Buffer_Offset, (size_t)Element_Size);
    Open_Buffer_Finalize(MI);

    //Filling
    Merge(*MI, StreamKind_Last, 0, StreamPos_Last);
    delete MI; //MI=NULL;

    //Positionning
    Element_Offset=(size_t)Element_Size;
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
