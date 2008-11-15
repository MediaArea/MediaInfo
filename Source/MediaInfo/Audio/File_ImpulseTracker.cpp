// File_ImpulseTracker - Info for Impulse Tracker files
// Copyright (C) 2008-2008 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_IT_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_ImpulseTracker.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_ImpulseTracker::FileHeader_Parse()
{
    //Parsing
    Ztring SongName;
    int32u Signature;
    int16u OrdNum, InsNum, SmpNum, PatNum, Flags, Special;
    int8u  VersionMajor, VersionMinor, SoftwareVersionMajor, SoftwareVersionMinor, IS, TS;
    bool Stereo;
    Get_B4(Signature,                                           "Signature");

    FILLING_BEGIN();
        if (Signature!=0x494D504D)
        {
            Finnished();
            return;
        }
    FILLING_END()

    Get_Local(26, SongName,                                     "Song name");
    Skip_L1(                                                    "Unknown");
    Skip_L1(                                                    "Unknown");
    Get_L2 (OrdNum,                                             "Orders count");
    Get_L2 (InsNum,                                             "Instruments count");
    Get_L2 (SmpNum,                                             "Samples count");
    Get_L2 (PatNum,                                             "Paterns count");
    Get_L1 (SoftwareVersionMinor,                               "Cwt/v (Minor)");
    Get_L1 (SoftwareVersionMajor,                               "Cwt/v (Major)");
    Get_L1 (VersionMinor,                                       "Cwt (Minor)");
    Get_L1 (VersionMajor,                                       "Cwt (Major)");
    Get_L2 (Flags,                                              "Flags");
        Get_Flags (Flags, 0, Stereo,                            "Stereo");
        Skip_Flags(Flags, 1,                                    "Vol0MixOptimizations");
        Skip_Flags(Flags, 2,                                    "Use instruments/Samples");
        Skip_Flags(Flags, 3,                                    "Linear/Amiga slides");
        Skip_Flags(Flags, 4,                                    "Old/IT Effects");
    Get_L2 (Special,                                            "Special");
        Skip_Flags(Special, 0,                                  "Song Message attached");
    Skip_L1(                                                    "Global volume");
    Skip_L1(                                                    "Mix volume");
    Get_L1 (IS,                                                 "Initial Speed");
    Get_L1 (TS,                                                 "Initial Temp");
    Skip_L1(                                                    "Panning separation between channels");
    Skip_L1(                                                    "0");
    Skip_L2(                                                    "Message Length");
    Skip_L4(                                                    "Message Offset");
    Skip_L1(                                                    "Unknown");
    Skip_L1(                                                    "Unknown");
    Skip_L1(                                                    "Unknown");
    Skip_L1(                                                    "Unknown");
    Skip_L1(                                                    "Unknown");
    Skip_XX(64,                                                 "Chnl Pan");
    Skip_XX(64,                                                 "Chnl Vol");
    Skip_XX(OrdNum,                                             "Orders");
    Skip_XX(InsNum*4,                                           "Instruments");
    Skip_XX(SmpNum*4,                                           "Samples");
    Skip_XX(PatNum*4,                                           "Patterns");

    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        Fill(Stream_General, 0, General_Format, "Impulse Tracker");
        Fill(Stream_General, 0, General_Format_Version, Ztring(_T("Version "))+Ztring::ToZtring(VersionMajor)+_T(".")+Ztring::ToZtring(VersionMinor/16)+Ztring::ToZtring(VersionMinor%16));
        Fill(Stream_General, 0, General_Track, SongName);
        Fill(Stream_General, 0, General_Encoded_Application, Ztring(_T("Impulse Tracker ")+Ztring::ToZtring(SoftwareVersionMajor)+_T(".")+Ztring::ToZtring(SoftwareVersionMinor/16)+Ztring::ToZtring(SoftwareVersionMinor%16)));
        Fill(Stream_General, 0, "BPM", TS);

        Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, StreamPos_Last, Audio_Channel_s_, Stereo?2:1);
        
        Finnished();
    FILLING_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_IT_YES
