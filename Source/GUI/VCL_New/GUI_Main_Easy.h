/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Borland VCL GUI for MediaInfo, Text Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Easy_H
#define GUI_Main_Easy_H
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
#include <OleCtrls.hpp>
#include <StdCtrls.hpp>
#include <ToolWin.hpp>
#include <vector>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Easy
//***************************************************************************

class GUI_Main_Easy : public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Easy(Core* C, TWinControl* Owner);
    ~GUI_Main_Easy();

    //Actions
    void GUI_Refresh();
    void GUI_Refresh_Partial();
    void GUI_Resize();
    void GUI_Resize_Partial();

private:
    //GUI
    TComboBox* Select;
    //std::vector<std::vector<GUI_Main_Easy_Box*> > Boxes;

    //Events
    //void OnChoice(wxCommandEvent& event);

private:
    TPanel* VCL;
};

#endif
