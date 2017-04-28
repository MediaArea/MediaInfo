/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
// Compilation condition
#ifndef MEDIAINFOGUI_EXPORT_NO
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <vcl.h>
#include <ZenLib/Conf.h>
#pragma hdrstop
#include "GUI/VCL/GUI_Export.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TExportF *ExportF;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Preferences.h"
#include <ZenLib/ZtringListListF.h>
#include <ZenLib/FileName.h>
#include <ZenLib/File.h>
using namespace MediaInfoNameSpace;
using namespace ZenLib;
#ifdef _UNICODE
    #define ZEN_UNICODE(A) A.w_str()
#else
    #define ZEN_UNICODE(A) Ztring().From_Unicode(A.c_str()).c_str()
#endif //_UNICODE
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
__fastcall TExportF::TExportF(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
void TExportF::Name_Adapt()
{
    FileName FN=ZEN_UNICODE(Name->Text);

    if (FN.Name_Get().size()==0)
        FN.Name_Set(__T("Example"));

         if (Export->ActivePage==Export_CSV)
    {
        FN.Extension_Set(__T("csv"));
        SaveDialog1->DefaultExt=__T("csv");
        SaveDialog1->Filter=__T("CSV File|*.csv");
    }
    else if (Export->ActivePage==Export_Sheet)
    {
        FN.Extension_Set(__T("csv"));
        SaveDialog1->DefaultExt=__T("csv");
        SaveDialog1->Filter=__T("CSV File|*.csv");
    }
    else if (Export->ActivePage==Export_Text)
    {
        FN.Extension_Set(__T("txt"));
        SaveDialog1->DefaultExt=__T("txt");
        SaveDialog1->Filter=__T("Text File|*.txt");
    }
    else if (Export->ActivePage==Export_HTML)
    {
        FN.Extension_Set(__T("html"));
        SaveDialog1->DefaultExt=__T("html");
        SaveDialog1->Filter=__T("HTML File|*.html");
    }
    else if (Export->ActivePage==Export_XML)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_MPEG7)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("MPEG-7 File|*.xml");
    }
    else if (Export->ActivePage==Export_PBCore)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_PBCore2)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_EBUCore_1_5)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_EBUCore_1_6)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_EBUCore_1_8_ps)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_EBUCore_1_8_sp)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_FIMS_1_1)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_FIMS_1_2)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_FIMS_1_3)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_reVTMD)
    {
        FN.Extension_Set(__T("xml"));
        SaveDialog1->DefaultExt=__T("xml");
        SaveDialog1->Filter=__T("XML File|*.xml");
    }
    else if (Export->ActivePage==Export_Custom)
    {
        if (Prefs->Details[Prefs_Custom](Stream_Max+2, 1).size()>0 && Prefs->Details[Prefs_Custom](Stream_Max+2, 1)[0]==__T('<')) //test if HTML
        {
            FN.Extension_Set(__T("html"));
            SaveDialog1->DefaultExt=__T("html");
            SaveDialog1->Filter=__T("HTML files|*.htm *.html");
        }
        else
        {
            FN.Extension_Set(__T("txt"));
            SaveDialog1->DefaultExt=__T("txt");
            SaveDialog1->Filter=__T("Text files|*.txt");
        }
    }

    Name->Text=FN.c_str();
}
//---------------------------------------------------------------------------

