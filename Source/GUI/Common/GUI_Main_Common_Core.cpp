/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include "Common/Core.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Common_Core::GUI_Main_Common_Core(Core* Core_)
{
    //Internal
    C=Core_;
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


