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
#include "GUI/WxWidgets/GUI_Main_Easy_Box.h"
#include "GUI/WxWidgets/GUI_Main_Easy.h"
#include "GUI/WxWidgets/GUI_Main_FileDrop.h"
#include "Common/Core.h"
#include <wx/choice.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/button.h>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
BEGIN_EVENT_TABLE(GUI_Main_Easy_Box, wxPanel)
    //Button
    EVT_BUTTON(26991, GUI_Main_Easy_Box::OnClick)
END_EVENT_TABLE()

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Easy_Box::GUI_Main_Easy_Box(Core* Core_, GUI_Main_Easy* Parent, wxWindow* Left, wxWindow* Top, stream_t StreamKind, size_t StreamPos)
    : wxPanel(Parent, 26991, wxPoint(0, 0), wxSize(Parent->GetClientSize().GetWidth()-0, Parent->GetClientSize().GetHeight()-0)),
    GUI_Main_Easy_Box_Core(Core_, Parent, StreamKind, StreamPos)
{
    //Internal
    GUI_Main_Easy_Box::Parent=Parent;
    GUI_Main_Easy_Box::Left=Left;
    GUI_Main_Easy_Box::Top=Top;

    //Creation
    Box=new wxStaticBox  (this, 26991, wxEmptyString);
    Text=new wxStaticText(this, 26991, wxEmptyString);
    Tags=new wxStaticText(this, 26991, wxEmptyString);
    Button=new wxButton  (this, 26991, wxEmptyString);

    //Drag and Drop
    #if wxUSE_DRAG_AND_DROP && defined(__WXMAC__)
        SetDropTarget(new FileDrop(Core_));
        Box->SetDropTarget(new FileDrop(Core_));
        Text->SetDropTarget(new FileDrop(Core_));
        Tags->SetDropTarget(new FileDrop(Core_));
        Button->SetDropTarget(new FileDrop(Core_));
    #endif //wxUSE_DRAG_AND_DROP

    //Update
    GUI_Resize();
    GUI_Refresh();
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Easy_Box::GUI_Refresh()
{
    //Translation
    Box->SetLabel(Box_Get().c_str());

    //Info
    Text->SetLabel(Text_Get().c_str());
    Text->SetToolTip(ToolTip_Get().c_str());
    Tags->SetLabel(Tags_Get().c_str());

    if (Button_Show())
    {
        Button->SetLabel(Button_Get().c_str());
        Button->Show();
    }
}

//---------------------------------------------------------------------------
void GUI_Main_Easy_Box::GUI_Resize()
{
    //Configuring
    if (MustHide())
    {
        SetSize     (Left?(Left->GetPosition().x+Left->GetSize().GetWidth()):2,
                    Top?(Top->GetPosition().y+Top->GetSize().GetHeight()):0,
                    0,
                    0);
        Hide();
        return;
    }

    //Calculating Parent data
    int Parent_Width=(Parent->GetClientSize().GetWidth()-1)/(int)Boxes_Show_Get();
    if (IsLastBox())
        Parent_Width+=(Parent->GetClientSize().GetWidth()-1)%(int)Boxes_Show_Get();
    const int Box_Boundary=3;

    //Text
    Text->SetSize  (Box_Boundary,
                    Box->GetCharHeight(),
                    (Parent_Width-Box_Boundary*2-(Tags_Get().empty()?0:Box->GetCharWidth()))/(Tags_Get().empty()?1:2)-1,
                    Text->GetCharHeight());
    Tags->SetSize  (Text->GetPosition().x+Text->GetSize().GetWidth()+Box->GetCharWidth(),
                    Text->GetPosition().y,
                    Tags_Get().empty()?0:Text->GetSize().GetWidth(),
                    Text->GetSize().GetHeight());
    int Text_Width=Text->GetSize().GetWidth();
    Text->SetLabel(Text_Get().c_str());
    Tags->SetLabel(Tags_Get().c_str());
    Text->Wrap(Text_Width);
    Tags->Wrap(Text_Width);
    Text->SetSize  (-1,
                    -1,
                    Text_Width,
                    Text->GetSize().GetHeight()>Text->GetCharHeight()*(int)Lines_Count_Get()*2?Text->GetCharHeight()*(int)Lines_Count_Get()*2:(unsigned int)-1);
    Tags->SetSize  (-1,
                    -1,
                    Text_Width,
                    Tags->GetSize().GetHeight()>Tags->GetCharHeight()*(int)Lines_Count_Get()*2?Text->GetCharHeight()*(int)Lines_Count_Get()*2:(unsigned int)-1);

    //Button
    Button->SetSize(Box_Boundary,
                    Text->GetPosition().y+(Text->GetSize().GetHeight()>Tags->GetSize().GetHeight()?Text->GetSize().GetHeight():Tags->GetSize().GetHeight()),
                    Parent_Width-Box_Boundary*2,
                    Button_Show()?(Button->GetBestSize().GetHeight()):0);

    //Panel
    Box->SetSize   (0,
                    0,
                    Parent_Width-2,
                    Button->GetPosition().y+Button->GetSize().GetHeight()+Box_Boundary);
    SetSize         (Left?(Left->GetPosition().x+Left->GetSize().GetWidth()):1,
                    Top?(Top->GetPosition().y+Top->GetSize().GetHeight()):0,
                    Parent_Width,
                    Box->GetSize().GetHeight());

    if (Button_Show())
        Button->Show();
    else
        Button->Hide();
    Show();
}

//---------------------------------------------------------------------------
void GUI_Main_Easy_Box::OnClick(wxCommandEvent&)
{
    //Showing
    //wxMessageBox(Button_Click().c_str(), __T("Should launch"), wxOK | wxICON_INFORMATION, this);
    #if defined (_WINDOWS)
        ShellExecute(NULL, __T("open"), Button_Click().c_str(), NULL, NULL, 0); //wxExecute(__T("cmd /C start ")+Button_Click());
    #elif defined (_MACOS) || defined (_MACOSX)
        wxExecute((__T("open ")+Button_Click()).c_str());
    #else
        wxExecute(__T("xdg-open ")+Button_Click());
    #endif
}
