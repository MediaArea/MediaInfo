// File_Module - Info for Module files
// Copyright (C) 2008-2008 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WXMHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILXMY or FXMNESS FOR A PARTICULAR PURPOSE.  See the
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
#if defined(MEDIAINFO_MOD_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Module.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Module::FileHeader_Parse()
{
    //Parsing
    Ztring ModuleName, SamplesName;
    int32u Signature;
    Get_Local(20, ModuleName,                                   "Module name");
    for (int8u Pos=0; Pos<31; Pos++)
    {
        Element_Begin();
        Get_Local(22, SamplesName,                              "Sample's name"); Element_Name(SamplesName);
        Skip_B2(                                                "Sample length");
        Skip_B1(                                                "Finetune value for the sample");
        Skip_B1(                                                "Volume of the sample");
        Skip_B2(                                                "Start of sample repeat offset");
        Skip_B2(                                                "Length of sample repeat");
        Element_End();
    }
    Skip_B1(                                                    "Number of song positions");
    Skip_B1(                                                    "0x8F");
    Skip_XX(128,                                                "Pattern table");
    Get_C4 (Signature,                                          "Signature");

    FILLING_BEGIN();
        switch (Signature)
        {
            case 0x4D2E4B2E : //M.K.
            case 0x4D214B21 : //M!K!
            case 0x664C5434 : //FLT4
            case 0x664C5438 : //FLT8
            case 0x3663684E : //6CHN
            case 0x3863684E : //8CHN
                                break;
            default : Finnished();
                      return;
        }
    FILLING_END()

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "Module");

        Stream_Prepare(Stream_Audio);

        Finnished();
    FILLING_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MOD_YES
