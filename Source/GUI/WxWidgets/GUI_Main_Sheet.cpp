// GUI_Main_Sheet - WxWidgets GUI for MediaInfo
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
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#include "GUI/WxWidgets/GUI_Main_Sheet.h"
#include "Common/Core.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Events
//***************************************************************************

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Sheet::GUI_Main_Sheet(Core* _C, wxWindow* parent)
    : wxPanel(parent, -1, wxPoint(0, 0), wxSize(parent->GetClientSize().GetWidth()-0, parent->GetClientSize().GetHeight()-0)),
    GUI_Main_Common_Core(_C)
{
    //Update
    GUI_Refresh();
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_Sheet::GUI_Refresh()
{
}

void GUI_Main_Sheet::GUI_Resize()
{
    SetSize(0, 0, GetParent()->GetClientSize().GetWidth()-0, GetParent()->GetClientSize().GetHeight()-0);
}
