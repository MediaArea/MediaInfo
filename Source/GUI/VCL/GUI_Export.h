// GUI_Export - Export interface of MediaInfo
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
#ifndef GUI_ExportH
#define GUI_ExportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ComCtrls.hpp>
#include <StdCtrls.hpp>
#include <Dialogs.hpp>
#include "Dialogs.hpp"
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
#include <ZenLib/Ztring.h>
//---------------------------------------------------------------------------
class TExportF : public TForm
{
__published:    // IDE-managed Components
    TButton *Cancel;
    TButton *OK;
    TGroupBox *Export_Choose;
    TPageControl *Export;
    TTabSheet *Export_CSV;
    TTabSheet *Export_Text;
    TTabSheet *Export_HTML;
    TTabSheet *Export_Custom;
    TGroupBox *Name_Choose;
    TEdit *Name;
    TButton *Name_FileSelect;
    TLabel *CSV_Stream_Video_Caption;
    TComboBox *CSV_Stream_Video;
    TLabel *CSV_Stream_Audio_Caption;
    TComboBox *CSV_Stream_Audio;
    TLabel *CSV_Stream_Text_Caption;
    TComboBox *CSV_Stream_Text;
    TLabel *CSV_Stream_Chapters_Caption;
    TComboBox *CSV_Stream_Chapters;
    TLabel *CSV_Stream_Video_Warning;
    TLabel *CSV_Stream_Audio_Warning;
    TLabel *CSV_Stream_Text_Warning;
    TLabel *CSV_Stream_Chapters_Warning;
    TCheckBox *Text_Advanced;
    TCheckBox *HTML_Advanced;
    TLabel *CSV_Quote_Caption;
    TLabel *CSV_Separator_Col_Caption;
    TComboBox *CSV_Quote;
    TComboBox *CSV_Separator_Col;
    TLabel *CSV_Separator_Line_Caption;
    TComboBox *CSV_Separator_Line;
    TCheckBox *CSV_Advanced;
    TCheckBox *Custom_One;
    TTabSheet *Export_Sheet;
    TLabel *Sheet_Quote_Caption;
    TComboBox *Sheet_Quote;
    TLabel *Sheet_Separator_Col_Caption;
    TComboBox *Sheet_Separator_Col;
    TLabel *Sheet_Separator_Line_Caption;
    TComboBox *Sheet_Separator_Line;
    TCheckBox *File_Append;
    TLabel *CSV_Decimal_Caption;
    TComboBox *CSV_Decimal;
    TSaveDialog *SaveDialog1;
    void __fastcall Name_FileSelectClick(TObject *Sender);
    void __fastcall ExportChange(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall CSV_Stream_VideoChange(TObject *Sender);
    void __fastcall CSV_Stream_AudioChange(TObject *Sender);
    void __fastcall CSV_Stream_TextChange(TObject *Sender);
    void __fastcall CSV_Stream_ChaptersChange(TObject *Sender);
private:    // User declarations
    void Name_Adapt();
    void Export_Run();
    void CSV_Stream_Change (TComboBox* Box, TLabel* Label, MediaInfoNameSpace::stream_t Stream);
    void GUI_Configure();
    MediaInfoNameSpace::MediaInfoList *ToExport;
public:        // User declarations
    __fastcall TExportF(TComponent* Owner);
    int Run(MediaInfoNameSpace::MediaInfoList &MI, ZenLib::Ztring DefaultFolder);
};
//---------------------------------------------------------------------------
extern PACKAGE TExportF *ExportF;
//---------------------------------------------------------------------------
#endif
