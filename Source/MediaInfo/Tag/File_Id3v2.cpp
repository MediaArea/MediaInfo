// File_Id3 - Info for ID3v2 tagged files
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

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_ID3V2_YES) || defined(MEDIAINFO_FLAC_YES)
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#include "ZenLib/Conf.h"
using namespace ZenLib;

//---------------------------------------------------------------------------
extern const char* Id3v2_PictureType(int8u Type)
{
    switch (Type)
    {
        case 0x01 :
        case 0x02 : return "File icon";
        case 0x03 : return "Cover (front)";
        case 0x04 : return "Cover (back)";
        case 0x05 : return "Leaflet page";
        case 0x06 : return "Media";
        case 0x07 :
        case 0x08 : return "Performer";
        case 0x09 : return "Conductor";
        case 0x0A : return "Performer";
        case 0x0B : return "Composer";
        case 0x0C : return "Lyricist";
        case 0x0D : return "Recording Location";
        case 0x0E : return "During recording";
        case 0x0F : return "During performance";
        case 0x10 : return "Screen capture";
        case 0x12 : return "Illustration";
        case 0x13 : return "Performer logo";
        case 0x14 : return "Publisher logo";
        default   : return "";
    }
}

} //NameSpace

//---------------------------------------------------------------------------
#endif //...
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_ID3V2_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_Id3v2.h"
#include "ZenLib/Utils.h"
#include "ZenLib/ZtringListList.h"
#include "ZenLib/ZtringListList.h"
#include "ZenLib/Base64/base64.h"
#include <cstring>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Const
//***************************************************************************

const char* Id3v2_TextEnc[]=
{
    "ISO 8859-1",
    "UTF-16",
    "UTF-16BE",
    "UTF-8",
};

const char* Id3v2_RGAD_Name_code[]=
{
    "",
    "Radio Gain Adjustment",
    "Audiophile Gain Adjustment",
    "",
    "",
    "",
    "",
    "",
};

const char* Id3v2_RGAD_Originator_code[]=
{
    "",
    "Pre-set by artist/producer/mastering engineer",
    "Set by user",
    "Determined automatically",
    "",
    "",
    "",
    "",
};

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Id3v2::File_Id3v2()
:File__Analyze()
{
    //Temp
    Id3v2_Size=0;
    Unsynchronisation_Frame=false;
}

