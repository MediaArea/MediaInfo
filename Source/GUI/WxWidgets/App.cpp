/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wdeprecated-declarations" // GetCurrentProcess is deprecated
        ProcessSerialNumber PSN;
        GetCurrentProcess(&PSN);
        TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
        #pragma GCC diagnostic pop
        setenv("LC_CTYPE", "UTF-8", 0);
    #endif //__WXMAC__

    setlocale(LC_ALL, "");

    GUI_Main *frame = new GUI_Main(argc, argv, wxPoint(50, 50), wxSize(800-100, 600-100));
    frame->Show(TRUE);
    return TRUE;
}


