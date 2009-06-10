// File_Mpeg4 - Info for MPEG-4 files
// Copyright (C) 2005-2009 Jerome Martinez, Zen@MediaArea.net
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
// Main part          
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
#include "MediaInfo/Multiple/File_Mpeg4.h"
#include "MediaInfo/Multiple/File_Mpeg4_Descriptors.h"
#if defined(MEDIAINFO_MPEGPS_YES)
    #include "MediaInfo/Multiple/File_MpegPs.h"
#endif
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Const
//***************************************************************************

namespace Elements
{
    const int64u mdat=0x6D646174;
    const int64u moov_meta______=0x2D2D2D2D;
    const int64u moov_meta___ART=0xA9415254;
    const int64u moov_meta___alb=0xA9616C62;
    const int64u moov_meta___aut=0xA9617574;
    const int64u moov_meta___cmt=0xA9636D74;
    const int64u moov_meta___cpy=0xA9637079;
    const int64u moov_meta___day=0xA9646179;
    const int64u moov_meta___des=0xA9646573;
    const int64u moov_meta___dir=0xA9646972;
    const int64u moov_meta___dis=0xA9646973;
    const int64u moov_meta___edl=0xA965646C;
    const int64u moov_meta___enc=0xA9656E63;
    const int64u moov_meta___fmt=0xA9666D74;
    const int64u moov_meta___gen=0xA967656E;
    const int64u moov_meta___grp=0xA9677270;
    const int64u moov_meta___hos=0xA9686F73;
    const int64u moov_meta___inf=0xA9696E66;
    const int64u moov_meta___key=0xA96B6579;
    const int64u moov_meta___mak=0xA96D616B;
    const int64u moov_meta___mod=0xA96D6F64;
    const int64u moov_meta___nam=0xA96E616D;
    const int64u moov_meta___prd=0xA9707264;
    const int64u moov_meta___PRD=0xA9505244;
    const int64u moov_meta___prf=0xA9707266;
    const int64u moov_meta___req=0xA9726571;
    const int64u moov_meta___src=0xA9737263;
    const int64u moov_meta___swr=0xA9737772;
    const int64u moov_meta___too=0xA9746F6F;
    const int64u moov_meta___wrn=0xA977726E;
    const int64u moov_meta___wrt=0xA9777274;
    const int64u moov_meta__aART=0x61415254;
    const int64u moov_meta__albm=0x616C626D;
    const int64u moov_meta__auth=0x61757468;
    const int64u moov_meta__cpil=0x6370696C;
    const int64u moov_meta__cprt=0x63707274;
    const int64u moov_meta__covr=0x636F7672;
    const int64u moov_meta__disk=0x6469736B;
    const int64u moov_meta__dscp=0x64736370;
    const int64u moov_meta__gnre=0x676E7265;
    const int64u moov_meta__name=0x6E616D65;
    const int64u moov_meta__perf=0x70657266;
    const int64u moov_meta__pgap=0x70676170;
    const int64u moov_meta__titl=0x7469746C;
    const int64u moov_meta__tool=0x746F6F6C;
    const int64u moov_meta__trkn=0x74726B6E;
    const int64u moov_meta__tmpo=0x746D706F;
    const int64u moov_meta__year=0x79656172;
}

