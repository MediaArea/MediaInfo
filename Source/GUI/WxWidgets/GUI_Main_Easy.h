/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// WxWidgets GUI for MediaInfo, Easy Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_EasyH
#define GUI_Main_EasyH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Easy_Core.h"
#include <wx/panel.h>
#include <vector>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class wxChoice;
class wxStaticBox;
class wxStaticText;
class wxButton;
class GUI_Main_Easy_Box;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Easy
//***************************************************************************

class GUI_Main_Easy : public wxPanel, public GUI_Main_Easy_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Easy(Core* C, wxWindow *parent);
    ~GUI_Main_Easy();

    //Actions
    void GUI_Refresh();
    void GUI_Refresh_Partial();
    void GUI_Resize();
    void GUI_Resize_Partial();

private:
    //GUI
    wxChoice* Select;
    std::vector<std::vector<GUI_Main_Easy_Box*> > Boxes;

    //Events
    void OnChoice(wxCommandEvent& event);

    //wxWidgets
    DECLARE_EVENT_TABLE() //Any class wishing to process wxWindows events must use this macro
};

#endif
