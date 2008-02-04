// File_Vc1 - Info for VC-1 files
// Copyright (C) 2007-2008 Jerome Martinez, Zen@MediaArea.net
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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_VC1_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Vc1.h"
#include <ZenLib/BitStream.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const char* Vc1_Profile[]=
{
    "Simple",
    "Main",
    "Complex",
    "Advanced",
};

//---------------------------------------------------------------------------
const char* Vc1_ChromaFormat[]=
{
    "",
    "4:2:0",
    "",
    "",
};

//---------------------------------------------------------------------------
const float32 Vc1_PixelAspectRatio[]=
{
    (float32)1, //Reserved
    (float32)1,
    (float32)12/(float32)11,
    (float32)10/(float32)11,
    (float32)16/(float32)11,
    (float32)40/(float32)33,
    (float32)24/(float32)11,
    (float32)20/(float32)11,
    (float32)32/(float32)11,
    (float32)80/(float32)33,
    (float32)18/(float32)11,
    (float32)15/(float32)11,
    (float32)64/(float32)33,
    (float32)160/(float32)99,
    (float32)1, //Reserved
    (float32)1, //Custom
};

//---------------------------------------------------------------------------
const float32 Vc1_FrameRate_enr(int8u Code)
{
    switch (Code)
    {
        case 0x01 : return (float32)24000;
        case 0x02 : return (float32)25000;
        case 0x03 : return (float32)30000;
        case 0x04 : return (float32)50000;
        case 0x05 : return (float32)60000;
        case 0x06 : return (float32)48000;
        case 0x07 : return (float32)72000;
        default   : return (float32)0;
    }
}

//---------------------------------------------------------------------------
const float32 Vc1_FrameRate_dr(int8u Code)
{
    switch (Code)
    {
        case 0x01 : return (float32)1000;
        case 0x02 : return (float32)1001;
        default   : return (float32)1000; //Not sure, must be precised
    }
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Vc1::File_Vc1()
:File__Analyze()
{
    //In
    Frame_Count_Valid=30;
    FrameIsAlwaysComplete=false;

    //Count
    Frame_Count=0;

    //Temp
    coded_width=0;
    coded_height=0;
    framerateexp=0;
    frameratecode_enr=0;
    frameratecode_dr=0;
    profile=0;
    level=0;
    chromaformat=0;
    AspectRatio=0;
    AspectRatioX=0;
    AspectRatioY=0;
    interlace=false;
    tfcntrflag=false;
    framerate_present=false;
    framerate_form=false;

    //Default stream values
    Stream[0x0F].Searching_Payload=true;
}

//---------------------------------------------------------------------------
void File_Vc1::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonVC1())
        return;
}

