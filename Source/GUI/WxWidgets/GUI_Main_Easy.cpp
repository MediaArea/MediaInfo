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
#include "GUI/WxWidgets/GUI_Main_Easy.h"
#include "GUI/WxWidgets/GUI_Main_Easy_Box.h"
#include "GUI/WxWidgets/GUI_Main_FileDrop.h"
#include "Common/Core.h"
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/button.h>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(GUI_Main_Easy, wxPanel)
    EVT_CHOICE (26981, GUI_Main_Easy::OnChoice)
END_EVENT_TABLE()
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Easy::GUI_Main_Easy(Core* Core_, wxWindow* parent)
    : wxPanel(parent, 26981, wxPoint(0, 0), wxSize(parent->GetClientSize().GetWidth()-0, parent->GetClientSize().GetHeight()-0)),
    GUI_Main_Easy_Core(Core_)
{
    //Creation - Select
    Select=new wxChoice(this, 26981);

    //Creation - Other
    Boxes.resize(Stream_Max);
    for (size_t StreamPos=0; StreamPos<Stream_Max; StreamPos++)
    {
        Boxes[StreamPos].resize(Boxes_Count_Get(StreamPos));
        for (size_t Pos=0; Pos<Boxes[StreamPos].size(); Pos++)
            Boxes[StreamPos][Pos]=new GUI_Main_Easy_Box(C, this, Pos==0?NULL:Boxes[StreamPos][Pos-1], StreamPos==0?(wxWindow*)Select:(wxWindow*)Boxes[StreamPos-1][0], (stream_t)StreamPos, Pos);
    }

    //Drag and Drop
    #if wxUSE_DRAG_AND_DROP && defined(__WXMAC__)
        SetDropTarget(new FileDrop(C));
        Select->SetDropTarget(new FileDrop(C));
    #endif //wxUSE_DRAG_AND_DROP

    //Update
    GUI_Resize();
    GUI_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main_Easy::~GUI_Main_Easy()
{
    for (size_t StreamPos=0; StreamPos<Stream_Max; StreamPos++)
    {
        Boxes[StreamPos].resize(Boxes_Count_Get(StreamPos));
        for (size_t Pos=0; Pos<Boxes[StreamPos].size(); Pos++)
            delete Boxes[StreamPos][Pos]; //Boxes[StreamPos][Pos]=NULL;
    }
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Easy::GUI_Refresh()
{
    //The choice list
    Select->Clear();
    size_t FilesCount=FilesCount_Get();
    for (File_Pos=0; File_Pos<FilesCount; File_Pos++)
        Select->Append(FileName_Get().c_str());
    File_Pos=0;
    Select->SetSelection((int)File_Pos);

    GUI_Refresh_Partial();
}

//---------------------------------------------------------------------------
void GUI_Main_Easy::GUI_Refresh_Partial()
{
    //For each box
    for (size_t StreamPos=0; StreamPos<Stream_Max; StreamPos++)
        for (size_t Pos=0; Pos<Boxes[StreamPos].size(); Pos++)
            Boxes[StreamPos][Pos]->GUI_Refresh();

    //Resize some boxes if needed
    GUI_Resize_Partial();
}

//---------------------------------------------------------------------------
void GUI_Main_Easy::GUI_Resize()
{
    //Global and Select
    SetSize(0, 0, GetParent()->GetClientSize().GetWidth()-0, GetParent()->GetClientSize().GetHeight()-0);
    Select->SetSize(0, 0, GetClientSize().GetWidth(), Select->GetBestSize().GetHeight());

    //Other
    GUI_Resize_Partial();
}

//---------------------------------------------------------------------------
void GUI_Main_Easy::GUI_Resize_Partial()
{
    //For each box
    for (size_t StreamPos=0; StreamPos<Stream_Max; StreamPos++)
        for (size_t Pos=0; Pos<Boxes[StreamPos].size(); Pos++)
            Boxes[StreamPos][Pos]->GUI_Resize();
}

//***************************************************************************
// Events
//***************************************************************************

void GUI_Main_Easy::OnChoice(wxCommandEvent& event)
{
    File_Pos=Select->GetSelection();
    GUI_Refresh_Partial();
}

