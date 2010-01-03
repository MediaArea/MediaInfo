// GUI_Main_HTML - VCL GUI for MediaInfo
// Copyright (C) 2007-2010 MediaArea.net SARL, Info@MediaArea.net
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
#include <vcl.h>
#pragma hdrstop
#include "Common/Core.h"
#include "GUI/VCL_New/GUI_Main_HTML.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_HTML::GUI_Main_HTML(Core* _C, TWinControl* Owner)
    : GUI_Main_Common_Core(_C)
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
    if (FileName_Temp==_T(""))
    {
        FileName_Temp=FileName::TempFileName_Create(_T("MI_"));
        File::Delete(FileName_Temp);
        FileName_Temp+=_T(".html");
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
