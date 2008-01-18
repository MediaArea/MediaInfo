// ----------------------------------------------------------------------------
#include "PreRelease/WxWidgets/App.h"
#include "PreRelease/WxWidgets/GUI_Main.h"

// ----------------------------------------------------------------------------
bool App::OnInit()
{
    GUI_Main *frame = new GUI_Main(wxPoint(50, 50), wxSize(450, 340));
    frame->Show(TRUE);
    return TRUE;
}
