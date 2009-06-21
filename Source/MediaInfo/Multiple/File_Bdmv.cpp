// File_Mpls - Info for Blu-ray Movie files
// Copyright (C) 2009-2009 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_BDMV_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Bdmv.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
namespace Elements
{
    const int32u CLPI=0x48444D56; //HDMV
    const int32u MPLS=0x4D504C53;
}

//***************************************************************************
// Infos
//***************************************************************************

//---------------------------------------------------------------------------
const char* Mpls_Offsets[]=
{
    "Playlists offset",
    "Chapters offset",
    "Reserved",
    "Reserved",
    "Reserved",
};

//---------------------------------------------------------------------------
const char* Clpi_Offsets[]=
{
    "Unknown offset",
    "Streams offset",
    "Unknown offset",
    "Unknown offset",
    "Unknown offset",
};

//---------------------------------------------------------------------------
const char* Bdmv_Type(int32u Magic, size_t Type)
{
    switch (Magic)
    {
        case Elements::CLPI : return Clpi_Offsets[Type];
        case Elements::MPLS : return Mpls_Offsets[Type];
        default             : return "";
    }
}

//---------------------------------------------------------------------------
const char* Clpi_Format(int8u StreamType)
{
    switch (StreamType)
    {
        case 0x01 : return "MPEG-1 Video";
        case 0x02 : return "MPEG-2 Video";
        case 0x03 : return "MPEG-1 Audio";
        case 0x04 : return "MPEG-2 Audio";
        case 0x80 : return "PCM";
        case 0x81 : return "AC-3";
        case 0x82 : return "DTS";
        case 0x83 : return "TrueHD";
        case 0x84 : return "E-AC-3";
        case 0x85 : return "DTS";
        case 0x86 : return "DTS";
        case 0x90 : return "PGS";
        case 0x91 : return "Interactive";
        case 0x92 : return "Subtitle";
        case 0x1B : return "AVC";
        case 0xA1 : return "E-AC-3"; //Secondary
        case 0xA2 : return "DTS"; //Secondary
        case 0xEA : return "VC-1";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
const char* Clpi_Format_Profile(int8u StreamType)
{
    switch (StreamType)
    {
        case 0x85 : return "HD";
        case 0x86 : return "MA";
        case 0xA2 : return "HD"; //Secondary
        default   : return "";
    }
}

//---------------------------------------------------------------------------
stream_t Clpi_Type(int8u StreamType)
{
    switch (StreamType)
    {
        case 0x01 : return Stream_Video;
        case 0x02 : return Stream_Video;
        case 0x03 : return Stream_Audio;
        case 0x04 : return Stream_Audio;
        case 0x80 : return Stream_Audio;
        case 0x81 : return Stream_Audio;
        case 0x82 : return Stream_Audio;
        case 0x83 : return Stream_Audio;
        case 0x84 : return Stream_Audio;
        case 0x85 : return Stream_Audio;
        case 0x86 : return Stream_Audio;
        case 0x90 : return Stream_Text;
        case 0x91 : return Stream_Max;
        case 0x92 : return Stream_Text;
        case 0x1B : return Stream_Video;
        case 0xA1 : return Stream_Audio;
        case 0xA2 : return Stream_Audio;
        case 0xEA : return Stream_Video;
        default   : return Stream_Max;
    }
}

//---------------------------------------------------------------------------
const char* Clpi_Video_Format[]=
{
    "",
    "480i",
    "576i",
    "480p",
    "1080i",
    "720p",
    "1080p",
    "576p",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Clpi_Video_Interlacement[]=
{
    "",
    "Interlaced",
    "Interlaced",
    "PPF",
    "Interlaced",
    "PPF",
    "PPF",
    "PPF",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Clpi_Video_Standard[]=
{
    "",
    "NTSC",
    "PAL",
    "NTSC",
    "",
    "",
    "",
    "PAL",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
int16u Clpi_Video_Width[]=
{
    0,
    720,
    720,
    720,
    1920,
    1280,
    1920,
    720,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
int16u Clpi_Video_Height[]=
{
    0,
    480,
    576,
    480,
    1080,
    720,
    1080,
    576,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
float32 Clpi_Video_FrameRate[]=
{
     0.000,
    23.976,
    24.000,
    25.000,
    29.970,
     0.000,
    50.000,
    59.940,
     0.000,
     0.000,
     0.000,
     0.000,
     0.000,
     0.000,
     0.000,
     0.000,
};

//---------------------------------------------------------------------------
float32 Clpi_Video_AspectRatio[]=
{
    0.000,
    0.000,
    1.333,
    1.778,
    2.210,
    0.000,
    0.000,
    0.000,
    0.000,
    0.000,
    0.000,
    0.000,
    0.000,
    0.000,
    0.000,
    0.000,
};

//---------------------------------------------------------------------------
int8u Clpi_Audio_Channels[]=
{
    0,
    1,
    0,
    2,
    0,
    0,
    0, //Multi 6-8
    0,
    0,
    0,
    0,
    0,
    0, //Combo
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
int32u Clpi_Audio_SamplingRate[]=
{
        0,
    48000,
        0,
        0,
    96000,
   192000,
        0,
        0,
        0,
        0,
        0,
        0,
    48000, //192000?
    48000, // 96000?
        0,
        0,
};

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
Ztring Bdmv_Decimal_Hexa(int64u Number)
{
    Ztring Temp;
    Temp.From_Number(Number);
    Temp+=_T(" (0x");
    Temp+=Ztring::ToZtring(Number, 16);
    Temp+=_T(")");
    return Temp;
}

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Bdmv::FileHeader_Begin()
{
    //Element_Size
    if (Buffer_Size<4)
        return false; //Must wait for more data

    switch (CC4(Buffer))
    {
        case Elements::CLPI :
        case Elements::MPLS :
                              break;
        default             : Reject("Blu-ray");
                              return false;
    }

    //All should be OK...
    return true;
}

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Bdmv::Read_Buffer_Continue()
{
    if (Buffer_Size<File_Size)
    {
        Element_WaitForMoreData();
        return; //Wait for more data
    }

    //Parsing
    int32u Magic;
    int16u VersionH;
    Element_Begin("Header");
    Get_C4 (Magic,                                              "Magic");
    Data_Accept("MPLS");
    Get_C2 (VersionH,                                           "Version (High)");
    Skip_C2(                                                    "Version (Low)");
    Element_End();

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        switch (Magic)
        {
            case Elements::CLPI : Fill(Stream_General, 0, General_Format, "Blu-ray Clip info"); break;
            case Elements::MPLS : Fill(Stream_General, 0, General_Format, "Blu-ray Playlist"); break;
            default             : ;
        }
    FILLING_END();

    if (VersionH==0x3031 || VersionH==0x3032) //Version 1 or 2
    {
        Element_Begin("Offsets");
        for (size_t Type=0; Type<5; Type++)
        {
            int32u Offset;
            Get_B4 (Offset,                                     Bdmv_Type(Magic, Type));
            Types[Offset]=Type;
        }
        Element_End();

        for (std::map<int32u, size_t>::iterator Type=Types.begin(); Type!=Types.end(); Type++)
        {
            if (Type->first>=Element_Offset)
            {
                if (Type->first>Element_Offset)
                    Skip_XX(Type->first-Element_Offset,         "Unknown");

                Element_Begin();
                int32u Size;
                Get_B4 (Size,                                   "Size");
                int64u End=Element_Offset+Size;
                switch (Magic)
                {
                    case Elements::CLPI :
                                            switch(Type->second)
                                            {
                                                case 1 : Clpi_Streams  (); break;
                                                default: Element_Name("Unknown");
                                            }
                                            break;
                    case Elements::MPLS :
                                            switch(Type->second)
                                            {
                                                case 0 : Mpls_Playlists(); break;
                                                case 1 : Mpls_Chapters (); break;
                                                default: Element_Name("Unknown");
                                            }
                                            break;
                    default             :   Element_Name("Unknown");
                }
                if (End>Element_Offset)
                    Skip_XX(End-Element_Offset,                 "Unknown");
                Element_End();
            }
        }

        if (Element_Size>Element_Offset)
            Skip_XX(Element_Size-Element_Offset,                "Unknown");
    }
    else
        Skip_XX(Element_Size-Element_Offset,                    "Unknown");
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Bdmv::Clpi_Streams()
{
    Element_Name("Streams");

    //Parsing
    int8u Count;
    Skip_B4(                                                    "Unknown");
    Skip_B4(                                                    "Unknown");
    Get_B1 (Count,                                              "Streams count");
    Skip_B1(                                                    "Unknown");
    for (int16u Pos=0; Pos<Count; Pos++)
    {
        Element_Begin("Stream");
        int16u PID;
        int8u Length;
        Get_B2 (PID,                                            "PID");
        Get_B1 (Length,                                         "Length");
        int64u End=Element_Offset+Length;
        Get_B1 (StreamType,                                     "Stream type"); Param_Info(Clpi_Format(StreamType)); Element_Info(Clpi_Format(StreamType));
        switch (Clpi_Type(StreamType))
        {
            case Stream_Video : Clpi_Streams_Video(); break;
            case Stream_Audio : Clpi_Streams_Audio(); break;
            case Stream_Text  : Clpi_Streams_Text() ; break;
            default           : ;
        }

        if (End-Element_Offset)
            Skip_XX(End-Element_Offset,                         "Unknown");
        Element_End();

        FILLING_BEGIN();
            if (StreamKind_Last!=Stream_Max)
            {
                Fill(StreamKind_Last, StreamPos_Last, "ID", PID, 10, true);
                Fill(StreamKind_Last, StreamPos_Last, "ID/String", Bdmv_Decimal_Hexa(PID), true);
            }
        FILLING_END();
    }
}

//---------------------------------------------------------------------------
void File_Bdmv::Clpi_Streams_Video()
{
    //Parsing
    int8u Format, FrameRate, AspectRatio;
    BS_Begin();
    Get_S1 (4, Format,              "Format"); Param_Info(Clpi_Video_Format[Format]);
    Get_S1 (4, FrameRate,           "Frame rate"); Param_Info(Clpi_Video_FrameRate[FrameRate]);
    Get_S1 (4, AspectRatio,         "Aspect ratio"); Param_Info(Clpi_Video_AspectRatio[AspectRatio]);
    Skip_BS(4,                      "Reserved");
    BS_End();

    FILLING_BEGIN();
        Stream_Prepare(Stream_Video);
        Fill(Stream_Video, StreamPos_Last, Video_Format, Clpi_Format(StreamType));
        if (Clpi_Video_Width[Format])
            Fill(Stream_Video, StreamPos_Last, Video_Width, Clpi_Video_Width[Format]);
        if (Clpi_Video_Height[Format])
            Fill(Stream_Video, StreamPos_Last, Video_Height, Clpi_Video_Height[Format]);
        Fill(Stream_Video, StreamPos_Last, Video_Interlacement, Clpi_Video_Interlacement[StreamType]);
        Fill(Stream_Video, StreamPos_Last, Video_Standard, Clpi_Video_Standard[Format]);
        if (Clpi_Video_FrameRate[FrameRate])
            Fill(Stream_Video, StreamPos_Last, Video_FrameRate, Clpi_Video_FrameRate[FrameRate]);
        if (Clpi_Video_Height[AspectRatio])
            Fill(Stream_Video, StreamPos_Last, Video_DisplayAspectRatio, Clpi_Video_AspectRatio[AspectRatio]);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Bdmv::Clpi_Streams_Audio()
{
    //Parsing
    int8u Channels, SamplingRate;
    BS_Begin();
    Get_S1 (4, Channels,            "Channel layout"); Param_Info(Clpi_Audio_Channels[Channels]);
    Get_S1 (4, SamplingRate,        "Sampling Rate"); Param_Info(Clpi_Audio_SamplingRate[SamplingRate]);
    BS_End();
    Info_Local(3, Language,         "Language"); Element_Info(Language);

    FILLING_BEGIN();
        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, StreamPos_Last, Audio_Format, Clpi_Format(StreamType));
        Fill(Stream_Audio, StreamPos_Last, Audio_Format_Profile, Clpi_Format_Profile(StreamType));
        if (Clpi_Audio_Channels[Channels])
            Fill(Stream_Audio, StreamPos_Last, Audio_Channel_s_, Clpi_Audio_Channels[Channels]);
        if (Clpi_Audio_SamplingRate[SamplingRate])
            Fill(Stream_Audio, StreamPos_Last, Audio_SamplingRate, Clpi_Audio_SamplingRate[SamplingRate]);
        Fill(Stream_Audio, StreamPos_Last, Audio_Language, Language);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Bdmv::Clpi_Streams_Text()
{
    //Parsing
    if (StreamType==0x92) //Subtitle
        Skip_B1(                    "Unknown");
    Info_Local(3, Language,         "Language"); Element_Info(Language);

    FILLING_BEGIN();
        Stream_Prepare(Stream_Text);
        Fill(Stream_Text, StreamPos_Last, Text_Format, Clpi_Format(StreamType));
        Fill(Stream_Text, StreamPos_Last, Text_Language, Language);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Bdmv::Mpls_Playlists()
{
    Element_Name("Playlists");

    //Parsing
    int64u Duration=0;
    int16u Count;
    Skip_B2(                                                    "Unknown");
    Get_B2 (Count,                                              "Count");
    Skip_B2(                                                    "Unknown");
    for (int16u Pos=0; Pos<Count; Pos++)
    {
        Element_Begin();
        Ztring M2ts_Name;
        int16u Size;
        Get_B2 (Size,                                           "Size");
        Get_Local (5, M2ts_Name,                                "File name");
        Skip_Local(4,                                           "M2TS");
        Skip_B1(                                                "Unknown");
        BS_Begin();
            Skip_S1(3,                                          "Unknown");
            Skip_SB(                                            "Angle?");
            Skip_S1(4,                                          "Condition");
        BS_End();
        Skip_B1(                                                "Unknown");
        Info_B4(Time_In,                                        "Time (In)"); Param_Info((float32)Time_In/45000);
        Info_B4(Time_Out,                                       "Time (Out)"); Param_Info((float32)Time_Out/45000);

        Skip_XX(Size-20,                                        "Unknown");
        Element_End(M2ts_Name, 2+Size);

        if (Time_Out>Time_In)
            Duration+=Time_Out-Time_In;
    }

    if (Duration)
        Fill(Stream_General, 0, General_Duration, Duration);
}

//---------------------------------------------------------------------------
void File_Bdmv::Mpls_Chapters()
{
    Element_Name("Chapters");

    Stream_Prepare(Stream_Menu);
    Fill(Stream_Menu, StreamPos_Last, Menu_Chapters_Pos_Begin, Count_Get(Stream_Menu, StreamPos_Last), 10, true);

    //Parsing
    int16u Count;
    Get_B2 (Count,                                              "Count");
    for (int16u Pos=0; Pos<Count; Pos++)
    {
        Element_Begin("Chapter");
        int8u Clip;
        Skip_B1(                                                "Unknown");
        Get_B1 (Clip,                                           "Clip?");
        if (Clip==1)
        {
            int32u Time;
            int16u FileIndex;
            Get_B2 (FileIndex,                                  "Stream file index");
            Get_B4 (Time,                                       "Time"); Param_Info(Time/45000, " seconds");
            Skip_B2(                                            "Unknown");
            Skip_B4(                                            "Unknown");

            FILLING_BEGIN();
                if (FileIndex==0) //We currently handle only the first file
                    Fill(Stream_Menu, 0, Ztring().Duration_From_Milliseconds(Time/45).To_UTF8().c_str(), Ztring::ToZtring(Pos));
            FILLING_END();
        }
        else
        {
            Skip_XX(12,                                         "Unknwon");
        }
        Element_End();
    }

    Fill(Stream_Menu, StreamPos_Last, Menu_Chapters_Pos_End, Count_Get(Stream_Menu, StreamPos_Last), 10, true);
}

} //NameSpace

#endif //MEDIAINFO_BDMV_YES