//---------------------------------------------------------------------------
void File_Vc1::Read_Buffer_Finalize()
{
    //In case of partial data, and finalizing is forced (example: DecConfig in .mp4), but with at least one frame
    if (Count_Get(Stream_General)==0 && Frame_Count>0)
        FrameHeader_Fill();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Vc1::Header_Begin()
{
    //Must have enough buffer for having header
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && CC3(Buffer+Buffer_Offset)!=0x000001)
    {
        Trusted_IsNot("VC-1, Synchronisation lost");
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //Quick search
    if (!Header_Parser_QuickSearch())
        return false;

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_Vc1::Header_Parse()
{
    //Parsing
    int8u start_code;
    Skip_B3(                                                    "synchro");
    Get_B1 (start_code,                                         "start_code");
    if (!Header_Parse_Fill_Size())
    {
        Element_WaitForMoreData();
        return;
    }

    //Filling
    Header_Fill_Code(start_code, Ztring().From_CC1(start_code));
}

//---------------------------------------------------------------------------
void File_Vc1::Data_Parse()
{
    //Parse
    switch (Element_Code)
    {
        case 0x0A: EndOfSequence(); break;
        case 0x0B: Slice(); break;
        case 0x0C: Field(); break;
        case 0x0D: FrameHeader(); break;
        case 0x0E: EntryPointHeader(); break;
        case 0x0F: SequenceHeader(); break;
        case 0x1B: UserDefinedSlice(); break;
        case 0x1C: UserDefinedField(); break;
        case 0x1D: UserDefinedFrameHeader(); break;
        case 0x1E: UserDefinedEntryPointHeader(); break;
        case 0x1F: UserDefinedSequenceHeader(); break;
        default:
                Trusted_IsNot("Unattended element!");
    }
}

//---------------------------------------------------------------------------
bool File_Vc1::Header_Parse_Fill_Size()
{
    //Look for next Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if Header_Parse_Fill_Size() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset+4;
    while (Buffer_Offset_Temp+4<=Buffer_Size
        && CC3(Buffer+Buffer_Offset_Temp)!=0x000001)
        Buffer_Offset_Temp++;

    //Must wait more data?
    if (Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (FrameIsAlwaysComplete || File_Offset+Buffer_Size==File_Size)
            Buffer_Offset_Temp=Buffer_Size; //We are sure that the next bytes are a start
        else
            return false;
    }

    //OK, we continue
    Header_Fill_Size(Buffer_Offset_Temp-Buffer_Offset);
    Buffer_Offset_Temp=0;
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packet "0A"
void File_Vc1::EndOfSequence()
{
    Element_Name("EndOfSequence");
}

//---------------------------------------------------------------------------
// Packet "0B"
void File_Vc1::Slice()
{
    Element_Name("Slice");
}

//---------------------------------------------------------------------------
// Packet "0C"
void File_Vc1::Field()
{
    Element_Name("Field");
}

//---------------------------------------------------------------------------
// Packet "0D"
void File_Vc1::FrameHeader()
{
    //Counting
    if (File_Offset+Buffer_Offset+Element_Size==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;

    //Name
    Element_Name("FrameHeader");
    Element_Info(Ztring(_T("Frame ")+Ztring::ToZtring(Frame_Count)));

    //Parsing
    bool TypeNotP, TypeNotB, TypeNotI, TypeNotBI;
    BS_Begin();
    if (interlace)
    {
        bool Unknown;
        Get_SB (   Unknown,                                     "Unknown_maybe_top_field_first");
        if (Unknown)
            Skip_BS(1,                                          "Unknown");
    }
    Get_SB (   TypeNotP,                                        "TypeNotP");
    if (TypeNotP)
    {
        Get_SB (   TypeNotB,                                    "TypeNotB");
        if (TypeNotB)
        {
            Get_SB (   TypeNotI,                                "TypeNotI");
            if (TypeNotI)
            {
                Get_SB (   TypeNotBI,                           "TypeNotBI");
                if (TypeNotBI)
                {
                    Param_Info("Skipped");
                }
                else
                {
                    Param_Info("BI");
                }
            }
            else
            {
                Param_Info("I");
            }
        }
        else
        {
            Param_Info("B");
        }
    }
    else
    {
        Param_Info("P");
    }
    if (tfcntrflag)
        Skip_BS(8,                                              "tfcntrflag related");
    int32u Flags;
    Get_BS (2, Flags,                                           "Flags");
    BS_End();

    FILLING_BEGIN();
        //NextCode
        NextCode_Test();
        NextCode_Clear();
        NextCode_Add(0x0D);
        NextCode_Add(0x0F);

        //Filling only if not already done
        if (Frame_Count>=Frame_Count_Valid && Count_Get(Stream_Video)==0)
            FrameHeader_Fill();

        //Autorisation of other streams
        Stream[0x0D].Searching_Payload=true;
        Stream[0x0F].Searching_Payload=true;
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Vc1::FrameHeader_Fill()
{
    //Calculating - PixelAspectRatio
    float32 PixelAspectRatio;
    if (AspectRatio!=0x0F)
        PixelAspectRatio=Vc1_PixelAspectRatio[AspectRatio];
    else if (AspectRatioY)
        PixelAspectRatio=((float)AspectRatioX)/((float)AspectRatioY);
    else
        PixelAspectRatio=1; //Unknown

    //Calculating - FrameRate
    float32 FrameRate;
    if (framerate_present)
    {
        if (framerate_form)
            FrameRate=((float32)(framerateexp+1))/32;
        else
            FrameRate=Vc1_FrameRate_enr(frameratecode_enr)/Vc1_FrameRate_dr(frameratecode_dr);
    }
    else
        FrameRate=0;

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "VC-1");
    Stream_Prepare(Stream_Video);
    Fill("Codec", "VC-1");

    Fill("Codec_Profile", Ztring(Vc1_Profile[profile])+_T('@')+Ztring::ToZtring(level));
    Fill("Chroma", Vc1_ChromaFormat[chromaformat]);
    Fill("Interlacement", interlace?"Interlaced":"PPF");
    Fill("Width", (coded_width+1)*2);
    Fill("Height", (coded_height+1)*2);
    if (PixelAspectRatio!=0)
        Fill("PixelAspectRatio", PixelAspectRatio);
    if (FrameRate!=0)
        Fill("FrameRate", FrameRate);

    //Jumping
    if (Frame_Count>=Frame_Count_Valid)
    {
        Element_End();
        Info("VC-1, Jumping to end of file");
        Finnished();
    }
}

//---------------------------------------------------------------------------
// Packet "0E"
void File_Vc1::EntryPointHeader()
{
    Element_Name("EntryPointHeader");

    FILLING_BEGIN();
        //NextCode
        NextCode_Test();
        NextCode_Clear();
        NextCode_Add(0x0D);

        //Autorisation of other streams
        Stream[0x0D].Searching_Payload=true;
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "0F"
void File_Vc1::SequenceHeader()
{
    Element_Name("SequenceHeader");

    //Parsing
    BS_Begin();
    Get_S1 ( 2, profile,                                        "profile"); Param_Info(Vc1_Profile[profile]);
    if (profile==3) //Advanced
    {
        Get_S1 ( 3, level,                                      "level");
        Get_S1 ( 2, chromaformat,                               "chromaformat"); Param_Info(Vc1_ChromaFormat[chromaformat]);
        Skip_S1( 3,                                             "frmrtq_postproc");
        Skip_S1( 5,                                             "bitrtq_postproc");
        Skip_SB(                                                "postprocflag");
        Get_S2 (12, coded_width,                                "coded_width"); Param_Info((coded_width+1)*2, " pixels");
        Get_S2 (12, coded_height,                               "coded_height"); Param_Info((coded_height+1)*2, " pixels");
        Skip_SB(                                                "pulldown");
        Get_SB (    interlace,                                  "interlace");
        Get_SB (    tfcntrflag,                                 "tfcntrflag");
        Skip_SB(                                                "finterpflag");
        Skip_SB(                                                "reserved");
        Skip_SB(                                                "Progressive Segmented Frame");
        TEST_SB_SKIP(                                           "display_extended_info");
            Skip_S2(14,                                         "Display x");
            Skip_S2(14,                                         "Display y");
            TEST_SB_SKIP(                                       "aspectratio_present");
                Get_S1 ( 4, AspectRatio,                        "AspectRatio"); Param_Info(Vc1_PixelAspectRatio[AspectRatio]);
                if (AspectRatio==0x0F)
                {
                    Get_S1 ( 8, AspectRatioX,                   "AspectRatioX");
                    Get_S1 ( 8, AspectRatioY,                   "AspectRatioY");
                }
                TEST_SB_GET(framerate_present,                  "framerate_present");
                    TEST_SB(framerate_form,                     "framerate_form");
                        Get_S2 (16, framerateexp,               "framerateexp"); Param_Info((float32)((framerateexp+1)/32.0), 3, " fps");
                    TEST_SB_ELSE();
                        Get_S1 ( 8, frameratecode_enr,          "frameratecode_enr"); Param_Info(Vc1_FrameRate_enr(frameratecode_enr));
                        Get_S1 ( 8, frameratecode_dr,           "frameratecode_dr"); Param_Info(Vc1_FrameRate_dr(frameratecode_dr));
                    TEST_SB_END();
                TEST_SB_END();
            TEST_SB_END();
            TEST_SB_SKIP(                                       "Unknown");
                Skip_S1( 8,                                     "color_prim");
                Skip_S1( 8,                                     "transfer_char");
                Skip_S1( 8,                                     "matrix_coef");
            TEST_SB_END();
        TEST_SB_END();
        TEST_SB_SKIP(                                           "hrd_param_flag");
            int8u hrd_num_leaky_buckets;
            Get_S1 ( 5, hrd_num_leaky_buckets,                  "hrd_num_leaky_buckets");
            Skip_S1( 4,                                         "bitrate exponent");
            Skip_S1( 4,                                         "buffer size exponent");
            for (int8u Pos=0; Pos<hrd_num_leaky_buckets; Pos++)
            {
                Element_Begin("leaky_bucket");
                Skip_S2(16,                                     "hrd_rate");
                Skip_S2(16,                                     "hrd_buffer");
                Element_End();
            }
        TEST_SB_END();
        //Continue... Something is missing, need specs
    }
    else
    {
        //Unknown, too less info for doing something, abandonning
        return;
    }
    BS_End();

    FILLING_BEGIN();
        //NextCode
        NextCode_Clear();
        NextCode_Add(0x0D);
        NextCode_Add(0x0E);

        //Autorisation of other streams
        Stream[0x0D].Searching_Payload=true;
        Stream[0x0E].Searching_Payload=true;
    FILLING_END();
}

//---------------------------------------------------------------------------
// Packet "1B"
void File_Vc1::UserDefinedSlice()
{
    Element_Name("UserDefinedSlice");
}

//---------------------------------------------------------------------------
// Packet "1C"
void File_Vc1::UserDefinedField()
{
    Element_Name("UserDefinedField");
}

//---------------------------------------------------------------------------
// Packet "1D"
void File_Vc1::UserDefinedFrameHeader()
{
    Element_Name("UserDefinedFrameHeader");
}

//---------------------------------------------------------------------------
// Packet "1E"
void File_Vc1::UserDefinedEntryPointHeader()
{
    Element_Name("UserDefinedEntryPointHeader");
}

//---------------------------------------------------------------------------
// Packet "1F"
void File_Vc1::UserDefinedSequenceHeader()
{
    Element_Name("UserDefinedSequenceHeader");
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Vc1::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+4<=Buffer_Size
        && CC3(Buffer+Buffer_Offset)!=0x000001)
        Buffer_Offset++;
    if (Buffer_Offset+4>Buffer_Size)
    {
        //Parsing last bytes
        if (Buffer_Offset+3==Buffer_Size)
        {
            if (CC3(Buffer+Buffer_Offset)!=0x000001)
            {
                Buffer_Offset++;
                if (CC2(Buffer+Buffer_Offset)!=0x0000)
                {
                    Buffer_Offset++;
                    if (CC1(Buffer+Buffer_Offset)!=0x00)
                        Buffer_Offset++;
                }
            }
        }

        return false;
    }

    //Synched is OK
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_Vc1::Header_Parser_QuickSearch()
{
    while (           Buffer_Offset+4<=Buffer_Size
      &&   CC3(Buffer+Buffer_Offset)==0x000001)
    {
        //Getting start_code
        int8u start_code=CC1(Buffer+Buffer_Offset+3);

        //Searching start
        if (Stream[start_code].Searching_Payload)
            return true;

        //Getting size
        Buffer_Offset+=4;
        while(Buffer_Offset+4<=Buffer_Size && CC3(Buffer+Buffer_Offset)!=0x000001)
            Buffer_Offset++;
    }

    if (Buffer_Offset+4>Buffer_Size)
    {
        Synched=false;
        Synchronize();
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
bool File_Vc1::Detect_NonVC1 ()
{
    //File_Size
    if (File_Size<=192*4)
        return false; //We can't do detection

    //Element_Size
    if (Buffer_Size<=192*4)
        return true; //Must wait for more data

    //Detect mainly DAT files, and the parser is not enough precise to detect them later
    if (CC4(Buffer)==CC4("RIFF"))
    {
        Finnished();
        return true;
    }

    //Detect TS files, and the parser is not enough precise to detect them later
    while (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset)!=0x47) //Look for first Sync word
        Buffer_Offset++;
    if (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset+188)==0x47 && CC1(Buffer+Buffer_Offset+188*2)==0x47 && CC1(Buffer+Buffer_Offset+188*3)==0x47)
    {
        Finnished();
        return true;
    }
    Buffer_Offset=0;

    //Detect BDAV files, and the parser is not enough precise to detect them later
    while (Buffer_Offset<192 && CC1(Buffer+Buffer_Offset+4)!=0x47) //Look for first Sync word
        Buffer_Offset++;
    if (Buffer_Offset<192 && CC1(Buffer+Buffer_Offset+192+4)==0x47 && CC1(Buffer+Buffer_Offset+192*2+4)==0x47 && CC1(Buffer+Buffer_Offset+192*3+4)==0x47)
    {
        Finnished();
        return true;
    }
    Buffer_Offset=0;

    //Seems OK
    return false;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Vc1::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
            break;
        case (Stream_Chapters) :
            break;
        case (Stream_Image) :
            break;
        case (Stream_Menu) :
            break;
        case (Stream_Max) :
            break;
    }
}

} //NameSpace

#endif //MEDIAINFO_VC1_YES

