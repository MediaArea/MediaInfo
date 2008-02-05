// File_Ac3 - Info for AC3 files
// Copyright (C) 2004-2008 Jerome Martinez, Zen@MediaArea.net
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

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_AC3_YES) || defined(MEDIAINFO_DVDV_YES) || defined(MEDIAINFO_MPEGTS_YES) || defined(MEDIAINFO_MPEGPS_YES)
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#include "ZenLib/Conf.h"
using namespace ZenLib;

//---------------------------------------------------------------------------
extern const int32u AC3_SamplingRate[]=
{ 48000,  44100,  32000,      0,};

//---------------------------------------------------------------------------
extern const int32u AC3_SamplingRate2[]=
{ 24000,  22050,  16000,      0,};

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
const char*  AC3_ChannelPositions[]=
{
    "L R",
    "C",
    "L R",
    "L C R",
    "Front: L R,   Rear: C",
    "Front: L C R, Rear: C",
    "Front: L R,   Rear: L R",
    "Front: L C R, Rear: L R",
};

//---------------------------------------------------------------------------
extern const int8u AC3_Channels[]=
{2, 1, 2, 3, 3, 4, 4, 5};

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
int16u AC3_FrameSize_Get(int32u frmsizecod, int32u fscod)
{
    bool Padding=(frmsizecod%2)?true:false;
    int32u frame_size_id=frmsizecod/2;

    if (frame_size_id>26 || fscod>3)
        return 0;

    int32u FrameSize=AC3_FrameSize[frame_size_id][fscod];
    if (fscod==1 && Padding)
        FrameSize+=2; // frame lengths are padded by 1 word (16 bits) at 44100 Hz
    return FrameSize;
}

//---------------------------------------------------------------------------
const char*  AC3_Mode[]=
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
const char*  AC3_Surround[]=
{
    "",
    "(No surround)",
    "(Surround)",
    "",
};
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
#include <ZenLib/Utils.h>
#include <ZenLib/BitStream.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Ac3::File_Ac3()
{
    //In
    Frame_Count_Valid=32;

    //Configuration
    File_MaximumOffset=64*1024;

    //External info
    Delay=0;

    //Temp - Global
    Frame_Count=0;

    //Temp - Technical info
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
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ac3::Header_Begin()
{
    //Must have enough buffer for having header
    if (Buffer_Offset+2>Buffer_Size)
        return false;

    //Quick test of synchro
    if (Synched && CC2(Buffer+Buffer_Offset)!=0x0B77)
    {
        Trusted_IsNot("AC3, Synchronisation lost");
        Synched=false;
    }

    //Synchro
    if (!Synched && !Synchronize())
        return false;

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_Ac3::Header_Parse()
{
    //Parsing
    Skip_B2(                                                    "syncword");

    //Testing bsid before parsing
    int32u Temp;
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
        Header_Fill_Size(AC3_FrameSize_Get(frmsizecod, fscod));
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
        Header_Fill_Size(2+frmsiz*2);
        numblks=numblkscod==3?6:numblkscod+1;
    }
    else
        //Filling
        Header_Fill_Size(Element_Offset); //Unknown!

    //Filling
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
    Skip_XX(Element_Size,                                       "Data");

    //Filling
    if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
        Data_Parse_Fill();
}

//---------------------------------------------------------------------------
void File_Ac3::Data_Parse_Fill()
{
    if (bsid<=0x08)
    {
        Stream_Prepare(Stream_General);
        Fill("Format", "AC3");
        Stream_Prepare(Stream_Audio);
        Fill("Codec", "AC3");

        Fill("SamplingRate", AC3_SamplingRate[fscod]);
        if (frmsizecod/2<19)
        {
            int32u BitRate=AC3_BitRate[frmsizecod/2]*1000;
            Fill("BitRate", BitRate);
            if (Delay>100 && BitRate>0)
                Fill("Delay", (float)Delay*8*1000/BitRate, 0);
        }

        if (acmod==0)
            Fill("Codec_Profile", "Dual Mono");
        int8u Channels=AC3_Channels[acmod];
        Ztring ChannelPositions; ChannelPositions.From_Local(AC3_ChannelPositions[acmod]);
        if (lfeon)
        {
            Channels+=1;
            ChannelPositions+=_T(", Subwoofer");
        }
        Fill("Channel(s)", Channels);
        Fill("ChannelPositions", ChannelPositions);
        if (dsurmod==2)
            Fill("Codec_Profile", "Dolby Digital");
        Fill("BitRate_Mode", "CBR");
    }

    if (bsid==0x10)
    {
        Stream_Prepare(Stream_General);
        Fill("Format", "AC3+");
        Stream_Prepare(Stream_Audio);
        Fill("Codec", "AC3+");

        Fill("BitRate_Mode", "CBR");
        if (numblks>0)
            Fill("BitRate", ((frmsiz*2+2)*8*(750/numblks))/4);

        if (fscod!=2)
            Fill("SamplingRate", AC3_SamplingRate[fscod]);
        else
            Fill("SamplingRate", AC3_SamplingRate2[fscod2]);

        if (chanmap==0)
        {
            if (acmod==0)
                Fill("Codec_Profile", "Dual Mono");
            int8u Channels=AC3_Channels[acmod];
            Ztring ChannelPositions; ChannelPositions.From_Local(AC3_ChannelPositions[acmod]);
            if (lfeon)
            {
                Channels+=1;
                ChannelPositions+=_T(", Subwoofer");
            }
            Fill("Channel(s)", Channels);
            Fill("ChannelPositions", ChannelPositions);
        }
    }

    if (File_Offset+Buffer_Size<File_Size)
    {
        //No more need data
        Element_End();
        Info("AC3, Jumping to end of file");
        Finnished();
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ac3::Synchronize()
{
    //Synchronizing
    while (Buffer_Offset+2<=Buffer_Size
        && CC2(Buffer+Buffer_Offset)!=0x0B77)
        Buffer_Offset++;
    if (Buffer_Offset+2>Buffer_Size)
    {
        //Parsing last bytes
        if (Buffer_Offset+2==Buffer_Size)
        {
            if (CC1(Buffer+Buffer_Offset)!=0x0B)
                Buffer_Offset++;
        }

        //Delay
        if (Frame_Count==0)
            Delay+=Buffer_Offset;

        return false;
    }

    //Delay
    if (Frame_Count==0)
        Delay+=Buffer_Offset;

    //Synched is OK
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
void File_Ac3::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("OveralBitRate", "R");
            Fill_HowTo("PlayTime", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("BitRate_Mode", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("ChannelPositions", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("Codec", "R");
            Fill_HowTo("Resolution", "N");
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

#endif //MEDIAINFO_AC3_YES

