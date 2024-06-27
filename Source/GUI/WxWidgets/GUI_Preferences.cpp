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
#include "wx/config.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
#include "GUI/WxWidgets/GUI_Preferences.h"
#include "GUI/WxWidgets/GUI_Main.h"

//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Preference_Panel_General
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Preference_Panel_General::GUI_Preference_Panel_General(wxWindow* Parent, GUI_Main* Main) : wxPanel(Parent), Main(Main)
{
    // Init
    wxSizer* Sizer=new wxBoxSizer(wxVERTICAL);

    // Default view
    ViewsLabel=new wxStaticText(this, wxID_ANY, wxT("Default view:"));
    wxString Views[]=
    {
        wxT("Easy"),
        wxT("Text"),
        wxT("HTML"),
        wxT("XML"),
        wxT("JSON"),
        wxT("MPEG-7 (strict)"),
        wxT("MPEG-7 (relaxed)"),
        wxT("MPEG-7 (extended)"),
        wxT("PBCore 1.2"),
        wxT("PBCore 2.0"),
        wxT("EBUCore 1.5"),
        wxT("EBUCore 1.6"),
        wxT("EBUCore 1.8 (acq. metadata: parameter then segment)"),
        wxT("EBUCore 1.8 (acq. metadata: segment then parameter)"),
        wxT("EBUCore 1.8 (acq. metadata: parameter then segment, json output)"),
        wxT("EBUCore 1.8 (acq. metadata: segment then parameter, json output)"),
        wxT("FIMS 1.1 (beta)"),
        wxT("FIMS 1.2 (beta)"),
        wxT("NISO Z39.87"),
        wxT("reVTMD")
    };
    ViewsComboBox=new wxComboBox(this, wxID_ANY, wxT("Select an option"), wxDefaultPosition,
                                 wxDefaultSize, WXSIZEOF(Views), Views, wxCB_READONLY);
    ViewsComboBox->Bind(wxEVT_COMBOBOX, &GUI_Preference_Panel_General::ChangeDefaultView, this);

    // Font size
    SizesLabel=new wxStaticText(this, wxID_ANY, wxT("Font size for text views:"));
    wxString Sizes[]=
    {
        wxT("Extra small"),
        wxT("Very small"),
        wxT("Small"),
        wxT("Normal"),
        wxT("Large"),
        wxT("Very large"),
        wxT("Extra large")
    };
    SizesComboBox=new wxComboBox(this, wxID_ANY, wxT("Select an option"), wxDefaultPosition,
                                 wxDefaultSize, WXSIZEOF(Sizes), Sizes, wxCB_READONLY);
    SizesComboBox->Bind(wxEVT_COMBOBOX, &GUI_Preference_Panel_General::ChangeTextSize, this);

    Sizer->Add(ViewsLabel, 0, wxALL | wxALIGN_LEFT, 20);
    Sizer->Add(ViewsComboBox, 0, wxALL | wxEXPAND, 20);
    Sizer->Add(SizesLabel, 0, wxALL | wxALIGN_LEFT, 20);
    Sizer->Add(SizesComboBox, 0, wxALL | wxEXPAND, 20);
    SetSizerAndFit(Sizer);
}

