// File_Ac3 - Info for AC3 files
// Copyright (C) 2004-2009 Jerome Martinez, Zen@MediaArea.net
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

//***************************************************************************
// Infos (Common)
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_AC3_YES) || defined(MEDIAINFO_DVDV_YES) || defined(MEDIAINFO_MPEGPS_YES) || defined(MEDIAINFO_MPEGTS_YES)
//---------------------------------------------------------------------------

#include "ZenLib/Conf.h"
using namespace ZenLib;

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern const int32u AC3_SamplingRate[]=
{ 48000,  44100,  32000,      0,};

//---------------------------------------------------------------------------
extern const char*  AC3_Mode[]=
{
    "complete main (CM)",
    "music and effects (ME)",
    "visually impaired (VI)",
    "hearing impaired (HI)",
    "dialogue (D)",
    "commentary (C)",
    "emergency (E)",
    "voice over (VO)",
};

//---------------------------------------------------------------------------
extern const char*  AC3_Surround[]=
{
    "",
    "(No surround)",
    "(Surround)",
    "",
};

//---------------------------------------------------------------------------
extern const int16u AC3_BitRate[]=
{

     32,
     40,
     48,
     56,
     64,
     80,
     96,
    112,
    128,
    160,
    192,
    224,
    256,
    320,
    384,
    448,
    512,
    576,
    640,
};

//---------------------------------------------------------------------------
extern const int8u AC3_Channels[]=
{2, 1, 2, 3, 3, 4, 4, 5};

//---------------------------------------------------------------------------
} //NameSpace

//---------------------------------------------------------------------------
#endif //...
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_AC3_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Ac3.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
const int32u AC3_SamplingRate2[]=
{ 24000,  22050,  16000,      0,};

//---------------------------------------------------------------------------
const char*  AC3_ChannelPositions[]=
{
    "L R",
    "C",
    "L R",
    "L C R",
    "Front: L R,   Surround: C",
    "Front: L C R, Surround: C",
    "Front: L R,   Surround: L R",
    "Front: L C R, Surround: L R",
};

//---------------------------------------------------------------------------
const char*  AC3_ChannelPositions2[]=
{
    "2/0",
    "1/0",
    "2/0",
    "3/0",
    "2/1",
    "3/1",
    "2/2",
    "3/2",
};

