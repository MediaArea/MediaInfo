// File_Bdav - Info for BluRay Audio/Video files
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
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_BDAV_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Bdav.h"
#include "MediaInfo/Multiple/File_MpegTs.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Bdav::File_Bdav()
: File_MpegTs()
{
    TS_Size=188+4;
    BDAV_Size=4; //BDAV header
}

//***************************************************************************
// Base
//***************************************************************************

//---------------------------------------------------------------------------
void File_Bdav::Read_Buffer_Finalize()
{
    File_MpegTs::Read_Buffer_Finalize();

    //Fill General
    Fill(Stream_General, 0, General_Format, "BDAV", Unlimited, false, true);
}

} //NameSpace

#endif //MEDIAINFO_BDAV_YES