//---------------------------------------------------------------------------
bool GUI_Preference_Panel_General::TransferDataToWindow()
{
    wxConfigBase* Config=wxConfigBase::Get();

    wxString View=Config->Read(wxT("/View"), wxT("Easy"));
    if (View==wxT("Easy"))
        ViewsComboBox->SetSelection(0);
    else if (View==wxT("Text"))
        ViewsComboBox->SetSelection(1);
    else if (View==wxT("HTML"))
        ViewsComboBox->SetSelection(2);
    else if (View==wxT("XML"))
        ViewsComboBox->SetSelection(3);
    else if (View==wxT("JSON"))
        ViewsComboBox->SetSelection(4);
    else if (View==wxT("MPEG7_Strict"))
        ViewsComboBox->SetSelection(5);
    else if (View==wxT("MPEG7_Relaxed"))
        ViewsComboBox->SetSelection(6);
    else if (View==wxT("MPEG7_Extended"))
        ViewsComboBox->SetSelection(7);
    else if (View==wxT("PBCore_1_2"))
        ViewsComboBox->SetSelection(8);
    else if (View==wxT("PBCore_2_0"))
        ViewsComboBox->SetSelection(9);
    else if (View==wxT("EBUCore_1_5"))
        ViewsComboBox->SetSelection(10);
    else if (View==wxT("EBUCore_1_6"))
        ViewsComboBox->SetSelection(11);
    else if (View==wxT("EBUCore_1_8_ps"))
        ViewsComboBox->SetSelection(12);
    else if (View==wxT("EBUCore_1_8_sp"))
        ViewsComboBox->SetSelection(13);
    else if (View==wxT("EBUCore_1_8_ps_json"))
        ViewsComboBox->SetSelection(14);
    else if (View==wxT("EBUCore_1_8_sp_json"))
        ViewsComboBox->SetSelection(15);
    else if (View==wxT("FIMS_1_1"))
        ViewsComboBox->SetSelection(16);
    else if (View==wxT("FIMS_1_2"))
        ViewsComboBox->SetSelection(17);
    else if (View==wxT("NISO_Z39_87"))
        ViewsComboBox->SetSelection(18);
    else if (View==wxT("reVTMD"))
        ViewsComboBox->SetSelection(19);
    else
        ViewsComboBox->SetSelection(0);

    long TextSize=Config->ReadLong(wxT("/TextSize"), 0);
    if (TextSize+3<SizesComboBox->GetCount())
        SizesComboBox->SetSelection(TextSize+3);
    else
        SizesComboBox->SetSelection(3);

    return true;
}

//---------------------------------------------------------------------------
bool GUI_Preference_Panel_General::TransferDataFromWindow()
{
    wxConfigBase* Config=wxConfigBase::Get();
    if (Config==NULL)
        return false;

    switch (ViewsComboBox->GetSelection())
    {
        case 0: Config->Write(wxT("/View"), wxT("Easy")); break;
        case 1: Config->Write(wxT("/View"), wxT("Text")); break;
        case 2: Config->Write(wxT("/View"), wxT("HTML")); break;
        case 3: Config->Write(wxT("/View"), wxT("XML")); break;
        case 4: Config->Write(wxT("/View"), wxT("JSON")); break;
        case 5: Config->Write(wxT("/View"), wxT("MPEG7_Strict")); break;
        case 6: Config->Write(wxT("/View"), wxT("MPEG7_Relaxed")); break;
        case 7: Config->Write(wxT("/View"), wxT("MPEG7_Extended")); break;
        case 8: Config->Write(wxT("/View"), wxT("PBCore_1_2")); break;
        case 9: Config->Write(wxT("/View"), wxT("PBCore_2_0")); break;
        case 10: Config->Write(wxT("/View"), wxT("EBUCore_1_5")); break;
        case 11: Config->Write(wxT("/View"), wxT("EBUCore_1_6")); break;
        case 12: Config->Write(wxT("/View"), wxT("EBUCore_1_8_ps")); break;
        case 13: Config->Write(wxT("/View"), wxT("EBUCore_1_8_sp")); break;
        case 14: Config->Write(wxT("/View"), wxT("EBUCore_1_8_ps_json")); break;
        case 15: Config->Write(wxT("/View"), wxT("EBUCore_1_8_sp_json")); break;
        case 16: Config->Write(wxT("/View"), wxT("FIMS_1_1")); break;
        case 17: Config->Write(wxT("/View"), wxT("FIMS_1_2")); break;
        case 18: Config->Write(wxT("/View"), wxT("NISO_Z39_87")); break;
        case 19: Config->Write(wxT("/View"), wxT("reVTMD")); break;
        default: Config->Write(wxT("/View"), wxT("Easy")); break;
    }

    Config->Write(wxT("/TextSize"), SizesComboBox->GetSelection()-3);
    Main->View_Refresh();

    return true;
}

//---------------------------------------------------------------------------
void GUI_Preference_Panel_General::UpdateSettingsIfNecessary()
{
    if (wxPreferencesEditor::ShouldApplyChangesImmediately())
        TransferDataFromWindow();
}

//---------------------------------------------------------------------------
void GUI_Preference_Panel_General::ChangeDefaultView(wxCommandEvent&)
{
    UpdateSettingsIfNecessary();
}

