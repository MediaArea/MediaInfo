/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
:wxFrame(NULL, -1, __T("MediaInfo PreRelease"), pos, size, style)
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

    //Menu
    wxMenu *Menu_File = new wxMenu;
    Menu_File->Append(1, __T("Create other language files from All.csv"));
    Menu_File->Append(2, __T("Update All.csv from other language files"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(Menu_File, __T("&File"));
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
