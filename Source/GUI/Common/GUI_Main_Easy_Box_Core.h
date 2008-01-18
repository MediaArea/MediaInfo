// GUI_Main_Easy_Box_Core - Core GUI for MediaInfo
// Copyright (C) 2007-2008 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// GUI for MediaInfo, Common Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Easy_Box_CoreH
#define GUI_Main_Easy_Box_CoreH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Core.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class GUI_Main_Easy_Core;
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Easy_Box_Core
//***************************************************************************

class GUI_Main_Easy_Box_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Easy_Box_Core(Core* _C, GUI_Main_Easy_Core* Parent_Core, stream_t StreamKind, size_t StreamPos);
    virtual ~GUI_Main_Easy_Box_Core() {};

    //Actions - Per StreamKind
    size_t Boxes_Count_Get ();
    size_t Boxes_Show_Get  ();
    size_t Lines_Count_Get ();

    //Actions - Per Stream
    bool   MustHide    ();
    bool   IsLastBox   ();
    String Box_Get     ();
    String Text_Get    ();
    String ToolTip_Get ();
    String Tags_Get    ();
    String Button_Get  ();
    bool   Button_Show ();
    String Button_Click();

private:
    //Internal
    Core* C;
    GUI_Main_Easy_Core* Parent_Core;

    //Configuration
    stream_t StreamKind;
    size_t   StreamPos;
};

#endif