//---------------------------------------------------------------------------
Ztring Mpeg4_Encoded_Library(int32u Vendor)
{
    switch (Vendor)
    {
        case 0x33495658 : return _T("3ivX");                //3IVX
        case 0x6170706C : return _T("Apple QuickTime");     //appl
        case 0x6E696B6F : return _T("Nikon");               //niko
        case 0x6F6C796D : return _T("Olympus");             //olym
        case 0x6F6D6E65 : return _T("Omneon");              //omne
        default: return Ztring().From_CC4(Vendor);
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpeg4::File_Mpeg4()
:File__Analyze()
{
    //Configuration
    DataMustAlwaysBeComplete=false;

    //Temp
    mdat_MustParse=false;
    moov_Done=false;
    moov_trak_mdia_mdhd_TimeScale=0;
    TimeScale=1;
    Vendor=0x00000000;
    IsParsing_mdat=false;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg4::Read_Buffer_Finalize()
{
    //For each stream
    std::map<int32u, stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
    {
        //Preparing
        StreamKind_Last=Temp->second.StreamKind;
        StreamPos_Last=Temp->second.StreamPos;

        //Parser specific
        if (Temp->second.Parser)
        {
            //Finalizing and Merging
            Open_Buffer_Finalize(Temp->second.Parser);
            if (StreamKind_Last==Stream_General)
            {
                //Special case for TimeCode without link
                for (std::map<int32u, stream>::iterator Target=Stream.begin(); Target!=Stream.end(); Target++)
                    if (Target->second.StreamKind!=Stream_General)
                        Merge(*Temp->second.Parser, Target->second.StreamKind, 0, Target->second.StreamPos);
            }
            else
            {
                //Hacks - Before
                Ztring FrameRate_Temp, FrameRate_Mode_Temp;
                if (StreamKind_Last==Stream_Video)
                {
                    FrameRate_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate);
                    FrameRate_Mode_Temp=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate_Mode);
                }

                Merge(*Temp->second.Parser, StreamKind_Last, 0, StreamPos_Last);

                //Hacks - After
                if (StreamKind_Last==Stream_Video)
                {
                    if (!FrameRate_Temp.empty() && FrameRate_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate))
                        Fill(Stream_Video, StreamPos_Last, Video_FrameRate, FrameRate_Temp, true);
                    if (!FrameRate_Mode_Temp.empty() && FrameRate_Mode_Temp!=Retrieve(Stream_Video, StreamPos_Last, Video_FrameRate_Mode))
                        Fill(Stream_Video, StreamPos_Last, Video_FrameRate_Mode, FrameRate_Mode_Temp, true);
                }

                //TimeCode specific
                if (StreamKind_Last==Stream_Video && Retrieve(Stream_Menu, StreamPos_Last, Menu_Format)==_T("TimeCode"))
                {
                    Clear(Stream_Menu, StreamPos_Last, "Duration");
                    Clear(Stream_Menu, StreamPos_Last, "StreamSize");
                }

                //Special case: DV with Audio or/and Text in the video stream
                if (StreamKind_Last==Stream_Video && Temp->second.Parser && (Temp->second.Parser->Count_Get(Stream_Audio) || Temp->second.Parser->Count_Get(Stream_Text)))
                {
                    //Video and Audio are together
                    size_t Audio_Count=Temp->second.Parser->Count_Get(Stream_Audio);
                    for (size_t Audio_Pos=0; Audio_Pos<Audio_Count; Audio_Pos++)
                    {
                        Fill_Flush();
                        Stream_Prepare(Stream_Audio);
                        size_t Pos=Count_Get(Stream_Audio)-1;
                        Merge(*Temp->second.Parser, Stream_Audio, Audio_Pos, StreamPos_Last);
                        Fill(Stream_Audio, Pos, Audio_MuxingMode, "DV");
                        Fill(Stream_Audio, Pos, Audio_Duration, Retrieve(Stream_Video, Temp->second.StreamPos, Video_Duration));
                        Fill(Stream_Audio, Pos, "MuxingMode_MoreInfo", _T("Muxed in Video #")+Ztring().From_Number(Temp->second.StreamPos+1));
                        Fill(Stream_Audio, Pos, Audio_StreamSize, "0"); //Included in the DV stream size
                        Ztring ID=Retrieve(Stream_Audio, Pos, Audio_ID);
                        Fill(Stream_Audio, Pos, Audio_ID, Retrieve(Stream_Video, Temp->second.StreamPos, Video_ID)+_T("-")+ID, true);
                    }

                    //Video and Text are together
                    size_t Text_Count=Temp->second.Parser->Count_Get(Stream_Text);
                    for (size_t Text_Pos=0; Text_Pos<Text_Count; Text_Pos++)
                    {
                        Fill_Flush();
                        Stream_Prepare(Stream_Text);
                        size_t Pos=Count_Get(Stream_Text)-1;
                        Merge(*Temp->second.Parser, Stream_Text, Text_Pos, StreamPos_Last);
                        Fill(Stream_Text, Pos, "MuxingMode", "MPEG Video");
                        Fill(Stream_Text, Pos, "MuxingMode_MoreInfo", _T("Muxed in Video #")+Ztring().From_Number(Temp->second.StreamPos+1));
                        Fill(Stream_Text, Pos, Text_StreamSize, 0); //Included in the DV stream size
                        Ztring ID=Retrieve(Stream_Text, Pos, Text_ID);
                        Fill(Stream_Text, Pos, Text_ID, Retrieve(Stream_Video, Temp->second.StreamPos, Video_ID)+_T("-")+ID, true);
                    }
                }
            }
        }

        Temp++;
    }
    if (Vendor!=0x00000000 && Vendor!=0xFFFFFFFF)
        Fill(Stream_General, 0, General_Encoded_Library, Mpeg4_Encoded_Library(Vendor));

    //Purge what is not needed anymore
    if (!File_Name.empty()) //Only if this is not a buffer, with buffer we can have more data
    {
        Stream.clear();
        mdat_Pos.clear();
    }
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpeg4::Header_Begin()
{
    if (IsParsing_mdat && Element_Level==0)
        Element_Begin();

    return true;
}

//---------------------------------------------------------------------------
void File_Mpeg4::Header_Parse()
{
    //mdat
    if (IsParsing_mdat)
    {
        //Filling
        Header_Fill_Code(mdat_Pos.begin()->second.StreamID, Ztring::ToZtring(mdat_Pos.begin()->second.StreamID));
        Header_Fill_Size(mdat_Pos.begin()->second.Size);
        if (Buffer_Offset+mdat_Pos.begin()->second.Size<=Buffer_Size)
            mdat_Pos.erase(mdat_Pos.begin()); //Only if we will not need it later (in case of partial data, this function will be called again for the same chunk)
        return;
    }

    //Parsing
    int64u Size;
    int32u Size_32, Name;
    Get_B4 (Size_32,                                            "Size");
    if (Size_32==0 && (Element_Size==4 || Element_Size==8))
    {
        //Filling
        Header_Fill_Code(0, "Junk");
        Header_Fill_Size(4);
        return;
    }
    Size=Size_32;
    Get_C4 (Name,                                               "Name");
    if (Size<8)
    {
        //Special case: until the end of the atom
            if (Size==0)
        {
            Size=File_Size-(File_Offset+Buffer_Offset);
        }
        //Special case: Big files, size is 64-bit
        else if (Size==1)
        {
            //Reading Extended size
            Get_B8 (Size,                                       "Size (Extended)");
        }
        //Not in specs!
        else
        {
            Size=File_Size-(File_Offset+Buffer_Offset);
        }
    }

    //Filling
    Header_Fill_Code(Name, Ztring().From_CC4(Name));
    Header_Fill_Size(Size);
}

//---------------------------------------------------------------------------
bool File_Mpeg4::BookMark_Needed()
{
    if (Stream.empty())
        return false;
        
    File_GoTo=0; //Reseting it
    return true;
}

//---------------------------------------------------------------------------
//Get language string from 2CC
char* File_Mpeg4::Language_Get(int x)
{
    if (x<0x100 || x==0x7FFF || x==0xFFFF)
    {
        //Old Quicktime method, I never saw it, skip it
        Language_Result[0] = '\0';
        Language_Result[1] = '\0';
        Language_Result[2] = '\0';
    }
    else
    {
        Language_Result[3] = '\0';
        Language_Result[2] = (x>> 0&0x1F)+0x60;
        Language_Result[1] = (x>> 5&0x1F)+0x60;
        Language_Result[0] = (x>>10&0x1F)+0x60;
    }
    return Language_Result;
}

//---------------------------------------------------------------------------
//Get Metadata definition from 4CC
File_Mpeg4::method File_Mpeg4::Metadata_Get(std::string &Parameter, int64u Meta)
{
    switch (Meta)
    {
        //http://atomicparsley.sourceforge.net/mpeg-4files.html
        case Elements::moov_meta___ART : Parameter="Performer"; return Method_String;
        case Elements::moov_meta___alb : Parameter="Album"; return Method_String;
        case Elements::moov_meta___aut : Parameter="Performer"; return Method_String;
        case Elements::moov_meta___cmt : Parameter="Comment"; return Method_String;
        case Elements::moov_meta___cpy : Parameter="Copyright"; return Method_String;
        case Elements::moov_meta___day : Parameter="Encoded_Date"; return Method_String;
        case Elements::moov_meta___des : Parameter="Title/More"; return Method_String;
        case Elements::moov_meta___dir : Parameter="Director"; return Method_String;
        case Elements::moov_meta___dis : Parameter="TermsOfUse"; return Method_String;
        case Elements::moov_meta___edl : Parameter="Tagged_Date"; return Method_String;
        case Elements::moov_meta___enc : Parameter="Encoded_Application"; return Method_String;
        case Elements::moov_meta___fmt : Parameter="Origin"; return Method_String;
        case Elements::moov_meta___gen : Parameter="Genre"; return Method_String;
        case Elements::moov_meta___grp : Parameter="Genre"; return Method_String;
        case Elements::moov_meta___hos : Parameter="HostComputer"; return Method_String;
        case Elements::moov_meta___inf : Parameter="Title/More"; return Method_String;
        case Elements::moov_meta___key : Parameter="Keywords"; return Method_String;
        case Elements::moov_meta___mak : Parameter="Make"; return Method_String;
        case Elements::moov_meta___mod : Parameter="Model"; return Method_String;
        case Elements::moov_meta___nam : Parameter="Title"; return Method_String3;
        case Elements::moov_meta___prd : Parameter="Producer"; return Method_String;
        case Elements::moov_meta___PRD : Parameter="Product"; return Method_String;
        case Elements::moov_meta___prf : Parameter="Performer"; return Method_String;
        case Elements::moov_meta___req : Parameter="Comment"; return Method_String;
        case Elements::moov_meta___src : Parameter="DistribtedBy"; return Method_String;
        case Elements::moov_meta___swr : Parameter="Encoded_Application"; return Method_String;
        case Elements::moov_meta___too : Parameter="Encoded_Application"; return Method_String;
        case Elements::moov_meta___wrn : Parameter="Warning"; return Method_String;
        case Elements::moov_meta___wrt : Parameter="ScreenplayBy"; return Method_String;
        case Elements::moov_meta__auth : Parameter="Performer"; return Method_String2;
        case Elements::moov_meta__albm : Parameter="Album"; return Method_String2; //Has a optional track number after the NULL byte
        case Elements::moov_meta__aART : Parameter="Performer"; return Method_String2;
        case Elements::moov_meta__cpil : Parameter.clear(); return Method_None;
        case Elements::moov_meta__cprt : Parameter="Copyright"; return Method_String2;
        case Elements::moov_meta__disk : Parameter="Part"; return Method_Binary;
        case Elements::moov_meta__dscp : Parameter="Title/More"; return Method_String2;
        case Elements::moov_meta__gnre : Parameter="Genre"; return Method_String2;
        case Elements::moov_meta__name : Parameter="Title"; return Method_String;
        case Elements::moov_meta__perf : Parameter="Performer"; return Method_String2;
        case Elements::moov_meta__pgap : Parameter.clear(); return Method_None;
        case Elements::moov_meta__titl : Parameter="Title"; return Method_String2;
        case Elements::moov_meta__tool : Parameter="Encoded_Application"; return Method_String3;
        case Elements::moov_meta__trkn : Parameter="Track"; return Method_Binary;
        case Elements::moov_meta__year : Parameter="Recorded_Date"; return Method_String2;
        case Elements::moov_meta__tmpo : Parameter.clear(); return Method_None;
        default :
            {
                Parameter.clear();
                Parameter.append(1, (char)((Meta&0xFF000000)>>24));
                Parameter.append(1, (char)((Meta&0x00FF0000)>>16));
                Parameter.append(1, (char)((Meta&0x0000FF00)>> 8));
                Parameter.append(1, (char)((Meta&0x000000FF)>> 0));
                return Method_String;
            }
    }
}

//---------------------------------------------------------------------------
//Get Metadata definition from string
File_Mpeg4::method File_Mpeg4::Metadata_Get(std::string &Parameter, const std::string &Meta)
{
         if (Meta=="com.apple.quicktime.copyright") Parameter="Copyright";
    else if (Meta=="com.apple.quicktime.displayname") Parameter="Title";
    else if (Meta=="iTunNORM") Parameter="";
    else if (Meta=="iTunes_CDDB_IDs") Parameter="";
    else if (Meta=="iTunSMPB") Parameter="";
    else Parameter=Meta;
    return Method_String;
}

//---------------------------------------------------------------------------
void File_Mpeg4::Descriptors()
{
    //Preparing
    File_Mpeg4_Descriptors MI;
    MI.KindOfStream=StreamKind_Last;
    MI.Parser_DoNotFreeIt=true;
    Open_Buffer_Init(&MI);

    //Parsing
    Open_Buffer_Continue(&MI, Buffer+Buffer_Offset+(size_t)Element_Offset, (size_t)(Element_Size-Element_Offset));
    Open_Buffer_Finalize(&MI);

    //Filling
    Merge(MI, StreamKind_Last, 0, StreamPos_Last);

    //Position
    Element_Offset=Element_Size;

    //Parser from Descriptor 
    if (MI.Parser)
    {
        if (Stream[moov_trak_tkhd_TrackID].Parser)
            delete Stream[moov_trak_tkhd_TrackID].Parser; //Stream[moov_trak_tkhd_TrackID].Parser=NULL
        Stream[moov_trak_tkhd_TrackID].Parser=MI.Parser;
        mdat_MustParse=true;
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEG4_YES
