// File_Riff - Info for RIFF files
// Copyright (C) 2002-2009 Jerome Martinez, Zen@MediaArea.net
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
// Elements part
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
#ifdef MEDIAINFO_RIFF_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Riff.h"
#if defined(MEDIAINFO_OGG_YES)
    #include "MediaInfo/Multiple/File_Ogg.h"
    #include "MediaInfo/Multiple/File_Ogg_SubElement.h"
#endif
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_AVC_YES)
    #include "MediaInfo/Video/File_Avc.h"
#endif
#if defined(MEDIAINFO_MPEGA_YES)
    #include "MediaInfo/Audio/File_Mpega.h"
#endif
#if defined(MEDIAINFO_AC3_YES)
    #include "MediaInfo/Audio/File_Ac3.h"
#endif
#if defined(MEDIAINFO_DTS_YES)
    #include "MediaInfo/Audio/File_Dts.h"
#endif
#if defined(MEDIAINFO_MPEG4_YES)
    #include "MediaInfo/Audio/File_Mpeg4_AudioSpecificConfig.h"
#endif
#if defined(MEDIAINFO_ADTS_YES)
    #include "MediaInfo/Audio/File_Adts.h"
#endif
#if defined(MEDIAINFO_JPEG_YES)
    #include "MediaInfo/Image/File_Jpeg.h"
#endif
#if defined(MEDIAINFO_OTHERTEXT_YES)
    #include "MediaInfo/Text/File_OtherText.h"
#endif
#if defined(MEDIAINFO_ADPCM_YES)
    #include "MediaInfo/Audio/File_Adpcm.h"
#endif
#if defined(MEDIAINFO_PCM_YES)
    #include "MediaInfo/Audio/File_Pcm.h"
#endif
#if defined(MEDIAINFO_ID3_YES)
    #include "MediaInfo/Tag/File_Id3.h"
#endif
#if defined(MEDIAINFO_ID3V2_YES)
    #include "MediaInfo/Tag/File_Id3v2.h"
#endif
#if defined(MEDIAINFO_DVDIF_YES)
    #include "MediaInfo/Multiple/File_DvDif.h"
#endif
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
std::string ExtensibleWave_ChannelMask (int32u ChannelMask)
{
    std::string Text;
    if ((ChannelMask&0x0007)!=0x0000)
        Text+="Front:";
    if (ChannelMask&0x0001)
        Text+=" L";
    if (ChannelMask&0x0004)
        Text+=(ChannelMask&0x0001)?", C":" C";
    if (ChannelMask&0x0002)
        Text+=(ChannelMask&0x0003)?", R":" R";

    if ((ChannelMask&0x0030)!=0x0000)
        Text+="/";
    if (ChannelMask&0x0010)
        Text+=" L";
    if (ChannelMask&0x0020)
        Text+=(ChannelMask&0x0010)?", R":" R";

    if ((ChannelMask&0x0008)!=0x0000)
        Text+=", LFE";

    return Text;
}

//---------------------------------------------------------------------------
std::string ExtensibleWave_ChannelMask2 (int32u ChannelMask)
{
    std::string Text;
    int8u Count=0;
    if (ChannelMask&0x0001)
        Count++;
    if (ChannelMask&0x0004)
        Count++;
    if (ChannelMask&0x0002)
        Count++;
    if (Count)
    {
        Text+=Ztring::ToZtring(Count).To_UTF8();
        Count=0;
    }

    if (ChannelMask&0x0010)
        Count++;
    if (ChannelMask&0x0020)
        Count++;
    if (Count)
    {
        Text+="/"+Ztring::ToZtring(Count).To_UTF8();
        Count=0;
    }

    if (ChannelMask&0x0008)
        Text+=".1";

    return Text;
}

//***************************************************************************
// Const
//***************************************************************************

namespace Elements
{
    const int32u FORM=0x464F524D;
    const int32u LIST=0x4C495354;
    const int32u ON2_=0x4F4E3220;
    const int32u RIFF=0x52494646;
    const int32u RF64=0x52463634;

