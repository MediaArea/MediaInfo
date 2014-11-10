/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#include "GUI/WxWidgets/GUI_Main_Tree.h"
#include "Common/Core.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(GUI_Main_Tree, wxTreeCtrl)
    EVT_TREE_ITEM_EXPANDING (26983, GUI_Main_Tree::OnItemExpanding)
    EVT_TREE_ITEM_COLLAPSING(26983, GUI_Main_Tree::OnItemCollapsing)
END_EVENT_TABLE()
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Tree::GUI_Main_Tree(Core* Core_, wxWindow* parent)
        : wxTreeCtrl(parent, 26983, wxPoint(0, 0), wxSize(parent->GetClientSize().GetWidth()-0, parent->GetClientSize().GetHeight()-0)),
        GUI_Main_Common_Core(Core_)
{
    //Update
    GUI_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main_Tree::~GUI_Main_Tree()
{
    Hide();

    DeleteAllItems();
    TextPos.clear();
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Tree::GUI_Refresh()
{
    Hide();

    //Retrieving info
    Text=C->Inform_Get();

    //Clear
    DeleteAllItems();
    TextPos.clear();

    //Showing
    if (!Text.empty())
    {
        SetWindowStyle(wxTR_HIDE_ROOT);
        wxTreeItemId RootID=AddRoot(wxT("Root"));
        TextPos[RootID.m_pItem]=(size_t)-1;
        Item_Show(RootID);
    }

    Show();
}

void GUI_Main_Tree::GUI_Resize()
{
    SetSize(0, 0, GetParent()->GetClientSize().GetWidth()-0, GetParent()->GetClientSize().GetHeight()-0);
}

//---------------------------------------------------------------------------
void GUI_Main_Tree::Item_Show(const wxTreeItemId &Item)
{
    DeleteChildren(Item);

    wxTreeItemId ItemID;
    //wxFont Font(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    //wxFont Font(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    size_t Level=0;
    size_t Pos1=TextPos[Item.m_pItem];
    size_t Pos2=0;
    size_t LevelBase;
    if (Pos1!=(size_t)-1)
    {
        //Not the root level
        LevelBase=Text.find_first_not_of(__T(' '), Pos1+9)-(Pos1+9);
        Pos1=Text.find(__T('\n'), Pos1)+1;
    }
    else
    {
        //Root level
        LevelBase=(size_t)-1;
        Pos1=0;
    }
    while (Pos2!=(size_t)-1)
    {
        Pos2=Text.find(__T('\n'), Pos1); //Pos to the next line
        #ifdef WIN32
            #define SIZE 1
        #else
            #define SIZE 0
        #endif
        Level=Text.find_first_not_of(__T(' '), Pos1+9)-(Pos1+9);
        if (Level==LevelBase+1)
        {
            //Showing line
            String Line=Text.substr(Pos1, Pos2-Pos1-SIZE);
            if (Line.size()>10 && Line[9]==__T(' '))
            {
                Line.erase(9, Level);
            }
            if (!Line.empty())
            {
                ItemID=AppendItem(Item, Line.c_str());
                TextPos[ItemID.m_pItem]=Pos1;
                //SetItemFont(ItemID, Font);
            }
        }
        else if (Level==LevelBase+2)
            SetItemHasChildren(ItemID); //SubLevel elements detected, we are showing the availability of childrens
        else if (Level<=LevelBase && LevelBase!=(size_t)-1) //This is an upper level, finnished (except for root level, there is no upper level)
            Pos2=(size_t)-1;
        Pos1=Pos2+1;
    }
}

//***************************************************************************
// Events
//***************************************************************************

void GUI_Main_Tree::OnItemExpanding(wxTreeEvent& event)
{
    Item_Show(event.GetItem());
}

void GUI_Main_Tree::OnItemCollapsing(wxTreeEvent& event)
{
    //CollapseAndReset(event.GetItem());
    //SetItemHasChildren(event.GetItem());
}
