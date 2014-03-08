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
#include "GUI/WxWidgets/GUI_Main_Text.h"
#include "GUI/WxWidgets/GUI_Main_FileDrop.h"
#include "Common/Core.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Text::GUI_Main_Text(Core* Core_, wxWindow* parent)
    : wxTextCtrl(parent, -1, __T(""), wxPoint(0, 0), wxSize(parent->GetClientSize().GetWidth()-0, parent->GetClientSize().GetHeight()-0), wxTE_READONLY|wxTE_MULTILINE|wxTE_RICH|wxTE_RICH2),
    GUI_Main_Common_Core(Core_)
{
    wxFont Font;
    Font.SetFamily(wxFONTFAMILY_MODERN);
    wxTextAttr Attr;
    Attr.SetFont(Font);
    #if wxCHECK_VERSION(2, 9, 0)
        Attr.SetFontSize(8);
    #endif
    SetDefaultStyle(Attr);

    //Drag and Drop
    #if wxUSE_DRAG_AND_DROP && defined(__WXMAC__)
        SetDropTarget(new FileDrop(C));
    #endif //wxUSE_DRAG_AND_DROP

    //Update
    GUI_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main_Text::~GUI_Main_Text()
{
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Text::GUI_Refresh()
{
    Clear();
    WriteText(C->Inform_Get().c_str());
}

void GUI_Main_Text::GUI_Resize()
{
    SetSize(0, 0, GetParent()->GetClientSize().GetWidth()-0, GetParent()->GetClientSize().GetHeight()-0);
}
