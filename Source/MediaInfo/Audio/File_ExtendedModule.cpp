// File_ExtendedModule - Info for Extended Module files
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
#if defined(MEDIAINFO_XM_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_ExtendedModule.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_ExtendedModule::FileHeader_Parse()
{
    //Parsing
    Ztring ModuleName, TrackerName;
    std::string Signature;
    int32u HeaderSize;
    int16u Length, Channels, Patterns, Instruments, Flags, Tempo, BPM;
    int8u  Mark, VersionMinor, VersionMajor;
    Get_String(17, Signature,                                   "Signature");

    FILLING_BEGIN();
        if (Signature!="Extended Module: ")
        {
            Finnished();
            return;
        }
    FILLING_END()

    Get_Local(20, ModuleName,                                   "Module name");
    Get_L1 (Mark,                                               "0x1A");

    FILLING_BEGIN();
        if (Mark!=0x1A)
        {
            Finnished();
            return;
        }
    FILLING_END()

    Get_Local(20, TrackerName,                                  "Tracker name");
    Get_L1 (VersionMinor,                                       "Version (minor)");
    Get_L1 (VersionMajor,                                       "Version (major)");
    Get_L4 (HeaderSize,                                         "Header size");
    Get_L2 (Length,                                             "Song Length");
    Skip_L2(                                                    "Restart position");
    Get_L2 (Channels,                                           "Number of channels");
    Get_L2 (Patterns,                                           "Number of patterns");
    Get_L2 (Instruments,                                        "Number of instruments");
    Get_L2 (Flags,                                              "Flags");
    Get_L2 (Tempo,                                              "Tempo");
    Get_L2 (BPM,                                                "BPM");
    Skip_XX(256,                                                "Pattern order table");

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "Extended Module");
        Fill(Stream_General, 0, General_Format_Version, Ztring(_T("Version"))+Ztring::ToZtring(VersionMajor)+_T(".")+Ztring::ToZtring(VersionMinor/10)+Ztring::ToZtring(VersionMinor%10));
        Fill(Stream_General, 0, General_Track, ModuleName.Trim(_T(' ')));
        Fill(Stream_General, 0, General_Encoded_Application, TrackerName.Trim(_T(' ')));
        Fill(Stream_General, 0, "Tempo", Tempo);
        Fill(Stream_General, 0, "BPM", BPM);

        for (int16u Pos=0; Pos<Channels; Pos++)
        {
            Stream_Prepare(Stream_Audio);
        }

        Finnished();
    FILLING_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_XM_YES
