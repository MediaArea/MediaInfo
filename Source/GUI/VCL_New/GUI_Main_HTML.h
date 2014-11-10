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
#ifndef GUI_Main_HTML_H
#define GUI_Main_HTML_H
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
#include "SHDocVw_OCX.h"
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_HTML
//***************************************************************************

class GUI_Main_HTML : public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_HTML(Core* C, TWinControl* Owner);
    ~GUI_Main_HTML();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();

private:
    TCppWebBrowser* VCL;
};

#endif
