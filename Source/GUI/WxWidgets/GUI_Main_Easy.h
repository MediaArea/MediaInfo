// GUI_Main_Easy - WxWidgets GUI for MediaInfo
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
