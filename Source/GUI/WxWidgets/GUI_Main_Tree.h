/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
    GUI_Main_Tree(Core* Core_, wxWindow *parent);
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
    MediaInfoNameSpace::String Text;

    //wxWidgets
    DECLARE_EVENT_TABLE() //Any class wishing to process wxWindows events must use this macro
};

#endif
