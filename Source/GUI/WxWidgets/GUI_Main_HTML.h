// GUI_Main_HTML - WxWidgets GUI for MediaInfo
// Copyright (C) 2002-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// WxWidgets GUI for MediaInfo, HTML Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_HTMLH
#define GUI_Main_HTMLH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <wx/html/htmlwin.h>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_HTML
//***************************************************************************

class GUI_Main_HTML : public wxHtmlWindow, public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_HTML(Core* C, wxWindow *parent);
    ~GUI_Main_HTML();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();
};

#endif
