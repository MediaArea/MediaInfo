// File_Aac - Info for AAC (Raw) files
// Copyright (C) 2008-2009 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_AAC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Aac.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Buffer - Global
//***************************************************************************

//---------------------------------------------------------------------------
void File_Aac::Read_Buffer_Continue()
{
    //It is impossible to detect... Default is no detection, only filling

    //Filling
    Stream_Prepare(Stream_General);
    Fill(Stream_General, 0, General_Format, "AAC");
    Stream_Prepare(Stream_Audio);
    Fill(Stream_General, 0, Audio_Format, "AAC");
    Fill(Stream_General, 0, Audio_Codec, "AAC");
    Ztring Profile;
    int8u Version=0, SBR=2, PS=2;
         if (0);
    else if (Codec==_T("A_AAC/MPEG2/MAIN"))     {Version=2; Profile=_T("Main");}
    else if (Codec==_T("A_AAC/MPEG2/LC"))       {Version=2; Profile=_T("LC");   SBR=0;}
    else if (Codec==_T("A_AAC/MPEG2/LC/SBR"))   {Version=2; Profile=_T("LC");   SBR=1;}
    else if (Codec==_T("A_AAC/MPEG2/SSR"))      {Version=2; Profile=_T("SSR");}
    else if (Codec==_T("A_AAC/MPEG4/MAIN"))     {Version=4; Profile=_T("Main");}
    else if (Codec==_T("A_AAC/MPEG4/LC"))       {Version=4; Profile=_T("LC");   SBR=0;}
    else if (Codec==_T("A_AAC/MPEG4/LC/SBR"))   {Version=4; Profile=_T("LC");   SBR=1; PS=0;}
    else if (Codec==_T("A_AAC/MPEG4/LC/SBR/PS")){Version=4; Profile=_T("LC");   SBR=1; PS=1;}
    else if (Codec==_T("A_AAC/MPEG4/SSR"))      {Version=4; Profile=_T("SSR");}
    else if (Codec==_T("A_AAC/MPEG4/LTP"))      {Version=4; Profile=_T("LTP");}
    else if (Codec==_T("raac"))                 {           Profile=_T("LC");}
    else if (Codec==_T("racp"))                 {           Profile=_T("LC");   SBR=1; PS=0;}

    if (Version>0)
        Fill(Stream_Audio, 0, Audio_Format_Version, Version==2?"Version 2":"Version 4");
    Fill(Stream_Audio, 0, Audio_Format_Profile, Profile);
    if (SBR!=2)
    {
        if (SBR)
            Fill(Stream_Audio, 0, Audio_Format_Settings, "SBR");
        Fill(Stream_Audio, 0, Audio_Format_Settings_SBR, SBR?"Yes":"No");
    }
    if (PS!=2)
    {
        if (PS)
            Fill(Stream_Audio, 0, Audio_Format_Settings, "PS");
        Fill(Stream_Audio, 0, Audio_Format_Settings_PS, PS?"Yes":"No");
    }

    Accept("AAC");
    Finish("AAC");
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_AAC_YES
