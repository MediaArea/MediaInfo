/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Preferences interface of MediaInfo
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation condition
#ifndef MEDIAINFOGUI_PREFS_NO
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "GUI/VCL/GUI_Preferences_Sheet.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Preferences.h"
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
//---------------------------------------------------------------------------


//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TPreferences_SheetF::TPreferences_SheetF(TComponent* Owner)
    : TForm(Owner)
{
    Columns_Count=0;

    //Columns count
    for (int I1=0; I1<100; I1++)
        ColumnsCount->Items->Add(WideString(I1));
    ColumnsCount->ItemIndex=1;
}
//---------------------------------------------------------------------------

int TPreferences_SheetF::Run(const Ztring &Name)
{
    EditedSheet.Load(Prefs->BaseFolder+Prefs->FolderNames[Prefs_Sheet]+__T("\\")+Name+__T(".csv"));
    ColumnsCount->ItemIndex=EditedSheet(__T("ColumnsCount"), 1).To_int32s();
    Columns_Adapt();
    for (int Pos=0; Pos<Columns_Count; Pos++)
    {
        Ztring Z1=__T("Column");
        Z1+=Ztring::ToZtring(Pos);
        int ItemIndex;
        AnsiString A=EditedSheet(Z1, 1).c_str();
        //Kind
        ItemIndex=Column_Kind[Pos]->Items->IndexOf(EditedSheet(Z1, 1).c_str());
        if (ItemIndex!=-1)
            Column_Kind[Pos]->ItemIndex=ItemIndex;
        Column_Kind0Change(Column_Kind[Pos]);
        //Pos
        ItemIndex=Column_Pos[Pos]->Items->IndexOf(EditedSheet(Z1, 2).c_str());
        if (ItemIndex!=-1)
            Column_Pos[Pos]->ItemIndex=ItemIndex;
        Column_Pos0Change(Column_Kind[Pos]);
        //Parameter
        ItemIndex=Column_Parameter[Pos]->Items->IndexOf(EditedSheet(Z1, 3).c_str());
        if (ItemIndex!=-1)
            Column_Parameter[Pos]->ItemIndex=ItemIndex;
        Column_Parameter0Change(Column_Parameter[Pos]);
        //Size
        ItemIndex=Column_Size[Pos]->Items->IndexOf(EditedSheet(Z1, 4).c_str());
        if (ItemIndex!=-1)
            Column_Size[Pos]->ItemIndex=ItemIndex;
    }
    Column_Size0Change(NULL);
    Sheet_Show->Cells[0][1]="(Example)";

    return ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_SheetF::ColumnsCountChange(TObject *Sender)
{
    EditedSheet(__T("ColumnsCount"))=GUI_Text(ColumnsCount->Text);
    Columns_Adapt();
}
//---------------------------------------------------------------------------

void TPreferences_SheetF::Columns_Adapt()
{
    int Before=Columns_Count;
    Columns_Count=EditedSheet(__T("ColumnsCount")).To_int32s();
    if (Columns_Count<0)
        Columns_Count=0;
    else if (Columns_Count>SHEETF_COLUMNS)
        Columns_Count=SHEETF_COLUMNS;

    if (Before<Columns_Count)
    {
        //More Columns
        for (int Pos=Before; Pos<Columns_Count; Pos++)
        {
            Column_Text[Pos]=new TLabel(this);
            Column_Text[Pos]->Parent=this;
            Column_Text[Pos]->Top=90+25*Pos;
            Column_Text[Pos]->Left=405;
            Column_Text[Pos]->Width=100;
            Column_Size[Pos]=new TComboBox(this);
            Column_Size[Pos]->Top=90+25*Pos;
            Column_Size[Pos]->Left=355;
            Column_Size[Pos]->Width=45;
            Column_Size[Pos]->Style=Stdctrls::csDropDownList;
            Column_Size[Pos]->OnChange=Column_Size0Change;
            Column_Size[Pos]->Parent=this;
            for (int I1=0; I1<100; I1++)
                Column_Size[Pos]->Items->Add(WideString(I1));
            Column_Size[Pos]->ItemIndex=10;
            Column_Parameter[Pos]=new TComboBox(this);
            Column_Parameter[Pos]->Top=90+25*Pos;
            Column_Parameter[Pos]->Left=205;
            Column_Parameter[Pos]->Style=Stdctrls::csDropDownList;
            Column_Parameter[Pos]->OnChange=Column_Parameter0Change;
            Column_Parameter[Pos]->Parent=this;
            Column_Pos[Pos]=new TComboBox(this);
            Column_Pos[Pos]->Top=90+25*Pos;
            Column_Pos[Pos]->Left=155;
            Column_Pos[Pos]->Width=45;
            Column_Pos[Pos]->Style=Stdctrls::csDropDownList;
            Column_Pos[Pos]->OnChange=Column_Pos0Change;
            Column_Pos[Pos]->Parent=this;
            for (int I1=0; I1<16; I1++)
                Column_Pos[Pos]->Items->Add(WideString(I1));
            Column_Pos[Pos]->ItemIndex=0;
            Column_Pos0Change(Column_Pos[Pos]);
            Column_Kind[Pos]=new TComboBox(this);
            Column_Kind[Pos]->Top=90+25*Pos;
            Column_Kind[Pos]->Left=5;
            Column_Kind[Pos]->Style=Stdctrls::csDropDownList;
            Column_Kind[Pos]->OnChange=Column_Kind0Change;
            Column_Kind[Pos]->Parent=this;
            Column_Kind[Pos]->Items->SetText(
                                               L"General\r\n"
                                                "Video\r\n"
                                                "Audio\r\n"
                                                "Text\r\n"
                                                "Chapters\r\n");
            Column_Kind[Pos]->ItemIndex=0;
            Column_Kind0Change(Column_Kind[Pos]);
        }
    }
    else
    {
        //Less columns
        for (int Pos=Before-1; Pos>=Columns_Count; Pos--)
        {
            delete Column_Text[Pos];
            delete Column_Size[Pos];
            delete Column_Parameter[Pos];
            delete Column_Pos[Pos];
            delete Column_Kind[Pos];
        }
    }

    //Manage Sheet_Show
    Column_Size0Change(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_SheetF::Column_Kind0Change(TObject *Sender)
{
    //Look for position of Sender
    int Pos=0;
    while (Sender!=Column_Kind[Pos])
        Pos++;
    //Set Pos to 0
    Column_Pos[Pos]->ItemIndex=0;

    //Fill Column_Parameter
    Column_Parameter[Pos]->Items->Clear();
    if (Column_Kind[Pos]->ItemIndex==0) //General
        Infos_Parameters[Pos].Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(Prefs->Translate(__T("General"))+__T("\r\n"), __T("\r\n\r\n")));
    if (Column_Kind[Pos]->ItemIndex==1) //Video
        Infos_Parameters[Pos].Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(Prefs->Translate(__T("Video"))+__T("\r\n"), __T("\r\n\r\n")));
    if (Column_Kind[Pos]->ItemIndex==2) //Audio
        Infos_Parameters[Pos].Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(Prefs->Translate(__T("Audio"))+__T("\r\n"), __T("\r\n\r\n")));
    if (Column_Kind[Pos]->ItemIndex==3) //Text
        Infos_Parameters[Pos].Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(Prefs->Translate(__T("Text"))+__T("\r\n"), __T("\r\n\r\n")));
    if (Column_Kind[Pos]->ItemIndex==4) //Chapters
        Infos_Parameters[Pos].Write(Ztring(MediaInfoNameSpace::MediaInfo::Option_Static(__T("Info_Parameters_CSV"))).SubString(Prefs->Translate(__T("Chapters"))+__T("\r\n"), __T("")));
    for (size_t Info_Pos=0; Info_Pos<Infos_Parameters[Pos].size(); Info_Pos++)
        Column_Parameter[Pos]->Items->Add(Infos_Parameters[Pos].Read(Info_Pos, 0).c_str());
    Column_Parameter[Pos]->ItemIndex=0;
    Column_Parameter0Change(Column_Parameter[Pos]);
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_SheetF::Column_Parameter0Change(TObject *Sender)
{
    //Look for position of Sender
    int Pos=0;
    while (Sender!=Column_Parameter[Pos])
        Pos++;

    //Fill Column_Text
    Column_Text[Pos]->Caption=Infos_Parameters[Pos](Column_Parameter[Pos]->ItemIndex, 1).c_str();

    //Fill Sheet_Show
    Sheet_Show->Cells[Pos][0]=Infos_Parameters[Pos](Column_Parameter[Pos]->ItemIndex, 0).c_str();
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_SheetF::FormResize(TObject *Sender)
{
    OK->Left=ClientWidth-OK->Width;
    Cancel->Left=OK->Left-Cancel->Width;
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_SheetF::OKClick(TObject *Sender)
{
    //Set count
    EditedSheet(__T("ColumnsCount"))=Ztring::ToZtring(Columns_Count);

    //Set lines
    for (int Pos=0; Pos<Columns_Count; Pos++)
    {
        Ztring Z1=__T("Column");
        Z1+=Ztring::ToZtring(Pos);
        EditedSheet(Z1, 1)=GUI_Text(Column_Kind[Pos]->Text);
        EditedSheet(Z1, 2)=GUI_Text(Column_Pos[Pos]->Text);
        EditedSheet(Z1, 3)=GUI_Text(Column_Parameter[Pos]->Text);
        EditedSheet(Z1, 4)=GUI_Text(Column_Size[Pos]->Text);
    }

    //Delete extra lines no more needed
    for (int Pos=SHEETF_COLUMNS-1; Pos>=Columns_Count; Pos--)
    {
        Ztring Z1=__T("Column");
        Z1+=Ztring::ToZtring(Pos);
        int I1=EditedSheet.Find(Z1);
        if (I1!=-1)
            EditedSheet.erase(&EditedSheet(I1));
    }

    //TODO: Purge blank lines, but why is there blank lines?
    while (EditedSheet.Find(__T(""))!=-1)
    {
        int I1=EditedSheet.Find(__T(""));
        EditedSheet.erase(&EditedSheet(I1));
    }

    //Save file
    EditedSheet.Save();
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_SheetF::Column_Size0Change(TObject *Sender)
{
    //Sum of sizes
    int Total=0;
    for (int Pos=0; Pos<Columns_Count; Pos++)
    #ifdef UNICODE
        Total+=Ztring(WideString(Column_Size[Pos]->Text).c_bstr()).To_int32s();
    #else
        Total+=Ztring().From_Unicode(Column_Size[Pos]->Text.c_bstr()).To_int32s();
    #endif

    //Adapt column sizes
    Sheet_Show->ColCount=Columns_Count;
    for (int Pos=0; Pos<Columns_Count; Pos++)
    #ifdef UNICODE
         Sheet_Show->ColWidths[Pos]=Ztring(WideString(Column_Size[Pos]->Text).c_bstr()).To_int32s()*Sheet_Show->ClientWidth/Total;
    #else
         Sheet_Show->ColWidths[Pos]=Ztring().From_Unicode(Column_Size[Pos]->Text.c_bstr()).To_int32s()*Sheet_Show->ClientWidth/Total;
    #endif
}
//---------------------------------------------------------------------------

void __fastcall TPreferences_SheetF::Column_Pos0Change(TObject *Sender)
{
    return;
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_PREFS_NO

