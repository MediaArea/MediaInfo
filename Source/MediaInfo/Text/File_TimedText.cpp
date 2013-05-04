// File_TimeText - Info for Time Text (MPEG-4 Part 17)
// Copyright (C) 2013-2013 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Pre-compilation
#include "MediaInfo/PreComp.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Setup.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_TIMEDTEXT_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Text/File_TimedText.h"
#include "tinyxml2.h"
using namespace tinyxml2;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_TimedText::File_TimedText()
{
    //Configuration
    ParserName=__T("Timed Text");

    //Temp
    #ifdef MEDIAINFO_MPEG4_YES
        IsChapter=false;
    #endif //MEDIAINFO_MPEG4_YES
}

//***************************************************************************
// Buffer - Per element
//***************************************************************************

//---------------------------------------------------------------------------
void File_TimedText::Header_Parse()
{
    //Parsing
    int16u Size;
    Get_B2 (Size,                                               "Size");

    //Filling
    Header_Fill_Code(0, "Block");
    Header_Fill_Size(Element_Offset+Size);
}

//---------------------------------------------------------------------------
void File_TimedText::Data_Parse()
{
    //Parsing
    Ztring Value;
    Get_UTF8 (Element_Size, Value,                              "Value");

    FILLING_BEGIN();
        if (!Status[IsAccepted])
        {
            Accept();
            #ifdef MEDIAINFO_MPEG4_YES
                if (IsChapter)
                {
                    Stream_Prepare(Stream_Menu);
                }
                else
            #endif //MEDIAINFO_MPEG4_YES
                {
                    Stream_Prepare(Stream_Text);
                    Fill(Stream_Text, 0, Text_Format, "Timed Text");
                }
        }
        #ifdef MEDIAINFO_MPEG4_YES
            if (IsChapter)
            {
            }
            else
        #endif //MEDIAINFO_MPEG4_YES
            {
                Finish();
            }

        #ifdef MEDIAINFO_MPEG4_YES
            if (IsChapter && FrameInfo.DTS!=(int64u)-1)
                Fill(Stream_Menu, 0, Ztring().Duration_From_Milliseconds(FrameInfo.DTS/1000000).To_UTF8().c_str(), Value);
        #endif //MEDIAINFO_MPEG4_YES
    FILLING_END();
}

} //NameSpace

#endif //MEDIAINFO_TIMEDTEXT_YES
