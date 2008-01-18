//----------------------------------------------------------------------------
#include "PreRelease/WxWidgets/GUI_Main.h"

#include <wx/html/htmlwin.h>
#include <wx/file.h>
#include <wx/textctrl.h>
#include "PreRelease/OldFiles.h"
#include "PreRelease/Ressources.h"
//----------------------------------------------------------------------------
//Menu
BEGIN_EVENT_TABLE(GUI_Main, wxFrame)
    EVT_MENU(1,  GUI_Main::OldFiles)
    EVT_MENU(2, GUI_Main::Ressources)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
GUI_Main::GUI_Main(const wxPoint& pos, const wxSize& size, long style)
:wxFrame(NULL, -1, _T("MediaInfoLib - Too old files test"), pos, size, style)
{
    // set the frame icon
    SetIcon(wxICON(mondrian));

    //Menu
    wxMenu *Menu_File = new wxMenu;
    Menu_File->Append(1, _T("Test if there are too old files"));
    Menu_File->Append(2, _T("Update ressource files"));

    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(Menu_File, _T("&File"));
    SetMenuBar(menuBar);

    Text=new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxTE_MULTILINE|wxTE_RICH2);
}

GUI_Main::~GUI_Main()
{
}


void GUI_Main::OldFiles(wxCommandEvent& WXUNUSED(event))
{
    Text->SetValue(OldFiles_Test().c_str());
}

void GUI_Main::Ressources(wxCommandEvent& WXUNUSED(event))
{
    Text->SetValue(Ressources_Create().c_str());
}
