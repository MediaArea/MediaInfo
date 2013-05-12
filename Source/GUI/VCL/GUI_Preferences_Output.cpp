/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
    AncienItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_OutputF::ListeChange(TObject *Sender)
{
    Ztring C1=Memo->Text.c_str();
    C1.FindAndReplace(__T("\r\n"), __T("\\r\\n"));
    Output(AncienItemIndex, 1)=C1;

    C1=Output.Read(Liste->ItemIndex, 1);
    C1.FindAndReplace(__T("\\r\\n"), __T("\r\n"));
    Memo->Text=C1.c_str();
    AncienItemIndex=Liste->ItemIndex;

    //Gestion texte
    Infos->Items->Clear();
    if (Liste->ItemIndex==0)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(__T("General\r\n"), __T("\r\n\r\n")));
    if (Liste->ItemIndex==1)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(__T("Video\r\n"), __T("\r\n\r\n")));
    if (Liste->ItemIndex==2)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(__T("Audio\r\n"), __T("\r\n\r\n")));
    if (Liste->ItemIndex==3)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(__T("Text\r\n"), __T("\r\n\r\n")));
    if (Liste->ItemIndex==4)
        Infos_Parameters.Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(__T("Chapters\r\n"), __T("\r\n\r\n")));

    for (size_t Pos=0; Pos<Infos_Parameters.size(); Pos++)
        Infos->Items->Add(Infos_Parameters.Read(Pos, 0).c_str());
}
//---------------------------------------------------------------------------
void __fastcall TPreferences_OutputF::OKClick(TObject *Sender)
{
    ListeChange(NULL);
    Output.Save(Repertoire+__T("\\MediaInfo.View.")+Nom+__T(".csv"));
}
//---------------------------------------------------------------------------

int TPreferences_OutputF::Afficher(const Ztring &Folder, const Ztring &Name)
{
    Repertoire=Folder;
    Nom=Name;
    Output.Load(Folder+__T("\\MediaInfo.View.")+Nom+__T(".csv"));

    Liste->ItemIndex=0;

    Ztring C1=Output.Read(0, 1);
    C1.FindAndReplace(__T("\\r\\n"), __T("\r\n"));
    Memo->Text=C1.c_str();
    AncienItemIndex=Liste->ItemIndex;

    ListeChange(NULL);

    return ShowModal();
}

void __fastcall TPreferences_OutputF::InfosChange(TObject *Sender)
{
    AnsiString S1=Memo->Text;
    AnsiString S2=AnsiString(__T("%"))+Infos->Text+("%");
    S1.Insert(S2, Memo->SelStart+1);
    Memo->Text=S1;
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_PREFS_NO

