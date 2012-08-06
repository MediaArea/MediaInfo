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

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Easy_Core.h"
#include "Common/Core.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Easy_Core::GUI_Main_Easy_Core(Core* _C)
    : GUI_Main_Common_Core(_C)
{
}

//***************************************************************************
// Actions
//***************************************************************************

//---------------------------------------------------------------------------
size_t GUI_Main_Easy_Core::Boxes_Count_Get(size_t StreamKind)
{
    switch (StreamKind)
    {
        case 0 : return 1;
        case 1 : return 1;
        case 2 : return 2;
        case 3 : return 3;
        default: return 0;
    }
}

