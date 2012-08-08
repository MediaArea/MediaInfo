// GUI_Main_FileDrop - FileDrop function
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
#include "GUI/WxWidgets/GUI_Main_FileDrop.h"
#include "GUI/WxWidgets/GUI_Main.h"
#include "Common/Core.h"
#include "wx/datetime.h"
#include "wx/file.h"
#include "wx/dir.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Functions
//***************************************************************************

//---------------------------------------------------------------------------
#if wxUSE_DRAG_AND_DROP
bool FileDrop::OnDropFiles(wxCoord, wxCoord, const wxArrayString& FileNames)
{
    #if !defined(__WXMSW__)
        wxArrayString FileNames2;
        for (size_t Pos=0; Pos<FileNames.size(); Pos++)
        {
            if (wxFile::Exists(FileNames[Pos]) || wxDir::Exists(FileNames[Pos]))
                FileNames2.Add(FileNames[Pos]);
            else
            {
                wxString Temp(FileNames[Pos]);
                std::string Temp2;
                for (size_t Temp_Pos=0; Temp_Pos<Temp.size(); Temp_Pos++)
                    Temp2.append(1, (char)Temp[Temp_Pos]);
                wxString Temp3(Temp2.c_str(), wxConvLocal);
                if (wxFile::Exists(Temp3) || wxDir::Exists(Temp3))
                    FileNames2.Add(Temp3);
                else
                    FileNames2.Add(FileNames[Pos]);
            }
        }
    #endif //!defined(__WXMSW__)

    C->Menu_File_Open_Files_Begin();

    wxDateTime Begin=wxDateTime::UNow();
    for (size_t Pos=0; Pos<FileNames.size(); Pos++)
    #if defined(__WXMSW__)
        C->Menu_File_Open_Files_Continue(FileNames[Pos].c_str());
    #else //defined(__WXMSW__)
        C->Menu_File_Open_Files_Continue(FileNames2[Pos].c_str());
    #endif //defined(__WXMSW__)
    wxTimeSpan Span=wxDateTime::UNow()-Begin;
    ((GUI_Main*)C->GUI_Main_Handler)->View_Refresh();
    if (((GUI_Main*)C->GUI_Main_Handler)->GetStatusBar()==NULL)
        ((GUI_Main*)C->GUI_Main_Handler)->CreateStatusBar();
    ((GUI_Main*)C->GUI_Main_Handler)->GetStatusBar()->SetLabel(Span.Format(__T("%Ss %lms")));
    return true;
}
#endif //wxUSE_DRAG_AND_DROP
