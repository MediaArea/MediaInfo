/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#include "GUI/WxWidgets/GUI_Main_HTML.h"
#include "GUI/WxWidgets/GUI_Main_FileDrop.h"
#include "Common/Core.h"
#include <wx/fs_mem.h>
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_HTML::GUI_Main_HTML(Core* Core_, wxWindow* parent)
    : wxHtmlWindow(parent, -1, wxPoint(1, 1), wxSize(parent->GetClientSize().GetWidth()-1, parent->GetClientSize().GetHeight()-1)),
    GUI_Main_Common_Core(Core_)
{
    //Drag and Drop
    #if wxUSE_DRAG_AND_DROP && defined(__WXMAC__)
        SetDropTarget(new FileDrop(C));
    #endif //wxUSE_DRAG_AND_DROP

    //Update
    GUI_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main_HTML::~GUI_Main_HTML()
{
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_HTML::GUI_Refresh()
{
    std::string Temp(wxConvUTF8.cWX2MB(C->Inform_Get().c_str()));
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxMemoryFSHandler::AddFile(__T("index.html"), Temp.c_str(), Temp.size());
    LoadPage(__T("memory:index.html"));
    wxMemoryFSHandler::RemoveFile(__T("index.html"));
}

void GUI_Main_HTML::GUI_Resize()
{
    SetSize(1, 1, GetParent()->GetClientSize().GetWidth()-1, GetParent()->GetClientSize().GetHeight()-1);
}
