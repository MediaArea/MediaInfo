/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Common/Core.h"
#include "GUI/VCL_New/GUI_Main_HTML.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_HTML::GUI_Main_HTML(Core* Core_, TWinControl* Owner)
    : GUI_Main_Common_Core(Core_)
{
    //VCL specific
    VCL=new TCppWebBrowser(Owner);
    //VCL->Parent=Owner;

    //Update
    GUI_Refresh();
}

//---------------------------------------------------------------------------
GUI_Main_HTML::~GUI_Main_HTML()
{
    //VCL specific
    delete VCL; //VCL=NULL
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
void GUI_Main_HTML::GUI_Refresh()
{
    MediaInfoLib::String S1=C->Inform_Get().c_str();
    /*
    File F;
    if (FileName_Temp==__T(""))
    {
        FileName_Temp=FileName::TempFileName_Create(__T("MI_"));
        File::Delete(FileName_Temp);
        FileName_Temp+=__T(".html");
    }
    F.Create(FileName_Temp, true);
    F.Write(S1);
    F.Close();
    //Navigate
    WideString ToShow=FileName_Temp.c_str();
    Page_HTML_HTML->Navigate((wchar_t*)GUI_Text(ToShow));
    */
    VCL->Navigate(L"about:blank");
}

void GUI_Main_HTML::GUI_Resize()
{
    VCL->Left=0;
    VCL->Top=0;
    VCL->Width=100;//VCL->Parent->ClientWidth;
    VCL->Height=100;//VCL->Parent->ClientHeight;
}
