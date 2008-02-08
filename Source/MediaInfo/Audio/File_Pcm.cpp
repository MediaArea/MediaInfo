// File_Pcm - Info for PCM files
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
#if defined(MEDIAINFO_PCM_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Pcm.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Pcm::Read_Buffer_Finalize()
{
    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "PCM");
    Stream_Prepare(Stream_Audio);

    Ztring Firm, Endianness, Sign, Law, ITU, Resolution, Codec_New;
    if (0)
        ;
    else if (Codec==_T("VOB"))              {Firm=_T("");       Endianness=_T("Big");    Sign=_T("Signed");   Resolution=_T("24"); Codec_New=_T("PCM");} //PCM Signed 16 bits Big Endian, Interleavement is for 2 samples*2 channels L0-1/L0-0/R0-1/R0-0/L1-1/L1-0/R1-1/R1-0/L0-2/R0-2/L1-2/R1-2, http://wiki.multimedia.cx/index.php?title=PCM
    else if (Codec==_T("A_PCM/INT/BIG"))    {Firm=_T("");       Endianness=_T("Big");}
    else if (Codec==_T("A_PCM/INT/LITTLE")) {Firm=_T("");       Endianness=_T("Little");}
    else if (Codec==_T("A_PCM/INT/FLOAT"))  {Firm=_T("");       Endianness=_T("Float");}
    else if (Codec==_T("ima4"))             {Firm=_T("IMA");}
    else if (Codec==_T("raw "))             {                   Endianness=_T("Little"); Sign=_T("Unsigned"); Codec_New=_T("PCM");}
    else if (Codec==_T("twos"))             {                   Endianness=_T("Big");    Sign=_T("Signed");}
    else if (Codec==_T("sowt"))             {                   Endianness=_T("Little"); Sign=_T("Signed");}
    else if (Codec==_T("in24"))             {                   Endianness=_T("Big");    Sign=_T("Unsigned"); Resolution=_T("24");}
    else if (Codec==_T("in32"))             {                   Endianness=_T("Big");    Sign=_T("Unsigned"); Resolution=_T("32");}
    else if (Codec==_T("fl32"))             {                   Endianness=_T("Float");                       Resolution=_T("32");}
    else if (Codec==_T("fl64"))             {                   Endianness=_T("Float");                       Resolution=_T("64");}
    else if (Codec==_T("alaw"))             {                   Law=_T("A-Law");}
    else if (Codec==_T("alaw"))             {                   Law=_T("U-Law");}
    else if (Codec==_T("SWF ADPCM"))        {Firm=_T("SWF");}
    else if (Codec==_T("1"))                {                   Endianness=_T("Little"); Sign=_T("Unsigned");}
    else if (Codec==_T("2"))                {Firm=_T("Microsoft");}
    else if (Codec==_T("3"))                {                   Endianness=_T("Float");}
    else if (Codec==_T("6"))                {                   Law=_T("A-Law");}
    else if (Codec==_T("7"))                {                   Law=_T("U-Law");}
    else if (Codec==_T("10"))               {Firm=_T("OKI");}
    else if (Codec==_T("11"))               {Firm=_T("Intel");}
    else if (Codec==_T("12"))               {Firm=_T("Mediaspace");}
    else if (Codec==_T("13"))               {Firm=_T("Sierra");}
    else if (Codec==_T("14"))               {Firm=_T("Antex");}
    else if (Codec==_T("17"))               {Firm=_T("Dialogic");}
    else if (Codec==_T("18"))               {Firm=_T("Mediavision");}
    else if (Codec==_T("20"))               {Firm=_T("Yamaha");}
    else if (Codec==_T("33"))               {Firm=_T("Antex");}
    else if (Codec==_T("36"))               {Firm=_T("DSP Solution");}
    else if (Codec==_T("38"))               {Firm=_T("Natural MicroSystems");}
    else if (Codec==_T("39"))               {Firm=_T("Crystal Semiconductor");}
    else if (Codec==_T("3B"))               {Firm=_T("Rockwell");}
    else if (Codec==_T("40"))               {Firm=_T("Antex Electronics");}
    else if (Codec==_T("42"))               {Firm=_T("IBM");}
    else if (Codec==_T("45"))               {Firm=_T("Microsoft"); ITU=_T("G.726");}
    else if (Codec==_T("64"))               {Firm=_T("Apicom"); ITU=_T("G.726");}
    else if (Codec==_T("65"))               {Firm=_T("Apicom"); ITU=_T("G.722");}
    else if (Codec==_T("85"))               {Firm=_T("DataFusion Systems"); ITU=_T("G.726");}
    else if (Codec==_T("8B"))               {Firm=_T("Infocom"); ITU=_T("G.721");}
    else if (Codec==_T("97"))               {Firm=_T("ZyXEL");}
    else if (Codec==_T("100"))              {Firm=_T("Rhetorex");}
    else if (Codec==_T("125"))              {Firm=_T("Sanyo");}
    else if (Codec==_T("140"))              {Firm=_T("Dictaphone"); ITU=_T("G.726");}
    else if (Codec==_T("170"))              {Firm=_T("Unisys");}
    else if (Codec==_T("171"))              {Firm=_T("Unisys"); Law=_T("A-Law");}
    else if (Codec==_T("172"))              {Firm=_T("Unisys"); Law=_T("U-Law");}
    else if (Codec==_T("175"))              {Firm=_T("SyCom"); ITU=_T("G.726");}
    else if (Codec==_T("178"))              {Firm=_T("Knownledge");}
    else if (Codec==_T("200"))              {Firm=_T("Creative");}
    else if (Codec==_T("210"))              {Firm=_T("Uher");}
    else if (Codec==_T("285"))              {Firm=_T("Norcom Voice Systems");}
    else if (Codec==_T("1001"))             {Firm=_T("Olivetti");}
    else if (Codec==_T("1C03"))             {Firm=_T("Lucent"); ITU=_T("G.723");}
    else if (Codec==_T("1C0C"))             {Firm=_T("Lucent"); ITU=_T("G.723");}
    else if (Codec==_T("4243"))             {ITU=_T("G.726");}
    else if (Codec==_T("A105"))             {ITU=_T("G.726");}
    else if (Codec==_T("A107"))             {ITU=_T("G.726");}

    if (!Codec_New.empty())
    {
        Fill("Codec", Codec_New);
        Fill("Codec/String", Codec_New);
        Fill("Codec/Family", Codec_New);
    }
    if (!Firm.empty())
    {
        Fill("Codec_Settings", Firm);
        Fill("Codec_Settings_Firm", Firm);
    }
    if (!Endianness.empty())
    {
        Fill("Codec_Settings", Endianness);
        Fill("Codec_Settings_Endianness", Endianness);
    }
    if (!Sign.empty())
    {
        Fill("Codec_Settings", Sign);
        Fill("Codec_Settings_Sign", Sign);
    }
    if (!Law.empty())
    {
        Fill("Codec_Settings", Law);
        Fill("Codec_Settings_Law", Law);
    }
    if (!ITU.empty())
    {
        Fill("Codec_Settings", ITU);
        Fill("Codec_Settings_ITU", ITU);
    }
    Fill("Resolution", Resolution);
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Pcm::HowTo(stream_t StreamKind)
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

#endif //MEDIAINFO_PCM_YES
