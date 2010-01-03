// GUI_Main_HTML - WxWidgets GUI for MediaInfo
// Copyright (C) 2007-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
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
