// GUI_Main_Easy - VCL GUI for MediaInfo
// Copyright (C) 2007-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Common/Core.h"
#include "GUI/VCL_New/GUI_Main_Easy.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Easy::GUI_Main_Easy(Core* _C, TWinControl* Owner)
    : GUI_Main_Common_Core(_C)
{
    //VCL
    VCL=new TPanel(Owner);
    VCL->Parent=Owner;

    //Creation - Select
    Select=new TComboBox(Owner);
    Select->Parent=Owner;

    //Update
    GUI_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main_Easy::~GUI_Main_Easy()
{
    delete VCL; //VCL=NULL
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Easy::GUI_Refresh()
{
    //The choice list
/*
    Select->Clear();
    size_t FilesCount=FilesCount_Get();
    for (File_Pos=0; File_Pos<FilesCount; File_Pos++)
        Select->Append(FileName_Get());
    File_Pos=0;
    Select->SetSelection(File_Pos);
*/

    GUI_Refresh_Partial();
}

//---------------------------------------------------------------------------
void GUI_Main_Easy::GUI_Refresh_Partial()
{
/*
    //For each box
    for (size_t StreamPos=0; StreamPos<Stream_Max; StreamPos++)
        for (size_t Pos=0; Pos<Boxes[StreamPos].size(); Pos++)
            Boxes[StreamPos][Pos]->GUI_Refresh();
*/

    //Resize some boxes if needed
    GUI_Resize_Partial();
}

//---------------------------------------------------------------------------
void GUI_Main_Easy::GUI_Resize()
{
    //Global and Select
    VCL->Left=0;
    VCL->Top=0;
    VCL->Width=VCL->Parent->ClientWidth;
    VCL->Height=VCL->Parent->ClientHeight;
    Select->Left=0;
    Select->Top=0;
    Select->Width=Select->Parent->ClientWidth;
    Select->Height=20;

    //Other
    GUI_Resize_Partial();
}

//---------------------------------------------------------------------------
void GUI_Main_Easy::GUI_Resize_Partial()
{
/*
    //For each box
    for (size_t StreamPos=0; StreamPos<Stream_Max; StreamPos++)
        for (size_t Pos=0; Pos<Boxes[StreamPos].size(); Pos++)
            Boxes[StreamPos][Pos]->GUI_Resize();
*/
}

//***************************************************************************
// Events
//***************************************************************************

/*
void GUI_Main_Easy::OnChoice(wxCommandEvent& event)
{
    File_Pos=Select->GetSelection();
    GUI_Refresh_Partial();
}
*/

