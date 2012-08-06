// GUI_Main_Text - VCL GUI for MediaInfo
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
GUI_Main_Text::GUI_Main_Text(Core* _C, TWinControl* Owner)
    : GUI_Main_Common_Core(_C)
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
