/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// WxWidgets GUI for MediaInfo, HTML Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_HTMLH
#define GUI_Main_HTMLH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <wx/html/htmlwin.h>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_HTML
//***************************************************************************

class GUI_Main_HTML : public wxHtmlWindow, public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_HTML(Core* C, wxWindow *parent);
    ~GUI_Main_HTML();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();
};

#endif
