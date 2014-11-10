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
        C->Menu_File_Open_Files_Continue(String(FileNames[Pos].c_str()));
    #else //defined(__WXMSW__)
        C->Menu_File_Open_Files_Continue(String(FileNames2[Pos].c_str()));
    #endif //defined(__WXMSW__)
    wxTimeSpan Span=wxDateTime::UNow()-Begin;
    ((GUI_Main*)C->GUI_Main_Handler)->View_Refresh();
    if (((GUI_Main*)C->GUI_Main_Handler)->GetStatusBar()==NULL)
        ((GUI_Main*)C->GUI_Main_Handler)->CreateStatusBar();
    ((GUI_Main*)C->GUI_Main_Handler)->GetStatusBar()->SetLabel(Span.Format(__T("%Ss %lms")));
    return true;
}
#endif //wxUSE_DRAG_AND_DROP
