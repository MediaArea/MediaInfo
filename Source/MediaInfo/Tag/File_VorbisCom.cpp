// File_VorbisCom - Info for VorbisComments tagged files
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
#if defined(MEDIAINFO_VORBISCOM_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_VorbisCom.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_VorbisCom::File_VorbisCom()
:File__Analyze()
{
    //In
    StreamKind=Stream_General;
    StreamGoal=Stream_General;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_VorbisCom::FileHeader_Parse()
{
    //Parsing
    Element_Begin("Vorbis comment header");
        int32u vendor_length;
        Get_L4 (vendor_length,                                  "vendor_length");
        Skip_Local(vendor_length,                               "vendor_string");
        Get_L4 (user_comment_list_length,                       "user_comment_list_length");
    Element_End();

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        if (StreamKind!=Stream_General)
            Stream_Prepare(StreamKind);
        if (StreamGoal!=Stream_General && StreamGoal!=StreamKind)
            Stream_Prepare(StreamGoal);

        //Comments
        for (int32u Pos=0; Pos<user_comment_list_length; Pos++)
            Comment();

        //Positionning
        Finnished();
    FILLING_END();
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_VorbisCom::Comment()
{
    //Parsing
    Ztring comment;
    Element_Begin("Vorbis comment");
        int32u user_comment_length;
        Get_L4 (user_comment_length,                            "length");
        Get_UTF8(user_comment_length, comment,                  "comment");
        Element_Info(comment);
    Element_End();

    FILLING_BEGIN();
        Ztring Key=comment.SubString(_T(""), _T("="));
        Key.MakeUpperCase();
        Ztring Value=comment.SubString(_T("="), _T(""));

             if (Key==_T("ADDED_TIMESTAMP"))        Fill(Stream_General, 0, "Added_Date", Ztring().Date_From_Milliseconds_1601(Value.To_int64u()/1000));
        else if (Key==_T("ALBUM ARTIST"))           {if (Value!=Retrieve(Stream_General, 0, General_Performer)) Fill(Stream_General, 0, General_Performer, Value);}
        else if (Key==_T("ALBUM"))                  Fill(Stream_General, 0, General_Album, Value);
        else if (Key==_T("ARTIST"))                 {if (Value!=Retrieve(Stream_General, 0, General_Performer)) Fill(Stream_General, 0, General_Performer, Value);}
        else if (Key==_T("AUTHOR"))                 Fill(Stream_General, 0, General_Performer, Value);
        else if (Key==_T("COMMENT"))                Fill(Stream_General, 0, General_Comment, Value);
        else if (Key==_T("COMMENTS"))               Fill(Stream_General, 0, General_Comment, Value);
        else if (Key==_T("CONTACT"))                Fill(Stream_General, 0, General_Publisher, Value);
        else if (Key==_T("COPYRIGHT"))              Fill(Stream_General, 0, General_Copyright, Value);
        else if (Key==_T("DATE"))                   Fill(Stream_General, 0, General_Recorded_Date, Value);
        else if (Key==_T("DESCRIPTION"))            Fill(Stream_General, 0, General_Comment, Value);
        else if (Key==_T("ENCODER"))                Fill(Stream_General, 0, General_Encoded_Application, Value);
        else if (Key==_T("ENCODED_USING"))          Fill(Stream_General, 0, General_Encoded_Application, Value);
        else if (Key==_T("ENCODER_URL"))            Fill(Stream_General, 0, General_Encoded_Application_Url, Value);
        else if (Key==_T("GENRE"))                  Fill(Stream_General, 0, General_Genre, Value);
        else if (Key==_T("FIRST_PLAYED_TIMESTAMP")) Fill(StreamGoal,     0, "Played_First_Date", Ztring().Date_From_Milliseconds_1601(Value.To_int64u()/10000));
        else if (Key==_T("ISRC"))                   Fill(Stream_General, 0, General_ISRC, Value);
        else if (Key==_T("LANGUAGE"))               Fill(StreamKind,     0, "Language", Value);
        else if (Key==_T("LAST_PLAYED_TIMESTAMP"))  Fill(StreamGoal,     0, "Played_Last_Date", Ztring().Date_From_Milliseconds_1601(Value.To_int64u()/10000));
        else if (Key==_T("LICENCE"))                Fill(Stream_General, 0, General_TermsOfUse, Value);
        else if (Key==_T("LWING_GAIN"))             Fill(StreamGoal,     0, "ReplayGain_Gain", Value.To_float64(), 2);
        else if (Key==_T("LOCATION"))               Fill(Stream_General, 0, General_Recorded_Location, Value);
        else if (Key==_T("ORGANIZATION"))           Fill(Stream_General, 0, General_Producer, Value);
        else if (Key==_T("PERFORMER"))              Fill(Stream_General, 0, General_Performer, Value);
        else if (Key==_T("PLAY_COUNT"))             Fill(StreamGoal,     0, "Played_Count", Value.To_int64u());
        else if (Key==_T("REPLAYGAIN_ALBUM_GAIN"))  Fill(Stream_General, 0, "Album_ReplayGain_Gain", Value.To_float64(), 2);
        else if (Key==_T("REPLAYGAIN_ALBUM_PEAK"))  Fill(Stream_General, 0, "Album_ReplayGain_Peak", Value.To_float64(), 6);
        else if (Key==_T("REPLAYGAIN_TRACK_GAIN"))  Fill(StreamKind,     0, "ReplayGain_Gain",       Value.To_float64(), 2);
        else if (Key==_T("REPLAYGAIN_TRACK_PEAK"))  Fill(StreamKind,     0, "ReplayGain_Peak",       Value.To_float64(), 6);
        else if (Key==_T("TITLE"))                  Fill(Stream_General, 0, General_Title, Value);
        else if (Key==_T("TRACKNUMBER"))            Fill(Stream_General, 0, General_Track_Position, Value);
        else if (Key==_T("VERSION"))                Fill(Stream_General, 0, General_Track_More, Value);
        else if (Key.find(_T("CHAPTER"))==0)
        {
            if (Count_Get(Stream_Chapters)==0)
                Stream_Prepare(Stream_Chapters);
            if (Key.find(_T("NAME"))==Error)
            {
                Chapter_Pos=Key.SubString(_T("CHAPTER"), _T(""));
                Chapter_Time=Value;
            }
            else
            {
                Value.FindAndReplace(_T("\n"), _T(""), Count_Get(Stream_Text)-1); //Some chapters names have extra characters, not needed
                Value.FindAndReplace(_T("\r"), _T(""), Count_Get(Stream_Text)-1); //Some chapters names have extra characters, not needed
                Value.FindAndReplace(_T(" "),  _T(""), Count_Get(Stream_Text)-1); //Some chapters names have extra characters, not needed
                Fill(Stream_Chapters, 0, Chapter_Pos.To_Local().c_str(), Chapter_Time+_T(" ")+Value);
            }
        }
        else                                Fill(Stream_General, 0, comment.SubString(_T(""), _T("=")).To_Local().c_str(), Value);
    FILLING_END();
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_VorbisCom::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
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

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_VORBISCOM_YES

