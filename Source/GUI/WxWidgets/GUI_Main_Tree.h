// GUI_Main_Tree - WxWidgets GUI for MediaInfo
// Copyright (C) 2007-2008 Jerome Martinez, Zen@MediaArea.net
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
// WxWidgets GUI for MediaInfo, Tree Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_TreeH
#define GUI_Main_TreeH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <wx/treectrl.h>
#include <map>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Tree
//***************************************************************************

class GUI_Main_Tree : public wxTreeCtrl, public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Tree(Core* _C, wxWindow *parent);
    ~GUI_Main_Tree();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();
    void Item_Show(const wxTreeItemId &Item);

    //Events
    void OnItemExpanding(wxTreeEvent& event);
    void OnItemCollapsing(wxTreeEvent& event);

    //Temp
    std::map<wxTreeItemIdValue, size_t> TextPos;
    MediaInfoLib::String Text;

    //wxWidgets
    DECLARE_EVENT_TABLE() //Any class wishing to process wxWindows events must use this macro
};

#endif
