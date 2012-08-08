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

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include "Common/Core.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Common_Core::GUI_Main_Common_Core(Core* _C)
{
    //Internal
    C=_C;
    File_Pos=(size_t)-1;
}

//***************************************************************************
// Actions - Global
//***************************************************************************

//---------------------------------------------------------------------------
size_t GUI_Main_Common_Core::FilesCount_Get()
{
    return C->MI->Count_Get();
}

//---------------------------------------------------------------------------
size_t GUI_Main_Common_Core::FilesPos_Get()
{
    return File_Pos;
}

//***************************************************************************
// Actions - Per file
//***************************************************************************

//---------------------------------------------------------------------------
String GUI_Main_Common_Core::FileName_Get()
{
    return C->MI->Get(File_Pos, Stream_General, 0, __T("CompleteName")).c_str();
}

//***************************************************************************
// Actions - Per StreamKind
//***************************************************************************

//---------------------------------------------------------------------------
size_t GUI_Main_Common_Core::StreamsCount_Get(stream_t StreamKind)
{
    return C->MI->Count_Get(File_Pos, StreamKind);
}

//***************************************************************************
// Actions - Per Stream
//***************************************************************************

//---------------------------------------------------------------------------
String GUI_Main_Common_Core::Summary_Get(stream_t StreamKind, size_t StreamPos)
{
    C->MI->Option(__T("Inform"), __T("Summary"));
    return C->MI->Get(File_Pos, StreamKind, StreamPos, __T("Inform")).c_str();
}

//---------------------------------------------------------------------------
String GUI_Main_Common_Core::Inform_Get(stream_t StreamKind, size_t StreamPos)
{
    C->MI->Option(__T("Inform"), __T(""));
    return C->MI->Get(File_Pos, StreamKind, StreamPos, __T("Inform")).c_str();
}

//---------------------------------------------------------------------------
String GUI_Main_Common_Core::CodecID_Url_Get(stream_t StreamKind, size_t StreamPos)
{
    if (!C->MI->Get(File_Pos, StreamKind, StreamPos, __T("CodecID/Url")).empty())
        return C->MI->Get(File_Pos, StreamKind, StreamPos, __T("CodecID/Url")).c_str();
    else
        return C->MI->Get(File_Pos, StreamKind, StreamPos, __T("Format/Url")).c_str();
}


