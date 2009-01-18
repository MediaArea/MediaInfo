// GUI_Main_Easy_Box - WxWidgets GUI for MediaInfo
// Copyright (C) 2007-2009 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
