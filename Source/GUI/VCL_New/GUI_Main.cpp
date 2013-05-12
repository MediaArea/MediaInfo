/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Common/Core.h"
#include "GUI/VCL_New/GUI_Main.h"
#include "GUI/VCL_New/GUI_Main_Easy.h"
#include "GUI/VCL_New/GUI_Main_Tree.h"
#include "GUI/VCL_New/GUI_Main_HTML.h"
#include "GUI/VCL_New/GUI_Main_Text.h"

#include "GUI_Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGUI_Main_ *GUI_Main_;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// For Unicode management
// Here only because this is useful in all MediaInfo
#ifdef _UNICODE
    static std::wstring String_Void;
    #define GUI_Text(A) ((A).Length()>0?(A).c_bstr():String_Void.c_str())
#else
    #define GUI_Text(A) (A.c_str())
    //#include <wx/String.h>
    //static std::string String_Void;
    //#define GUI_Text(A) (A?wxString(A.c_bstr(), wxConvLocal).c_str():String_Void.c_str())
#endif //_UNICODE
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
__fastcall TGUI_Main_::TGUI_Main_(TComponent* Owner)
    : TForm(Owner)
{
    //Core
    C=new Core;

    //Menu and ToolBar
    //Menu_Create();
    //ToolBar_Create();

    //Status bar
    //CreateStatusBar();

    //GUI
    View=NULL;
    //CenterOnScreen();

    //Drag n drop
    //#if wxUSE_DRAG_AND_DROP
    //    FD=new FileDrop(C, this);
    //    SetDropTarget(FD);
    //#endif //wxUSE_DRAG_AND_DROP

    //Defaults
    Menu_View_TextClick(NULL);
    //Menu_Debug_Demux_None->Check(); //Default to no Debug Demux
}

__fastcall TGUI_Main_::~TGUI_Main_()
{
    delete C; //C=NULL;
    delete View; //View=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::Menu_File_Open_FileClick(TObject *Sender)
{
    if (!OpenDialog1->Execute())
        return;

    //if (M_Options_CloseAllAuto->Checked)
    //    M_File_Close_AllClick(Sender);

    //Retrieving filenames, manage them
    C->Menu_File_Open_Files_Begin();
    if (OpenDialog1->Files->Count==1)
        //un fichier
        C->Menu_File_Open_Files_Continue(GUI_Text(OpenDialog1->FileName));
    else
        //Plusieurs selections
        for (int Pos=0; Pos<OpenDialog1->Files->Count; Pos++)
            C->Menu_File_Open_Files_Continue(GUI_Text(OpenDialog1->Files->Strings[Pos]));

    //Showing
    View->GUI_Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::FormResize(TObject *Sender)
{
    if (View)
        View->GUI_Resize();
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::Menu_File_ExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::Menu_View_EasyClick(TObject *Sender)
{
    //Configuring
    C->Menu_View_Easy();

    //Showing
    GUI_Main_Common_Core* View_New=new GUI_Main_Easy(C, this);
    View_New->GUI_Resize();
    delete View; View=View_New;
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::Menu_View_SheetClick(TObject *Sender)
{
    //Configuring
    C->Menu_View_Sheet();

    //Showing
    //GUI_Main_Common_Core* View_New=new GUI_Main_Sheet(C, this);
    //View_New->GUI_Resize();
    //delete View; View=View_New;
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::Menu_View_TreeClick(TObject *Sender)
{
    //Configuring
    C->Menu_View_Tree();

    //Showing
    GUI_Main_Common_Core* View_New=new GUI_Main_Tree(C, this);
    View_New->GUI_Resize();
    delete View; View=View_New;
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::Menu_View_HTMLClick(TObject *Sender)
{
    //Configuring
    C->Menu_View_HTML();

    //Showing
    GUI_Main_Common_Core* View_New=new GUI_Main_HTML(C, this);
    View_New->GUI_Resize();
    delete View; View=View_New;
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::Menu_View_TextClick(TObject *Sender)
{
    //Configuring
    C->Menu_View_Text();

    //Showing
    GUI_Main_Common_Core* View_New=new GUI_Main_Text(C, this);
    View_New->GUI_Resize();
    delete View; View=View_New;
}
//---------------------------------------------------------------------------

void __fastcall TGUI_Main_::Menu_Debug_DetailsClick(TObject *Sender)
{
    //Configuring
    if (Menu_Debug_Details->Checked)
        C->Menu_Debug_Details(1);
    else
        C->Menu_Debug_Details(0);

    //Showing - Menu
    Menu_View_Easy->Enabled=!Menu_Debug_Details->Checked;
    Menu_View_Sheet->Enabled=false;
    Menu_View_Tree->Enabled=Menu_Debug_Details->Checked;
    Menu_View_HTML->Enabled=!Menu_Debug_Details->Checked;
    Menu_View_Text->Enabled=true;

    //Showing - View
    if (Menu_Debug_Details->Checked && (!Menu_View_Tree->Checked && !Menu_View_Text->Checked))
    {
        Menu_View_Tree->Checked=true;
        Menu_View_TreeClick(Sender);
    }
    if (!Menu_Debug_Details->Checked && (!Menu_View_HTML->Checked && !Menu_View_Text->Checked))
    {
        Menu_View_HTML->Checked=true;
        Menu_View_HTMLClick(Sender);
    }
    else
        View->GUI_Refresh();

}
//---------------------------------------------------------------------------

