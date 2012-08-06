// GUI_Main_Common_Core - Core GUI for MediaInfo
// Copyright (C) 2007-2012 MediaArea.net SARL, Info@MediaArea.net
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
// GUI for MediaInfo, Common Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Common_CoreH
#define GUI_Main_Common_CoreH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_DLL_RUNTIME
    #include "MediaInfoDLL/MediaInfoDLL.h"
    #define MediaInfoNameSpace MediaInfoDLL
#elif defined MEDIAINFO_DLL_STATIC
    #include "MediaInfoDLL/MediaInfoDLL_Static.h"
    #define MediaInfoNameSpace MediaInfoDLL
#else
    #include "MediaInfo/MediaInfoList.h"
    #define MediaInfoNameSpace MediaInfoLib
#endif
using namespace MediaInfoNameSpace;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Common_Core
//***************************************************************************

class GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Common_Core(Core* _C);
    virtual ~GUI_Main_Common_Core() {};

    //Actions to be overloded
            void GUI_UpdateAll() {GUI_Resize(); GUI_Refresh();}
    virtual void GUI_Refresh()   {};
    virtual void GUI_Resize()    {};

    //Actions - Global
    size_t  FilesCount_Get      ();
    size_t  FilesPos_Get        ();

    //Actions - Per File
    MediaInfoNameSpace::String  FileName_Get        ();

    //Actions - Per StreamKind
    size_t  StreamsCount_Get    (stream_t StreamKind);

    //Actions - Per Stream
    MediaInfoNameSpace::String  Summary_Get         (stream_t StreamKind, size_t StreamPos);
    MediaInfoNameSpace::String  Inform_Get          (stream_t StreamKind, size_t StreamPos);
    MediaInfoNameSpace::String  CodecID_Url_Get     (stream_t StreamKind, size_t StreamPos);

//protected: //Should be protected, but Borland VCL does NOT accept multiple classes, so we need of data
    //Internal
    Core* C;
    size_t File_Pos;
};

#endif
