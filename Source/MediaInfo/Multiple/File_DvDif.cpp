// File_DvDif - Info for DV-DIF files
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_DVDIF_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_DvDif.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char*  Dv_sct[]=
{
    "Header",
    "Subcode",
    "Aux",
    "Audio",
    "Video",
    "",
    "",
    "",
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_DvDif::File_DvDif()
:File__Analyze()
{
    //Temp
    Subcode_First=true;
    Aux_video_control_Parsed=false;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Header_Parse()
{
    Trusted=1; //We must not accept errors in the header (mainly because of problems in Multiple parsing)

    //Parsing
    int8u SectionType;
    BS_Begin();
    Get_S1 ( 3, SectionType,                                    "Section Type");
    Skip_S1( 5,                                                 "Section Type (more)"); //11111 if Header or Subcode, 10110 if Vaux, Audio or Video
    Skip_S1( 4,                                                 "DIF sequence number"); //0-9 for 525/60; 0-11 for 625/50
    Skip_S1( 1,                                                 "Channel number");
    Mark_1();
    Mark_1();
    Mark_1();
    BS_End();
    Get_B1 (DIFBlockNumber,                                     "DIF block number"); //Video: 0-134, Audio: 0-8

    Header_Fill_Code(SectionType, Dv_sct[SectionType]);
    Header_Fill_Size(80);
}

//---------------------------------------------------------------------------
void File_DvDif::Data_Parse()
{
    Element_Info(DIFBlockNumber);

    switch (Element_Code)
    {
        case 0 : Header(); break;
        case 1 : Subcode(); break;
        case 2 : Aux(); break;
        case 3 : Audio(); break;
        case 4 : Video(); break;
        default: Skip_XX(Element_Size,                          "Unknown");
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::Header()
{
    BS_Begin();
    Skip_S1( 1,                                                 "dsf"); //0=PAL, 1=NTSC
    Mark_0();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();

    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 3,                                                 "apt"); //Track application ID, 0=not 4:2:0, 1=4:2:0

    Skip_S1( 1,                                                 "tfl - Audio data is not valid");
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Skip_S1( 3,                                                 "apl - Audio application ID");

    Skip_S1( 1,                                                 "tf2 - Video data is not valid");
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Skip_S1( 3,                                                 "ap2 - Video application ID");

    Skip_S1( 1,                                                 "tf3 - Subcode is not valid");
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Skip_S1( 3,                                                 "ap3 - Subcode application ID");

    BS_End();
    Skip_XX(72,                                                 "Unused");
}

//---------------------------------------------------------------------------
void File_DvDif::Subcode()
{
    for (int8u syb_num=0; syb_num<6; syb_num++)
    {
        Element_Begin("ssyb");
        BS_Begin();
        Skip_S1( 1,                                             "FR ID"); //1=first half, 0=second
        if (Subcode_First && syb_num==0)
            Skip_S1( 3,                                         "ap3 - Subcode application ID");
        else if (!Subcode_First && syb_num==5)
        {
            Skip_S1( 1,                                         "Reserved");
            Skip_S1( 1,                                         "Reserved");
            Skip_S1( 1,                                         "Reserved");
        }
        else
            Skip_S1( 3,                                         "apt - track application ID");
        Skip_S1( 1,                                             "Reserved");
        Skip_S1( 1,                                             "Reserved");
        Skip_S1( 1,                                             "Reserved");
        Skip_S1( 1,                                             "Reserved");

        Skip_S1( 1,                                             "Reserved");
        Skip_S1( 1,                                             "Reserved");
        Skip_S1( 1,                                             "Reserved");
        Skip_S1( 1,                                             "Reserved");
        Skip_S1( 4,                                             "syb_num");

        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        Mark_1();
        BS_End();

        Skip_B5(                                                "Unknwon");
        Element_End();
    }
    Skip_XX(29,                                                 "Unused");

    Subcode_First=!Subcode_First; //First --> Second, or Second --> First (only 2 Subcodes)
}

//---------------------------------------------------------------------------
void File_DvDif::Aux()
{
    for (int8u i=0; i<15; i++)
    {
        Element_Begin();
        int8u PackType;
        Get_B1 (PackType,                                       "Pack Type");
        Element_Name(Ztring().From_Number(PackType, 16));
        switch(PackType)
        {
            case 0x60 : Aux_video_source(); break;
            case 0x61 : Aux_video_control(); break;
            case 0x62 : Aux_video_recdate(); break;
            case 0x63 : Aux_video_rectime(); break;
            case 0xFF : Skip_B4(                                "Unused"); break;
            default: Skip_B4(                                   "Unknown");
        }
        Element_End();
    }
    Skip_XX(2,                                                  "Unused");
}

//---------------------------------------------------------------------------
void File_DvDif::Aux_video_source()
{
    Element_Name("Aux_video_source");

    BS_Begin();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();

    Skip_SB(                                                    "Color");
    Skip_SB(                                                    "CLF is not valid");
    Skip_S1( 2,                                                 "color frames id");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");

    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "System"); //0=PAL, 1=NTSC
    Skip_S1( 4,                                                 "stype?"); //0=not 4:2:2, 4=4:2:2

    BS_End();
    Skip_B1(                                                    "VISC");
}

//---------------------------------------------------------------------------
void File_DvDif::Aux_video_control()
{
    Element_Name("Aux_video_control");

    bool Interlaced;

    BS_Begin();
    Skip_S1( 2,                                                 "Copy generation management (CGMS)"); //0=Free
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");
    Skip_S1( 1,                                                 "Reserved");

    Mark_1();
    Mark_1();
    Mark_0();
    Mark_0();
    Mark_1();
    Skip_S1( 3,                                                 "aspect"); //0=4/3, 2=16/9, 3=letterbox, 7=16/9

    Skip_SB(                                                    "Frame (if not, Field)"); //1=Frame, 0=Field
    Skip_SB(                                                    "First Field"); //0=Field 2, 1=Field 1
    Skip_SB(                                                    "Frame Change"); //0=Same picture as before
    Get_SB (    Interlaced,                                     "Interlaced"); //1=Interlaced
    Mark_1();
    Mark_1();
    Mark_0();
    Mark_0();

    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();
    Mark_1();

    BS_End();


    FILLING_BEGIN();
        if (!Aux_video_control_Parsed)
        {
            Stream_Prepare(Stream_General);
            Fill(Stream_General, 0, General_Format, "DV");
            Stream_Prepare(Stream_Video);
            Fill(Stream_Video, 0, General_Codec, "DV");
            if (Interlaced)
                Fill(Stream_Video, 0, Video_Interlacement, "Interlaced");

            Aux_video_control_Parsed=true;
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_DvDif::Aux_video_recdate()
{
    Element_Name("Aux_video_recdate");

    BS_Begin();

    int8u Temp;
    int16u Year=0;
    int8u  Month=0, Day=0;
    Skip_S1( 8,                                                 "nothing");
    Skip_S1( 2,                                                 "nothing");
    Get_S1 ( 2, Temp,                                           "");
    Day+=Temp*10;
    Get_S1 ( 4, Temp,                                           "");
    Day+=Temp;
    Skip_S1( 3,                                                 "nothing");
    Get_S1 ( 1, Temp,                                           "");
    Month+=Temp*10;
    Get_S1 ( 4, Temp,                                           "");
    Month+=Temp;
    Get_S1 ( 4, Temp,                                           "");
    Year+=Temp*10;
    Get_S1 ( 4, Temp,                                           "");
    Year+=Temp;
    Year+=Year<25?2000:1900;
    Element_Info(Ztring::ToZtring(Year)+_T("-")+Ztring::ToZtring(Month)+_T("-")+Ztring::ToZtring(Day));

    BS_End();
}

//---------------------------------------------------------------------------
void File_DvDif::Aux_video_rectime()
{
    Element_Name("Aux_video_rectime");

    BS_Begin();

    int8u Temp;
    int64u Time=0;
    Get_S1 ( 4, Temp,                                           "");
    Time+=Temp*10;
    Get_S1 ( 4, Temp,                                           "");
    Time+=Temp;
    Get_S1 ( 4, Temp,                                           "");
    Time+=Temp*10*1000;
    Get_S1 ( 4, Temp,                                           "");
    Time+=Temp*1000;
    Get_S1 ( 4, Temp,                                           "");
    Time+=Temp*10*60*1000;
    Get_S1 ( 4, Temp,                                           "");
    Time+=Temp*60*1000;
    Skip_S1( 2,                                                 "nothing");
    Get_S1 ( 2, Temp,                                           "");
    Time+=Temp*10*60*60*1000;
    Get_S1 ( 4, Temp,                                           "");
    Time+=Temp*60*60*1000;
    Element_Info(Ztring().Duration_From_Milliseconds(Time));

    BS_End();
}

//---------------------------------------------------------------------------
void File_DvDif::Audio()
{
    Skip_XX(Element_Size-Element_Offset,                        "Unknown");
}

//---------------------------------------------------------------------------
void File_DvDif::Video()
{
    Skip_XX(Element_Size-Element_Offset,                        "Unknown");
    Finnished();
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_DvDif::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "");
            Fill_HowTo("BitRate", "");
            Fill_HowTo("Width", "");
            Fill_HowTo("Height", "");
            Fill_HowTo("DisplayAspectRatio", "");
            Fill_HowTo("FrameRate", "");
            break;
        case (Stream_Audio) :
            Fill_HowTo("Codec", "");
            Fill_HowTo("BitRate", "");
            Fill_HowTo("Channel(s)", "");
            Fill_HowTo("SamplingRate", "");
            Fill_HowTo("Resolution", "");
            Fill_HowTo("Language", "");
            Fill_HowTo("Language_More", "");
            Fill_HowTo("Codec", "");
            Fill_HowTo("Language", "");
            Fill_HowTo("Language_More", "");
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

#endif //MEDIAINFO_DV_YES

