// GUI_About - About interface of MediaInfo
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
// Compilation condition
#ifndef MEDIAINFOGUI_ABOUT_NO
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "GUI/VCL/GUI_About.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutF *AboutF;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Preferences.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const ZenLib::Char* MEDIAINFO_ABOUT=     _T("MediaInfo X.X.X.X\\r\\nCopyright (C) 2002-2011 MediaArea.net SARL");
const ZenLib::Char* MEDIAINFO_URL=       _T("http://mediainfo.sourceforge.net");
const ZenLib::Char* MEDIAINFO_NEWVERSION=_T("http://sourceforge.net/projects/mediainfo/files/");
const ZenLib::Char* MEDIAINFO_DONATE=    _T("http://mediainfo.sourceforge.net/Donate");
const ZenLib::Char* MEDIAINFO_MAILTO=    _T("mailto:zenitram@users.sourceforge.net");
//---------------------------------------------------------------------------

//***************************************************************************
// Class
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TAboutF::TAboutF(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TAboutF::FormShow(TObject *Sender)
{
    //Information
    ZenLib::Ztring C1;
    C1+=MEDIAINFO_ABOUT;
    C1.FindAndReplace(_T("X.X.X.X"), MediaInfo_Version_GUI);
    C1+=_T("\r\n\r\n");
    C1+=Prefs->Translate(_T("MediaInfo_About")).c_str();
    C1.FindAndReplace(_T("\\r\\n"), _T("\r\n"), 0, ZenLib::Ztring_Recursive);
    Memo->Text=C1.c_str();

    //Translation
    Caption=Prefs->Translate(_T("About")).c_str();
    OK->Caption=Prefs->Translate(_T("OK")).c_str();
    WebSite->Caption=Prefs->Translate(_T("Go to WebSite")).c_str();
    NewVersion->Caption=Prefs->Translate(_T("CheckNewVersion")).c_str();
    Donate->Caption=Prefs->Translate(_T("Donate")).c_str();
    WriteMe->Caption=Prefs->Translate(_T("WriteMe")).c_str();
    if (Prefs->Translate(_T("  Author_Name")).size()==0)
        Translator->Visible=false;
    else
    {
        Translator->Caption=(Prefs->Translate(_T("Translator"))+Prefs->Translate(_T(": "))+Prefs->Translate(_T("  Author_Name"))).c_str();
        Translator->Visible=true;
    }
    Translator_Url=Prefs->Translate(_T("  Author_Email"));
    if (Translator_Url.size()==0 || Translator_Url==_T("Zen@mediaarea.net"))
        WriteToTranslator->Visible=false;
    else
    {
        WriteToTranslator->Caption=Prefs->Translate(_T("WriteToTranslator")).c_str();
        WriteToTranslator->Visible=true;
    }

    if (Prefs->Donated)
    {
        Digimetrics->Visible=false;
        Digimetrics_Label->Visible=false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TAboutF::NewVersionClick(TObject *Sender)
{
    ShellExecute(NULL, NULL, MEDIAINFO_NEWVERSION, NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
void __fastcall TAboutF::DonateClick(TObject *Sender)
{
	ShellExecute(NULL, NULL, MEDIAINFO_DONATE, NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
void __fastcall TAboutF::WriteMeClick(TObject *Sender)
{
    ShellExecute(NULL, NULL, MEDIAINFO_MAILTO, NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
void __fastcall TAboutF::WriteToTranslatorClick(TObject *Sender)
{
    ZenLib::Ztring Url=ZenLib::Ztring(_T("mailto:"))+Translator_Url;
    ShellExecute(NULL, NULL, Url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

//---------------------------------------------------------------------------
void __fastcall TAboutF::WebSiteClick(TObject *Sender)
{
    ShellExecute(NULL, NULL, MEDIAINFO_URL, NULL, NULL, SW_SHOWNORMAL);
}


//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_ABOUT_NO



void __fastcall TAboutF::DigimetricsClick(TObject *Sender)
{
    ShellExecute(NULL, NULL, _T("http://digi-metrics.com/"), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

