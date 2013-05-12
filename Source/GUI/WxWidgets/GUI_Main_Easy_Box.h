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
#ifndef GUI_Main_Easy_BoxH
#define GUI_Main_Easy_BoxH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Easy_Box_Core.h"
#include <wx/panel.h>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class GUI_Main_Easy;
class wxWindow;
class wxChoice;
class wxStaticBox;
class wxStaticText;
class wxButton;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Easy_Box
//***************************************************************************

class GUI_Main_Easy_Box : public wxPanel, public GUI_Main_Easy_Box_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Easy_Box(Core* C, GUI_Main_Easy* Parent, wxWindow* Left, wxWindow* Top, stream_t StreamKind, size_t StreamPos);

    //Actions
    void GUI_Refresh();
    void GUI_Resize();

    //Events
    void OnClick(wxCommandEvent &event);

private:
    //GUI
    wxStaticBox*  Box;
    wxStaticText* Text;
    wxButton*     Button;
    wxStaticText* Tags;

    //Internal
    GUI_Main_Easy*  Parent;
    wxWindow*       Left;
    wxWindow*       Top;

    //Helpers
    void Box_Refresh(wxStaticText *Text, wxButton *Button, stream_t StreamKind, size_t StreamPos);

    //wxWidgets
    DECLARE_EVENT_TABLE() //Any class wishing to process wxWindows events must use this macro
};

#endif
