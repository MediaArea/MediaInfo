// File_Png - Info for PNG files
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// PNG - Format
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// From http://www.fileformat.info/format/png/
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_PNG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Image/File_Png.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Png::Read_Buffer_Continue()
{
    //Integrity
    if (Buffer_Size<8)
        return;

    //Parsing
    if (CC4(Buffer)==0x89504E47 && CC4(Buffer+4)==0x0D0A1A0A)
    {
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "PNG");

        Stream_Prepare(Stream_Image);
        Fill(Stream_Image, 0, Image_Format, "LZ77");
        Fill(Stream_Image, 0, Image_Format_Profile, "Variant");
        Fill(Stream_Image, 0, Image_Codec, "LZ77 variant");
    }
    else if (CC4(Buffer)==0x8A4E4E47 && CC4(Buffer+4)==0x0D0A1A0A)
    {
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "MNG");

        Stream_Prepare(Stream_Image);
        Fill(Stream_Image, 0, Image_Codec, "MNG");
        Fill(Stream_Image, 0, Image_Format, "MNG");
    }
    else if (CC4(Buffer)==0x8B4A4E47 && CC4(Buffer+4)==0x0D0A1A0A)
    {
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "JNG");

        Stream_Prepare(Stream_Image);
        Fill(Stream_Image, 0, Image_Format, "JNG");
        Fill(Stream_Image, 0, Image_Codec, "JNG");
    }

    //No need of more
    Finnished();
}

} //NameSpace

#endif
