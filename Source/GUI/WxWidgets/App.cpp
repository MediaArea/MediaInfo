// GUI for MediaInfo
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

#ifdef __WXMAC__
    #include <ApplicationServices/ApplicationServices.h>
#endif //__WXMAC__

// ----------------------------------------------------------------------------
IMPLEMENT_APP(App)
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
bool App::OnInit()
{
    #ifdef __WXMAC__
        ProcessSerialNumber PSN;
        GetCurrentProcess(&PSN);
        TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
        setenv("LC_CTYPE", "UTF-8", 0);
    #endif //__WXMAC__

    setlocale(LC_ALL, "");

    GUI_Main *frame = new GUI_Main(argc, argv, wxPoint(50, 50), wxSize(800-100, 600-100));
    frame->Show(TRUE);
    return TRUE;
}


