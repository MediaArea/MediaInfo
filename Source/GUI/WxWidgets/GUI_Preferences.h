/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// WxWidgets GUI for MediaInfo, Preferences Dialog
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_PreferencesH
#define GUI_PreferencesH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "wx/preferences.h"
#include <wx/panel.h>
#include <vector>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class wxChoice;
class wxComboBox;
class wxStaticText;
class GUI_Main;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Preference_Panel_General
//***************************************************************************

class GUI_Preference_Panel_General : public wxPanel
{
public:
    GUI_Preference_Panel_General(wxWindow* Parent, GUI_Main* Main); 
    virtual bool TransferDataToWindow() override;
    virtual bool TransferDataFromWindow() override;

private:
    void UpdateSettingsIfNecessary();
    void ChangeDefaultView(wxCommandEvent&);
    void ChangeTextSize(wxCommandEvent&);

    GUI_Main* Main;
    wxStaticText* ViewsLabel;
    wxComboBox* ViewsComboBox;
    wxStaticText* SizesLabel;
    wxComboBox* SizesComboBox;
};

//***************************************************************************
// GUI_Preferences_Page_General
//***************************************************************************

class GUI_Preferences_Page_General : public wxStockPreferencesPage
{
public:
    GUI_Preferences_Page_General(GUI_Main* Main);
    virtual wxWindow *CreateWindow(wxWindow* Parent) override;

private:
    GUI_Main* Main;
};

#endif