//---------------------------------------------------------------------------
const int16u AC3_FrameSize[27][4]=
{
    { 128,  138,  192,    0},
    { 160,  174,  240,    0},
    { 192,  208,  288,    0},
    { 224,  242,  336,    0},
    { 256,  278,  384,    0},
    { 320,  348,  480,    0},
    { 384,  416,  576,    0},
    { 448,  486,  672,    0},
    { 512,  556,  768,    0},
    { 640,  696,  960,    0},
    { 768,  834, 1152,    0},
    { 896,  974, 1344,    0},
    {1024, 1114, 1536,    0},
    {1280, 1392, 1920,    0},
    {1536, 1670, 2304,    0},
    {1792, 1950, 2688,    0},
    {2048, 2228, 3072,    0},
    {2304, 2506, 3456,    0},
    {2560, 2786, 3840,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    { 768,    0,    0,    0},
};

//---------------------------------------------------------------------------
int16u AC3_FrameSize_Get(int8u frmsizecod, int8u fscod)
{
    bool Padding=(frmsizecod%2)?true:false;
    int16u frame_size_id=frmsizecod/2;

    if (frame_size_id>26 || fscod>3)
        return 0;

    int16u FrameSize=AC3_FrameSize[frame_size_id][fscod];
    if (fscod==1 && Padding)
        FrameSize+=2; // frame lengths are padded by 1 word (16 bits) at 44100 Hz
    return FrameSize;
}

//---------------------------------------------------------------------------
// CRC_16_Table
// A CRC is computed like this:
// Init: int32u CRC_16 = 0x0000;
// for each data byte do
//     CRC_16=(CRC_16<<8) ^ CRC_16_Table[(CRC_16>>8)^(data_byte)];
int16u CRC_16_Table[256] =
{
    0x0000, 0x8005, 0x800f, 0x000a, 0x801b, 0x001e, 0x0014, 0x8011, 
    0x8033, 0x0036, 0x003c, 0x8039, 0x0028, 0x802d, 0x8027, 0x0022, 
    0x8063, 0x0066, 0x006c, 0x8069, 0x0078, 0x807d, 0x8077, 0x0072, 
    0x0050, 0x8055, 0x805f, 0x005a, 0x804b, 0x004e, 0x0044, 0x8041, 
    0x80c3, 0x00c6, 0x00cc, 0x80c9, 0x00d8, 0x80dd, 0x80d7, 0x00d2, 
    0x00f0, 0x80f5, 0x80ff, 0x00fa, 0x80eb, 0x00ee, 0x00e4, 0x80e1, 
    0x00a0, 0x80a5, 0x80af, 0x00aa, 0x80bb, 0x00be, 0x00b4, 0x80b1, 
    0x8093, 0x0096, 0x009c, 0x8099, 0x0088, 0x808d, 0x8087, 0x0082, 
    0x8183, 0x0186, 0x018c, 0x8189, 0x0198, 0x819d, 0x8197, 0x0192, 
    0x01b0, 0x81b5, 0x81bf, 0x01ba, 0x81ab, 0x01ae, 0x01a4, 0x81a1, 
    0x01e0, 0x81e5, 0x81ef, 0x01ea, 0x81fb, 0x01fe, 0x01f4, 0x81f1, 
    0x81d3, 0x01d6, 0x01dc, 0x81d9, 0x01c8, 0x81cd, 0x81c7, 0x01c2, 
    0x0140, 0x8145, 0x814f, 0x014a, 0x815b, 0x015e, 0x0154, 0x8151, 
    0x8173, 0x0176, 0x017c, 0x8179, 0x0168, 0x816d, 0x8167, 0x0162, 
    0x8123, 0x0126, 0x012c, 0x8129, 0x0138, 0x813d, 0x8137, 0x0132, 
    0x0110, 0x8115, 0x811f, 0x011a, 0x810b, 0x010e, 0x0104, 0x8101, 
    0x8303, 0x0306, 0x030c, 0x8309, 0x0318, 0x831d, 0x8317, 0x0312, 
    0x0330, 0x8335, 0x833f, 0x033a, 0x832b, 0x032e, 0x0324, 0x8321, 
    0x0360, 0x8365, 0x836f, 0x036a, 0x837b, 0x037e, 0x0374, 0x8371, 
    0x8353, 0x0356, 0x035c, 0x8359, 0x0348, 0x834d, 0x8347, 0x0342, 
    0x03c0, 0x83c5, 0x83cf, 0x03ca, 0x83db, 0x03de, 0x03d4, 0x83d1, 
    0x83f3, 0x03f6, 0x03fc, 0x83f9, 0x03e8, 0x83ed, 0x83e7, 0x03e2, 
    0x83a3, 0x03a6, 0x03ac, 0x83a9, 0x03b8, 0x83bd, 0x83b7, 0x03b2, 
    0x0390, 0x8395, 0x839f, 0x039a, 0x838b, 0x038e, 0x0384, 0x8381, 
    0x0280, 0x8285, 0x828f, 0x028a, 0x829b, 0x029e, 0x0294, 0x8291, 
    0x82b3, 0x02b6, 0x02bc, 0x82b9, 0x02a8, 0x82ad, 0x82a7, 0x02a2, 
    0x82e3, 0x02e6, 0x02ec, 0x82e9, 0x02f8, 0x82fd, 0x82f7, 0x02f2, 
    0x02d0, 0x82d5, 0x82df, 0x02da, 0x82cb, 0x02ce, 0x02c4, 0x82c1, 
    0x8243, 0x0246, 0x024c, 0x8249, 0x0258, 0x825d, 0x8257, 0x0252, 
    0x0270, 0x8275, 0x827f, 0x027a, 0x826b, 0x026e, 0x0264, 0x8261, 
    0x0220, 0x8225, 0x822f, 0x022a, 0x823b, 0x023e, 0x0234, 0x8231, 
    0x8213, 0x0216, 0x021c, 0x8219, 0x0208, 0x820d, 0x8207, 0x0202
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Ac3::File_Ac3()
:File__Analyze()
{
    //Configuration
    MustSynchronize=true;
    Buffer_TotalBytes_FirstSynched_Max=32*1024;

    //In
    Frame_Count_Valid=64;
    MustParse_dac3=false;
    MustParse_dec3=false;

    //Temp
    Frame_Count=0;
    HD_Count=0;
    chanmap=0;
    frmsiz=0;
    fscod=0;
    fscod2=0;
    frmsizecod=0;
    bsid=0;
    bsmod=0;
    acmod=0;
    dsurmod=0;
    numblks=0;
    lfeon=false;
    dxc3_Parsed=false;
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ac3::FileHeader_Begin()
{
    //Specific cases
    if (MustParse_dac3 || MustParse_dec3)
        return true;

    //Must have enough buffer for having header
    if (Buffer_Size<4)
        return false; //Must wait for more data

    //False positives detection: detect Matroska files, AC-3 parser is not smart enough
    if (CC4(Buffer)==0x1A45DFA3) //EBML
    {
        IsFinished=true;
        return false;
    }

    //All should be OK...
    return true;
}

//***************************************************************************
// Buffer - Synchro
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ac3::Synchronize()
{
    //Specific cases
    if (MustParse_dac3 || MustParse_dec3)
        return true;

    //Synchronizing
    while (Buffer_Offset+2<=Buffer_Size)
    {
        while (Buffer_Offset+2<=Buffer_Size)
        {
            if (CC2(Buffer+Buffer_Offset)==0x0B77)
                break; //while()
            Buffer_Offset++;
        }

        if (Buffer_Offset+2<=Buffer_Size) //Testing if CRC is coherant
        {
            //Retrieving some info
            if (Buffer_Offset+6>Buffer_Size)
                return false; //Need more data

            int8u bsid =CC1(Buffer+Buffer_Offset+5)>>3;
            int16u Size=0;
            if (bsid<=0x08)
            {
                int8u fscod     =(CC1(Buffer+Buffer_Offset+4)>>6)&0x03;
                int8u frmsizecod=(CC1(Buffer+Buffer_Offset+4)   )&0x3F;
                Size=AC3_FrameSize_Get(frmsizecod, fscod);
            }
            else if (bsid>0x0A && bsid<=0x10)
            {
                int16u frmsiz=CC2(Buffer+Buffer_Offset+2)&0x07FF;
                Size=2+frmsiz*2;
            }
            if (Size!=0)
            {
                if (Buffer_Offset+Size>Buffer_Size)
                    return false; //Need more data

                //Testing
                int16u CRC_16=0x0000;
                const int8u* CRC_16_Buffer=Buffer+Buffer_Offset+2; //After syncword
                while(CRC_16_Buffer<Buffer+Buffer_Offset+Size)
                {
                    CRC_16=(CRC_16<<8) ^ CRC_16_Table[(CRC_16>>8)^(*CRC_16_Buffer)];
                    CRC_16_Buffer++;
                }
                if (CRC_16!=0x0000)
                    Buffer_Offset++;
                else
                    break;
            }
            else
                Buffer_Offset++;
        }
    }

    //Parsing last bytes if needed
    if (Buffer_Offset+2>Buffer_Size)
    {
        if (Buffer_Offset+1==Buffer_Size && CC1(Buffer+Buffer_Offset)!=0x0B)
            Buffer_Offset++;
        return false;
    }

    //Synched
    return true;
}

//---------------------------------------------------------------------------
bool File_Ac3::Synched_Test()
{
    //Specific cases
    if (MustParse_dac3 || MustParse_dec3)
        return true;

    //Must have enough buffer for having header
    if (Buffer_Offset+2>Buffer_Size)
        return false;

    //Quick test of synchro
    if (CC2(Buffer+Buffer_Offset)!=0x0B77)
    {
        Synched=false;

        //TrueHD detection
        if (Frame_Count>=1 && HD_Count+1==Frame_Count && bsid<=0x08)
        {
            HD_Count++; //I didn't find a better solution
            Info("HD");
            if (!Synchronize())
                return false;
            Synched=true;
            return true;
        }
    }

    //We continue
    return true;
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ac3::Read_Buffer_Continue()
{
    if (MustParse_dac3)
        dac3();
    if (MustParse_dec3)
        dec3();
}

//---------------------------------------------------------------------------
void File_Ac3::Read_Buffer_Finalize()
{
    //In case of partial data, and finalizing is forced
    if (!IsAccepted && dxc3_Parsed)
        Data_Parse_Fill();
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ac3::Header_Parse()
{
    //Parsing
    Skip_B2(                                                        "syncword");

    //Testing bsid before parsing
    int32u Temp;
    int16u Size;
    Peek_B4(Temp);
    bsid=(int8u)((Temp&0x000000F8)>>3);
    if (bsid<=0x08)
    {
        Skip_B2(                                                    "crc1");
        BS_Begin();
        Get_S1 ( 2, fscod,                                          "fscod - Sample Rate Code"); Param_Info(AC3_SamplingRate[fscod], " Hz");
        Get_S1 ( 6, frmsizecod,                                     "frmsizecod - Frame Size Code"); if (frmsizecod/2<19) {Param_Info(AC3_BitRate[frmsizecod/2]*1000, " bps");}
        Get_S1 ( 5, bsid,                                           "bsid - Bit Stream Identification");
        Get_S1 (3, bsmod,                                           "bsmod - Bit Stream Mode"); Param_Info(AC3_Mode[bsmod]);
        Get_S1 (3, acmod,                                           "acmod - Audio Coding Mode"); Param_Info(AC3_ChannelPositions[acmod]);
        if ((acmod&1) && acmod!=1) //central present
            Skip_S1(2,                                              "cmixlev - Center Mix Level");
        if (acmod&4) //back present
            Skip_S1(2,                                              "surmixlev - Surround Mix Level");
        if (acmod==2)
        {
            Get_S1 (2, dsurmod,                                     "dsurmod - Dolby Surround Mode"); Param_Info(AC3_Surround[dsurmod]);
        }
        Get_SB (   lfeon,                                           "lfeon - Low Frequency Effects");
        BS_End();

        //Filling
        Size=AC3_FrameSize_Get(frmsizecod, fscod);
    }
    else if (bsid>0x0A && bsid<=0x10)
    {
        int8u  strmtyp, numblkscod;
        BS_Begin();
        Get_S1 ( 2, strmtyp,                                        "strmtyp");
        Skip_S1( 3,                                                 "substreamid");
        Get_S2 (11, frmsiz,                                         "frmsiz");
        Get_S1 ( 2, fscod,                                          "fscod");
        if (fscod==3)
        {
            Get_S1 ( 2, fscod2,                                     "fscod2");
            numblkscod=3;
        }
        else
            Get_S1 ( 2, numblkscod,                                 "numblkscod");
        Get_S1 (3, acmod,                                           "acmod - Audio Coding Mode"); Param_Info(AC3_ChannelPositions[acmod]);
        Get_SB (   lfeon,                                           "lfeon - Low Frequency Effects");
        Get_S1 ( 5, bsid,                                           "bsid - Bit Stream Identification");
        TEST_SB_SKIP(                                               "compre");
            Skip_S1(8,                                              "compr");
        TEST_SB_END();
        if (acmod==0) //1+1 mode
        {
            Skip_SB(                                                "dialnorm2");
            TEST_SB_SKIP(                                           "compre");
                Skip_S1(1,                                          "compr");
            TEST_SB_END();
        }
        if (strmtyp==1) //dependent stream
        {
            TEST_SB_SKIP(                                           "chanmape");
                Get_S2(16, chanmap,                                 "chanmap");
            TEST_SB_END();
        }

        //Filling
        Size=2+frmsiz*2;
        numblks=numblkscod==3?6:numblkscod+1;
    }
    else
        //Filling
        Size=(int16u)Element_Offset;

    //CRC
    if (Buffer_Offset+Size>Buffer_Size)
        Element_WaitForMoreData();
    int16u CRC_16=0x0000;
    if (Buffer_Offset+Size<=Buffer_Size) //Only if we have a complete frame, Element_WaitForMoreData() is NOK if the file truncated
    {
        for (int16u CRC_16_Pos=2; CRC_16_Pos<Size; CRC_16_Pos++) //After syncword
            CRC_16=(CRC_16<<8) ^ CRC_16_Table[(CRC_16>>8)^(Buffer[Buffer_Offset+CRC_16_Pos])];
    }

    //Filling
    Header_Fill_Size(CRC_16==0x0000?Size:Element_Offset); //If CRC_16!=0x0000, CRC error
    Header_Fill_Code(0, "Frame");
}

//---------------------------------------------------------------------------
void File_Ac3::Data_Parse()
{
    //Counting
    if (File_Offset+Buffer_Offset+Element_Size==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;

    //Name
    Element_Info(Ztring::ToZtring(Frame_Count));

    //Parsing
    if (Element_IsComplete_Get())
        Skip_XX(Element_Size,                                   "Data");
    else
    {
        Element_Info("(Uncomplete)");
        Skip_XX(Element_Size,                                   "Data (Uncomplete)");
    }

    //Filling
    if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
        Data_Parse_Fill();
}

//---------------------------------------------------------------------------
void File_Ac3::Data_Parse_Fill()
{
    if (bsid<=0x08)
    {
        bool IsTrueHD=Frame_Count>=2 && (HD_Count+1==Frame_Count);
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "AC-3");
        if (IsTrueHD)
            Fill(Stream_General, 0, General_Format_Profile, "TrueHD");
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, Audio_Format, "AC-3");
        if (IsTrueHD)
            Fill(Stream_Audio, 0, Audio_Format_Profile, "TrueHD");
        Fill(Stream_Audio, 0, Audio_Codec, IsTrueHD?"TrueHD":"AC3");

        Fill(Stream_Audio, 0, Audio_SamplingRate, AC3_SamplingRate[fscod]);
        if (frmsizecod/2<19 && !IsTrueHD)
        {
            int32u BitRate=AC3_BitRate[frmsizecod/2]*1000;
            Fill(Stream_Audio, 0, Audio_BitRate, BitRate);
            if (Buffer_TotalBytes_FirstSynched>100 && BitRate>0)
                Fill(Stream_Audio, 0, Audio_Delay, (float)Buffer_TotalBytes_FirstSynched*8*1000/BitRate, 0);
        }

        if (acmod==0)
        {
            Fill(Stream_Audio, 0, Audio_Format_Profile, "Dual Mono");
            Fill(Stream_Audio, 0, Audio_Codec_Profile, "Dual Mono");
        }
        int8u Channels=AC3_Channels[acmod];
        Ztring ChannelPositions; ChannelPositions.From_Local(AC3_ChannelPositions[acmod]);
        Ztring ChannelPositions2; ChannelPositions2.From_Local(AC3_ChannelPositions2[acmod]);
        if (lfeon)
        {
            Channels+=1;
            ChannelPositions+=_T(", LFE");
            ChannelPositions2+=_T(".1");
        }
        Fill(Stream_Audio, 0, Audio_Channel_s_, Channels);
        Fill(Stream_Audio, 0, Audio_ChannelPositions, ChannelPositions);
        Fill(Stream_Audio, 0, Audio_ChannelPositions_String2, ChannelPositions2);
        if (dsurmod==2)
        {
            Fill(Stream_Audio, 0, Audio_Format_Profile, "Dolby Digital");
            Fill(Stream_Audio, 0, Audio_Codec_Profile, "Dolby Digital");
        }
        Fill(Stream_Audio, 0, Audio_BitRate_Mode, IsTrueHD?"VBR":"CBR");
    }

    if (bsid==0x10)
    {
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "E-AC-3");
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, Audio_Format, "E-AC-3");
        Fill(Stream_Audio, 0, Audio_Codec, "AC3+");

        Fill(Stream_Audio, 0, Audio_BitRate_Mode, "CBR");
        if (numblks>0)
            Fill(Stream_Audio, 0, Audio_BitRate, ((frmsiz*2+2)*8*(750/numblks))/4);

        if (fscod!=2)
            Fill(Stream_Audio, 0, Audio_SamplingRate, AC3_SamplingRate[fscod]);
        else
            Fill(Stream_Audio, 0, Audio_SamplingRate, AC3_SamplingRate2[fscod2]);

        if (chanmap==0)
        {
            if (acmod==0)
            {
                Fill(Stream_Audio, 0, Audio_Format_Profile, "Dual Mono");
                Fill(Stream_Audio, 0, Audio_Codec_Profile, "Dual Mono");
            }
            int8u Channels=AC3_Channels[acmod];
            Ztring ChannelPositions; ChannelPositions.From_Local(AC3_ChannelPositions[acmod]);
            if (lfeon)
            {
                Channels+=1;
                ChannelPositions+=_T(", LFE");
            }
            Fill(Stream_Audio, 0, Audio_Channel_s_, Channels);
            Fill(Stream_Audio, 0, Audio_ChannelPositions, ChannelPositions);
        }
    }

    //No more need data
    Accept("AC-3");
    Finish("AC-3");
}

//---------------------------------------------------------------------------
void File_Ac3::dac3()
{
    BS_Begin();
    Get_S1 (2, fscod,                                           "fscod");
    Get_S1 (5, bsid,                                            "bsid");
    Get_S1 (3, bsmod,                                           "bsmod");
    Get_S1 (3, acmod,                                           "acmod");
    Get_SB (   lfeon,                                           "lfeon");
    Get_S1 (5, frmsizecod,                                      "bit_rate_code"); frmsizecod*=2;
    Skip_S1(5,                                                  "reserved");
    BS_End();

    MustParse_dac3=false;
    dxc3_Parsed=true;
}

//---------------------------------------------------------------------------
void File_Ac3::dec3()
{
    //Parsing
    BS_Begin();
    int8u num_ind_sub;
    Skip_S2(13,                                                 "data_rate");
    Get_S1 ( 3, num_ind_sub,                                    "num_ind_sub");
    for (int8u Pos=0; Pos<num_ind_sub; Pos++)
    {
        Element_Begin("independent substream");
        int8u num_dep_sub;
        Get_S1 (2, fscod,                                       "fscod");
        Get_S1 (5, bsid,                                        "bsid");
        Get_S1 (3, bsmod,                                       "bsmod");
        Get_S1 (3, acmod,                                       "acmod");
        Get_SB (   lfeon,                                       "lfeon");
        Skip_S1(3,                                              "reserved");
        Get_S1 (4, num_dep_sub,                                 "num_dep_sub");
        if (num_dep_sub>0)
            Skip_S2(9,                                          "chan_loc");
        else
            Skip_SB(                                            "reserved");
        Element_End();
    }
    BS_End();

    MustParse_dec3=false;
    dxc3_Parsed=true;
}

} //NameSpace

#endif //MEDIAINFO_AC3_YES