//---------------------------------------------------------------------------
void GUI_Preference_Panel_General::ChangeTextSize(wxCommandEvent&)
{
    UpdateSettingsIfNecessary();
}

//***************************************************************************
// GUI_Preference_Panel_Advanced
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Preference_Panel_Advanced::GUI_Preference_Panel_Advanced(wxWindow* Parent, GUI_Main* Main) : wxPanel(Parent), Main(Main)
{
    // Init
    wxSizer* Sizer=new wxBoxSizer(wxVERTICAL);

    // Captions display option
    DisplayCaptionsLabel=new wxStaticText(this, wxID_ANY, wxT("Handling of 608/708 streams:"));
    wxString DisplayCaptionsOptions[]=
    {
        wxT("When content is detected"),
        wxT("When content or a command is detected"),
        wxT("Even when no content or command is detected")
    };
    DisplayCaptionsComboBox=new wxComboBox(this, wxID_ANY, wxT("Select an option"), wxDefaultPosition,
                                 wxDefaultSize, WXSIZEOF(DisplayCaptionsOptions), DisplayCaptionsOptions, wxCB_READONLY);
    DisplayCaptionsComboBox->Bind(wxEVT_COMBOBOX, &GUI_Preference_Panel_Advanced::ChangeCaptionsDisplayOption, this);


    Sizer->Add(DisplayCaptionsLabel, 0, wxALL | wxALIGN_LEFT, 20);
    Sizer->Add(DisplayCaptionsComboBox, 0, wxALL | wxEXPAND, 20);
    SetSizerAndFit(Sizer);
}

//---------------------------------------------------------------------------
bool GUI_Preference_Panel_Advanced::TransferDataToWindow()
{
    wxConfigBase* Config=wxConfigBase::Get();

    wxString Option=Config->Read(wxT("/DisplayCaptions"), wxT("Command"));
    if (Option==wxT("Content"))
        DisplayCaptionsComboBox->SetSelection(0);
    else if (Option==wxT("Command"))
        DisplayCaptionsComboBox->SetSelection(1);
    else if (Option==wxT("Stream"))
        DisplayCaptionsComboBox->SetSelection(2);
    else
        DisplayCaptionsComboBox->SetSelection(1);

    return true;
}

//---------------------------------------------------------------------------
bool GUI_Preference_Panel_Advanced::TransferDataFromWindow()
{
    wxConfigBase* Config=wxConfigBase::Get();
    if (Config==NULL)
        return false;

    switch (DisplayCaptionsComboBox->GetSelection())
    {
        case 0: Config->Write(wxT("/DisplayCaptions"), wxT("Content")); break;
        case 1: Config->Write(wxT("/DisplayCaptions"), wxT("Command")); break;
        case 2: Config->Write(wxT("/DisplayCaptions"), wxT("Stream")); break;
        default: Config->Write(wxT("/DisplayCaptions"), wxT("Command")); break;
    }

    Main->View_Refresh();

    return true;
}

//---------------------------------------------------------------------------
void GUI_Preference_Panel_Advanced::UpdateSettingsIfNecessary()
{
    if (wxPreferencesEditor::ShouldApplyChangesImmediately())
        TransferDataFromWindow();
}

//---------------------------------------------------------------------------
void GUI_Preference_Panel_Advanced::ChangeCaptionsDisplayOption(wxCommandEvent&)
{
    UpdateSettingsIfNecessary();
}

//***************************************************************************
// GUI_Preferences_Page_General
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Preferences_Page_General::GUI_Preferences_Page_General(GUI_Main* Main) : wxStockPreferencesPage(Kind_General), Main(Main)
{
}

//---------------------------------------------------------------------------
wxWindow* GUI_Preferences_Page_General::CreateWindow(wxWindow* Parent)
{
    return new GUI_Preference_Panel_General(Parent, Main);
}

//***************************************************************************
// GUI_Preferences_Page_Advanced
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Preferences_Page_Advanced::GUI_Preferences_Page_Advanced(GUI_Main* Main) : wxStockPreferencesPage(Kind_Advanced), Main(Main)
{
}

//---------------------------------------------------------------------------
wxWindow* GUI_Preferences_Page_Advanced::CreateWindow(wxWindow* Parent)
{
    return new GUI_Preference_Panel_Advanced(Parent, Main);
}
