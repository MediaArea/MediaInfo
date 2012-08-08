// GUI_Main_HTML - WxWidgets GUI for MediaInfo
// Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
GUI_Main_HTML::GUI_Main_HTML(Core* _C, wxWindow* parent)
    : wxHtmlWindow(parent, -1, wxPoint(1, 1), wxSize(parent->GetClientSize().GetWidth()-1, parent->GetClientSize().GetHeight()-1)),
    GUI_Main_Common_Core(_C)
{
    int Size[7]={ 7, 8, 10, 12, 16, 22, 30};
    SetFonts(wxEmptyString, wxEmptyString, Size);

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