int TExportF::Run(MediaInfoNameSpace::MediaInfoList &MI, ZenLib::Ztring DefaultFolder)
{
    //Aquistion of datas
    if (Name->Text.Length()==0)
        Name->Text=DefaultFolder.c_str();
    ToExport=&MI;

    //GUI
    GUI_Configure();

    Ztring Info=MI.Option(__T("Inform_Get"), Ztring());

    if (Info==__T("XML"))
        Export->ActivePage=Export_XML;
    else if (Info==__T("MPEG-7"))
        Export->ActivePage=Export_MPEG7;
    else if (Info==__T("PBCore_1.2"))
        Export->ActivePage=Export_PBCore;
    else if (Info==__T("PBCore_2.0"))
        Export->ActivePage=Export_PBCore2;
    else if (Info==__T("EBUCore_1.5"))
        Export->ActivePage=Export_EBUCore_1_5;
    else if (Info==__T("EBUCore_1.6"))
        Export->ActivePage=Export_EBUCore_1_6;
    else if (Info==__T("EBUCore_1.8_ps"))
        Export->ActivePage=Export_EBUCore_1_8_ps;
    else if (Info==__T("EBUCore_1.8_sp"))
        Export->ActivePage=Export_EBUCore_1_8_sp;
    else if (Info==__T("FIMS_1.1"))
        Export->ActivePage=Export_FIMS_1_1;
    else if (Info==__T("FIMS_1.2"))
        Export->ActivePage=Export_FIMS_1_2;
    else if (Info==__T("FIMS_1.3"))
        Export->ActivePage=Export_FIMS_1_3;
    else if (Info==__T("reVTMD"))
        Export->ActivePage=Export_reVTMD;

    ExportChange(NULL);

    return ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Name_FileSelectClick(TObject *Sender)
{
    SaveDialog1->InitialDir=Name->Text;

    if (!SaveDialog1->Execute())
        return;

    Name->Text=SaveDialog1->FileName;
    Name_Adapt();
}
//---------------------------------------------------------------------------

void TExportF::Export_Run()
{
    //Create text for the file
    Ztring Text;
    Ztring Append_Separator=__T("\r\n");

         if (Export->ActivePage==Export_CSV)
    {
        //Full information
        bool MediaInfo_Complete;
        if (CSV_Advanced->Checked)
            MediaInfo_Complete=true;
        else
            MediaInfo_Complete=false;

        //General
        ZtringListListF CSV;
        ZtringListList Parameters;
        Parameters.Write(MediaInfo::Option_Static(__T("Info_Parameters_CSV")));
        int Pos_Start=1;
        int Pos_End=Parameters.Find(__T("Video"))-1;
        int CSV_Pos=0;

        for (int I1=0; I1<Pos_End-Pos_Start; I1++)
            if (MediaInfo_Complete || ToExport->Get(0, Stream_General, 0, I1, Info_Options)[InfoOption_ShowInInform]==__T('Y'))
            {
                CSV(0, CSV_Pos)=Ztring(__T("General "))+Parameters(Pos_Start+I1, 0);
                for (int FilePos=0; FilePos<ToExport->Count_Get(); FilePos++)
                    CSV(1+FilePos, CSV_Pos)=ToExport->Get(FilePos, Stream_General, 0, I1);
                CSV_Pos++;
            }

        //Video
        Pos_Start=Pos_End+2;
        Pos_End=Parameters.Find(__T("Audio"))-1;

        for (int I1=0; I1<Pos_End-Pos_Start; I1++)
        {
            for (int Count=0; Count<CSV_Stream_Video->ItemIndex; Count++)
            if (MediaInfo_Complete || ToExport->Get(0, Stream_Video, 0, I1, Info_Options)[InfoOption_ShowInInform]==__T('Y'))
                {
                    CSV(0, CSV_Pos)=Ztring(__T("Video "))+Ztring::ToZtring(Count)+__T(" ")+Parameters(Pos_Start+I1, 0);
                    for (int FilePos=0; FilePos<ToExport->Count_Get(); FilePos++)
                        CSV(1+FilePos, CSV_Pos)=ToExport->Get(FilePos, Stream_Video, 0, I1);
                    CSV_Pos++;
                }
        }

        //Audio
        Pos_Start=Pos_End+2;
        Pos_End=Parameters.Find(__T("Text"))-1;
        for (int Count=0; Count<CSV_Stream_Audio->ItemIndex; Count++)
        {
            for (int I1=0; I1<Pos_End-Pos_Start; I1++)
            if (MediaInfo_Complete || ToExport->Get(0, Stream_Audio, 0, I1, Info_Options)[InfoOption_ShowInInform]==__T('Y'))
                {
                    CSV(0, CSV_Pos)=Ztring(__T("Audio "))+Ztring::ToZtring(Count)+__T(" ")+Parameters(Pos_Start+I1, 0);
                    for (int FilePos=0; FilePos<ToExport->Count_Get(); FilePos++)
                        CSV(1+FilePos, CSV_Pos)=ToExport->Get(FilePos, Stream_Audio, Count, I1);
                    CSV_Pos++;
                }
        }

        //Text
        Pos_Start=Pos_End+2;
        Pos_End=Parameters.Find(__T("Chapters"))-1;
        for (int Count=0; Count<CSV_Stream_Text->ItemIndex; Count++)
        {
            for (int I1=0; I1<Pos_End-Pos_Start; I1++)
                if (MediaInfo_Complete || ToExport->Get(0, Stream_Text, 0, I1, Info_Options)[InfoOption_ShowInInform]==__T('Y'))
                {
                    CSV(0, CSV_Pos)=Ztring(__T("Text "))+Ztring::ToZtring(Count)+__T(" ")+Parameters(Pos_Start+I1, 0);
                    for (int FilePos=0; FilePos<ToExport->Count_Get(); FilePos++)
                        CSV(1+FilePos, CSV_Pos)=ToExport->Get(FilePos, Stream_Text, Count, I1);
                    CSV_Pos++;
                }
        }

        //Chapters
        Pos_Start=Pos_End+2;
        Pos_End=Parameters.size()-1;
        for (int Count=0; Count<CSV_Stream_Other->ItemIndex; Count++)
        {
            for (int I1=0; I1<Pos_End-Pos_Start; I1++)
            if (MediaInfo_Complete || ToExport->Get(0, Stream_Other, 0, I1, Info_Options)[InfoOption_ShowInInform]==__T('Y'))
                {
                    CSV(0, CSV_Pos)=Ztring(__T("Chapters "))+Ztring::ToZtring(Count)+__T(" ")+Parameters(Pos_Start+I1, 0);
                    for (int FilePos=0; FilePos<ToExport->Count_Get(); FilePos++)
                        CSV(1+FilePos, CSV_Pos)=ToExport->Get(FilePos, Stream_Other, Count, I1);
                    CSV_Pos++;
                }
        }

        //Separators
        Ztring Separator_Col=ZEN_UNICODE(CSV_Separator_Col->Text);
        if (Separator_Col==__T("(Tab)"))
            Separator_Col=__T("\t");
        Ztring Separator_Line=ZEN_UNICODE(CSV_Separator_Line->Text);
        if (Separator_Line==__T("(Default)"))
        #ifdef WIN32
            Separator_Line=__T("\r\n");
        #else
            #error
        #endif //WIN32
        Separator_Line.FindAndReplace(__T("\\r"), __T("\r"));
        Separator_Line.FindAndReplace(__T("\\n"), __T("\n"));
        Append_Separator=Separator_Line.c_str();
        Ztring Quote=ZEN_UNICODE(CSV_Quote->Text);
        CSV.Separator_Set(0, Separator_Line);
        CSV.Separator_Set(1, Separator_Col);
        CSV.Quote_Set(Quote);

        if (File_Append->Checked)
            CSV.Delete(0);
        Text=CSV.Read().c_str();
    }
    else if (Export->ActivePage==Export_Sheet)
    {
        ZtringListListF SheetF;
        //Configure
        for (size_t Pos=0; Pos<Prefs->Details[Prefs_Sheet].size(); Pos++)
        {
            Ztring Z1=__T("Column"); Z1+=Ztring::ToZtring(Pos);
            //Searching kind of stream
            stream_t S;
            ZenLib::Char C=__T('G');
            if (Prefs->Details[Prefs_Sheet].Find(Z1)==(size_t)-1)
                break;
            C=Prefs->Details[Prefs_Sheet](Z1, 1)[0];
            switch (C)
            {
              case __T('G'): S=Stream_General; break;
              case __T('V'): S=Stream_Video; break;
              case __T('A'): S=Stream_Audio; break;
              case __T('T'): S=Stream_Text; break;
              case __T('C'): S=Stream_Other; break;
              default: S=Stream_General;
            }
            SheetF(0, Pos)=ToExport->Get(0, S, Prefs->Details[Prefs_Sheet](Z1, 2).To_int32u(), Prefs->Details[Prefs_Sheet](Z1, 3), Info_Name_Text);
            if (C!=__T('G'))
                SheetF(0, Pos)=Prefs->Details[Prefs_Sheet](Z1, 1)+Prefs->Details[Prefs_Sheet](Z1, 2)+__T(" ")+SheetF(0, Pos);
        }
        //Show all available files
        for (int FilePos=0; FilePos<ToExport->Count_Get(); FilePos++)
            for (size_t Pos=0; Pos<Prefs->Details[Prefs_Sheet].size(); Pos++)
            {
                Ztring Z1=__T("Column"); Z1+=Ztring::ToZtring(Pos);
                //Searching Stream kind
                stream_t S;
                ZenLib::Char C=__T('G');
                if (Prefs->Details[Prefs_Sheet].Find(Z1)==(size_t)-1)
                    break;
                C=Prefs->Details[Prefs_Sheet](Z1, 1)[0];
                switch (C)
                {
                  case __T('G'): S=Stream_General; break;
                  case __T('V'): S=Stream_Video; break;
                  case __T('A'): S=Stream_Audio; break;
                  case __T('T'): S=Stream_Text; break;
                  case __T('C'): S=Stream_Other; break;
                  default: S=Stream_General;
                }
                //Showing
                SheetF(1+FilePos, Pos)=ToExport->Get(FilePos, S, Prefs->Details[Prefs_Sheet](Z1, 2).To_int32u(), Prefs->Details[Prefs_Sheet](Z1, 3));
            }

        //Separators
        Ztring Separator_Col=ZEN_UNICODE(Sheet_Separator_Col->Text);
        if (Separator_Col==__T("(Tab)"))
            Separator_Col=__T("\t");
        Ztring Separator_Line=ZEN_UNICODE(Sheet_Separator_Line->Text);
        if (Separator_Line==__T("(Default)"))
        #ifdef WIN32
            Separator_Line=__T("\r\n");
        #else
            #error
        #endif //WIN32
        if (Separator_Line==__T("\\r\\n"))
            Separator_Line=__T("\r\n");
        if (Separator_Line==__T("\\r"))
            Separator_Line=__T("\r");
        if (Separator_Line==__T("\\n"))
            Separator_Line=__T("\n");
        Ztring Quote=ZEN_UNICODE(Sheet_Quote->Text);
        Append_Separator=Separator_Line.c_str();
        SheetF.Separator_Set(0, Separator_Line);
        SheetF.Separator_Set(1, Separator_Col);
        SheetF.Quote_Set(Quote);

        if (File_Append->Checked)
            SheetF.Delete(0);
        Text=SheetF.Read().c_str();
    }
    else if (Export->ActivePage==Export_Text)
    {
        ToExport->Option_Static(__T("Inform"));
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_HTML)
    {
        ToExport->Option_Static(__T("Inform"), __T("HTML"));
        Text=ToExport->Inform().c_str();
        Append_Separator=__T("<hr>\r\n");
    }
    else if (Export->ActivePage==Export_XML)
    {
        ToExport->Option_Static(__T("Inform"), __T("XML"));
        if (Export_XML_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".mediainfo.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_MPEG7)
    {
        ToExport->Option_Static(__T("Inform"), __T("MPEG-7"));
        if (Export_MPEG7_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".mpeg7.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_PBCore)
    {
        ToExport->Option_Static(__T("Inform"), __T("PBCore_1.2"));
        if (Export_PBCore_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".PBCore.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_PBCore2)
    {
        ToExport->Option_Static(__T("Inform"), __T("PBCore_2.0"));
        if (Export_PBCore2_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".PBCore2.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_EBUCore_1_5)
    {
        ToExport->Option_Static(__T("Inform"), __T("EBUCore_1.5"));
        if (Export_EBUCore_1_5_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".EBUCore.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_EBUCore_1_6)
    {
        ToExport->Option_Static(__T("Inform"), __T("EBUCore_1.6"));
        if (Export_EBUCore_1_6_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".EBUCore.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_EBUCore_1_8_ps)
    {
        ToExport->Option_Static(__T("Inform"), __T("EBUCore_1.8_parameterSegment"));
		if (Export_EBUCore_1_8_ps_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".EBUCore.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_EBUCore_1_8_sp)
    {
        ToExport->Option_Static(__T("Inform"), __T("EBUCore_1.8_segmentParameter"));
        if (Export_EBUCore_1_8_sp_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".EBUCore.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_FIMS_1_1)
    {
        ToExport->Option_Static(__T("Inform"), __T("FIMS_1.1"));
        if (Export_FIMS_1_1_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".FIMS.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_FIMS_1_2)
    {
        ToExport->Option_Static(__T("Inform"), __T("FIMS_1.2"));
        if (Export_FIMS_1_2_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".FIMS.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_FIMS_1_3)
    {
        ToExport->Option_Static(__T("Inform"), __T("FIMS_1.3"));
        if (Export_FIMS_1_3_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".FIMS.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_reVTMD)
    {
        ToExport->Option_Static(__T("Inform"), __T("reVTMD"));
        if (Export_reVTMD_SideCar->Checked)
        {
            for (size_t Pos=0; Pos<ToExport->Count_Get(); Pos++)
            {
                Text=ToExport->Inform(Pos).c_str();
                File F;
                F.Create(Ztring(ToExport->Get(Pos, Stream_General, 0, __T("CompleteName")).c_str())+__T(".reVTMD.xml"));
                F.Write(Text);
            }
            return;
        }
        Text=ToExport->Inform().c_str();
    }
    else if (Export->ActivePage==Export_Custom)
    {
        ToExport->Option_Static(__T("Inform"), Prefs->Details[Prefs_Custom].Read());
        if (Custom_One->State==cbChecked)
        {
            for (int FilePos=0; FilePos<ToExport->Count_Get(); FilePos++)
            {
                Ztring Z1=ToExport->Inform(FilePos).c_str();
                //Put begin and end of file
                Z1=Prefs->Details[Prefs_Custom](Stream_Max+2, 1)+Z1; //Begin
                Z1+=Prefs->Details[Prefs_Custom](Stream_Max+4, 1); //End
                Z1.FindAndReplace(__T("\\r\\n"),__T( "\r\n"), 0, Ztring_Recursive);
                Text=Z1.c_str();;//Write file
                File F;
                FileName FN=ZEN_UNICODE(Name->Text);
                FN.Name_Set(FN.Name_Get()+Ztring::ToZtring(FilePos).c_str());
                F.Open(FN, File::Access_Write);
                F.Write(Text);
            }
            return; //No need to save the file, already done
        }
        else
            Text=ToExport->Inform().c_str();
    }

    //Writing file
    File F;
    if (File_Append->Checked)
    {
        F.Open(ZEN_UNICODE(Name->Text), File::Access_Write_Append);
        F.Write(Append_Separator);
    }
    else
        F.Create(ZEN_UNICODE(Name->Text));
    F.Write(Text);
}
//---------------------------------------------------------------------------

void __fastcall TExportF::ExportChange(TObject *Sender)
{
    if (Export->ActivePage==Export_XML)
    {
        Export_XML_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_XML_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_MPEG7)
    {
        Export_MPEG7_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_MPEG7_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_PBCore)
    {
        Export_PBCore_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_PBCore_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_PBCore2)
    {
        Export_PBCore2_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_PBCore2_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_EBUCore_1_5)
    {
        Export_EBUCore_1_5_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_EBUCore_1_5_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_EBUCore_1_6)
    {
        Export_EBUCore_1_6_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_EBUCore_1_6_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_EBUCore_1_8_ps)
    {
        Export_EBUCore_1_8_ps_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
		Name_Choose->Visible=Export_EBUCore_1_8_ps_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_EBUCore_1_8_sp)
    {
        Export_EBUCore_1_8_sp_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_EBUCore_1_8_sp_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_FIMS_1_1)
    {
        Export_FIMS_1_1_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_FIMS_1_1_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_FIMS_1_2)
    {
        Export_FIMS_1_2_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_FIMS_1_2_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_FIMS_1_3)
    {
        Export_FIMS_1_3_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_FIMS_1_3_SideCar->Checked?false:true;;
    }
    else if (Export->ActivePage==Export_reVTMD)
    {
        Export_reVTMD_SideCarClick(Sender);
        File_Append->Checked=false;
        File_Append->Visible=false;
        Name_Choose->Visible=Export_reVTMD_SideCar->Checked?false:true;;
    }
    else
    {
        File_Append->Visible=true;
        Name_Choose->Visible=true;
    }

    Name_Adapt();
}
//---------------------------------------------------------------------------

void __fastcall TExportF::OKClick(TObject *Sender)
{
    Export_Run();
}
//---------------------------------------------------------------------------

void TExportF::CSV_Stream_Change (TComboBox* Box, TLabel* Label, stream_t Stream)
{
    //Show warning if needed
    bool Warning=false;
    for (int Pos=0; Pos<ToExport->Count_Get(); Pos++)
        if (ToExport->Count_Get(Pos, Stream)>Box->ItemIndex)
            Warning=true;
    Label->Visible=Warning;
}

//---------------------------------------------------------------------------

void __fastcall TExportF::CSV_Stream_VideoChange(TObject *Sender)
{
    CSV_Stream_Change(CSV_Stream_Video, CSV_Stream_Video_Warning, Stream_Video);
}
//---------------------------------------------------------------------------

void __fastcall TExportF::CSV_Stream_AudioChange(TObject *Sender)
{
    CSV_Stream_Change(CSV_Stream_Audio, CSV_Stream_Audio_Warning, Stream_Audio);
}
//---------------------------------------------------------------------------

void __fastcall TExportF::CSV_Stream_TextChange(TObject *Sender)
{
    CSV_Stream_Change(CSV_Stream_Text, CSV_Stream_Text_Warning, Stream_Text);
}
//---------------------------------------------------------------------------

void __fastcall TExportF::CSV_Stream_OtherChange(TObject *Sender)
{
    CSV_Stream_Change(CSV_Stream_Other, CSV_Stream_Other_Warning, Stream_Other);
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_XML_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_XML_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_MPEG7_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_MPEG7_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_PBCore_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_PBCore_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_PBCore2_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_PBCore2_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_EBUCore_1_5_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_EBUCore_1_5_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_EBUCore_1_6_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_EBUCore_1_6_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_EBUCore_1_8_ps_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_EBUCore_1_8_ps_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_EBUCore_1_8_sp_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_EBUCore_1_8_sp_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_FIMS_1_1_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_FIMS_1_1_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_FIMS_1_2_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_FIMS_1_2_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_FIMS_1_3_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_FIMS_1_3_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void __fastcall TExportF::Export_reVTMD_SideCarClick(TObject *Sender)
{
    Name_Choose->Visible=Export_reVTMD_SideCar->Checked?false:true;
}
//---------------------------------------------------------------------------

void TExportF::GUI_Configure()
{
    //Translation
    Caption=Prefs->Translate(__T("Export")).c_str();
    Export_Choose->Caption=Prefs->Translate(__T("Choose export format")).c_str();
    CSV_Stream_Video_Caption->Caption=Prefs->Translate(__T("How many video streams?")).c_str();
    CSV_Stream_Video_Warning->Caption=Prefs->Translate(__T("Warning : more streams in the files")).c_str();
    CSV_Stream_Audio_Caption->Caption=Prefs->Translate(__T("How many audio streams?")).c_str();
    CSV_Stream_Audio_Warning->Caption=Prefs->Translate(__T("Warning : more streams in the files")).c_str();
    CSV_Stream_Text_Caption->Caption=Prefs->Translate(__T("How many text streams?")).c_str();
    CSV_Stream_Text_Warning->Caption=Prefs->Translate(__T("Warning : more streams in the files")).c_str();
    CSV_Stream_Other_Caption->Caption=Prefs->Translate(__T("How many chapters streams?")).c_str();
    CSV_Stream_Other_Warning->Caption=Prefs->Translate(__T("Warning : more streams in the files")).c_str();
    CSV_Separator_Col_Caption->Caption=Prefs->Translate(__T("Separator_Columns")).c_str();
    CSV_Decimal_Caption->Caption=Prefs->Translate(__T("Decimal point")).c_str();
    CSV_Separator_Line_Caption->Caption=Prefs->Translate(__T("Separator_Lines")).c_str();
    CSV_Quote_Caption->Caption=Prefs->Translate(__T("Quote character")).c_str();
    CSV_Advanced->Caption=Prefs->Translate(__T("Advanced mode")).c_str();
    Export_Sheet->Caption=Prefs->Translate(__T("Sheet")).c_str();
    Sheet_Separator_Col_Caption->Caption=Prefs->Translate(__T("Separator_Columns")).c_str();
    Sheet_Separator_Line_Caption->Caption=Prefs->Translate(__T("Separator_Lines")).c_str();
    Sheet_Quote_Caption->Caption=Prefs->Translate(__T("Quote character")).c_str();
    Export_Text->Caption=Prefs->Translate(__T("Text")).c_str();
    Text_Advanced->Caption=Prefs->Translate(__T("Advanced mode")).c_str();
    Export_HTML->Caption=Prefs->Translate(__T("HTML")).c_str();
    HTML_Advanced->Caption=Prefs->Translate(__T("Advanced mode")).c_str();
    Export_Custom->Caption=Prefs->Translate(__T("Custom")).c_str();
    Custom_One->Caption=Prefs->Translate(__T("One output file per input file")).c_str();
    Name_Choose->Caption=Prefs->Translate(__T("Choose filename")).c_str();
    File_Append->Caption=Prefs->Translate(__T("File_Append")).c_str();
    OK->Caption=Prefs->Translate(__T("OK")).c_str();
    Cancel->Caption=Prefs->Translate(__T("Cancel")).c_str();

    //Sheet - Warnings
    CSV_Stream_Change(CSV_Stream_Video, CSV_Stream_Video_Warning, Stream_Video);
    CSV_Stream_Change(CSV_Stream_Audio, CSV_Stream_Audio_Warning, Stream_Audio);
    CSV_Stream_Change(CSV_Stream_Text, CSV_Stream_Text_Warning, Stream_Text);
    CSV_Stream_Change(CSV_Stream_Other, CSV_Stream_Other_Warning, Stream_Other);
}

//***************************************************************************
// C++
//***************************************************************************

#endif //MEDIAINFOGUI_EXPORT_NO

