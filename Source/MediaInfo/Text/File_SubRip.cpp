// File_SubRip - Info for SubRip files
// Copyright (C) 2012-2012 MediaArea.net SARL, Info@MediaArea.net
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
#if defined(MEDIAINFO_SUBRIP_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Text/File_SubRip.h"
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_SubRip::Read_Buffer_Continue()
{
    if (!IsSub && (Buffer_Size<File_Size && Buffer_Size<65536))
    {
        Element_WaitForMoreData();
        return;
    }

    ZtringListList Temp;
    Temp.Separator_Set(0, __T("\r\n\r\n"));
    Temp.Separator_Set(1, __T("\r\n"));

    Temp.Write(Ztring().From_UTF8((const char*)Buffer, Buffer_Size>65536?65536:Buffer_Size));
    if (Temp.empty())
        Temp.Write(Ztring().From_Local((const char*)Buffer, Buffer_Size>65536?65536:Buffer_Size)); // Trying from local code page

    size_t IsOk=0;
    size_t IsNok=0;
    for (size_t Pos=0; Pos<Temp.size(); Pos++)
    {
        if (Temp(Pos, 0).To_int64u()==Pos+1)
            IsOk++;
        else
            IsNok++;

        if (Temp(Pos, 1).size()>22 && Temp(Pos, 1)[2]==__T(':') && Temp(Pos, 1)[5]==__T(':') && Temp(Pos, 1).find(__T(" --> "))!=string::npos)
            IsOk++;
        else
            IsNok++;
    }

    if (!IsOk || IsNok>IsOk/2)
    {
        Reject();
        return;
    }

    if (!Status[IsAccepted])
    {
        Accept();
        Fill(Stream_General, 0, General_Format, "SubRip");
        Stream_Prepare(Stream_Text);
        Fill(Stream_Text, 0, "Format", "SubRip");
        Fill(Stream_Text, 0, "Codec", "SubRip");
    }

    Element_Offset=File_Size;
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_SUBRIP_YES
