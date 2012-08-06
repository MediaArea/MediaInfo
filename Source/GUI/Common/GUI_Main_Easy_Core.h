// GUI_Main_Easy_Core - Core GUI for MediaInfo
// Copyright (C) 2007-2012 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// GUI for MediaInfo, Commin Part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef GUI_Main_Easy_CoreH
#define GUI_Main_Easy_CoreH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Common_Core.h"
//---------------------------------------------------------------------------

//***************************************************************************
// GUI_Main_Easy_Core
//***************************************************************************

class GUI_Main_Easy_Core : public GUI_Main_Common_Core
{
public:
    //Constructor/Destructor
    GUI_Main_Easy_Core(Core* _C);
    virtual ~GUI_Main_Easy_Core() {};

    //Actions
    size_t Boxes_Count_Get(size_t StreamKind);
};

#endif
