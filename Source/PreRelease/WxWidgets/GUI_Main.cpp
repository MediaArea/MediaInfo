// MediaInfo
// Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//----------------------------------------------------------------------------
#include "PreRelease/WxWidgets/GUI_Main.h"

#include <wx/html/htmlwin.h>
#include <wx/file.h>
#include <wx/textctrl.h>
#include "PreRelease/Language_Others.h"
#include "PreRelease/Language_All.h"
//----------------------------------------------------------------------------
//Menu
BEGIN_EVENT_TABLE(GUI_Main, wxFrame)
    EVT_MENU(1,  GUI_Main::Language_All)
    EVT_MENU(2,  GUI_Main::Language_Others)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
GUI_Main::GUI_Main(const wxPoint& pos, const wxSize& size, long style)
:wxFrame(NULL, -1, _T("MediaInfo PreRelease"), pos, size, style)
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

    //Menu
    wxMenu *Menu_File = new wxMenu;
    Menu_File->Append(1, _T("Create other language files from All.csv"));
    Menu_File->Append(2, _T("Update All.csv from other language files"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(Menu_File, _T("&File"));
    SetMenuBar(menuBar);

    Text=new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxTE_MULTILINE|wxTE_RICH2);
}

GUI_Main::~GUI_Main()
{
}


void GUI_Main::Language_All(wxCommandEvent& WXUNUSED(event))
{
    Text->SetValue(Language_All_Run().c_str());
}

void GUI_Main::Language_Others(wxCommandEvent& WXUNUSED(event))
{
    Text->SetValue(Language_Others_Run().c_str());
}
