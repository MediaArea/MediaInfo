// GUI_Main_FileDrop - FileDrop function
// Copyright (C) 2002-2011 MediaArea.net SARL, Info@MediaArea.net
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
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#include "GUI/WxWidgets/GUI_Main_FileDrop.h"
#include "GUI/WxWidgets/GUI_Main.h"
#include "Common/Core.h"
#include "wx/datetime.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Functions
//***************************************************************************

//---------------------------------------------------------------------------
#if wxUSE_DRAG_AND_DROP
bool FileDrop::OnDropFiles(wxCoord, wxCoord, const wxArrayString& FileNames)
{
    C->Menu_File_Open_Files_Begin();

    wxDateTime Begin=wxDateTime::UNow();
    for (size_t Pos=0; Pos<FileNames.size(); Pos++)
        C->Menu_File_Open_Files_Continue(FileNames[Pos].c_str());
    wxTimeSpan Span=wxDateTime::UNow()-Begin;
    ((GUI_Main*)C->GUI_Main_Handler)->View_Refresh();
    if (((GUI_Main*)C->GUI_Main_Handler)->GetStatusBar()==NULL)
        ((GUI_Main*)C->GUI_Main_Handler)->CreateStatusBar();
    ((GUI_Main*)C->GUI_Main_Handler)->GetStatusBar()->SetLabel(Span.Format(_T("%Ss %lms")));
    return true;
}
#endif //wxUSE_DRAG_AND_DROP
