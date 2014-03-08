/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Common/Core.h"
#include "GUI/VCL_New/GUI_Main_Tree.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Tree::GUI_Main_Tree(Core* Core_, TWinControl* Owner)
    : GUI_Main_Common_Core(Core_)
{
    //VCL specific
    VCL=new TTreeView(Owner);
    VCL->Parent=Owner;

    //Update
    GUI_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main_Tree::~GUI_Main_Tree()
{
    //VCL specific
    delete VCL; //VCL=NULL

    //Hide();

    //DeleteAllItems();
    TextPos.clear();
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Tree::GUI_Refresh()
{
    //Hide();

    //Retrieving info
    Text=C->Inform_Get();

    //Clear
    VCL->Items->Clear();
    TextPos.clear();

    //Showing
    if (!Text.empty())
    {
        TTreeNode* Root=VCL->Items->Add(NULL, "XXX");
        TextPos[Root]=(size_t)-1;
        Item_Show(Root);
    }

    //Show();
}

//---------------------------------------------------------------------------
void GUI_Main_Tree::GUI_Resize()
{
    VCL->Left=0;
    VCL->Top=0;
    VCL->Width=VCL->Parent->ClientWidth;
    VCL->Height=VCL->Parent->ClientHeight;
}

//---------------------------------------------------------------------------
void GUI_Main_Tree::Item_Show(TTreeNode *Item)
{
    size_t Pos1=TextPos[Item];
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
        size_t Level=Text.find_first_not_of(__T(' '), Pos1+9)-(Pos1+9);
        if (Level==LevelBase+1)
        {
            //Showing line
            MediaInfoLib::String Line=Text.substr(Pos1, Pos2-Pos1-SIZE);
            if (Line.size()>10 && Line[9]==__T(' '))
            {
                Line.erase(9, Level);
            }
            if (!Line.empty())
            {
                TTreeNode* ItemID==VCL->Items->Add(Item, Line.c_str());
                TextPos[ItemID]=Pos1;
            }
        }
        /*
        else if (Level==LevelBase+2)
            SetItemHasChildren(ItemID); //SubLevel elements detected, we are showing the availability of childrens
        else if (Level<=LevelBase && LevelBase!=(size_t)-1) //This is an upper level, finnished (except for root level, there is no upper level)
            Pos2=(size_t)-1;
        */
        Pos1=Pos2+1;
    }
}

