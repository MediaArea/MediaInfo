// File_TimedText - Info for Time Text (MPEG-4 Part 17)
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
//
// Information about Timed Text (MPEG-4 Part 17)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_TimedTextH
#define MediaInfo_File_TimedTextH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Sami
//***************************************************************************

class File_TimedText : public File__Analyze
{
public:
    //Constructor/Destructor
    File_TimedText();

    #ifdef MEDIAINFO_MPEG4_YES
        bool IsChapter;
    #endif //MEDIAINFO_MPEG4_YES

private :
    //Buffer - Per element
    void Header_Parse();
    void Data_Parse();
};

} //NameSpace

#endif

