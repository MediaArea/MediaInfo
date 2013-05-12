/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// WxWidgets GUI for MediaInfo, Text Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_TextH
#define GUI_Main_TextH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <wx/textctrl.h>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Text
//***************************************************************************

class GUI_Main_Text : public wxTextCtrl, public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Text(Core* C, wxWindow *parent);
    ~GUI_Main_Text();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();
};

#endif
