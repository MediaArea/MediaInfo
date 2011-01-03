// IPreferences_Output - Preferences interface of MediaInfo
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
//
// Preferences interface of MediaInfo (Output definition part)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation condition
#ifndef MEDIAINFOGUI_PREFS_NO
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "GUI_Preferences_Output.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_DLL_RUNTIME
    #include "MediaInfoDLL/MediaInfoDLL.h"
    #define MediaInfoNameSpace MediaInfoDLL
#elif defined MEDIAINFO_DLL_STATIC
    #include "MediaInfoDLL/MediaInfoDLL_Static.h"
    #define MediaInfoNameSpace MediaInfoDLL
#else
    #include "MediaInfo/MediaInfoList.h"
    #define MediaInfoNameSpace MediaInfoLib
#endif
using namespace MediaInfoNameSpace;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ZtringListList Infos_Parameters;
TPreferences_OutputF *Preferences_OutputF;
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TPreferences_OutputF::TPreferences_OutputF(TComponent* Owner)
    : TForm(Owner)
{
    //AncienItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_OutputF::ListeChange(TObject *Sender)
{
    Ztring C1=Memo->Text.c_str();
    C1.FindAndReplace(_T("\r\n"), _T("\\r\\n"));
    Output(AncienItemIndex, 1)=C1;

    C1=Output.Read(Liste->ItemIndex, 1);
    C1.FindAndReplace(_T("\\r\\n"), _T("\r\n"));
    Memo->Text=C1.c_str();
    AncienItemIndex=Liste->ItemIndex;

    //Gestion texte
    Infos->Items->Clear();
    if (Liste->ItemIndex==0)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(_T("Info_Parameters_CSV"))).SubString(_T("General\r\n"), _T("\r\n\r\n")));
    if (Liste->ItemIndex==1)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(_T("Info_Parameters_CSV"))).SubString(_T("Video\r\n"), _T("\r\n\r\n")));
    if (Liste->ItemIndex==2)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(_T("Info_Parameters_CSV"))).SubString(_T("Audio\r\n"), _T("\r\n\r\n")));
    if (Liste->ItemIndex==3)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(_T("Info_Parameters_CSV"))).SubString(_T("Text\r\n"), _T("\r\n\r\n")));
    if (Liste->ItemIndex==4)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(_T("Info_Parameters_CSV"))).SubString(_T("Chapters\r\n"), _T("\r\n\r\n")));

    for (size_t Pos=0; Pos<Infos_Parameters.size(); Pos++)
        Infos->Items->Add(Infos_Parameters.Read(Pos, 0).c_str());
}
//---------------------------------------------------------------------------
void __fastcall TPreferences_OutputF::OKClick(TObject *Sender)
{
    ListeChange(NULL);
    Output.Save(Repertoire+_T("\\MediaInfo.View.")+Nom+_T(".csv"));
}
//---------------------------------------------------------------------------

int TPreferences_OutputF::Afficher(const Ztring &Folder, const Ztring &Name)
{
    Repertoire=Folder;
    Nom=Name;
    Output.Load(Folder+_T("\\MediaInfo.View.")+Nom+_T(".csv"));

    Liste->ItemIndex=0;

    Ztring C1=Output.Read(0, 1);
    C1.FindAndReplace(_T("\\r\\n"), _T("\r\n"));
    Memo->Text=C1.c_str();
    AncienItemIndex=Liste->ItemIndex;

    ListeChange(NULL);

    return ShowModal();
}

void __fastcall TPreferences_OutputF::InfosChange(TObject *Sender)
{
    AnsiString S1=Memo->Text;
    AnsiString S2=AnsiString(_T("%"))+Infos->Text+("%");
    S1.Insert(S2, Memo->SelStart+1);
    Memo->Text=S1;
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_PREFS_NO