//---------------------------------------------------------------------------
void File_Id3v2::Read_Buffer_Finalize()
{
    if (Count_Get(Stream_General)==0)
        return;

    //Specific formats (multiple Id3v2 tags for one MI tag)
    Ztring Recorded_Date;
    if (Retrieve(Stream_General, 0, General_Recorded_Date).empty() && !Year.empty())
    {
        Recorded_Date+=Year;
        if (!Month.empty() && !Month.empty())
        {
            Recorded_Date+=_T("-");
            Recorded_Date+=Year;
            Recorded_Date+=_T("-");
            Recorded_Date+=Day;
            if (!Month.empty() && !Month.empty())
            {
                Recorded_Date+=_T(" ");
                Recorded_Date+=Hour;
                Recorded_Date+=_T(":");
                Recorded_Date+=Minute;
            }
        }
    }
    if (!Recorded_Date.empty())
        Fill(Stream_General, 0, "Recorded_Date", Recorded_Date);

    //Special cases
    //-Position and total parts
    if (Retrieve(Stream_General, 0, General_Part_Position).find(_T("/"))!=Error)
    {
        Ztring Temp=Retrieve(Stream_General, 0, General_Part_Position);
        Fill(Stream_General, 0, General_Part_Position_Total, Temp.SubString(_T("/"), _T("")));
        Fill(Stream_General, 0, General_Part_Position, Temp.SubString(_T(""), _T("/")));
    }
    if (Retrieve(Stream_General, 0, General_Track_Position).find(_T("/"))!=Error)
    {
        const Ztring &Temp=Retrieve(Stream_General, 0, General_Track_Position);
        Fill(Stream_General, 0, General_Track_Position_Total, Temp.SubString(_T("/"), _T("")), true);
        Fill(Stream_General, 0, General_Track_Position, Temp.SubString(_T(""), _T("/")), true);
    }
    //-Genre
    if (Retrieve(Stream_General, 0, General_Genre).find(_T("("))==0)
    {
        const Ztring &Genre=Retrieve(Stream_General, 0, General_Genre);
        Fill(Stream_General, 0, General_Genre, Genre.SubString(_T("("), _T(")")), true); //Replace (nn) by nn
    }
    if (Retrieve(Stream_General, 0, General_Genre)==_T("0") || Retrieve(Stream_General, 0, General_Genre)==_T("255"))
        Clear(Stream_General, 0, General_Genre);
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Id3v2::FileHeader_Parse()
{
    //Parsing
    int32u Size;
    int8u Flags;
    bool ExtendedHeader;
    Skip_C3(                                                    "identifier");
    Get_B1 (Id3v2_Version,                                      "version_major");
    Skip_B1(                                                    "version_revision");
    Get_B1 (Flags,                                              "flags");
        Get_Flags (Flags, 7, Unsynchronisation_Global,          "Unsynchronisation");
        Get_Flags (Flags, 6, ExtendedHeader,                    "Extended header");
        Skip_Flags(Flags, 5,                                    "Experimental indicator");
    Get_B4 (Size,                                               "Size");
    Id3v2_Size=((Size>>0)&0x7F)
             | ((Size>>1)&0x3F80)
             | ((Size>>2)&0x1FC000)
             | ((Size>>3)&0x0FE00000);
    Param_Info(Id3v2_Size);
    if (ExtendedHeader)
    {
        int32u Size;
        Get_B4 (Size,                                           "Size");
        Skip_XX(Size,                                           "Extended header");
    }

    FILLING_BEGIN();
        //Versions
        switch (Id3v2_Version)
        {
            case 2 : break;
            case 3 : break;
            case 4 : break;
            default :
                Skip_XX(Id3v2_Size,                             "Data");
                return;
        }

        Stream_Prepare(Stream_General);
        Stream_Prepare(Stream_Audio);
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Id3v2::Header_Parse()
{
    if (Buffer_Offset+10>10+Id3v2_Size) //first 10 is minimum size of a tag, Second 10 is ID3v2 header size
    {
        //Not enough place for a tag, must be padding
        Header_Fill_Code(0xFFFFFFFF, "Padding");
        Header_Fill_Size(Buffer_Size-Buffer_Offset);
        return;
    }

    if (Buffer_Offset+10>Buffer_Size)
    {
        Element_WaitForMoreData();
        return; //Not enough buffer
    }

    //Testing padding
    int32u Frame_ID, Size;
    Frame_ID=CC1(Buffer+Buffer_Offset);
    if (Frame_ID==0x00)
    {
        //This is the padding
        Header_Fill_Code(0xFFFFFFFF, "Padding");
        Header_Fill_Size(10+Id3v2_Size-Buffer_Offset);
        return;
    }

    //Parsing
    if (Id3v2_Version==2)
    {
        Get_C3 (Frame_ID,                                       "Frame ID");
        Get_B3 (Size,                                           "Size");
    }
    else
    {
        int16u Flags;
        Get_C4 (Frame_ID,                                       "Frame ID");
        Get_B4 (Size,                                           "Size");
        Get_B2 (Flags,                                          "Flags");
        if (Id3v2_Version==3)
        {
            Skip_Flags(Flags, 15,                               "Tag alter preservation");
            Skip_Flags(Flags, 14,                               "File alter preservation");
            Skip_Flags(Flags, 13,                               "Read only");
            Skip_Flags(Flags,  7,                               "Compression");
            Skip_Flags(Flags,  6,                               "Encryption");
            Skip_Flags(Flags,  5,                               "Grouping identity");
        }
        if (Id3v2_Version==4)
        {
            Skip_Flags(Flags, 14,                               "Tag alter preservation");
            Skip_Flags(Flags, 13,                               "File alter preservation");
            Skip_Flags(Flags, 12,                               "Read only");
            Skip_Flags(Flags,  6,                               "Grouping identity");
            Skip_Flags(Flags,  3,                               "Compression");
            Skip_Flags(Flags,  2,                               "Encryption");
            Get_Flags (Flags,  1, Unsynchronisation_Frame,      "Unsynchronisation");
            Skip_Flags(Flags,  0,                               "Data length indicator");
        }
        if (Id3v2_Version!=3)
        {
            Size=((Size>>0)&0x7F)
               | ((Size>>1)&0x3F80)
               | ((Size>>2)&0x1FC000)
               | ((Size>>3)&0x0FE00000);
            Param_Info(Size);
        }
    }

    //Filling
    Ztring ToShow;
    if (Id3v2_Version==2)
        ToShow.From_CC3(Frame_ID);
    else
        ToShow.From_CC4(Frame_ID);
    Header_Fill_Code(Frame_ID, ToShow);
    Header_Fill_Size(Element_Offset+Size);
}

//---------------------------------------------------------------------------
void File_Id3v2::Data_Parse()
{
    //Unsynchronisation
    int8u* Buffer_Unsynch=NULL;
    const int8u* Save_Buffer=Buffer;
    size_t Save_Buffer_Offset=Buffer_Offset;
    int64u Save_Element_Size=Element_Size;
    size_t Element_Offset_Unsynch=0;
    std::vector<size_t> Unsynch_List;
    if (Unsynchronisation_Global || Unsynchronisation_Frame)
    {
        while (Element_Offset_Unsynch+3<=Element_Size)
        {
            if (CC2(Buffer+Buffer_Offset+(size_t)Element_Offset_Unsynch)==0xFF00)
                Unsynch_List.push_back(Element_Offset_Unsynch+1);
            Element_Offset_Unsynch++;
        }
        if (!Unsynch_List.empty())
        {
            //We must change the buffer for keeping out
            Element_Size=Save_Element_Size-Unsynch_List.size();
            Buffer_Offset=0;
            Buffer_Unsynch=new int8u[(size_t)Element_Size];
            for (size_t Pos=0; Pos<=Unsynch_List.size(); Pos++)
            {
                size_t Pos0=(Pos==Unsynch_List.size())?(size_t)Save_Element_Size:(Unsynch_List[Pos]);
                size_t Pos1=(Pos==0)?0:(Unsynch_List[Pos-1]+1);
                size_t Buffer_Unsynch_Begin=Pos1-Pos;
                size_t Save_Buffer_Begin  =Pos1;
                size_t Size=               Pos0-Pos1;
                std::memcpy(Buffer_Unsynch+Buffer_Unsynch_Begin, Save_Buffer+Save_Buffer_Offset+Save_Buffer_Begin, Size);
            }
            Buffer=Buffer_Unsynch;
        }
    }

    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case Id3::_NAME : Element_Info(_DETAIL); _NAME(); break;

    //Parsing
    Element_Value.clear();
    Element_Values.clear();
    switch (Element_Code)
    {
        ELEMENT_CASE(AENC, "Audio encryption");
        ELEMENT_CASE(APIC, "Attached picture");
        ELEMENT_CASE(ASPI, "Audio seek point index");
        ELEMENT_CASE(COMM, "Comments");
        ELEMENT_CASE(COMR, "Commercial frame");
        ELEMENT_CASE(ENCR, "Encryption method registration");
        ELEMENT_CASE(EQU2, "Equalisation (2)");
        ELEMENT_CASE(EQUA, "Equalization");
        ELEMENT_CASE(ETCO, "Event timing codes");
        ELEMENT_CASE(GEOB, "General encapsulated object");
        ELEMENT_CASE(GRID, "Group identification registration");
        ELEMENT_CASE(IPLS, "Involved people list");
        ELEMENT_CASE(LINK, "Linked information");
        ELEMENT_CASE(MCDI, "Music CD identifier");
        ELEMENT_CASE(MLLT, "MPEG location lookup table");
        ELEMENT_CASE(OWNE, "Ownership frame");
        ELEMENT_CASE(PCNT, "Play counter");
        ELEMENT_CASE(POPM, "Popularimeter");
        ELEMENT_CASE(POSS, "Position synchronisation frame");
        ELEMENT_CASE(PRIV, "Private frame");
        ELEMENT_CASE(RBUF, "Recommended buffer size");
        ELEMENT_CASE(RGAD, "Replay Gain Adjustment");
        ELEMENT_CASE(RVA2, "Relative volume adjustment (2)");
        ELEMENT_CASE(RVRB, "Reverb");
        ELEMENT_CASE(SEEK, "Seek frame");
        ELEMENT_CASE(SIGN, "Signature frame");
        ELEMENT_CASE(SYLT, "Synchronised lyric/text");
        ELEMENT_CASE(SYTC, "Synchronised tempo codes");
        ELEMENT_CASE(TALB, "Album/Movie/Show title");
        ELEMENT_CASE(TBPM, "BPM (beats per minute)");
        ELEMENT_CASE(TCMP, "iTunes Compilation Flag");
        ELEMENT_CASE(TCOM, "Composer");
        ELEMENT_CASE(TCON, "Content type");
        ELEMENT_CASE(TCOP, "Copyright message");
        ELEMENT_CASE(TDAT, "Date");
        ELEMENT_CASE(TDEN, "Encoding time");
        ELEMENT_CASE(TDLY, "Playlist delay");
        ELEMENT_CASE(TDOR, "Original release time");
        ELEMENT_CASE(TDRC, "Recording time");
        ELEMENT_CASE(TDRL, "Release time");
        ELEMENT_CASE(TDTG, "Tagging time");
        ELEMENT_CASE(TENC, "Encoded by");
        ELEMENT_CASE(TEXT, "Lyricist/Text writer");
        ELEMENT_CASE(TFLT, "File type");
        ELEMENT_CASE(TIME, "Time");
        ELEMENT_CASE(TIPL, "Involved people list");
        ELEMENT_CASE(TIT1, "Content group description");
        ELEMENT_CASE(TIT2, "Title/songname/content description");
        ELEMENT_CASE(TIT3, "Subtitle/Description refinement");
        ELEMENT_CASE(TKEY, "Initial key");
        ELEMENT_CASE(TLAN, "Language(s)");
        ELEMENT_CASE(TLEN, "Length");
        ELEMENT_CASE(TMCL, "Musician credits list");
        ELEMENT_CASE(TMED, "Media type");
        ELEMENT_CASE(TMOO, "Mood");
        ELEMENT_CASE(TOAL, "Original album/movie/show title");
        ELEMENT_CASE(TOFN, "Original filename");
        ELEMENT_CASE(TOLY, "Original lyricist(s)/text writer(s)");
        ELEMENT_CASE(TOPE, "Original artist(s)/performer(s)");
        ELEMENT_CASE(TORY, "Original release year");
        ELEMENT_CASE(TOWN, "File owner/licensee");
        ELEMENT_CASE(TPE1, "Lead performer(s)/Soloist(s)");
        ELEMENT_CASE(TPE2, "Band/orchestra/accompaniment");
        ELEMENT_CASE(TPE3, "Conductor/performer refinement");
        ELEMENT_CASE(TPE4, "Interpreted, remixed, or otherwise modified by");
        ELEMENT_CASE(TPOS, "Part of a set");
        ELEMENT_CASE(TPRO, "Produced notice");
        ELEMENT_CASE(TPUB, "Publisher");
        ELEMENT_CASE(TRCK, "Track number/Position in set");
        ELEMENT_CASE(TRDA, "Recording dates");
        ELEMENT_CASE(TRSN, "Internet radio station name");
        ELEMENT_CASE(TRSO, "Internet radio station owner");
        ELEMENT_CASE(TSIZ, "Size");
        ELEMENT_CASE(TSO2, "Performer order");
        ELEMENT_CASE(TSOA, "Album sort order");
        ELEMENT_CASE(TSOC, "Composer sort order");
        ELEMENT_CASE(TSOP, "Performer sort order");
        ELEMENT_CASE(TSOT, "Title sort order");
        ELEMENT_CASE(TSRC, "ISRC (international standard recording code)");
        ELEMENT_CASE(TSSE, "Software/Hardware and settings used for encoding");
        ELEMENT_CASE(TSST, "Set subtitle");
        ELEMENT_CASE(TXXX, "User defined text information frame");
        ELEMENT_CASE(TYER, "Year");
        ELEMENT_CASE(UFID, "Unique file identifier");
        ELEMENT_CASE(USER, "Terms of use");
        ELEMENT_CASE(USLT, "Unsynchronised lyric/text transcription");
        ELEMENT_CASE(WCOM, "Commercial information");
        ELEMENT_CASE(WCOP, "Copyright/Legal information");
        ELEMENT_CASE(WOAF, "Official audio file webpage");
        ELEMENT_CASE(WOAR, "Official artist/performer webpage");
        ELEMENT_CASE(WOAS, "Official audio source webpage");
        ELEMENT_CASE(WORS, "Official Internet radio station homepage");
        ELEMENT_CASE(WPAY, "Payment");
        ELEMENT_CASE(WPUB, "Publishers official webpage");
        ELEMENT_CASE(WXXX, "User defined URL link frame");
        ELEMENT_CASE(XRVA, "Relative volume adjustment (2)");
        ELEMENT_CASE(BUF,  "Recommended buffer size");
        ELEMENT_CASE(CNT,  "Play counter");
        ELEMENT_CASE(COM,  "Comments");
        ELEMENT_CASE(CRA,  "Audio encryption");
        ELEMENT_CASE(CRM,  "Encrypted meta frame");
        ELEMENT_CASE(EQU,  "Equalization");
        ELEMENT_CASE(ETC,  "Event timing codes");
        ELEMENT_CASE(GEO,  "General encapsulated object");
        ELEMENT_CASE(IPL,  "Involved people list");
        ELEMENT_CASE(LNK,  "Linked information");
        ELEMENT_CASE(MCI,  "Music CD Identifier");
        ELEMENT_CASE(MLL,  "MPEG location lookup table");
        ELEMENT_CASE(PIC_, "Attached picture");
        ELEMENT_CASE(POP,  "Popularimeter");
        ELEMENT_CASE(REV,  "Reverb");
        ELEMENT_CASE(RVA,  "Relative volume adjustment");
        ELEMENT_CASE(SLT,  "Synchronized lyric/text");
        ELEMENT_CASE(STC,  "Synced tempo codes");
        ELEMENT_CASE(TAL,  "Album/Movie/Show title");
        ELEMENT_CASE(TBP,  "BPM (Beats Per Minute)");
        ELEMENT_CASE(TCM,  "Composer");
        ELEMENT_CASE(TCO,  "Content type");
        ELEMENT_CASE(TCP,  "iTunes Compilation Flag");
        ELEMENT_CASE(TCR,  "Copyright message");
        ELEMENT_CASE(TDA,  "Date");
        ELEMENT_CASE(TDY,  "Playlist delay");
        ELEMENT_CASE(TEN,  "Encoded by");
        ELEMENT_CASE(TFT,  "File type");
        ELEMENT_CASE(TIM,  "Time");
        ELEMENT_CASE(TKE,  "Initial key");
        ELEMENT_CASE(TLA,  "Language(s)");
        ELEMENT_CASE(TLE,  "Length");
        ELEMENT_CASE(TMT,  "Media type");
        ELEMENT_CASE(TOA,  "Original artist(s)/performer(s)");
        ELEMENT_CASE(TOF,  "Original filename");
        ELEMENT_CASE(TOL,  "Original Lyricist(s)/text writer(s)");
        ELEMENT_CASE(TOR,  "Original release year");
        ELEMENT_CASE(TOT,  "Original album/Movie/Show title");
        ELEMENT_CASE(TP1,  "Lead artist(s)/Lead performer(s)/Soloist(s)/Performing group");
        ELEMENT_CASE(TP2,  "Band/Orchestra/Accompaniment");
        ELEMENT_CASE(TP3,  "Conductor/Performer refinement");
        ELEMENT_CASE(TP4,  "Interpreted,  remixed,  or otherwise modified by");
        ELEMENT_CASE(TPA,  "Part of a set");
        ELEMENT_CASE(TPB,  "Publisher");
        ELEMENT_CASE(TRC,  "ISRC (International Standard Recording Code)");
        ELEMENT_CASE(TRD,  "Recording dates");
        ELEMENT_CASE(TRK,  "Track number/Position in set");
        ELEMENT_CASE(TSI,  "Size");
        ELEMENT_CASE(TSS,  "Software/hardware and settings used for encoding");
        ELEMENT_CASE(TT1,  "Content group description");
        ELEMENT_CASE(TT2,  "Title/Songname/Content description");
        ELEMENT_CASE(TT3,  "Subtitle/Description refinement");
        ELEMENT_CASE(TXT,  "Lyricist/text writer");
        ELEMENT_CASE(TXX,  "User defined text information frame");
        ELEMENT_CASE(TYE,  "Year");
        ELEMENT_CASE(UFI,  "Unique file identifier");
        ELEMENT_CASE(ULT,  "Unsychronized lyric/text transcription");
        ELEMENT_CASE(WAF,  "Official audio file webpage");
        ELEMENT_CASE(WAR,  "Official artist/performer webpage");
        ELEMENT_CASE(WAS,  "Official audio source webpage");
        ELEMENT_CASE(WCM,  "Commercial information");
        ELEMENT_CASE(WCP,  "Copyright/Legal information");
        ELEMENT_CASE(WPB,  "Publishers official webpage");
        ELEMENT_CASE(WXX,  "User defined URL link frame");
        default : Skip_XX(Element_Size,                         "Data");
    }

    if (!Unsynch_List.empty())
    {
        //We must change the buffer for keeping out
        Element_Size=Save_Element_Size;
        Buffer_Offset=Save_Buffer_Offset;
        delete[] Buffer; Buffer=Save_Buffer;
        Buffer_Unsynch=NULL; //Same as Buffer...
        Element_Offset+=Unsynch_List.size();
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
//
void File_Id3v2::____()
{
    //Filling
    Fill_Name();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::T___()
{
    int8u Encoding;
    Get_B1 (Encoding,                                           "Text_encoding");
    switch (Encoding)
    {
        case 0 : Get_Local (Element_Size-1, Element_Value,      "Information"); break;
        case 1 : Get_UTF16 (Element_Size-1, Element_Value,      "Information"); break;
        case 2 : Get_UTF16B(Element_Size-1, Element_Value,      "Information"); break;
        case 3 : Get_UTF8  (Element_Size-1, Element_Value,      "Information"); break;
        default : ;
    }

    //Filling
    Fill_Name();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::T__X()
{
    //Integrity
    if (Element_Size<(Id3::TXXX?4:1))
    {
        Element_Values(1).clear();
        Element_Values(0).clear();
        return;
    }

    int8u Encoding;
    Get_B1 (Encoding,                                           "Text_encoding");
    if (Element_Code!=Id3::TXXX)
        Skip_C3(                                                "Language");
    size_t Value0_Size=0;
    switch (Encoding)
    {
        case 0 :
        case 3 : //1-byte char
                while (Element_Offset+Value0_Size<Element_Size && Buffer[Buffer_Offset+(size_t)Element_Offset+Value0_Size]!='\0')
                    Value0_Size++;
                if (Element_Offset+Value0_Size>=Element_Size)
                    return; //Problem
                switch (Encoding)
                {
                    case 0 : Get_Local (Value0_Size, Element_Values(0), "Short_content_descrip"); break;
                    case 3 : Get_UTF8  (Value0_Size, Element_Values(0), "Short_content_descrip"); break;
                    default : ;
                }
                Skip_B1(                                        "Null");
                switch (Encoding)
                {
                    case 0 : Get_Local (Element_Size-Element_Offset, Element_Values(1), "The_actual_text"); break;
                    case 3 : Get_UTF8  (Element_Size-Element_Offset, Element_Values(1), "The_actual_text"); break;
                    default : ;
                }
                break;
        case 1 :
        case 2 : //2-byte char
                while (Element_Offset+Value0_Size+1<Element_Size
                    && !(Buffer[Buffer_Offset+(size_t)Element_Offset+Value0_Size  ]=='\0'
                      && Buffer[Buffer_Offset+(size_t)Element_Offset+Value0_Size+1]=='\0')) //2-byte zero
                    Value0_Size+=2;
                if (Element_Offset+Value0_Size>=Element_Size)
                    return; //Problem
                switch (Encoding)
                {
                    case 1 : Get_UTF16 (Value0_Size, Element_Values(0), "Short_content_descrip"); break;
                    case 2 : Get_UTF16B(Value0_Size, Element_Values(0), "Short_content_descrip"); break;
                    default : ;
                }
                Skip_B2(                                        "Null");
                switch (Encoding)
                {
                    case 1 : Get_UTF16 (Element_Size-Element_Offset, Element_Values(1), "The_actual_text"); break;
                    case 2 : Get_UTF16B(Element_Size-Element_Offset, Element_Values(1), "The_actual_text"); break;
                    default : ;
                }
                break;
        default: //Unknown
                Skip_XX(Element_Size-Element_Offset,            "Unknown");
        ;
    }
}

//---------------------------------------------------------------------------
//
void File_Id3v2::W___()
{
    Get_Local(Element_Size, Element_Value,                      "URL");

    //Filling
    Fill_Name();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::W__X()
{
    if (Element_Size<1)
        return; //Problem

    int8u Encoding;
    Get_B1 (Encoding,                                           "Text_encoding");
    switch (Encoding)
    {
        case 0 : Get_Local (Element_Size-1, Element_Values(0),  "Description"); break;
        case 1 : Get_UTF16 (Element_Size-1, Element_Values(0),  "Description"); break;
        case 2 : Get_UTF16B(Element_Size-1, Element_Values(0),  "Description"); break;
        case 3 : Get_UTF8  (Element_Size-1, Element_Values(0),  "Description"); break;
        default : ;
    }
    Element_Offset=1;
    if (Encoding==1)
        Element_Offset+=Element_Values(0).size()*2+4; //UTF-16 BOM + UTF-16 NULL
    else if (Encoding==1 || Encoding==2)
        Element_Offset+=Element_Values(0).size()*2+2; //UTF-16 NULL
    else
        Element_Offset+=Element_Values(0).size()+1;   //UTF-8 NULL
    Get_Local(Element_Size-Element_Offset, Element_Values(1),      "URL");
}

//---------------------------------------------------------------------------
//
void File_Id3v2::APIC()
{
    int8u Encoding, PictureType;
    Ztring Mime, Description;
    Get_B1 (Encoding,                                           "Text_encoding");
    if (Id3v2_Version==2)
    {
        int32u Image_format;
        Get_C3(Image_format,                                    "Image_format");
        switch (Image_format)
        {
            case 0x504E47 : Mime="image/png";
            case 0x4A5047 : Mime="image/jpeg";
            default       : ;
        }
    }
    else
        Get_Local(Element_Size-1, Mime,                         "MIME_type");
    Element_Offset=1+Mime.size()+1;
    Get_B1 (PictureType,                                        "Picture_type"); Element_Info(Id3v2_PictureType(PictureType));
    Get_Local(Element_Size-Element_Offset, Description,         "Description");
    Element_Offset=1+Mime.size()+1+1+Description.size()+1;
    if (Element_Offset>Element_Size)
        return; //There is a problem
    std::string Data_Raw((const char*)(Buffer+(size_t)(Buffer_Offset+Element_Offset)), (size_t)(Element_Size-Element_Offset));
    std::string Data_Base64(Base64::encode(Data_Raw));

    //Filling
    Fill_Name();
    Fill(Stream_General, 0, General_Cover_Description, Description);
    Fill(Stream_General, 0, General_Cover_Type, Id3v2_PictureType(PictureType));
    Fill(Stream_General, 0, General_Cover_Mime, Mime);
    Fill(Stream_General, 0, General_Cover_Data, Data_Base64);
}

//---------------------------------------------------------------------------
//
void File_Id3v2::COMM()
{
    T__X();

    //Testing
         if (Element_Values(0)==_T("Songs-DB_Tempo")) return;
    else if (Element_Values(0)==_T("Songs-DB_Preference")) return;
    else if (Element_Values(0)==_T("MusicMatch_Tempo")) return;
    else if (Element_Values(0)==_T("MusicMatch_Mood"))
    {
        if (Retrieve(Stream_General, 0, General_Mood).empty())
            Element_Values(0)==_T("Mood");
        else
            return;
    }
    else if (Element_Values(0)==_T("MusicMatch_Preference")) return;

    //Filling
    if (Element_Values(0).empty())
        Element_Values(0)=_T("Comment");
    Fill_Name();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::RGAD()
{
    //Parsing
    float32 Peak_Amplitude;
    Get_BF4 (Peak_Amplitude,                                    "Peak Amplitude");
    while (Element_Offset+2<=Element_Size)
    {
        Element_Begin("Gain Adjustement", 2);
        int16u Replay_Gain_Adjustment;
        int8u  Name_code;
        bool   Sign_bit;
        BS_Begin();
        Get_S1 (3, Name_code,                                   "Name code"); Param_Info(Id3v2_RGAD_Name_code[Name_code]);
        Info_S1(3, Originator_code,                             "Originator code"); Param_Info(Id3v2_RGAD_Originator_code[Originator_code]);
        Get_SB (Sign_bit,                                       "Sign bit");
        Get_S2 (9, Replay_Gain_Adjustment,                      "Replay Gain Adjustment"); Param_Info ((Sign_bit?-1:1)*(float)Replay_Gain_Adjustment/10, 1, " dB");
        BS_End();
        Element_End();

        FILLING_BEGIN();
            switch (Name_code)
            {
                case 1 : if (Retrieve(Stream_Audio, 0, Audio_ReplayGain_Gain).empty()) //this tag is not precise, we prefer other RG tags
                            Fill(Stream_Audio, 0, Audio_ReplayGain_Gain, (Sign_bit?-1:1)*(float)Replay_Gain_Adjustment/10, 1);
                case 2 : if (Retrieve(Stream_General, 0, General_Album_ReplayGain_Gain).empty()) //this tag is not precise, we prefer other RG tags
                            Fill(Stream_General, 0, General_Album_ReplayGain_Gain, (Sign_bit?-1:1)*(float)Replay_Gain_Adjustment/10, 1);
            }
        FILLING_END();
    }

    FILLING_BEGIN();
        if (Peak_Amplitude && Retrieve(Stream_Audio, 0, Audio_ReplayGain_Peak).empty()) //this tag is not precise, we prefer other RG tags
            Fill(Stream_Audio, 0, Audio_ReplayGain_Peak, Peak_Amplitude, 6);
    FILLING_END();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::PRIV()
{
    //Parsing
    Ztring Owner;
    Get_Local(Element_Size, Owner,                              "Owner identifier");
    Element_Offset=Owner.size()+1;
    Skip_XX(Element_Size-Element_Offset,                        "Data");
}

//---------------------------------------------------------------------------
//
void File_Id3v2::USLT()
{
    T__X();

    //Filling
    if (!Element_Values(0).empty())
        Element_Values(1)=Element_Values(0)+MediaInfoLib::Config.Language_Get(_T(": "))+Element_Values(1);
    Element_Values(0)=_T("Lyrics");

    Fill_Name();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::TXXX()
{
    T__X();

    //Filling
    if (Element_Values(0).empty())
        Element_Values(0)=_T("Comment");
    Fill_Name();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::SYLT()
{
    int8u Encoding;
    Get_B1 (Encoding,                                           "Text encoding");
    Skip_C3(                                                    "Language");
    Skip_B1(                                                    "Time_stamp_format");
    Skip_B1(                                                    "Content_type");
    switch (Encoding)
    {
        case 0 : Get_Local (Element_Size-6, Element_Value,      "Short_content_descrip"); break;
        case 1 : Get_UTF16 (Element_Size-6, Element_Value,      "Short_content_descrip"); break;
        case 2 : Get_UTF16B(Element_Size-6, Element_Value,      "Short_content_descrip"); break;
        case 3 : Get_UTF8  (Element_Size-6, Element_Value,      "Short_content_descrip"); break;
        default : ;
    }

    //Filling
    Fill_Name();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::WXXX()
{
    W__X();

    //Filling
    if (Element_Values(0).empty())
        Element_Values(0)=_T("URL");
    Fill_Name();
}

//---------------------------------------------------------------------------
//
void File_Id3v2::Fill_Name()
{
    switch (Element_Code)
    {
        case Id3::AENC : break;
        case Id3::APIC : Fill(Stream_General, 0, General_Cover, "Yes"); break;
        case Id3::ASPI : break;
        case Id3::COMM : Fill(Stream_General, 0, Element_Values(0).To_UTF8().c_str(), Element_Values(1)); break;
        case Id3::COMR : Fill(Stream_General, 0, "Commercial frame", Element_Value); break;
        case Id3::ENCR : break;
        case Id3::EQU2 : break;
        case Id3::EQUA : break;
        case Id3::ETCO : break;
        case Id3::GEOB : break;
        case Id3::GRID : Fill(Stream_General, 0, "Group identification registration", Element_Value); break;
        case Id3::IPLS : Fill(Stream_General, 0, "Involved people list", Element_Value); break;
        case Id3::LINK : Fill(Stream_General, 0, "Linked information", Element_Value); break;
        case Id3::MCDI : Fill(Stream_General, 0, "MCDI", "Yes"); break;
        case Id3::MLLT : break;
        case Id3::OWNE : Fill(Stream_General, 0, General_Owner, Element_Value); break;
        case Id3::PCNT : break;
        case Id3::POPM : break;
        case Id3::POSS : break;
        case Id3::PRIV : break;
        case Id3::RBUF : break;
        case Id3::RVA2 : break;
        case Id3::RVRB : break;
        case Id3::SEEK : break;
        case Id3::SIGN : break;
        case Id3::SYLT : Fill(Stream_General, 0, General_Lyrics, Element_Value); break;
        case Id3::SYTC : break;
        case Id3::TALB : Fill(Stream_General, 0, General_Album, Element_Value); break;
        case Id3::TBPM : Fill(Stream_General, 0, General_BPM, Element_Value); break;
        case Id3::TCOM : Fill(Stream_General, 0, General_Composer, Element_Value); break;
        case Id3::TCON : Fill(Stream_General, 0, General_Genre, Element_Value); break;
        case Id3::TCOP : Fill(Stream_General, 0, General_Copyright, Element_Value); break;
        case Id3::TDA  :
        case Id3::TDAT : if (Element_Value.size()==4)
                         {
                            Month.assign(Element_Value.c_str(), 0, 2);
                            Day.assign  (Element_Value.c_str(), 2, 2); break;
                         }
        case Id3::TDEN : Normalize_Date(Element_Value); Fill(Stream_General, 0, "Encoded_Date", Element_Value); break;
        case Id3::TDLY : break;
        case Id3::TDOR : Normalize_Date(Element_Value); Fill(Stream_General, 0, "Original/Released_Date", Element_Value); break;
        case Id3::TDRC : Normalize_Date(Element_Value); Fill(Stream_General, 0, General_Recorded_Date, Element_Value); break;
        case Id3::TDRL : Normalize_Date(Element_Value); Fill(Stream_General, 0, General_Released_Date, Element_Value); break;
        case Id3::TDTG : Normalize_Date(Element_Value); Fill(Stream_General, 0, General_Tagged_Date, Element_Value); break;
        case Id3::TENC : Fill(Stream_General, 0, General_Encoded_Library, Element_Value); break;
        case Id3::TEXT : Fill(Stream_General, 0, General_Lyricist, Element_Value); break;
        case Id3::TFLT : Fill(Stream_General, 0, "File type", Element_Value); break;
        case Id3::TIM  :
        case Id3::TIME : if (Element_Value.size()==4)
                         {
                            Hour.assign  (Element_Value.c_str(), 0, 2);
                            Minute.assign(Element_Value.c_str(), 2, 2); break;
                         }
        case Id3::TIPL : Fill(Stream_General, 0, General_ThanksTo, Element_Value); break;
        case Id3::TIT1 : Fill(Stream_General, 0, General_ContentType, Element_Value); break;
        case Id3::TIT2 : Fill(Stream_General, 0, General_Track, Element_Value); break;
        case Id3::TIT3 : Fill(Stream_General, 0, General_Track_More, Element_Value); break;
        case Id3::TKEY : Fill(Stream_General, 0, "Initial key", Element_Value); break;
        case Id3::TLAN : Fill(Stream_Audio,   0, Audio_Language, Element_Value); break;
        case Id3::TLEN : break; //Fill(Stream_General, 0, "Length", Element_Value); break;
        case Id3::TMCL : Fill(Stream_General, 0, "Musician Credit List", Element_Value); break;
        case Id3::TMED : Fill(Stream_General, 0, "Media Type", Element_Value); break;
        case Id3::TMOO : Fill(Stream_General, 0, "Mood", Element_Value); break;
        case Id3::TOAL : Fill(Stream_General, 0, "Original/Album", Element_Value); break;
        case Id3::TOFN : Fill(Stream_General, 0, "Original/FileName", Element_Value); break;
        case Id3::TOLY : Fill(Stream_General, 0, "Original/Lyricist", Element_Value); break;
        case Id3::TOPE : Fill(Stream_General, 0, "Original/Performer", Element_Value); break;
        case Id3::TORY : Normalize_Date(Element_Value); Fill(Stream_General, 0, "Original/Released_Date", Element_Value); break;
        case Id3::TOWN : Fill(Stream_General, 0, General_Owner, Element_Value); break;
        case Id3::TPE1 : Fill(Stream_General, 0, General_Performer, Element_Value); break;
        case Id3::TPE2 : Fill(Stream_General, 0, General_Accompaniment, Element_Value); break;
        case Id3::TPE3 : Fill(Stream_General, 0, General_Conductor, Element_Value); break;
        case Id3::TPE4 : Fill(Stream_General, 0, General_RemixedBy, Element_Value); break;
        case Id3::TPOS : Fill(Stream_General, 0, General_Part_Position, Element_Value); break;
        case Id3::TPRO : Fill(Stream_General, 0, General_Producer_Copyright, Element_Value); break;
        case Id3::TPUB : Fill(Stream_General, 0, General_Publisher, Element_Value); break;
        case Id3::TRCK : Fill(Stream_General, 0, General_Track_Position, Element_Value); break;
        case Id3::TRDA : Normalize_Date(Element_Value); Fill(Stream_General, 0, "Recorded_Date", Element_Value); break;
        case Id3::TRSN : Fill(Stream_General, 0, General_ServiceName, Element_Value); break;
        case Id3::TRSO : Fill(Stream_General, 0, General_ServiceProvider, Element_Value); break;
        case Id3::TSIZ : Fill(Stream_General, 0, "Size", Element_Value); break;
        case Id3::TSO2 : Fill(Stream_General, 0, General_Performer_Sort, Element_Value); break;
        case Id3::TSOA : Fill(Stream_General, 0, General_Album_Sort, Element_Value); break;
        case Id3::TSOC : Fill(Stream_General, 0, "Composer/Sort", Element_Value); break;
        case Id3::TSOP : Fill(Stream_General, 0, General_Performer_Sort, Element_Value); break;
        case Id3::TSOT : Fill(Stream_General, 0, General_Track_Sort, Element_Value); break;
        case Id3::TSRC : Fill(Stream_General, 0, General_ISRC, Element_Value); break;
        case Id3::TSSE : Fill(Stream_General, 0, General_Encoded_Library_Settings, Element_Value); break;
        case Id3::TSST : Fill(Stream_General, 0, "Set subtitle", Element_Value); break;
        case Id3::TXXX :      if (Element_Values(0)==_T("first_played_timestamp")) Fill(Stream_General, 0, General_Played_First_Date,       Ztring().Date_From_Milliseconds_1601(Element_Values(1).To_int64u()/10000));
                         else if (Element_Values(0)==_T("last_played_timestamp"))  Fill(Stream_General, 0, General_Played_Last_Date,        Ztring().Date_From_Milliseconds_1601(Element_Values(1).To_int64u()/10000));
                         else if (Element_Values(0)==_T("play_count"))             Fill(Stream_General, 0, General_Played_Count,            Element_Values(1).To_int64u());
                         else if (Element_Values(0)==_T("added_timestamp"))        Fill(Stream_General, 0, General_Added_Date,              Ztring().Date_From_Milliseconds_1601(Element_Values(1).To_int64u()/10000));
                         else if (Element_Values(0)==_T("replaygain_album_gain"))  Fill(Stream_General, 0, General_Album_ReplayGain_Gain,   Element_Values(1).To_float64(), 2, true);
                         else if (Element_Values(0)==_T("replaygain_album_peak"))  Fill(Stream_General, 0, General_Album_ReplayGain_Peak,   Element_Values(1).To_float64(), 6, true);
                         else if (Element_Values(0)==_T("replaygain_track_gain"))  Fill(Stream_Audio,   0, Audio_ReplayGain_Gain,           Element_Values(1).To_float64(), 2, true);
                         else if (Element_Values(0)==_T("replaygain_track_peak"))  Fill(Stream_Audio,   0, Audio_ReplayGain_Peak,           Element_Values(1).To_float64(), 6, true);
                         else
                            Fill(Stream_General, 0, Element_Values(0).To_UTF8().c_str(), Element_Values(1));
                         break;
        case Id3::TYER : Year=Element_Value; break;
        case Id3::UFID : Fill(Stream_Audio,   0, "UID", Element_Value); break;
        case Id3::USER : Fill(Stream_General, 0, General_TermsOfUse, Element_Value); break;
        case Id3::USLT : Fill(Stream_General, 0, Element_Values(0).To_UTF8().c_str(), Element_Values(1)); break;
        case Id3::WCOM : Fill(Stream_General, 0, "Commercial information", Element_Value); break;
        case Id3::WCOP : Fill(Stream_General, 0, "Copyright/Legal information", Element_Value); break;
        case Id3::WOAF : Fill(Stream_General, 0, "Official audio file webpage", Element_Value); break;
        case Id3::WOAR : Fill(Stream_General, 0, "Performer/Url", Element_Value); break;
        case Id3::WOAS : Fill(Stream_General, 0, "Official audio source webpage", Element_Value); break;
        case Id3::WORS : Fill(Stream_General, 0, General_Service_Url, Element_Value); break;
        case Id3::WPAY : Fill(Stream_General, 0, "Payment", Element_Value); break;
        case Id3::WPUB : Fill(Stream_General, 0, "Publisher/Url", Element_Value); break;
        case Id3::WXXX : Fill(Stream_General, 0, Element_Values(0).To_UTF8().c_str(), Element_Values(1)); break;
        case Id3::BUF  : break;
        case Id3::CNT  : break;
        case Id3::COM  : Fill(Stream_General, 0, "Comment", Element_Value); break;
        case Id3::CRA  : break;
        case Id3::CRM  : break;
        case Id3::EQU  : break;
        case Id3::ETC  : break;
        case Id3::GEO  : break;
        case Id3::IPL  : Fill(Stream_General, 0, "Involved people list", Element_Value); break;
        case Id3::LNK  : Fill(Stream_General, 0, "Linked information,", Element_Value); break;
        case Id3::MCI  : Fill(Stream_General, 0, "MCDI", Element_Value); break;
        case Id3::MLL  : break;
        case Id3::PIC_ : Fill(Stream_General, 0, "Cover", "Yes"); break;
        case Id3::POP  : break;
        case Id3::REV  : break;
        case Id3::RVA  : break;
        case Id3::SLT  : break;
        case Id3::STC  : break;
        case Id3::TAL  : Fill(Stream_General, 0, "Album", Element_Value); break;
        case Id3::TBP  : Fill(Stream_General, 0, "BPM", Element_Value); break;
        case Id3::TCM  : Fill(Stream_General, 0, "Composer", Element_Value); break;
        case Id3::TCO  : Fill(Stream_General, 0, "Genre", Element_Value); break;
        case Id3::TCR  : Fill(Stream_General, 0, "Copyright", Element_Value); break;
        case Id3::TDY  : break;
        case Id3::TEN  : Fill(Stream_General, 0, "Encoded_Library", Element_Value); break;
        case Id3::TFT  : Fill(Stream_General, 0, "File type", Element_Value); break;
        case Id3::TKE  : Fill(Stream_General, 0, "Initial key", Element_Value); break;
        case Id3::TLA  : Fill(Stream_Audio,   0, Audio_Language, Element_Value); break;
        case Id3::TLE  : break;
        case Id3::TMT  : Fill(Stream_General, 0, "Media type", Element_Value); break;
        case Id3::TOA  : Fill(Stream_General, 0, "Original/Performer", Element_Value); break;
        case Id3::TOF  : Fill(Stream_General, 0, "Original/FileName", Element_Value); break;
        case Id3::TOL  : Fill(Stream_General, 0, "Original/Lyricist", Element_Value); break;
        case Id3::TOR  : Normalize_Date(Element_Value); Fill(Stream_General, 0, "Original/Released_Date", Element_Value); break;
        case Id3::TOT  : Fill(Stream_General, 0, "Original/Album", Element_Value); break;
        case Id3::TP1  : Fill(Stream_General, 0, "Performer", Element_Value); break;
        case Id3::TP2  : Fill(Stream_General, 0, "Accompaniment", Element_Value); break;
        case Id3::TP3  : Fill(Stream_General, 0, "Conductor", Element_Value); break;
        case Id3::TP4  : Fill(Stream_General, 0, "RemixedBy", Element_Value); break;
        case Id3::TPA  : Fill(Stream_General, 0, "Part/Position", Element_Value); break;
        case Id3::TPB  : Fill(Stream_General, 0, "Publisher", Element_Value); break;
        case Id3::TRC  : Fill(Stream_General, 0, "ISRC", Element_Value); break;
        case Id3::TRD  : Normalize_Date(Element_Value); Fill(Stream_General, 0, "Recorded_Date", Element_Value); break;
        case Id3::TRK  : Fill(Stream_General, 0, "Track/Position", Element_Value); break;
        case Id3::TSI  : break;
        case Id3::TSS  : break;
        case Id3::TT1  : Fill(Stream_General, 0, "ContentType", Element_Value); break;
        case Id3::TT2  : Fill(Stream_General, 0, "Track", Element_Value); break;
        case Id3::TT3  : Fill(Stream_General, 0, "Track/More", Element_Value); break;
        case Id3::TXT  : Fill(Stream_General, 0, "Lyricist", Element_Value); break;
        case Id3::TXX  : Fill(Stream_General, 0, Element_Values(0).To_UTF8().c_str(), Element_Values(1)); break;
        case Id3::TYE  : Year=Element_Value; break;
        case Id3::UFI  : Fill(Stream_Audio,   0, "UID", Element_Value); break;
        case Id3::ULT  : break;
        case Id3::WAF  : break;
        case Id3::WAR  : Fill(Stream_General, 0, General_Service_Url, Element_Value); break;
        case Id3::WAS  : Fill(Stream_General, 0, "Official audio source webpage", Element_Value); break;
        case Id3::WCM  : Fill(Stream_General, 0, "Commercial information", Element_Value); break;
        case Id3::WCP  : Fill(Stream_General, 0, "Copyright/Legal information", Element_Value); break;
        case Id3::WPB  : Fill(Stream_General, 0, "Publisher/Url", Element_Value); break;
        case Id3::WXX  : Fill(Stream_General, 0, Element_Values(0).To_UTF8().c_str(), Element_Values(1)); break;
        default : ;
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

void File_Id3v2::Normalize_Date(Ztring& Date)
{
    if (Date.size()<=8)
        return; //Format unknown
    Date[8]=_T(' '); //could be "T"
    Date=Ztring(_T("UTC "))+Date; //Id3v2 specify a UTC date
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES

