// GUI_Main_Text - VCL GUI for MediaInfo
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
// Borland VCL GUI for MediaInfo, Text Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Text_H
#define GUI_Main_Text_H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Classes.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Text
//***************************************************************************

class GUI_Main_Text : public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Text(Core* C, TWinControl* Owner);
    ~GUI_Main_Text();

    //Actions
    void GUI_Refresh();
    void GUI_Resize();

private:
    TMemo* VCL;
};

#endif
