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
#include "GUI/WxWidgets/GUI_Main.h"
#include "GUI/WxWidgets/GUI_Main_FileDrop.h"
#include "Common/Core.h"
#include "CLI/CommandLine_Parser.h"
#include <vector>
#include <algorithm>
using namespace std;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifndef __WXMSW__
    #include "Resource/Image/MediaInfo.xpm"
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//Get command line args in main()
#ifdef UNICODE
    #ifdef _WIN32
        #include <windows.h>
        #define GETCOMMANDLINE() \
            MediaInfoNameSpace::Char** argv=CommandLineToArgvW(GetCommandLineW(), &argc); \

    #else //WIN32
        #define GETCOMMANDLINE() \
            MediaInfoNameSpace::Char** argv=argv_ansi; \

    #endif //WIN32
#else //UNICODE
    #define GETCOMMANDLINE() \
        MediaInfoNameSpace::Char** argv=argv_ansi; \

#endif //UNICODE

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
// Constructor
GUI_Main::GUI_Main(int argc, MediaInfoNameSpace::Char** argv_ansi, const wxPoint& pos, const wxSize& size, long style)
:wxFrame(NULL, -1, __T("MediaInfo"), pos, size, style)
{
    //Set the frame icon
    #ifdef WIN32 //Win32 use the first icon as file icon
        SetIcon(wxICON(aaaaaaaa));
    #else
        SetIcon(wxICON(MediaInfo));
    #endif

    //Core
    C=new Core;
    C->GUI_Main_Handler=this;

    //Menu and ToolBar
    Menu_Create();
    ToolBar_Create();

    //Status bar
    CreateStatusBar();

    //GUI
    View=NULL;
    CenterOnScreen();

    //Drag and Drop
    #if wxUSE_DRAG_AND_DROP
        SetDropTarget(new FileDrop(C));
    #endif //wxUSE_DRAG_AND_DROP

    //Defaults
    Menu_View_Sheet->Enable(false); //Not yet available
    Menu_View_Tree->Enable (false); //Not yet available
    Menu_View_Easy->Check(); //Default to HTML with GUI.
    wxCommandEvent* EventTemp=new wxCommandEvent();
    OnMenu_View_Easy(*EventTemp);
    delete EventTemp; //This is done to be GCC-compatible...
    Menu_Debug_Demux_None->Check(); //Default to no Debug Demux

    //Command line
    GETCOMMANDLINE();
    vector<String> List;
    for (int Pos=1; Pos<argc; Pos++)
    {
        //First part of argument (before "=") should be case insensitive
        String Argument(argv[Pos]);
        size_t Egal_Pos=Argument.find(__T('='));
        if (Egal_Pos==string::npos)
            Egal_Pos=Argument.size();
        transform(Argument.begin(), Argument.begin()+Egal_Pos, Argument.begin(), (int(*)(int))tolower); //(int(*)(int)) is a patch for unix
        int Return=Parse (*C, Argument);
        if (Return<0)
            return; //no more tasks to do
        if (Return>0)
            List.push_back(argv[Pos]); //Append the filename to the list of filenames to parse
    }

    //Parse files
    C->Menu_File_Open_Files_Begin();
    for (size_t Pos=0; Pos<List.size(); Pos++)
        C->Menu_File_Open_Files_Continue(List[Pos]);
    View_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main::~GUI_Main()
{
    delete C; //C=NULL;
    delete View; //View=NULL;
}

//***************************************************************************
// Events
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main::OnSize(wxSizeEvent& WXUNUSED(event))
{
    if (View && IsShown())
        View->GUI_Resize();
}
