/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
GUI_Main_Easy::GUI_Main_Easy(Core* Core_, TWinControl* Owner)
    : GUI_Main_Common_Core(Core_)
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