    const int32u AIFC=0x41494643;
    const int32u AIFC_COMM=0x434F4D4D;
    const int32u AIFC_COMT=0x434F4D54;
    const int32u AIFC_FVER=0x46564552;
    const int32u AIFC_SSND=0x53534E44;
    const int32u AIFF=0x41494646;
    const int32u AIFF_COMM=0x434F4D4D;
    const int32u AIFF_COMT=0x434F4D54;
    const int32u AIFF_SSND=0x53534E44;
    const int32u AIFF__c__=0x28632920;
    const int32u AIFF_ANNO=0x414E4E4F;
    const int32u AIFF_AUTH=0x41555448;
    const int32u AIFF_NAME=0x4E414D45;
    const int32u AVI_=0x41564920;
    const int32u AVI__cset=0x63736574;
    const int32u AVI__exif=0x65786966;
    const int32u AVI__exif_ecor=0x65636F72;
    const int32u AVI__exif_emdl=0x656D646C;
    const int32u AVI__exif_emnt=0x656D6E74;
    const int32u AVI__exif_erel=0x6572656C;
    const int32u AVI__exif_etim=0x6574696D;
    const int32u AVI__exif_eucm=0x6575636D;
    const int32u AVI__exif_ever=0x65766572;
    const int32u AVI__goog=0x676F6F67;
    const int32u AVI__goog_GDAT=0x47444154;
    const int32u AVI__GMET=0x474D4554;
    const int32u AVI__hdlr=0x6864726C;
    const int32u AVI__hdlr_avih=0x61766968;
    const int32u AVI__hdlr_JUNK=0x4A554E4B;
    const int32u AVI__hdlr_strl=0x7374726C;
    const int32u AVI__hdlr_strl_indx=0x696E6478;
    const int32u AVI__hdlr_strl_JUNK=0x4A554E4B;
    const int32u AVI__hdlr_strl_strd=0x73747264;
    const int32u AVI__hdlr_strl_strf=0x73747266;
    const int32u AVI__hdlr_strl_strh=0x73747268;
    const int32u AVI__hdlr_strl_strh_auds=0x61756473;
    const int32u AVI__hdlr_strl_strh_iavs=0x69617673;
    const int32u AVI__hdlr_strl_strh_mids=0x6D696473;
    const int32u AVI__hdlr_strl_strh_vids=0x76696473;
    const int32u AVI__hdlr_strl_strh_txts=0x74787473;
    const int32u AVI__hdlr_strl_strn=0x7374726E;
    const int32u AVI__hdlr_strl_vprp=0x76707270;
    const int32u AVI__hdlr_odml=0x6F646D6C;
    const int32u AVI__hdlr_odml_dmlh=0x646D6C68;
    const int32u AVI__hdlr_ON2h=0x4F4E3268;
    const int32u AVI__idx1=0x69647831;
    const int32u AVI__INFO=0x494E464F;
    const int32u AVI__INFO_IARL=0x4941524C;
    const int32u AVI__INFO_IART=0x49415254;
    const int32u AVI__INFO_IAS1=0x49415331;
    const int32u AVI__INFO_IAS2=0x49415332;
    const int32u AVI__INFO_IAS3=0x49415333;
    const int32u AVI__INFO_IAS4=0x49415334;
    const int32u AVI__INFO_IAS5=0x49415335;
    const int32u AVI__INFO_IAS6=0x49415336;
    const int32u AVI__INFO_IAS7=0x49415337;
    const int32u AVI__INFO_IAS8=0x49415338;
    const int32u AVI__INFO_IAS9=0x49415339;
    const int32u AVI__INFO_ICDS=0x49434453;
    const int32u AVI__INFO_ICMS=0x49434D53;
    const int32u AVI__INFO_ICMT=0x49434D54;
    const int32u AVI__INFO_ICNT=0x49434E54;
    const int32u AVI__INFO_ICOP=0x49434F50;
    const int32u AVI__INFO_ICNM=0x49434E4D;
    const int32u AVI__INFO_ICRD=0x49435244;
    const int32u AVI__INFO_ICRP=0x49435250;
    const int32u AVI__INFO_IDIM=0x4944494D;
    const int32u AVI__INFO_IDIT=0x49444954;
    const int32u AVI__INFO_IDPI=0x49445049;
    const int32u AVI__INFO_IDST=0x49445354;
    const int32u AVI__INFO_IEDT=0x49454454;
    const int32u AVI__INFO_IENG=0x49454E47;
    const int32u AVI__INFO_IFRM=0x4946524D;
    const int32u AVI__INFO_IGNR=0x49474E52;
    const int32u AVI__INFO_IID3=0x49494433;
    const int32u AVI__INFO_IKEY=0x494B4559;
    const int32u AVI__INFO_ILGT=0x494C4754;
    const int32u AVI__INFO_ILNG=0x494C4E47;
    const int32u AVI__INFO_ILYC=0x494C5943;
    const int32u AVI__INFO_IMED=0x494D4544;
    const int32u AVI__INFO_IMP3=0x494D5033;
    const int32u AVI__INFO_IMUS=0x494D5553;
    const int32u AVI__INFO_INAM=0x494E414D;
    const int32u AVI__INFO_IPLT=0x49504C54;
    const int32u AVI__INFO_IPDS=0x49504453;
    const int32u AVI__INFO_IPRD=0x49505244;
    const int32u AVI__INFO_IPRT=0x49505254;
    const int32u AVI__INFO_IPRO=0x4950524F;
    const int32u AVI__INFO_IRTD=0x49525444;
    const int32u AVI__INFO_ISBJ=0x4953424A;
    const int32u AVI__INFO_ISGN=0x4953474E;
    const int32u AVI__INFO_ISTD=0x49535444;
    const int32u AVI__INFO_ISTR=0x49535452;
    const int32u AVI__INFO_ISFT=0x49534654;
    const int32u AVI__INFO_ISHP=0x49534850;
    const int32u AVI__INFO_ISRC=0x49535243;
    const int32u AVI__INFO_ISRF=0x49535246;
    const int32u AVI__INFO_ITCH=0x49544348;
    const int32u AVI__INFO_IWEB=0x49574542;
    const int32u AVI__INFO_IWRI=0x49575249;
    const int32u AVI__INFO_JUNK=0x4A554E4B;
    const int32u AVI__JUNK=0x4A554E4B;
    const int32u AVI__movi=0x6D6F7669;
    const int32u AVI__movi_rec_=0x72656320;
    const int32u AVI__movi_xxxx_____=0x00005F5F;
    const int32u AVI__movi_xxxx___db=0x00006462;
    const int32u AVI__movi_xxxx___dc=0x00006463;
    const int32u AVI__movi_xxxx___sb=0x00007362;
    const int32u AVI__movi_xxxx___tx=0x00007478;
    const int32u AVI__movi_xxxx___wb=0x00007762;
    const int32u AVIX=0x41564958;
    const int32u AVIX_idx1=0x69647831;
    const int32u AVIX_movi=0x6D6F7669;
    const int32u AVIX_movi_rec_=0x72656320;
    const int32u CADP=0x43414450;
    const int32u CMJP=0x434D4A50;
    const int32u CMP4=0x434D5034;
    const int32u IDVX=0x49445658;
    const int32u JUNK=0x4A554E4B;
    const int32u menu=0x6D656E75;
    const int32u MThd=0x4D546864;
    const int32u MTrk=0x4D54726B;
    const int32u PAL_=0x50414C20;
    const int32u RDIB=0x52444942;
    const int32u RMID=0x524D4944;
    const int32u RMMP=0x524D4D50;
    const int32u RMP3=0x524D5033;
    const int32u RMP3_data=0x64617461;
    const int32u RMP3_INFO=0x494E464F;
    const int32u RMP3_INFO_IID3=0x49494433;
    const int32u RMP3_INFO_ILYC=0x494C5943;
    const int32u RMP3_INFO_IMP3=0x494D5033;
    const int32u RMP3_INFO_JUNK=0x4A554E4B;
    const int32u SMV0=0x534D5630;
    const int32u SMV0_xxxx=0x534D563A;
    const int32u WAVE=0x57415645;
    const int32u WAVE_bext=0x62657874;
    const int32u WAVE_data=0x64617461;
    const int32u WAVE_ds64=0x64733634;
    const int32u WAVE_fact=0x66616374;
    const int32u WAVE_fmt_=0x666D7420;
    const int32u WAVE_ID3_=0x49443320;
    const int32u W3DI=0x57334449;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Riff::Data_Parse()
{
    //alignement specific
    if (Alignement_ExtraByte)
        Element_Size--;

    DATA_BEGIN
    LIST(AIFC)
        ATOM_BEGIN
        ATOM(AIFC_COMM)
        ATOM(AIFC_COMT)
        ATOM(AIFC_FVER)
        LIST_SKIP(AIFC_SSND)
        ATOM_DEFAULT(AIFC_xxxx)
        ATOM_END_DEFAULT
    LIST(AIFF)
        ATOM_BEGIN
        ATOM(AIFF_COMM)
        ATOM(AIFF_COMT)
        LIST_SKIP(AIFF_SSND)
        ATOM_DEFAULT(AIFF_xxxx)
        ATOM_END_DEFAULT
    LIST(AVI_)
        ATOM_BEGIN
        ATOM(AVI__cset)
        LIST(AVI__exif)
            ATOM_DEFAULT_ALONE(AVI__exif_xxxx)
        LIST(AVI__goog)
            ATOM_BEGIN
            ATOM(AVI__goog_GDAT)
            ATOM_END
        ATOM(AVI__GMET)
        LIST(AVI__hdlr)
            ATOM_BEGIN
            ATOM(AVI__hdlr_avih)
            ATOM(AVI__hdlr_JUNK)
            LIST(AVI__hdlr_strl)
                ATOM_BEGIN
                ATOM(AVI__hdlr_strl_indx)
                ATOM(AVI__hdlr_strl_JUNK)
                ATOM(AVI__hdlr_strl_strd)
                ATOM(AVI__hdlr_strl_strf)
                ATOM(AVI__hdlr_strl_strh)
                ATOM(AVI__hdlr_strl_strn)
                ATOM(AVI__hdlr_strl_vprp)
                ATOM_END
            LIST(AVI__hdlr_odml)
                ATOM_BEGIN
                ATOM(AVI__hdlr_odml_dmlh)
                ATOM_END
            ATOM(AVI__hdlr_ON2h)
            ATOM_DEFAULT(AVI__hdlr_xxxx)
            ATOM_END_DEFAULT
        LIST_SKIP(AVI__idx1)
        LIST(AVI__INFO)
            ATOM_BEGIN
            ATOM(AVI__INFO_IID3)
            ATOM(AVI__INFO_ILYC)
            ATOM(AVI__INFO_IMP3)
            ATOM(AVI__INFO_JUNK)
            ATOM_DEFAULT(AVI__INFO_xxxx)
            ATOM_END_DEFAULT
        ATOM(AVI__JUNK)
        LIST(AVI__movi)
            ATOM_BEGIN
            LIST(AVI__movi_rec_)
                ATOM_DEFAULT_ALONE(AVI__movi_xxxx)
            ATOM_DEFAULT(AVI__movi_xxxx)
            ATOM_END_DEFAULT
        ATOM_DEFAULT(AVI__xxxx)
        ATOM_END_DEFAULT
    LIST(AVIX) //OpenDML
        ATOM_BEGIN
        ATOM(AVIX_idx1)
        LIST(AVIX_movi)
            ATOM_BEGIN
            LIST(AVIX_movi_rec_)
                ATOM_DEFAULT_ALONE(AVIX_movi_xxxx)
            ATOM_DEFAULT(AVIX_movi_xxxx)
            ATOM_END_DEFAULT
        ATOM_END
    LIST(CADP)
        ATOM_BEGIN
        ATOM_END
    LIST(CMJP)
        ATOM_BEGIN
        ATOM_END
    ATOM(CMP4)
    ATOM(IDVX)
    LIST_SKIP(JUNK)
    LIST_SKIP(menu)
    ATOM(MThd)
    LIST_SKIP(MTrk)
    LIST_SKIP(PAL_)
    LIST_SKIP(RDIB)
    LIST_SKIP(RMID)
    LIST_SKIP(RMMP)
    LIST(RMP3)
        ATOM_BEGIN
        LIST(RMP3_data)
            break;
        LIST(RMP3_INFO)
            ATOM_BEGIN
            ATOM(RMP3_INFO_IID3)
            ATOM(RMP3_INFO_ILYC)
            ATOM(RMP3_INFO_IMP3)
            ATOM(RMP3_INFO_JUNK)
            ATOM_DEFAULT(RMP3_INFO_xxxx)
            ATOM_END_DEFAULT
        ATOM_END
    ATOM(SMV0)
    ATOM(SMV0_xxxx)
    ATOM(W3DI)
    LIST(WAVE)
        ATOM_BEGIN
        LIST(WAVE_bext)
            ATOM_BEGIN
            ATOM_END
        LIST(WAVE_data)
            break;
        ATOM(WAVE_ds64)
        ATOM(WAVE_fact)
        ATOM(WAVE_fmt_)
        ATOM(WAVE_ID3_)
        ATOM_END
    DATA_END

    if (Alignement_ExtraByte)
    {
        Element_Size++;
        if (File_GoTo==(size_t)-1)
            Skip_XX(1,                                          "Alignement");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Riff::AIFC()
{
    Data_Accept("AIFF Compressed");
    Element_Name("AIFF Compressed");

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "AIFF");
    Stream_Prepare(Stream_Audio);
}

//---------------------------------------------------------------------------
void File_Riff::AIFC_COMM()
{
    AIFF_COMM();
}

//---------------------------------------------------------------------------
void File_Riff::AIFC_COMT()
{
    AIFF_COMT();
}

//---------------------------------------------------------------------------
void File_Riff::AIFC_FVER()
{
    Element_Name("Format Version");

    //Parsing
    Skip_B4(                                                    "Version");
}

//---------------------------------------------------------------------------
void File_Riff::AIFC_SSND()
{
    AIFF_SSND();
}

//---------------------------------------------------------------------------
void File_Riff::AIFC_xxxx()
{
    AIFF_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::AIFF()
{
    Accept("AIFF");
    Element_Name("AIFF");

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "AIFF");
    Stream_Prepare(Stream_Audio);
}

//---------------------------------------------------------------------------
void File_Riff::AIFF_COMM()
{
    Element_Name("Common");

    int32u numSampleFrames;
    int16u numChannels, sampleSize;
    float80 sampleRate;
    //Parsing
    Get_B2 (numChannels,                                    "numChannels");
    Get_B4 (numSampleFrames,                                "numSampleFrames");
    Get_B2 (sampleSize,                                     "sampleSize");
    Get_BF10(sampleRate,                                    "sampleRate");
    if (Data_Remain()) //AIFC
    {
        int32u compressionType;
        Get_C4 (compressionType,                            "compressionType");
        Skip_PA(                                            "compressionName");

        //Filling
        CodecID_Fill(Ztring().From_CC4(compressionType), Stream_Audio, StreamPos_Last, InfoCodecID_Format_Mpeg4);
        Fill(Stream_Audio, StreamPos_Last, Audio_Codec, Ztring().From_CC4(compressionType));
    }
    else
    {
        //Filling
        Fill(Stream_Audio, StreamPos_Last, Audio_Format, "PCM");
        Fill(Stream_Audio, StreamPos_Last, Audio_Codec, "PCM");
    }
    Fill(Stream_Audio, StreamPos_Last, Audio_BitRate_Mode, "CBR");

    //Filling
    Fill(Stream_Audio, StreamPos_Last, Audio_Channel_s_, numChannels);
    Fill(Stream_Audio, StreamPos_Last, Audio_Resolution, sampleSize);
    if (sampleRate)
        Fill(Stream_Audio, StreamPos_Last, Audio_Duration, numSampleFrames/sampleRate*1000);
    Fill(Stream_Audio, StreamPos_Last, Audio_SamplingRate, sampleRate, 0);
}

//---------------------------------------------------------------------------
void File_Riff::AIFF_COMT()
{
    //Parsing
    int16u numComments;
    Get_B2(numComments,                                         "numComments");
    for (int16u Pos=0; Pos<=numComments; Pos++)
    {
        Ztring text;
        int16u count;
        Element_Begin("Comment");
        Skip_B4(                                                "timeStamp");
        Skip_B4(                                                "marker");
        Get_B2 (count,                                          "count");
        count+=count%1; //always even
        Get_Local(count, text,                                  "text");
        Element_End();

        //Filling
        Fill(Stream_General, 0, General_Comment, text);
    }
}

//---------------------------------------------------------------------------
void File_Riff::AIFF_SSND()
{
    Element_Name("Sound Data");

    Skip_XX(Element_TotalSize_Get(),                            "Data");
    
    //Filling
    Fill(Stream_Audio, 0, Audio_StreamSize, Element_TotalSize_Get());
    Finish("AIFF");
}

//---------------------------------------------------------------------------
void File_Riff::AIFF_xxxx()
{
    #define ELEMENT_CASE(_ELEMENT, _NAME) \
        case Elements::_ELEMENT : Element_Name(_NAME); Name=_NAME; break;

    //Known?
    std::string Name;
    switch(Element_Code)
    {
        ELEMENT_CASE(AIFF__c__, "Copyright");
        ELEMENT_CASE(AIFF_ANNO, "Comment");
        ELEMENT_CASE(AIFF_AUTH, "Performer");
        ELEMENT_CASE(AIFF_NAME, "Title");
        default : Skip_XX(Element_Size,                         "Unknown");
                  return;
    }

    //Parsing
    Ztring text;
    Get_Local(Element_Size, text,                               "text");

    //Filling
    Fill(Stream_General, 0, Name.c_str(), text);
}

//---------------------------------------------------------------------------
void File_Riff::AVI_()
{
    Data_Accept("AVI");
    Element_Name("AVI");

    //Test if there is only one AVI chunk
    if (Count_Get(Stream_General)>0)
    {
        Element_Info("Problem: 2 AVI chunks, this is not normal");
        Skip_XX(Element_TotalSize_Get(),                        "Data");
        return;
    }

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "AVI");

    //Configuring
    Buffer_MaximumSize=32*1024*1024;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__cset()
{
    Element_Name("Regional settings");

    //Parsing
    Skip_L2(                                                    "CodePage"); //TODO: take a look about IBM/MS RIFF/MCI Specification 1.0
    Skip_L2(                                                    "CountryCode");
    Skip_L2(                                                    "LanguageCode");
    Skip_L2(                                                    "Dialect");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__exif()
{
    Element_Name("Exif (Exchangeable Image File Format)");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__exif_xxxx()
{
    Element_Name("Value");

    //Parsing
    Ztring Value;
    Get_Local(Element_Size, Value,                              "Value");

    //Filling
    switch (Element_Code)
    {
        case Elements::AVI__exif_ecor : Fill(Stream_General, 0, "Make", Value); break;
        case Elements::AVI__exif_emdl : Fill(Stream_General, 0, "Model", Value); break;
        case Elements::AVI__exif_emnt : Fill(Stream_General, 0, "MakerNotes", Value); break;
        case Elements::AVI__exif_erel : Fill(Stream_General, 0, "RelatedImageFile", Value); break;
        case Elements::AVI__exif_etim : Fill(Stream_General, 0, "Written_Date", Value); break;
        case Elements::AVI__exif_eucm : Fill(Stream_General, 0, General_Comment, Value); break;
        case Elements::AVI__exif_ever : break; //Exif version
        default:                    Fill(Stream_General, 0, Ztring().From_CC4((int32u)Element_Code).To_Local().c_str(), Value);
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__goog()
{
    Element_Name("Google specific");

    //Filling
    Fill(Stream_General, 0, General_Format, "Google Video", Unlimited, false, true);
}

//---------------------------------------------------------------------------
void File_Riff::AVI__goog_GDAT()
{
    Element_Name("Google datas");
}

//---------------------------------------------------------------------------
// Google Metadata
//
void File_Riff::AVI__GMET()
{
    Element_Name("Google Metadatas");

    //Parsing
    Ztring Value; Value.From_Local((const char*)(Buffer+Buffer_Offset+0), (size_t)Element_Size);
    ZtringListList List;
    List.Separator_Set(0, _T("\n"));
    List.Separator_Set(1, _T(":"));
    List.Max_Set(1, 2);
    List.Write(Value);

    //Details
    if (MediaInfoLib::Config.Details_Get())
    {
        //for (size_t Pos=0; Pos<List.size(); Pos++)
        //    Details_Add_Info(Pos, List(Pos, 0).To_Local().c_str(), List(Pos, 1));
    }

    //Filling
    for (size_t Pos=0; Pos<List.size(); Pos++)
    {
        if (List(Pos, 0)==_T("title"))          Fill(Stream_General, 0, General_Title, List(Pos, 1));
        if (List(Pos, 0)==_T("description"))    Fill(Stream_General, 0, General_Title_More, List(Pos, 1));
        if (List(Pos, 0)==_T("url"))            Fill(Stream_General, 0, General_Title_Url, List(Pos, 1));
        if (List(Pos, 0)==_T("docid"))          Fill(Stream_General, 0, General_UniqueID, List(Pos, 1));
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr()
{
    Element_Name("AVI Header");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_avih()
{
    Element_Name("File header");

    //Parsing
    int32u MicrosecPerFrame, Flags;
    Get_L4 (MicrosecPerFrame,                                   "MicrosecPerFrame");
    Skip_L4(                                                    "MaxBytesPerSec");
    Skip_L4(                                                    "PaddingGranularity");
    Get_L4 (Flags,                                              "Flags");
        Skip_Flags(Flags,  4,                                   "HasIndex");
        Skip_Flags(Flags,  5,                                   "MustUseIndex");
        Skip_Flags(Flags,  8,                                   "IsInterleaved");
        Skip_Flags(Flags,  9,                                   "UseCKTypeToFindKeyFrames");
        Skip_Flags(Flags, 11,                                   "TrustCKType");
        Skip_Flags(Flags, 16,                                   "WasCaptureFile");
        Skip_Flags(Flags, 17,                                   "Copyrighted");
    Get_L4 (avih_TotalFrame,                                    "TotalFrames");
    Skip_L4(                                                    "InitialFrames");
    Skip_L4(                                                    "StreamsCount");
    Skip_L4(                                                    "SuggestedBufferSize");
    Skip_L4(                                                    "Width");
    Skip_L4(                                                    "Height");
    Skip_L4(                                                    "Reserved");
    Skip_L4(                                                    "Reserved");
    Skip_L4(                                                    "Reserved");
    Skip_L4(                                                    "Reserved");
    if(Element_Offset<Element_Size)
        Skip_XX(Element_Size-Element_Offset,                    "Unknown");

    //Filling
    if (MicrosecPerFrame>0)
        avih_FrameRate=1000000.0/MicrosecPerFrame;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_JUNK()
{
    Element_Name("Garbage");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_odml()
{
    Element_Name("OpenDML");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_odml_dmlh()
{
    Element_Name("OpenDML Header");

    //Parsing
    Get_L4(dmlh_TotalFrame,                                     "GrandFrames");
    if (Element_Offset<Element_Size)
        Skip_XX(Element_Size-Element_Offset,                    "Unknown");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_ON2h()
{
    Element_Name("On2 header");

    //Parsing
    Skip_XX(Element_Size,                                       "Unknown");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl()
{
    Element_Name("Stream info");
    Element_Info(stream_Count);

    //Clean up
    StreamKind_Last=Stream_Max;

    //Compute the current codec ID
    Stream_ID=(('0'+stream_Count/10)*0x01000000
              +('0'+stream_Count   )*0x00010000);
    stream_Count++;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_indx()
{
    Element_Name("Index");

    int32u Entry_Count, ChunkId;
    int16u LongsPerEntry;
    int8u  IndexType, IndexSubType;
    Get_L2 (LongsPerEntry,                                      "LongsPerEntry"); //Size of each entry in aIndex array
    Get_L1 (IndexSubType,                                       "IndexSubType");
    Get_L1 (IndexType,                                          "IndexType");
    Get_L4 (Entry_Count,                                        "EntriesInUse"); //Index of first unused member in aIndex array
    Get_C4 (ChunkId,                                            "ChunkId"); //FCC of what is indexed

    //Depends of size of structure...
    switch (IndexType)
    {
        case 0x01 : //AVI_INDEX_OF_CHUNKS
                    switch (IndexSubType)
                    {
                        case 0x00 : AVI__hdlr_strl_indx_StandardIndex(Entry_Count, ChunkId); break;
                        case 0x01 : AVI__hdlr_strl_indx_FieldIndex(Entry_Count, ChunkId); break; //AVI_INDEX_2FIELD
                        default: Skip_XX(Element_Size-Element_Offset, "Unknown");
                    }
                    break;
        case 0x0 : //AVI_INDEX_OF_INDEXES
                    switch (IndexSubType)
                    {
                        case 0x00 :
                        case 0x01 : AVI__hdlr_strl_indx_SuperIndex(Entry_Count, ChunkId); break; //AVI_INDEX_2FIELD
                        default: Skip_XX(Element_Size-Element_Offset, "Unknown");
                    }
                    break;
        default: Skip_XX(Element_Size-Element_Offset,           "Unknown");
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_indx_StandardIndex(int32u Entry_Count, int32u ChunkId)
{
    Element_Name("Standard Index");

    //Parsing
    int64u BaseOffset, StreamSize=0;
    Get_L8 (BaseOffset,                                         "BaseOffset");
    Skip_L4(                                                    "Reserved3");
    for (int32u Pos=0; Pos<Entry_Count; Pos++)
    {
        //Is too slow
        /*
        Element_Begin("Index");
        int32u Offset, Size;
        Get_L4 (Offset,                                         "Offset"); //BaseOffset + this is absolute file offset
        Get_L4 (Size,                                           "Size"); //Bit 31 is set if this is NOT a keyframe
        Element_Info(Size&0x7FFFFFFF);
        if (Size)
            Element_Info("KeyFrame");
        Element_End();
        */

        //Faster method
        int32u Offset=LittleEndian2int32u(Buffer+Buffer_Offset+(size_t)Element_Offset  );
        int32u Size  =LittleEndian2int32u(Buffer+Buffer_Offset+(size_t)Element_Offset+4)&0x7FFFFFFF;
        Element_Offset+=8;

        //Stream Position and size
        if (Pos<300)
        {
            Stream_Structure[BaseOffset+Offset-8].Name=ChunkId&0xFFFF0000;
            Stream_Structure[BaseOffset+Offset-8].Size=Size;
        }
        StreamSize+=(Size&0x7FFFFFFF);
        Stream[ChunkId&0xFFFF0000].PacketCount++;

        //Interleaved
        if (Pos==  0 && (ChunkId&0xFFFF0000)==0x30300000 && Interleaved0_1  ==0)
            Interleaved0_1 =BaseOffset+Offset-8;
        if (Pos==Entry_Count/10 && (ChunkId&0xFFFF0000)==0x30300000 && Interleaved0_10==0)
            Interleaved0_10=BaseOffset+Offset-8;
        if (Pos==  0 && (ChunkId&0xFFFF0000)==0x30310000 && Interleaved1_1  ==0)
            Interleaved1_1 =BaseOffset+Offset-8;
        if (Pos==Entry_Count/10 && (ChunkId&0xFFFF0000)==0x30310000 && Interleaved1_10==0)
            Interleaved1_10=BaseOffset+Offset-8;
    }
    Stream[ChunkId&0xFFFF0000].StreamSize+=StreamSize;
    if (Element_Offset<Element_Size)
        Skip_XX(Element_Size-Element_Offset,                    "Garbage");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_indx_FieldIndex(int32u Entry_Count, int32u)
{
    Element_Name("Field Index");

    //Parsing
    Skip_L8(                                                    "Offset");
    Skip_L4(                                                    "Reserved2");
    for (int32u Pos=0; Pos<Entry_Count; Pos++)
    {
        Element_Begin("Index");
        Skip_L4(                                                "Offset"); //BaseOffset + this is absolute file offset
        Skip_L4(                                                "Size"); //Bit 31 is set if this is NOT a keyframe
        Skip_L4(                                                "OffsetField2"); //Offset to second field
        Element_End();
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_indx_SuperIndex(int32u Entry_Count, int32u ChunkId)
{
    Element_Name("Index of Indexes");

    //Parsing
    int64u Offset;
    Skip_L4(                                                    "Reserved0");
    Skip_L4(                                                    "Reserved1");
    Skip_L4(                                                    "Reserved2");
    for (int32u Pos=0; Pos<Entry_Count; Pos++)
    {
        Element_Begin("Index of Indexes");
        Get_L8 (Offset,                                         "Offset");
        Skip_L4(                                                "Size"); //Size of index chunk at this offset
        Skip_L4(                                                "Duration"); //time span in stream ticks
        Index_Pos[Offset]=ChunkId;
        Element_End();
    }

    //We needn't anymore Old version
    NeedOldIndex=false;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_JUNK()
{
    Element_Name("Garbage");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strd()
{
    Element_Name("Stream datas");

    //Parsing
    Skip_XX(Element_Size,                                       "Unknown");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf()
{
    Element_Name("Stream format");

    //Parse depending of kind of stream
    switch (Stream[Stream_ID].fccType)
    {
        case Elements::AVI__hdlr_strl_strh_auds : AVI__hdlr_strl_strf_auds(); break;
        case Elements::AVI__hdlr_strl_strh_iavs : AVI__hdlr_strl_strf_iavs(); break;
        case Elements::AVI__hdlr_strl_strh_mids : AVI__hdlr_strl_strf_mids(); break;
        case Elements::AVI__hdlr_strl_strh_txts : AVI__hdlr_strl_strf_txts(); break;
        case Elements::AVI__hdlr_strl_strh_vids : AVI__hdlr_strl_strf_vids(); break;
        default :                                 Element_Info("Unknown");
    }

    //Registering stream
    Stream[Stream_ID].StreamKind=StreamKind_Last;
    Stream[Stream_ID].StreamPos=StreamPos_Last;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds()
{
    Element_Info("Audio");

    //Parsing
    int32u SamplesPerSec, AvgBytesPerSec;
    int16u FormatTag, Channels, BitsPerSample;
    Get_L2 (FormatTag,                                          "FormatTag");
    Get_L2 (Channels,                                           "Channels");
    Get_L4 (SamplesPerSec,                                      "SamplesPerSec");
    Get_L4 (AvgBytesPerSec,                                     "AvgBytesPerSec");
    Skip_L2(                                                    "BlockAlign");
    Get_L2 (BitsPerSample,                                      "BitsPerSample");

    //Filling
    Stream_Prepare(Stream_Audio);
    Stream[Stream_ID].Compression=FormatTag;
    Ztring Codec; Codec.From_Number(FormatTag, 16);
    Codec.MakeUpperCase();
    CodecID_Fill(Codec, Stream_Audio, StreamPos_Last, InfoCodecID_Format_Riff);
    Fill(Stream_Audio, StreamPos_Last, Audio_Codec, Codec); //May be replaced by codec parser
    Fill(Stream_Audio, StreamPos_Last, Audio_Codec_CC, Codec);
    Fill(Stream_Audio, StreamPos_Last, Audio_Channel_s_, Channels!=5?Channels:6);
    Fill(Stream_Audio, StreamPos_Last, Audio_SamplingRate, SamplesPerSec);
    Fill(Stream_Audio, StreamPos_Last, Audio_BitRate, AvgBytesPerSec*8);
    if (BitsPerSample) Fill(Stream_Audio, StreamPos_Last, Audio_Resolution, BitsPerSample);
    Stream[Stream_ID].AvgBytesPerSec=AvgBytesPerSec; //Saving bitrate for each stream
    if (SamplesPerSec && TimeReference!=(int64u)-1)
        Fill(Stream_Audio, 0, Audio_Delay, TimeReference/SamplesPerSec);

    //Creating the parser
         if (0);
    #if defined(MEDIAINFO_MPEGA_YES)
    else if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_KindofCodec).find(_T("MPEG-"))==0)
    {
        Stream[Stream_ID].Parser=new File_Mpega;
        Stream[Stream_ID].Parser->ShouldContinueParsing=true;
    }
    #endif
    #if defined(MEDIAINFO_AC3_YES)
    else if (FormatTag==0x2000)
    {
        Stream[Stream_ID].Parser=new File_Ac3;
        ((File_Ac3*)Stream[Stream_ID].Parser)->Frame_Count_Valid=2;
        Stream[Stream_ID].Parser->ShouldContinueParsing=true;
    }
    #endif
    #if defined(MEDIAINFO_DTS_YES)
    else if (FormatTag==0x2001
          || (FormatTag==0x1 && Retrieve(Stream_General, 0, General_Format)==_T("Wave"))) //Some DTS streams are coded "1"
    {
        Stream[Stream_ID].Parser=new File_Dts;
        ((File_Dts*)Stream[Stream_ID].Parser)->Frame_Count_Valid=2;
        Stream[Stream_ID].Parser->ShouldContinueParsing=true;
    }
    #endif
    #if defined(MEDIAINFO_ADTS_YES)
    else if (FormatTag==0xAAC || FormatTag==0xFF)
    {
        Stream[Stream_ID].Parser=new File_Adts;
        ((File_Adts*)Stream[Stream_ID].Parser)->Frame_Count_Valid=1;
        Stream[Stream_ID].Parser->ShouldContinueParsing=true;
    }
    #endif
    #if defined(MEDIAINFO_PCM_YES)
    else if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_KindofCodec).find(_T("PCM"))==0)
    {
        //Creating the parser
        File_Pcm MI;
        MI.Codec=Codec;

        //Parsing
        Open_Buffer_Init(&MI);
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset, 0);
        Open_Buffer_Finalize(&MI);

        //Filling
        Merge(MI, StreamKind_Last, 0, StreamPos_Last);
    }
    #endif
    #if defined(MEDIAINFO_ADPCM_YES)
    else if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_KindofCodec).find(_T("ADPCM"))==0)
    {
        //Creating the parser
        File_Adpcm MI;
        MI.Codec=Codec;

        //Parsing
        Open_Buffer_Init(&MI);
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset, 0);
        Open_Buffer_Finalize(&MI);

        //Filling
        Merge(MI, StreamKind_Last, 0, StreamPos_Last);
    }
    #endif
    #if defined(MEDIAINFO_OGG_YES)
    else if (MediaInfoLib::Config.Codec_Get(Codec, InfoCodec_KindofCodec)==_T("Vorbis")
          && FormatTag!=0x566F) //0x6F56 has config in this chunk
    {
        Stream[Stream_ID].Parser=new File_Ogg;
        Stream[Stream_ID].Parser->ShouldContinueParsing=true;
    }
    #endif
    Open_Buffer_Init(Stream[Stream_ID].Parser);

    //Options
    if (Element_Offset+2>Element_Size)
        return; //No options
        
    //Parsing
    int16u Option_Size;
    Get_L2 (Option_Size,                                        "cbSize");

    //Filling
    if (Option_Size>0)
    {
             if (0);
        else if (MediaInfoLib::Config.CodecID_Get(Stream_Audio, InfoCodecID_Format_Riff, Codec)==_T("MPEG Audio"))
        {
            if (Option_Size==12)
                AVI__hdlr_strl_strf_auds_Mpega();
            else
                Skip_XX(Option_Size,                            "MPEG Audio - Uknown");
        }
        else if (Codec==_T("AAC") || Codec==_T("FF"))
            AVI__hdlr_strl_strf_auds_Aac();
        else if (FormatTag==0x566F) //Vorbis with Config in this chunk
            AVI__hdlr_strl_strf_auds_Vorbis();
        else if (FormatTag==0x6750) //Vorbis with Config in this chunk
            AVI__hdlr_strl_strf_auds_Vorbis2();
        else if (FormatTag==0xFFFE) //Extensible Wave
            AVI__hdlr_strl_strf_auds_ExtensibleWave();
        else Skip_XX(Option_Size,                               "Unknown");
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds_Mpega()
{
    //Parsing
    Element_Begin("MPEG Audio options");
    Skip_L2(                                                    "ID");
    Skip_L4(                                                    "Flags");
    Skip_L2(                                                    "BlockSize");
    Skip_L2(                                                    "FramesPerBlock");
    Skip_L2(                                                    "CodecDelay");
    Element_End();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds_Aac()
{
    //Parsing
    Element_Begin("AAC options");
    #if defined(MEDIAINFO_MPEG4_YES)
        File_Mpeg4_AudioSpecificConfig MI;
        Open_Buffer_Init(&MI);
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Open_Buffer_Finalize(&MI);
        Merge(MI, StreamKind_Last, 0, StreamPos_Last);
    #else //MEDIAINFO_MPEG4_YES
        Skip_XX(Element_Size-Element_Offset,                    "(AudioSpecificConfig)");
    #endif
    Element_End();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds_Vorbis()
{
    //Parsing
    Element_Begin("Vorbis options");
    #if defined(MEDIAINFO_OGG_YES)
        File_Ogg_SubElement MI;
        Open_Buffer_Init(&MI);
        Element_Begin("Element sizes");
            //All elements parsing, except last one
            std::vector<size_t> Elements_Size;
            size_t Elements_TotalSize=0;
            int8u Elements_Count;
            Get_L1(Elements_Count,                                  "Element count");
            Elements_Size.resize(Elements_Count+1); //+1 for the last block
            for (int8u Pos=0; Pos<Elements_Count; Pos++)
            {
                int8u Size;
                Get_L1(Size,                                        "Size");
                Elements_Size[Pos]=Size;
                Elements_TotalSize+=Size;
            }
        Element_End();
        if (Element_Offset+Elements_TotalSize>Element_Size)
            return;
        //Adding the last block
        Elements_Size[Elements_Count]=(size_t)(Element_Size-(Element_Offset+Elements_TotalSize));
        Elements_Count++;
        //Parsing blocks
        for (int8u Pos=0; Pos<Elements_Count; Pos++)
        {
            Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Offset, Elements_Size[Pos]);
            Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Size, 0);
            Element_Offset+=Elements_Size[Pos];
        }
        //Finalizing
        Open_Buffer_Finalize(&MI);
        Merge(MI, StreamKind_Last, 0, StreamPos_Last);
        Element_Show();
    #else //MEDIAINFO_MPEG4_YES
        Skip_XX(Element_Size-Element_Offset,                    "(Vorbis headers)");
    #endif
    Element_End();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds_Vorbis2()
{
    //Parsing
    Skip_XX(8,                                                  "Vorbis Unknown");
    Element_Begin("Vorbis options");
    #if defined(MEDIAINFO_OGG_YES)
        File_Ogg_SubElement MI;
        Open_Buffer_Continue(Stream[Stream_ID].Parser, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Open_Buffer_Continue(Stream[Stream_ID].Parser, Buffer+Buffer_Offset+(size_t)Element_Size, 0);
        Open_Buffer_Finalize(Stream[Stream_ID].Parser);
        Merge(*Stream[Stream_ID].Parser, StreamKind_Last, 0, StreamPos_Last);
        Element_Show();
    #else //MEDIAINFO_MPEG4_YES
        Skip_XX(Element_Size-Element_Offset,                    "(Vorbis headers)");
    #endif
    Element_End();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds_ExtensibleWave()
{
    //Parsing
    int32u ChannelMask;
    Skip_L2(                                                    "ValidBitsPerSample / SamplesPerBlock");
    Get_L4 (ChannelMask,                                        "ChannelMask");
    Skip_UUID(                                                  "SubFormat");

    FILLING_BEGIN();
        Fill(Stream_Audio, StreamPos_Last, Audio_ChannelPositions, ExtensibleWave_ChannelMask(ChannelMask));
        Fill(Stream_Audio, StreamPos_Last, Audio_ChannelPositions_String2, ExtensibleWave_ChannelMask2(ChannelMask));
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_iavs()
{
    //Standard video header before Iavs?
    if (Element_Size==72)
    {
        Element_Begin();
            AVI__hdlr_strl_strf_vids();
        Element_End();
    }

    Element_Info("Interleaved Audio/Video");

    #ifdef MEDIAINFO_DVDIF_YES
        if (Element_Size<8*4)
            return;

        //Parsing
        DV_FromHeader=new File_DvDif();
        Open_Buffer_Init(DV_FromHeader);

        //DVAAuxSrc
        ((File_DvDif*)DV_FromHeader)->AuxToAnalyze=0x50; //Audio source
        Open_Buffer_Continue(DV_FromHeader, Buffer+Buffer_Offset+(size_t)Element_Offset, 4);
        Element_Offset+=4;
        //DVAAuxCtl
        ((File_DvDif*)DV_FromHeader)->AuxToAnalyze=0x51; //Audio control
        Open_Buffer_Continue(DV_FromHeader, Buffer+Buffer_Offset+(size_t)Element_Offset, 4);
        Element_Offset+=4;
        //DVAAuxSrc1
        Skip_L4(                                                "DVAAuxSrc1");
        //DVAAuxCtl1
        Skip_L4(                                                "DVAAuxCtl1");
        //DVVAuxSrc
        ((File_DvDif*)DV_FromHeader)->AuxToAnalyze=0x60; //Video source
        Open_Buffer_Continue(DV_FromHeader, Buffer+Buffer_Offset+(size_t)Element_Offset, 4);
        Element_Offset+=4;
        //DVAAuxCtl
        ((File_DvDif*)DV_FromHeader)->AuxToAnalyze=0x61; //Video control
        Open_Buffer_Continue(DV_FromHeader, Buffer+Buffer_Offset+(size_t)Element_Offset, 4);
        Element_Offset+=4;
        //Reserved
        Skip_L4(                                                "DVReserved");
        Skip_L4(                                                "DVReserved");

        Open_Buffer_Finalize(DV_FromHeader);

        Stream_Prepare(Stream_Video);
        Stream[Stream_ID].Parser=new File_DvDif;
        Open_Buffer_Init(Stream[Stream_ID].Parser);
        ((File_DvDif*)Stream[Stream_ID].Parser)->Frame_Count_Valid=2;

    #else //MEDIAINFO_DVDIF_YES
        //Parsing
        Skip_L4(                                                "DVAAuxSrc");
        Skip_L4(                                                "DVAAuxCtl");
        Skip_L4(                                                "DVAAuxSrc1");
        Skip_L4(                                                "DVAAuxCtl1");
        Skip_L4(                                                "DVVAuxSrc");
        Skip_L4(                                                "DVVAuxCtl");
        Skip_L4(                                                "DVReserved");
        Skip_L4(                                                "DVReserved");

        //Filling
        Ztring Codec; Codec.From_CC4(Stream[Stream_ID].fccHandler);
        Stream_Prepare(Stream_Video);
        float32 FrameRate=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate).To_float32();
        Fill(Stream_Video, StreamPos_Last, Video_Codec, Codec); //May be replaced by codec parser
        Fill(Stream_Video, StreamPos_Last, Video_Codec_CC, Codec);
             if (Codec==_T("dvsd")
              || Codec==_T("dvsl"))
        {
                                        Fill(Stream_Video, StreamPos_Last, Video_Width,  720);
                 if (FrameRate==25.000) Fill(Stream_Video, StreamPos_Last, Video_Height, 576);
            else if (FrameRate==29.970) Fill(Stream_Video, StreamPos_Last, Video_Height, 480);
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, 4.0/3);
        }
        else if (Codec==_T("dvhd"))
        {
                                        Fill(Stream_Video, StreamPos_Last, Video_Width,  1440);
                 if (FrameRate==25.000) Fill(Stream_Video, StreamPos_Last, Video_Height, 1152);
            else if (FrameRate==30.000) Fill(Stream_Video, StreamPos_Last, Video_Height,  960);
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, 4.0/3);
        }
        Stream_Prepare(Stream_Audio);
        CodecID_Fill(Codec, Stream_Audio, StreamPos_Last, InfoCodecID_Format_Riff);
        Fill(Stream_Audio, StreamPos_Last, Audio_Codec, Codec); //May be replaced by codec parser
        Fill(Stream_Audio, StreamPos_Last, Audio_Codec_CC, Codec);
    #endif //MEDIAINFO_DVDIF_YES
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_mids()
{
    Element_Info("Midi");

    //Filling
    Stream_Prepare(Stream_Audio);
    Fill(Stream_Audio, StreamPos_Last, Audio_Format, "MIDI");
    Fill(Stream_Audio, StreamPos_Last, Audio_Codec, "Midi");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_txts()
{
    Element_Info("Text");

    //Parsing
    Ztring Format;
    if (Element_Size)
    {
        Get_Local(10, Format,                                   "Format");
        Skip_XX(22,                                             "Unknown");
    }

    FILLING_BEGIN_PRECISE()
        Stream_Prepare(Stream_Text);

        if (Element_Size==0)
        {
            //Creating the parser
            #if defined(MEDIAINFO_OTHERTEXT_YES)
                Stream[Stream_ID].Parser=new File_OtherText;
                Open_Buffer_Init(Stream[Stream_ID].Parser);
            #endif
        }
        else
        {
            Fill(Stream_Text, StreamPos_Last, Text_Format, Format);
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_vids()
{
    Element_Info("Video");

    //Parsing
    int32u Compression, Width, Height;
    int16u Resolution;
    Skip_L4(                                                    "Size");
    Get_L4 (Width,                                              "Width");
    Get_L4 (Height,                                             "Height");
    Skip_L2(                                                    "Planes");
    Get_L2 (Resolution,                                         "BitCount");
    Get_C4 (Compression,                                        "Compression");
    Skip_L4(                                                    "SizeImage");
    Skip_L4(                                                    "XPelsPerMeter");
    Skip_L4(                                                    "YPelsPerMeter");
    Skip_L4(                                                    "ClrUsed");
    Skip_L4(                                                    "ClrImportant");

    //Filling
    Stream[Stream_ID].Compression=Compression;
    if (((Compression&0x000000FF)>=0x00000020 && (Compression&0x000000FF)<=0x0000007E
      && (Compression&0x0000FF00)>=0x00002000 && (Compression&0x0000FF00)<=0x00007E00
      && (Compression&0x00FF0000)>=0x00200000 && (Compression&0x00FF0000)<=0x007E0000
      && (Compression&0xFF000000)>=0x20000000 && (Compression&0xFF000000)<=0x7E000000)
     ||   Compression==0x00000000
       ) //Sometimes this value is wrong, we have to test this
    {
        if (Compression==CC4("DXSB"))
        {
            //Divx.com hack for subtitle, this is a text stream in a DivX Format
            Fill(Stream_General, 0, General_Format, "DivX", Unlimited, true, true);
            Stream_Prepare(Stream_Text);
        }
        else
            Stream_Prepare(Stream_Video);

        //Filling
        if (Compression==0x00000000)
        {
            CodecID_Fill(_T("RGB "), StreamKind_Last, StreamPos_Last, InfoCodecID_Format_Riff);
            Fill(StreamKind_Last, StreamPos_Last, "Codec", "RGB"); //Raw RGB, not handled by automatic codec mapping
        }
        else
        {
            CodecID_Fill(Ztring().From_CC4(Compression), StreamKind_Last, StreamPos_Last, InfoCodecID_Format_Riff);
            Fill(StreamKind_Last, StreamPos_Last, "Codec", Ztring().From_CC4(Compression).To_Local().c_str()); //FormatTag, may be replaced by codec parser
            Fill(StreamKind_Last, StreamPos_Last, "Codec/CC", Ztring().From_CC4(Compression).To_Local().c_str()); //FormatTag
        }
        Fill(StreamKind_Last, StreamPos_Last, "Width", Width, 10, true);
        Fill(StreamKind_Last, StreamPos_Last, "Height", Height, 10, true);
        Fill(StreamKind_Last, StreamPos_Last, "Resolution", Resolution);
    }
    else
    {
        //Some Stream headers are broken, must use AVISTREAMINFOA structure instead of AVIFILEINFOA
        Stream_Prepare(Stream_Video);
    }
    Stream[Stream_ID].StreamKind=StreamKind_Last;

    //Creating the parser
         if (0);
    #if defined(MEDIAINFO_MPEG4V_YES)
    else if (MediaInfoLib::Config.Codec_Get(Ztring().From_CC4(Compression), InfoCodec_KindofCodec).find(_T("MPEG-4V"))==0)
    {
        Stream[Stream_ID].Parser=new File_Mpeg4v;
        Stream[Stream_ID].Specific_IsMpeg4v=true;
        ((File_Mpeg4v*)Stream[Stream_ID].Parser)->FrameIsAlwaysComplete=true;
        Stream[Stream_ID].Parser->ShouldContinueParsing=true;
    }
    #endif
    #if defined(MEDIAINFO_AVC_YES)
    else if (MediaInfoLib::Config.Codec_Get(Ztring().From_CC4(Compression), InfoCodec_KindofCodec).find(_T("AVC"))==0)
    {
        Stream[Stream_ID].Parser=new File_Avc;
        ((File_Avc*)Stream[Stream_ID].Parser)->FrameIsAlwaysComplete=true;
    }
    #endif
    #if defined(MEDIAINFO_JPEG_YES)
    else if (Ztring().From_CC4(Compression)==_T("MJPG"))
    {
        Stream[Stream_ID].Parser=new File_Jpeg;
        ((File_Jpeg*)Stream[Stream_ID].Parser)->StreamKind=Stream_Video;
    }
    #endif
    #if defined(MEDIAINFO_DVDIF_YES)
    else if (MediaInfoLib::Config.Codec_Get(Ztring().From_CC4(Compression), InfoCodec_KindofCodec).find(_T("DV"))==0)
    {
        Stream[Stream_ID].Parser=new File_DvDif;
        ((File_DvDif*)Stream[Stream_ID].Parser)->Frame_Count_Valid=1;
        ((File_DvDif*)Stream[Stream_ID].Parser)->IgnoreAudio=true;
    }
    #endif
    Open_Buffer_Init(Stream[Stream_ID].Parser);

    //Options
    if (Element_Offset>=Element_Size)
        return; //No options
        
    //Filling
         if (0);
    else if (MediaInfoLib::Config.Codec_Get(Ztring().From_CC4(Compression), InfoCodec_KindofCodec).find(_T("AVC"))==0)
        AVI__hdlr_strl_strf_vids_Avc();
    else Skip_XX(Element_Size-Element_Offset,                   "Unknown");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_vids_Avc()
{
    //Parsing
    Element_Begin("AVC options");
    #if defined(MEDIAINFO_AVC_YES)
        ((File_Avc*)Stream[Stream_ID].Parser)->MustParse_SPS_PPS=true;
        ((File_Avc*)Stream[Stream_ID].Parser)->SizedBlocks=true;
        ((File_Avc*)Stream[Stream_ID].Parser)->MustSynchronize=false;
        Open_Buffer_Continue(Stream[Stream_ID].Parser, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
    #else //MEDIAINFO_AVC_YES
        Skip_XX(Element_Size-Element_Offset,                    "(AVC headers)");
    #endif
    Element_End();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strh()
{
    Element_Name("Stream header");

    //Parsing
    int32u fccType, fccHandler, Scale, Rate, Start, Length;
    int16u Left, Top, Right, Bottom;
    Get_C4 (fccType,                                            "fccType");
    switch (fccType)
    {
        case Elements::AVI__hdlr_strl_strh_auds :
            Get_L4 (fccHandler,                                 "fccHandler");
            break;
        default:
            Get_C4 (fccHandler,                                 "fccHandler");
    }
    Skip_L4(                                                    "Flags");
    Skip_L2(                                                    "Priority");
    Skip_L2(                                                    "Language");
    Skip_L4(                                                    "InitialFrames");
    Get_L4 (Scale,                                              "Scale");
    Get_L4 (Rate,                                               "Rate"); //Rate/Scale is stream tick rate in ticks/sec
    Get_L4 (Start,                                              "Start");
    Get_L4 (Length,                                             "Length");
    Skip_L4(                                                    "SuggestedBufferSize");
    Skip_L4(                                                    "Quality");
    Skip_L4(                                                    "SampleSize");
    Get_L2 (Left,                                               "Frame_Left");
    Get_L2 (Top,                                                "Frame_Top");
    Get_L2 (Right,                                              "Frame_Right");
    Get_L2 (Bottom,                                             "Frame_Bottom");
    if(Element_Offset<Element_Size)
        Skip_XX(Element_Size-Element_Offset,                    "Unknown");

    //Filling
    float32 FrameRate=0;
    if (Rate>0 && Scale>0)
    {
        FrameRate=((float32)Rate)/Scale;
        int64u Duration=float32_int64s((1000*(float32)Length)/FrameRate);
        if (avih_TotalFrame>0 //avih_TotalFrame is here because some files have a wrong Audio Duration if TotalFrame==0 (which is a bug, of course!)
        && (avih_FrameRate==0 || Duration<((float32)avih_TotalFrame)/avih_FrameRate*1000*1.10)  //Some file have a nearly perfect header, except that the value is false, trying to detect it (false if 10% more than 1st video)
        && (avih_FrameRate==0 || Duration>((float32)avih_TotalFrame)/avih_FrameRate*1000*0.90)) //Some file have a nearly perfect header, except that the value is false, trying to detect it (false if 10% less than 1st video)
            Fill(StreamKind_Last, StreamPos_Last, "Duration", Duration);
    }
    switch (fccType)
    {
        case Elements::AVI__hdlr_strl_strh_iavs :
        case Elements::AVI__hdlr_strl_strh_vids :
            if (FrameRate>0)  Fill(StreamKind_Last, StreamPos_Last, "FrameRate", FrameRate, 3);
            if (fccHandler==0x64767364) //For "dvsd" but fccType is wrong
                Fill(StreamKind_Last, StreamPos_Last, "PixelAspectRatio", 1.000);
        case Elements::AVI__hdlr_strl_strh_txts :
            if (Right-Left>0) Fill(StreamKind_Last, StreamPos_Last, "Width",  Right-Left);
            if (Bottom-Top>0) Fill(StreamKind_Last, StreamPos_Last, "Height", Bottom-Top);
            break;
        default: ;
    }
    Stream[Stream_ID].fccType=fccType;
    Stream[Stream_ID].fccHandler=fccHandler;
    Stream[Stream_ID].Rate=Rate;
    Stream[Stream_ID].Start=Start;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strn()
{
    Element_Name("Stream name");

    //Parsing
    Ztring Title;
    Get_Local(Element_Size, Title,                              "StreamName");

    //Filling
    Fill(StreamKind_Last, StreamPos_Last, "Title", Title);
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_vprp()
{
    Element_Name("Video properties");

    //Parsing
    int32u FieldPerFrame;
    Skip_L4(                                                    "VideoFormatToken");
    Skip_L4(                                                    "VideoStandard");
    Skip_L4(                                                    "VerticalRefreshRate");
    Skip_L4(                                                    "HTotalInT");
    Skip_L4(                                                    "VTotalInLines");
    Skip_L4(                                                    "FrameAspectRatio");
    Skip_L4(                                                    "FrameWidthInPixels");
    Skip_L4(                                                    "FrameHeightInLines");
    Get_L4 (FieldPerFrame,                                      "FieldPerFrame");
    for (int32u Pos=0; Pos<FieldPerFrame; Pos++)
    {
        Skip_L4(                                                "CompressedBMHeight");
        Skip_L4(                                                "CompressedBMWidth");
        Skip_L4(                                                "ValidBMHeight");
        Skip_L4(                                                "ValidBMWidth");
        Skip_L4(                                                "ValidBMXOffset");
        Skip_L4(                                                "ValidBMYOffset");
        Skip_L4(                                                "VideoXOffsetInT");
        Skip_L4(                                                "VideoYValidStartLine");
    }
    if(Element_Offset<Element_Size)
        Skip_XX(Element_Size-Element_Offset,                    "Unknown");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_xxxx()
{
    AVI__INFO_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__idx1()
{
    Element_Name("Index (old)");

    //Tests
    if (!NeedOldIndex || Idx1_Offset==(int64u)-1)
    {
        Skip_XX(Element_TotalSize_Get(),                            "Data");
        return;
    }
    else if (!Element_IsComplete_Get())
    {
        Element_WaitForMoreData();
        return;         
    }

    //Testing malformed index (index is based on start of the file, wrong)
    if (16<=Element_Size && Idx1_Offset+4==LittleEndian2int32u(Buffer+Buffer_Offset+(size_t)Element_Offset+ 8))
        Idx1_Offset=0; //Fixing base of movi atom, the index think it is the start of the file

    //Parsing
    std::map <int64u, size_t> Stream_Count;
    while (Element_Offset<Element_Size)
    {
        //Is too slow
        /*
        int32u ChunkID, Offset, Size;
        Element_Begin("Index");
        Get_C4 (ChunkID,                                        "ChunkID"); //Bit 31 is set if this is NOT a keyframe
        Info_L4(Flags,                                          "Flags");
            Skip_Flags(Flags, 0,                                "NoTime");
            Skip_Flags(Flags, 1,                                "Lastpart");
            Skip_Flags(Flags, 2,                                "Firstpart");
            Skip_Flags(Flags, 3,                                "Midpart");
            Skip_Flags(Flags, 4,                                "KeyFrame");
        Get_L4 (Offset,                                         "Offset"); //qwBaseOffset + this is absolute file offset
        Get_L4 (Size,                                           "Size"); //Bit 31 is set if this is NOT a keyframe
        Element_Info(Ztring().From_CC4(ChunkID));
        Element_Info(Size);

        //Stream Pos and Size
        int32u StreamID_Temp=(ChunkID&0xFFFF0000);
        Stream[StreamID_Temp].StreamSize+=Size;
        Stream_Pos[Idx1_Offset+Offset]=StreamID_Temp;
        Element_End();
        */

        //Faster method
        int32u StreamID=BigEndian2int32u   (Buffer+Buffer_Offset+(size_t)Element_Offset   )&0xFFFF0000;
        int32u Offset  =LittleEndian2int32u(Buffer+Buffer_Offset+(size_t)Element_Offset+ 8);
        int32u Size    =LittleEndian2int32u(Buffer+Buffer_Offset+(size_t)Element_Offset+12);
        Stream[StreamID].StreamSize+=Size;
        Stream[StreamID].PacketCount++;
        Stream_Structure[Idx1_Offset+Offset].Name=StreamID;
        Stream_Structure[Idx1_Offset+Offset].Size=Size;
        Element_Offset+=16;
    }

    //Interleaved
    size_t Pos0=0;
    size_t Pos1=0;
    for (std::map<int64u, stream_structure>::iterator Temp=Stream_Structure.begin(); Temp!=Stream_Structure.end(); Temp++)
    {
        switch (Temp->second.Name)
        {
            case 0x30300000 :
                if (Interleaved0_1==0) Interleaved0_1=Temp->first;
                if (Interleaved0_10==0)
                {
                    Pos0++;
                    if (Pos0>1)
                        Interleaved0_10=Temp->first;
                }
                break;
            case 0x30310000 :
                if (Interleaved1_1==0) Interleaved1_1=Temp->first;
                if (Interleaved1_10==0)
                {
                    Pos1++;
                    if (Pos1>1)
                        Interleaved1_10=Temp->first;
                }
                break;
            default:;
        }
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__INFO()
{
    Element_Name("Tags");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__INFO_IID3()
{
    Element_Name("ID3 Tag");

    //Parsing
    #if defined(MEDIAINFO_ID3_YES)
        File_Id3 MI;
        Open_Buffer_Init(&MI);
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Open_Buffer_Finalize(&MI);
        Merge(MI, Stream_General, 0, 0);
    #endif
}

//---------------------------------------------------------------------------
void File_Riff::AVI__INFO_ILYC()
{
    Element_Name("Lyrics");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__INFO_IMP3()
{
    Element_Name("MP3 Information");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__INFO_JUNK()
{
    Element_Name("Garbage");
}

//---------------------------------------------------------------------------
// List of information atoms
// Name                             X bytes, Pos=0
//
void File_Riff::AVI__INFO_xxxx()
{
    //Parsing
    Ztring Value;
    Get_Local(Element_Size, Value,                              "Value");

    //Filling
    stream_t StreamKind=Stream_General;
    size_t StreamPos=0;
    size_t Parameter=(size_t)-1;
    switch (Element_Code)
    {
        case 0x00000000               : Parameter=General_Comment; break;
        case Elements::AVI__INFO_IARL : Parameter=General_Archival_Location; break;
        case Elements::AVI__INFO_IART : Parameter=General_Director; break;
        case Elements::AVI__INFO_IAS1 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=0; break;
        case Elements::AVI__INFO_IAS2 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=1; break;
        case Elements::AVI__INFO_IAS3 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=2; break;
        case Elements::AVI__INFO_IAS4 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=3; break;
        case Elements::AVI__INFO_IAS5 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=4; break;
        case Elements::AVI__INFO_IAS6 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=5; break;
        case Elements::AVI__INFO_IAS7 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=6; break;
        case Elements::AVI__INFO_IAS8 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=7; break;
        case Elements::AVI__INFO_IAS9 : Parameter=Audio_Language; StreamKind=Stream_Audio; StreamPos=8; break;
        case Elements::AVI__INFO_ICDS : Parameter=General_CostumeDesigner; break;
        case Elements::AVI__INFO_ICMS : Parameter=General_CommissionedBy; break;
        case Elements::AVI__INFO_ICMT : Parameter=General_Comment; break;
        case Elements::AVI__INFO_ICNM : Parameter=General_DirectorOfPhotography; break;
        case Elements::AVI__INFO_ICNT : Parameter=General_Movie_Country; break;
        case Elements::AVI__INFO_ICOP : Parameter=General_Copyright; break;
        case Elements::AVI__INFO_ICRD : Parameter=General_Recorded_Date; Value.Date_From_String(Value.To_Local().c_str()); break;
        case Elements::AVI__INFO_ICRP : Parameter=General_Cropped; break;
        case Elements::AVI__INFO_IDIM : Parameter=General_Dimensions; break;
        case Elements::AVI__INFO_IDIT : Parameter=General_Mastered_Date; Value.Date_From_String(Value.To_Local().c_str()); break;
        case Elements::AVI__INFO_IDPI : Parameter=General_DotsPerInch; break;
        case Elements::AVI__INFO_IDST : Parameter=General_DistributedBy; break;
        case Elements::AVI__INFO_IEDT : Parameter=General_EditedBy; break;
        case Elements::AVI__INFO_IENG : Parameter=General_EncodedBy; break;
        case Elements::AVI__INFO_IGNR : Parameter=General_Genre; break;
        case Elements::AVI__INFO_IFRM : Parameter=General_Part_Position_Total; break;
        case Elements::AVI__INFO_IKEY : Parameter=General_Keywords; break;
        case Elements::AVI__INFO_ILGT : Parameter=General_Lightness; break;
        case Elements::AVI__INFO_ILNG : Parameter=Audio_Language; StreamKind=Stream_Audio; break;
        case Elements::AVI__INFO_IMED : Parameter=General_OriginalSourceMedium; break;
        case Elements::AVI__INFO_IMUS : Parameter=General_MusicBy; break;
        case Elements::AVI__INFO_INAM : Parameter=General_Title; break;
        case Elements::AVI__INFO_IPDS : Parameter=General_ProductionDesigner; break;
        case Elements::AVI__INFO_IPLT : Parameter=General_OriginalSourceForm_NumColors; break;
        case Elements::AVI__INFO_IPRD : Parameter=General_OriginalSourceForm_Name; break;
        case Elements::AVI__INFO_IPRO : Parameter=General_Producer; break;
        case Elements::AVI__INFO_IPRT : Parameter=General_Part_Position; break;
        case Elements::AVI__INFO_IRTD : Parameter=General_LawRating; break;
        case Elements::AVI__INFO_ISBJ : Parameter=General_Subject; break;
        case Elements::AVI__INFO_ISFT : Parameter=General_Encoded_Application; break;
        case Elements::AVI__INFO_ISGN : Parameter=General_Genre; break;
        case Elements::AVI__INFO_ISHP : Parameter=General_OriginalSourceForm_Sharpness; break;
        case Elements::AVI__INFO_ISRC : Parameter=General_OriginalSourceForm_DistributedBy; break;
        case Elements::AVI__INFO_ISRF : Parameter=General_OriginalSourceForm; break;
        case Elements::AVI__INFO_ISTD : Parameter=General_ProductionStudio; break;
        case Elements::AVI__INFO_ISTR : Parameter=General_Performer; break;
        case Elements::AVI__INFO_ITCH : Parameter=General_EncodedBy; break;
        case Elements::AVI__INFO_IWEB : Parameter=General_Movie_Url; break;
        case Elements::AVI__INFO_IWRI : Parameter=General_WrittenBy; break;
        default                       : ;
    }
    Element_Name(MediaInfoLib::Config.Info_Get(StreamKind, Parameter, Info_Name));
    Element_Info(Value);
    if (!Value.empty())
    {
        if (Parameter!=(size_t)-1)
            Fill(StreamKind, StreamPos, Parameter, Value);
        else
            Fill(StreamKind, StreamPos, Ztring().From_CC4((int32u)Element_Code).To_Local().c_str(), Value, true);
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__JUNK()
{
    Element_Name("Garbage"); //Library defined size for padding, often used to store library name

    //Detect DivX files
         if (CC5(Buffer+Buffer_Offset)==CC5("DivX "))
    {
        Fill(Stream_General, 0, General_Format, "DivX", Unlimited, true, true);
    }
    //MPlayer
    else if (CC8(Buffer+Buffer_Offset)==CC8("[= MPlay") && Retrieve(Stream_General, 0, General_Encoded_Library).empty())
        Fill(Stream_General, 0, General_Encoded_Library, "MPlayer");
    //FFMpeg broken files detection
    else if (CC8(Buffer+Buffer_Offset)==CC8("odmldmlh"))
        dmlh_TotalFrame=0; //this is not normal to have this string in a JUNK block!!! and in files tested, in this case TotalFrame is broken too
    //VirtualDubMod
    else if (CC8(Buffer+Buffer_Offset)==CC8("INFOISFT"))
    {
        int32u Size=LittleEndian2int32u(Buffer+Buffer_Offset+8);
        if (Size>Element_Size-12)
            Size=(int32u)Element_Size-12;
        Fill(Stream_General, 0, General_Encoded_Library, (const char*)(Buffer+Buffer_Offset+12), Size);
    }
    else if (CC8(Buffer+Buffer_Offset)==CC8("INFOIENG"))
    {
        int32u Size=LittleEndian2int32u(Buffer+Buffer_Offset+8);
        if (Size>Element_Size-12)
            Size=(int32u)Element_Size-12;
        Fill(Stream_General, 0, General_Encoded_Library, (const char*)(Buffer+Buffer_Offset+12), Size);
    }
    //Other libraries?
    else if (CC1(Buffer+Buffer_Offset)>=CC1("A") && CC1(Buffer+Buffer_Offset)<=CC1("z") && Retrieve(Stream_General, 0, General_Encoded_Library).empty())
        Fill(Stream_General, 0, General_Encoded_Library, (const char*)(Buffer+Buffer_Offset), (size_t)Element_Size);
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi()
{
    Element_Name("Datas");

    //Only the first time, no need in AVIX
    if (movi_Size==0)
    {
        Idx1_Offset=File_Offset+Buffer_Offset-4;
        BookMark_Set(); //Remenbering this place, for stream parsing in phase 2

        //For each stream
        std::map<int32u, stream>::iterator Temp=Stream.begin();
        while (Temp!=Stream.end())
        {
            if (!Temp->second.Parser && Temp->second.fccType!=Elements::AVI__hdlr_strl_strh_txts)
            {
                Temp->second.SearchingPayload=false;
                stream_Count--;
            }
            Temp++;
        }
    }

    //Filling
    movi_Size+=Element_TotalSize_Get();

    //We must parse moov?
    if (NeedOldIndex || (stream_Count==0 && Index_Pos.empty()))
    {
        //Jumping
        Skip_XX(Element_TotalSize_Get(),                        "Data");
        return;
    }

    //Jump to next useful data
    AVI__movi_StreamJump();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_rec_()
{
    Element_Name("Syncronisation");

    rec__Present=true;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_rec__xxxx()
{
    AVI__movi_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx()
{
    if (Element_Code==Elements::AVI__JUNK)
    {
        Skip_XX(Element_Size,                                    "Junk");
        AVI__movi_StreamJump();
        return;
    }

    Stream_ID=(int32u)(Element_Code&0xFFFF0000);

    Demux(Buffer+Buffer_Offset, (size_t)Element_Size, Ztring().From_CC4((int32u)Element_Code)+_T(".raw"));

    if (Stream_ID==0x69780000) //ix..
    {
        //AVI Standard Index Chunk
        AVI__hdlr_strl_indx();
        Stream_ID=(int32u)(Element_Code&0x0000FFFF)<<16;
        AVI__movi_StreamJump();
        return;
    }
    if ((Element_Code&0x0000FFFF)==0x00006978) //..ix (Out of specs, but found in a Adobe After Effects CS4 DV file
    {
        //AVI Standard Index Chunk
        AVI__hdlr_strl_indx();
        Stream_ID=(int32u)(Element_Code&0xFFFF0000);
        AVI__movi_StreamJump();
        return;
    }

    //Finished?
    if (!Stream[Stream_ID].SearchingPayload)
    {
        Element_DoNotShow();
        AVI__movi_StreamJump();
        return;
    }

    Stream[Stream_ID].PacketPos++;
    if (MediaInfoLib::Config.Details_Get())
    {
        switch (Element_Code&0x0000FFFF) //2 last bytes
        {
            case Elements::AVI__movi_xxxx_____ : Element_Info("DV"); break;
            case Elements::AVI__movi_xxxx___db :
            case Elements::AVI__movi_xxxx___dc : Element_Info("Video"); break;
            case Elements::AVI__movi_xxxx___sb :
            case Elements::AVI__movi_xxxx___tx : Element_Info("Text"); break;
            case Elements::AVI__movi_xxxx___wb : Element_Info("Audio"); break;
            default :                            Element_Info("Unknown"); break;
        }
        Element_Info(Stream[Stream_ID].PacketPos);
    }

    //Some specific stuff
    switch (Element_Code&0x0000FFFF) //2 last bytes
    {
        case Elements::AVI__movi_xxxx___tx : AVI__movi_xxxx___tx(); break;
        default : ;
    }

    //Parsing
    if (Stream[Stream_ID].Parser)
    {
        Open_Buffer_Continue(Stream[Stream_ID].Parser, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        if (Stream[Stream_ID].Parser->Buffer_Size>0)
            Stream[Stream_ID].ChunksAreComplete=false;
    }

    //Some specific stuff
    switch (Element_Code&0x0000FFFF) //2 last bytes
    {
        case Elements::AVI__movi_xxxx_____ :
        case Elements::AVI__movi_xxxx___db :
        case Elements::AVI__movi_xxxx___dc : AVI__movi_xxxx___dc(); break;
        case Elements::AVI__movi_xxxx___wb : AVI__movi_xxxx___wb(); break;
        default : ;
    }

    //We must always parse moov?
    AVI__movi_StreamJump();

    Element_Show();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx___dc()
{
    //Finalize (if requested)
    if (Stream[Stream_ID].Parser==NULL
    #if defined(MEDIAINFO_MPEG4V_YES)
     || Stream[Stream_ID].Specific_IsMpeg4v && ((File_Mpeg4v*)Stream[Stream_ID].Parser)->Frame_Count_InThisBlock>1 //Searching Packet bitstream, no more need if found
    #endif
     || Stream[Stream_ID].Parser->IsFinished
     || Stream[Stream_ID].PacketPos>=300)
    {
        Stream[Stream_ID].SearchingPayload=false;
        stream_Count--;
        return;
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx___tx()
{
    //Parsing
    int32u Name_Size;
    Ztring Value;
    Skip_C4(                                                    "GAB2");
    Skip_L1(                                                    "Zero");
    Skip_L2(                                                    "CodePage"); //2=Unicode
    Get_L4 (Name_Size,                                          "Name_Size");
    Skip_UTF16L(Name_Size,                                      "Name");
    Skip_L2(                                                    "Four");
    Skip_L4(                                                    "File_Size");

    //Skip it
    Stream[Stream_ID].SearchingPayload=false;
    stream_Count--;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx___wb()
{
    //Finalize (if requested)
    if ( Stream[Stream_ID].PacketPos>=4 //For having the chunk alignement
     && (Stream[Stream_ID].Parser==NULL
      || Stream[Stream_ID].Parser->IsFilled
      || Stream[Stream_ID].PacketPos>=300)
      || Element_Size>50000) //For PCM, we disable imediatly
    {
        Stream[Stream_ID].SearchingPayload=false;
        stream_Count--;
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_StreamJump()
{
    //Jump to next useful data
    if (!Index_Pos.empty())
    {
        if (Index_Pos.begin()->first<=File_Offset+Buffer_Offset && Element_Code!=Elements::AVI__movi)
            Index_Pos.erase(Index_Pos.begin());
        int64u ToJump=File_Size;
        if (!Index_Pos.empty())
            ToJump=Index_Pos.begin()->first;
        if (ToJump>File_Size)
            ToJump=File_Size;
        if (ToJump>=File_Offset+Buffer_Offset+Element_TotalSize_Get(Element_Level-2)) //We want always Element movi
            File_GoTo=File_Offset+Buffer_Offset+Element_TotalSize_Get(Element_Level-2); //Not in this chunk
        else if (ToJump!=File_Offset+Buffer_Offset+(Element_Code==Elements::AVI__movi?0:Element_Size))
            File_GoTo=ToJump; //Not just after
    }
    else if (stream_Count==0)
    {
        //Jumping
        Element_Show();
        if (rec__Present)
            Element_End();
        Info("movi, Jumping to end of chunk");
        if (SecondPass)
            Finish("AVI"); //The rest is already parsed
        else
            GoTo(File_Offset+Buffer_Offset+Element_TotalSize_Get(), "AVI");
    }
    else if (Stream_Structure_Temp!=Stream_Structure.end())
    {
        do
            Stream_Structure_Temp++;
        while (Stream_Structure_Temp!=Stream_Structure.end() && !Stream[(int32u)Stream_Structure_Temp->second.Name].SearchingPayload);
        if (Stream_Structure_Temp!=Stream_Structure.end())
        {
            int64u ToJump=Stream_Structure_Temp->first;
            if (ToJump>=File_Offset+Buffer_Offset+Element_TotalSize_Get(Element_Level-2))
                File_GoTo=File_Offset+Buffer_Offset+Element_TotalSize_Get(Element_Level-2); //Not in this chunk
            else if (ToJump!=File_Offset+Buffer_Offset+Element_Size)
                File_GoTo=ToJump; //Not just after
        }
        else
            Finish("AVI");
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__xxxx()
{
    Stream_ID=(int32u)(Element_Code&0xFFFF0000);

    if (Stream_ID==0x69780000) //ix..
    {
        //AVI Standard Index Chunk
        AVI__hdlr_strl_indx();
        Stream_ID=(int32u)(Element_Code&0x0000FFFF)<<16;
        AVI__movi_StreamJump();
        return;
    }
    if ((Element_Code&0x0000FFFF)==0x00006978) //..ix (Out of specs, but found in a Adobe After Effects CS4 DV file
    {
        //AVI Standard Index Chunk
        AVI__hdlr_strl_indx();
        Stream_ID=(int32u)(Element_Code&0xFFFF0000);
        AVI__movi_StreamJump();
        return;
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVIX()
{
    //Filling
    Fill(Stream_General, 0, General_Format_Profile, "OpenDML", Unlimited, true, true);
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_idx1()
{
    AVI__idx1();
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_movi()
{
    AVI__movi();
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_movi_rec_()
{
    AVI__movi_rec_();
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_movi_rec__xxxx()
{
    AVIX_movi_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_movi_xxxx()
{
    AVI__movi_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::CADP()
{
    Element_Name("CMP4 - ADPCM");

    //Parsing
    int32u Codec;
    Get_C4 (Codec,                                              "Codec");
    Skip_XX(Element_TotalSize_Get()-Element_Offset,             "Data");

    FILLING_BEGIN();
        Stream_Prepare(Stream_Audio);
        if (Codec==0x41647063) //Adpc
            Fill(Stream_Audio, StreamPos_Last, Audio_Format, "ADPCM");
        Fill(Stream_Audio, StreamPos_Last, Audio_StreamSize, Element_TotalSize_Get());
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::CMJP()
{
    Element_Name("CMP4 - MJPEG");

    //Parsing
    #ifdef MEDIAINFO_JPEG_YES
        Stream_ID=0;
        Stream[Stream_ID].Parser=new File_Jpeg;
        Open_Buffer_Init(Stream[Stream_ID].Parser);
        ((File_Jpeg*)Stream[Stream_ID].Parser)->StreamKind=Stream_Video;
        Open_Buffer_Continue(Stream[Stream_ID].Parser, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Open_Buffer_Finalize(Stream[Stream_ID].Parser);
        Element_Offset=Element_Size;
        Skip_XX(Element_TotalSize_Get()-Element_Offset,         "Other data");

        FILLING_BEGIN();
            Stream_Prepare(Stream_Video);
            Fill(Stream_Video, StreamPos_Last, Video_StreamSize, Element_TotalSize_Get());
            Merge(*Stream[Stream_ID].Parser, StreamKind_Last, 0, StreamPos_Last);
        FILLING_END();
    #else
        Skip_XX(Element_TotalSize_Get(),                        "Data");

        FILLING_BEGIN();
            Stream_Prepare(Stream_Video);
            Fill(Stream_Video, StreamKind_Last, Video_Format, "MJPEG");
            Fill(Stream_Video, StreamKind_Last, Video_StreamSize, Element_TotalSize_Get());
        FILLING_END();
    #endif
}

//---------------------------------------------------------------------------
void File_Riff::CMP4()
{
    Accept("CMP4");
    Element_Name("CMP4 - Header");

    //Parsing
    Ztring Title;
    Get_Local(Element_Size, Title,                              "Title");

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, "Format", "CMP4");
        Fill(Stream_General, 0, "Title", Title);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::IDVX()
{
    Element_Name("Tags");
}

//---------------------------------------------------------------------------
void File_Riff::JUNK()
{
    Element_Name("Junk");

    //Parse
    Skip_XX(Element_TotalSize_Get(),                            "Junk");
}

//---------------------------------------------------------------------------
void File_Riff::menu()
{
    Element_Name("DivX Menu");

    //Filling
    Stream_Prepare(Stream_Menu);
    Fill(Stream_Menu, StreamPos_Last, Menu_Format, "DivX Menu");
    Fill(Stream_Menu, StreamPos_Last, Menu_Codec, "DivX");
}

//---------------------------------------------------------------------------
void File_Riff::MThd()
{
    Element_Name("MIDI header");

    //Parsing
    Skip_B2(                                                    "format");
    Skip_B2(                                                    "ntrks");
    Skip_B2(                                                    "division");

    FILLING_BEGIN_PRECISE();
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "MIDI");

        Accept("MIDI");
    FILLING_ELSE();
        Reject("MIDI");
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::MTrk()
{
    Element_Name("MIDI Track");

    //Parsing
    Skip_XX(Element_TotalSize_Get(),                            "Data");

    FILLING_BEGIN();
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, StreamPos_Last, Audio_Format, "MIDI");
        Fill(Stream_Audio, StreamPos_Last, Audio_Codec, "Midi");

        Finish("MIDI");
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::PAL_()
{
    Accept("RIFF Palette");
    Element_Name("Format: RIFF Palette");

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "RIFF Palette");
}

//---------------------------------------------------------------------------
void File_Riff::RDIB()
{
    Accept("RIFF DIB");
    Element_Name("Format: RIFF DIB");

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "RIFF DIB");
}

//---------------------------------------------------------------------------
void File_Riff::RMID()
{
    Accept("RIFF MIDI");
    Element_Name("Format: RIFF MIDI");

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "RIFF MIDI");
}

//---------------------------------------------------------------------------
void File_Riff::RMMP()
{
    Accept("RIFF MMP");
    Element_Name("Format: RIFF MMP");

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "RIFF MMP");
}

//---------------------------------------------------------------------------
void File_Riff::RMP3()
{
    Accept("RMP3");
    Element_Name("Format: RMP3");

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "RMP3");
}

//---------------------------------------------------------------------------
void File_Riff::RMP3_data()
{
    Element_Name("Raw datas");

    //Parsing
    #if defined(MEDIAINFO_MPEGA_YES)
        File_Mpega MI;
        Open_Buffer_Init(&MI);
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Open_Buffer_Finalize(&MI);
        Merge(MI, Stream_Audio, 0, 0);
    #else
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, "Codec", "MPEG1/2 Audio");
    #endif

    //Positionning
    Element_Offset+=Element_TotalSize_Get();
}

//---------------------------------------------------------------------------
void File_Riff::SMV0()
{
    Accept("SMV");

    //Parsing
    int8u Version;
    Skip_C1(                                                    "Identifier (continuing)");
    Get_C1 (Version,                                            "Version");
    Skip_C3(                                                    "Identifier (continuing)");
    if (Version=='1')
    {
        int32u Width, Height, FrameRate, BlockSize, FrameCount;
        Get_B3 (Width,                                          "Width");
        Get_B3 (Height,                                         "Height");
        Skip_B3(                                                "0x000010");
        Skip_B3(                                                "0x000001");
        Get_B3 (BlockSize,                                      "Block size");
        Get_B3 (FrameRate,                                      "Frame rate");
        Get_B3 (FrameCount,                                     "Frame count");
        Skip_B3(                                                "0x000000");
        Skip_B3(                                                "0x000000");
        Skip_B3(                                                "0x000000");
        Skip_B3(                                                "0x010101");
        Skip_B3(                                                "0x010101");
        Skip_B3(                                                "0x010101");
        Skip_B3(                                                "0x010101");

        //Filling
        Fill(Stream_General, 0, General_Format_Profile, "SMV v1");
        Stream_Prepare(Stream_Video);
        Fill(Stream_Video, 0, Video_MuxingMode, "SMV v1");
        Fill(Stream_Video, 0, Video_Width, Width);
        Fill(Stream_Video, 0, Video_Height, Height);
        Fill(Stream_Video, 0, Video_FrameRate, (float)FrameRate);
        Fill(Stream_Video, 0, Video_FrameCount, FrameCount);

        Finish("SMV");
    }
    else if (Version=='2')
    {
        int32u Width, Height, FrameRate;
        Get_L3 (Width,                                          "Width");
        Get_L3 (Height,                                         "Height");
        Skip_L3(                                                "0x000010");
        Skip_L3(                                                "0x000001");
        Get_L3 (SMV_BlockSize,                                  "Block size");
        Get_L3 (FrameRate,                                      "Frame rate");
        Get_L3 (SMV_FrameCount,                                 "Frame count");
        Skip_L3(                                                "0x000001");
        Skip_L3(                                                "0x000000");
        Skip_L3(                                                "Frame rate");
        Skip_L3(                                                "0x010101");
        Skip_L3(                                                "0x010101");
        Skip_L3(                                                "0x010101");
        Skip_L3(                                                "0x010101");

        //Filling
        SMV_BlockSize+=3;
        SMV_FrameCount++;
        Fill(Stream_General, 0, General_Format_Profile, "SMV v2");
        Stream_Prepare(Stream_Video);
        Fill(Stream_Video, 0, Video_Format, "MJPEG");
        Fill(Stream_Video, 0, Video_Codec,  "MJPEG");
        Fill(Stream_Video, 0, Video_MuxingMode, "SMV v2");
        Fill(Stream_Video, 0, Video_Width, Width);
        Fill(Stream_Video, 0, Video_Height, Height);
        Fill(Stream_Video, 0, Video_FrameRate, FrameRate);
        Fill(Stream_Video, 0, Video_FrameCount, SMV_FrameCount);
        Fill(Stream_Video, 0, Video_StreamSize, SMV_BlockSize*SMV_FrameCount);
    }
    else
        Finish("SMV");
}

//---------------------------------------------------------------------------
void File_Riff::SMV0_xxxx()
{
    //Parsing
    int32u Size;
    Get_L3 (Size,                                              "Size");
    #if defined(MEDIAINFO_JPEG_YES)
        //Creating the parser
        File_Jpeg MI;
        Open_Buffer_Init(&MI);

        //Parsing
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Offset, Size);
        Open_Buffer_Finalize(&MI);

        //Filling
        Merge(MI, Stream_Video, 0, StreamPos_Last);

        //Positioning
        Element_Offset+=Size;
    #else
        //Parsing
        Skip_XX(Size,                                           "JPEG data");
    #endif
    Skip_XX(Element_Size-Element_Offset,                        "Padding");

    //Filling
    Data_GoTo(File_Offset+Buffer_Offset+(size_t)Element_Size+(SMV_FrameCount-1)*SMV_BlockSize, "SMV");
    SMV_BlockSize=0;
}

//---------------------------------------------------------------------------
void File_Riff::WAVE()
{
    Data_Accept("Wave");
    Element_Name("Format: Wave");

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "Wave");
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_bext()
{
    Element_Name("Broadcast extension");

    //Parsing
    Ztring Description, Originator, OriginationDate, OriginationTime, History;
    int16u Version;
    Get_Local(256, Description,                                 "Description");
    Get_Local( 32, Originator,                                  "Originator");
    Skip_Local(32,                                              "OriginatorReference");
    Get_Local( 10, OriginationDate,                             "OriginationDate");
    Get_Local(  8, OriginationTime,                             "OriginationTime");
    Get_L8   (     TimeReference,                               "TimeReference"); //To be divided by SamplesPerSec
    Get_L2   (     Version,                                     "Version");
    if (Version==1)
        Skip_XX(64,                                             "UMID");
    Skip_XX  (602-Element_Offset,                               "Reserved");
    if (Element_Offset<Element_Size)
        Get_Local(Element_Size-Element_Offset, History,         "History");

    FILLING_BEGIN();
        Fill(Stream_General, 0, General_Description, Description);
        Fill(Stream_General, 0, General_Producer, Originator);
        Fill(Stream_General, 0, General_Encoded_Date, _T("UTC ")+OriginationDate+_T(' ')+OriginationTime);
        Fill(Stream_General, 0, General_Encoded_Library_Settings, History);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_data()
{
    Element_Name("Raw datas");

    if (Element_TotalSize_Get()<100)
    {
        Skip_XX(Element_Size,                                   "Unknown");
        return; //This is maybe embeded in another container, and there is only the header (What is the junk?)
    }

    //Parsing
    Element_Code=CC4("00wb");
    AVI__movi_xxxx();
    if (File_GoTo==(int64u)-1)
        Skip_XX(Element_TotalSize_Get()-Element_Offset,         "Data");

    FILLING_BEGIN();
        Fill(Stream_Audio, 0, Audio_StreamSize, Element_TotalSize_Get());
        int64u Duration=Retrieve(Stream_Audio, 0, Audio_Duration).To_int64u();
        int64u BitRate=Retrieve(Stream_Audio, 0, Audio_BitRate).To_int64u();
        if (Duration)
        {
            int64u BitRate_New=Element_TotalSize_Get()*8*1000/Duration;
            if (BitRate_New<BitRate*0.95 || BitRate_New>BitRate*1.05)
                Fill(Stream_Audio, 0, Audio_BitRate, BitRate_New, 10, true); //Correcting the bitrate, it was false in the header
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_ds64()
{
    Element_Name("DataSize64");

    //Parsing
    int32u tableLength;
    Skip_L8(                                                    "riffSize"); //Is directly read from the header parser
    Get_L8 (WAVE_data_Size,                                     "dataSize");
    Get_L8 (WAVE_fact_samplesCount,                             "sampleCount");
    Get_L4 (tableLength,                                        "tableLength");
    for (int32u Pos=0; Pos<tableLength; Pos++)
        Skip_L8(                                                "table[]");
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_fact()
{
    Element_Name("Sample count");

    //Parsing
    int64u SamplesCount64;
    int32u SamplesCount;
    Get_L4 (SamplesCount,                                       "SamplesCount");
    SamplesCount64=SamplesCount;
    if (SamplesCount64==0xFFFFFFFF)
        SamplesCount64=SamplesCount64;

    FILLING_BEGIN();
        int32u SamplingRate=Retrieve(Stream_Audio, 0, Audio_SamplingRate).To_int32u();
        if (SamplingRate)
            Fill(Stream_Audio, 0, Audio_Duration, (SamplesCount64*1000)/SamplingRate);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_fmt_()
{
    //Compute the current codec ID
    Stream_ID=0x30300000;
    stream_Count=1;

    Stream[0x30300000].fccType=Elements::AVI__hdlr_strl_strh_auds;
    AVI__hdlr_strl_strf();
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_ID3_()
{
    Element_Name("ID3v2 tags");

    //Parsing
    #if defined(MEDIAINFO_ID3V2_YES)
        File_Id3v2 MI;
        Open_Buffer_Init(&MI);
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
        Open_Buffer_Finalize(&MI);
        Merge(MI, Stream_General, 0, 0);
    #endif
}

//---------------------------------------------------------------------------
void File_Riff::W3DI()
{
    Element_Name("IDVX tags (Out of specs!)");

    //Parsing
    int32u Size=(int32u)Element_Size;
    Ztring Title, Artist, Album, Unknown, Genre, Comment;
    int32u TrackPos;
    Get_Local(Size, Title,                                      "Title");
    Element_Offset=(int32u)Title.size();
    Size-=(int32u)Title.size();
    if (Size==0) return;
    Skip_L1(                                                    "Zero"); Size--; //NULL char
    Get_Local(Size, Artist,                                     "Artist");
    Element_Offset=(int32u)Title.size()+1+(int32u)Artist.size();
    Size-=(int32u)Artist.size();
    if (Size==0) return;
    Skip_L1(                                                    "Zero"); Size--; //NULL char
    Get_Local(Size, Album,                                      "Album");
    Element_Offset=(int32u)Title.size()+1+(int32u)Artist.size()+1+(int32u)Album.size();
    Size-=(int32u)Album.size();
    if (Size==0) return;
    Skip_L1(                                                    "Zero"); Size--; //NULL char
    Get_Local(Size, Unknown,                                    "Unknown");
    Element_Offset=(int32u)Title.size()+1+(int32u)Artist.size()+1+(int32u)Album.size()+1+(int32u)Unknown.size();
    Size-=(int32u)Unknown.size();
    if (Size==0) return;
    Skip_L1(                                                    "Zero"); Size--; //NULL char
    Get_Local(Size, Genre,                                      "Genre");
    Element_Offset=(int32u)Title.size()+1+(int32u)Artist.size()+1+(int32u)Album.size()+1+(int32u)Unknown.size()+1+(int32u)Genre.size();
    Size-=(int32u)Genre.size();
    if (Size==0) return;
    Skip_L1(                                                    "Zero"); Size--; //NULL char
    Get_Local(Size, Comment,                                    "Comment");
    Element_Offset=(int32u)Title.size()+1+(int32u)Artist.size()+1+(int32u)Album.size()+1+(int32u)Unknown.size()+1+(int32u)Genre.size()+1+(int32u)Comment.size();
    Size-=(int32u)Comment.size();
    if (Size==0) return;
    Skip_L1(                                                    "Zero"); Size--; //NULL char
    Get_L4 (TrackPos,                                           "Track_Position");
    if(Element_Offset+8<Element_Size)
        Skip_XX(Element_Size-Element_Offset,                    "Unknown");
    Element_Begin("Footer");
        Skip_L4(                                                "Size");
        Skip_C4(                                                "Name");
    Element_End();

    //Filling
    Fill(Stream_General, 0, General_Track, Title);
    Fill(Stream_General, 0, General_Performer, Artist);
    Fill(Stream_General, 0, General_Album, Album);
    Fill(Stream_General, 0, "Unknown", Unknown);
    Fill(Stream_General, 0, General_Genre, Genre);
    Fill(Stream_General, 0, General_Comment, Comment);
    Fill(Stream_General, 0, General_Track_Position, TrackPos);
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_RIFF_YES

