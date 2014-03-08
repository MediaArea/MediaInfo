/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Common/Core.h"
#include "GUI/VCL_New/GUI_Main_Text.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Text::GUI_Main_Text(Core* Core_, TWinControl* Owner)
    : GUI_Main_Common_Core(Core_)
{
    //VCL specific
    VCL=new TMemo(Owner);
    VCL->Parent=Owner;

    //Update
    GUI_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main_Text::~GUI_Main_Text()
{
    //VCL specific
    delete VCL; //VCL=NULL
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Text::GUI_Refresh()
{
    VCL->Text=C->Inform_Get().c_str();
}

void GUI_Main_Text::GUI_Resize()
{
    VCL->Left=0;
    VCL->Top=0;
    VCL->Width=VCL->Parent->ClientWidth;
    VCL->Height=VCL->Parent->ClientHeight;
}
