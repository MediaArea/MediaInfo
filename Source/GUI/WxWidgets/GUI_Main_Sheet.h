/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// WxWidgets GUI for MediaInfo, Sheet Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_SheetH
#define GUI_Main_SheetH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <wx/panel.h>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Sheet
//***************************************************************************

class GUI_Main_Sheet : public wxPanel, public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Sheet(Core* C, wxWindow *parent);

    //Actions
    void GUI_Refresh();
    void GUI_Resize();
};

#endif
