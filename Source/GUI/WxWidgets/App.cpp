// ----------------------------------------------------------------------------
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#include "GUI/WxWidgets/App.h"
#include "GUI/WxWidgets/GUI_Main.h"

// ----------------------------------------------------------------------------
IMPLEMENT_APP(App)
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
bool App::OnInit()
{
    GUI_Main *frame = new GUI_Main(wxPoint(50, 50), wxSize(800-100, 600-100));
    frame->Show(TRUE);
    return TRUE;
}


