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

#include "wx/config.h"

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

    wxConfigBase *pConfig = wxConfigBase::Get();

    wxString view = pConfig->Read(wxT("/View"), wxT("Easy"));

    //Defaults
    Menu_View_Sheet->Enable(false); //Not yet available
    Menu_View_Tree->Enable (false); //Not yet available

    wxCommandEvent* EventTemp=new wxCommandEvent();
    if (view==wxT("Easy"))
    {
        Menu_View_Easy->Check();
        OnMenu_View_Easy(*EventTemp);
    }
    else if (view==wxT("HTML"))
    {
        Menu_View_HTML->Check();
        OnMenu_View_HTML(*EventTemp);
    }
    else if (view==wxT("Text"))
    {
        Menu_View_Text->Check();
        OnMenu_View_Text(*EventTemp);
    }
    else if (view==wxT("XML"))
    {
        Menu_View_XML->Check();
        OnMenu_View_XML(*EventTemp);
    }
    else if (view==wxT("JSON"))
    {
        Menu_View_JSON->Check();
        OnMenu_View_JSON(*EventTemp);
    }
    else if (view==wxT("MPEG7_Strict"))
    {
        Menu_View_MPEG7_Strict->Check();
        OnMenu_View_MPEG7_Strict(*EventTemp);
    }
    else if (view==wxT("MPEG7_Relaxed") || view==wxT("MPEG7"))
    {
        Menu_View_MPEG7_Relaxed->Check();
        OnMenu_View_MPEG7_Relaxed(*EventTemp);
    }
    else if (view==wxT("MPEG7_Extended"))
    {
        Menu_View_MPEG7_Extended->Check();
        OnMenu_View_MPEG7_Extended(*EventTemp);
    }
    else if (view==wxT("PBCore_1_2"))
    {
        Menu_View_PBCore_1_2->Check();
        OnMenu_View_PBCore_1_2(*EventTemp);
    }
    else if (view==wxT("PBCore_2_0"))
    {
        Menu_View_PBCore_2_0->Check();
        OnMenu_View_PBCore_2_0(*EventTemp);
    }
    else if (view==wxT("EBUCore_1_5"))
    {
        Menu_View_EBUCore_1_5->Check();
        OnMenu_View_EBUCore_1_5(*EventTemp);
    }
    else if (view==wxT("EBUCore_1_6"))
    {
        Menu_View_EBUCore_1_6->Check();
        OnMenu_View_EBUCore_1_6(*EventTemp);
    }
    else if (view==wxT("EBUCore_1_8_ps"))
    {
        Menu_View_EBUCore_1_8_ps->Check();
        OnMenu_View_EBUCore_1_8_ps(*EventTemp);
    }
    else if (view==wxT("EBUCore_1_8_sp"))
    {
        Menu_View_EBUCore_1_8_sp->Check();
        OnMenu_View_EBUCore_1_8_sp(*EventTemp);
    }
    else if (view==wxT("EBUCore_1_8_ps_json"))
    {
        Menu_View_EBUCore_1_8_ps_json->Check();
        OnMenu_View_EBUCore_1_8_ps_json(*EventTemp);
    }
    else if (view==wxT("EBUCore_1_8_sp_json"))
    {
        Menu_View_EBUCore_1_8_sp_json->Check();
        OnMenu_View_EBUCore_1_8_sp_json(*EventTemp);
    }
    else if (view==wxT("FIMS_1_1"))
    {
        Menu_View_FIMS_1_1->Check();
        OnMenu_View_FIMS_1_1(*EventTemp);
    }
    else if (view==wxT("FIMS_1_2"))
    {
        Menu_View_FIMS_1_2->Check();
        OnMenu_View_FIMS_1_2(*EventTemp);
    }
    else if (view==wxT("reVTMD"))
    {
        Menu_View_reVTMD->Check();
        OnMenu_View_reVTMD(*EventTemp);
    }
    else if (view==wxT("NISO_Z39_87"))
    {
        Menu_View_NISO_Z39_87->Check();
        OnMenu_View_NISO_Z39_87(*EventTemp);
    }
    else
    {
        Menu_View_Easy->Check();
        OnMenu_View_Easy(*EventTemp);
    }


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

    //Save view state
    wxConfig *config = new wxConfig(wxT("mediainfo-gui"));


    wxConfigBase *pConfig = wxConfigBase::Get();
    if ( pConfig == NULL )
    {
        return;
    }

    bool rememberView;
    if ( !pConfig->Read(wxT("rememberView"), &rememberView))
    {
        rememberView=1;
        pConfig->Write(wxT("/rememberView"), 1);
    }

    if (rememberView == 1)
    {
        enum Core::kind currentView = C->Kind_Get();

        switch (currentView)
        {
            case 0:
                pConfig->Write(wxT("/View"), wxT("Easy"));
                break;
            case 1:
                pConfig->Write(wxT("/View"), wxT("Sheet"));
                break;
            case 2:
                pConfig->Write(wxT("/View"), wxT("Tree"));
                break;
            case 3:
                pConfig->Write(wxT("/View"), wxT("HTML"));
                break;
            case 4:
                pConfig->Write(wxT("/View"), wxT("Text"));
                break;
            case 5:
                pConfig->Write(wxT("/View"), wxT("XML"));
                break;
            case 6:
                pConfig->Write(wxT("/View"), wxT("JSON"));
                break;
            case 7:
                pConfig->Write(wxT("/View"), wxT("PBCore"));
                break;
            case 8:
                pConfig->Write(wxT("/View"), wxT("PBCore2"));
                break;
            case 9:
                pConfig->Write(wxT("/View"), wxT("reVTMD"));
                break;
            case 10:
                pConfig->Write(wxT("/View"), wxT("MPEG7_Strict"));
                break;
            case 11:
                pConfig->Write(wxT("/View"), wxT("MPEG7_Relaxed"));
                break;
            case 12:
                pConfig->Write(wxT("/View"), wxT("MPEG7_Extended"));
                break;
            case 13:
                pConfig->Write(wxT("/View"), wxT("EBUCore_1_5"));
                break;
            case 14:
                pConfig->Write(wxT("/View"), wxT("EBUCore_1_6"));
                break;
            case 15:
                pConfig->Write(wxT("/View"), wxT("EBUCore_1_8_ps"));
                break;
            case 16:
                pConfig->Write(wxT("/View"), wxT("EBUCore_1_8_sp"));
                break;
            case 17:
                pConfig->Write(wxT("/View"), wxT("EBUCore_1_8_ps_json"));
                break;
            case 18:
                pConfig->Write(wxT("/View"), wxT("EBUCore_1_8_sp_json"));
                break;
            case 19:
                pConfig->Write(wxT("/View"), wxT("FIMS_1_1"));
                break;
            case 20:
                pConfig->Write(wxT("/View"), wxT("FIMS_1_2"));
                break;
            case 21:
                pConfig->Write(wxT("/View"), wxT("FIMS_1_3"));
                break;
            case 22:
                pConfig->Write(wxT("/View"), wxT("NISO_Z39_87"));
                break;
        }
    }

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
