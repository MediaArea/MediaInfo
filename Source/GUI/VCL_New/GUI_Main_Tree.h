/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Borland VCL GUI for MediaInfo, Tree Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Tree_H
#define GUI_Main_Tree_H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <Grids.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include <map>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Tree_
//***************************************************************************

class GUI_Main_Tree_ : public TTreeView
{
public:
    //Constructor/Destructor
    GUI_Main_Tree_(Core* C, TComponent* Owner);
    __fastcall ~GUI_Main_Tree_();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();

private:
    GUI_Main_Common_Core* T;
};

//***************************************************************************
// GUI_Main_Tree
//***************************************************************************

class GUI_Main_Tree : public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Tree(Core* C, TWinControl* Owner);
    ~GUI_Main_Tree();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();
    void Item_Show(TTreeNode* Item);

    //Events
    //void OnItemExpanding(wxTreeEvent& event);
    //void OnItemCollapsing(wxTreeEvent& event);

    //Temp
    std::map<TTreeNode*, size_t> TextPos;
    MediaInfoLib::String Text;

private:
    TTreeView* VCL;
};

#endif
