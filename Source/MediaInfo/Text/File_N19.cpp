// File_N19 - Info for N19 files
// Copyright (C) 2010-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
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
#if defined(MEDIAINFO_N19_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Text/File_N19.h"
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Buffer - File header
//***************************************************************************

//---------------------------------------------------------------------------
bool File_N19::FileHeader_Begin()
{
    //Element_Size
    if (Buffer_Size<11)
        return false; //Must wait for more data

    int64u DiskFormatCode=CC8(Buffer+3);
    if (DiskFormatCode!=0x53544C32352E3031LL
     && DiskFormatCode!=0x53544C33302E3031LL)
    {
        Reject("N19");
        return false;
    }

    //All should be OK...
    return true;
}

//---------------------------------------------------------------------------
void File_N19::FileHeader_Parse()
{
    //Parsing
    Skip_B3(                                                    "Code Page Number");
    Skip_B8(                                                    "Disk Format Code");

    FILLING_BEGIN();
        Accept("N19");

        Fill(Stream_General, 0, General_Format, "N19");

        Stream_Prepare(Stream_Text);
        Fill(Stream_Text, 0, Text_Format, "N19");
    FILLING_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_EIA608_YES
