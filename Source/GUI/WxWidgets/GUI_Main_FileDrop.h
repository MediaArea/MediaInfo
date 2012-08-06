// GUI_Main_FileDrop - FileDrop function
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
// FileDrop function
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef FileDropH
#define FileDropH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include <wx/dnd.h>
//----------------------------------------------------------------------------

//---------------------------------------------------------------------------
class Core;
class GUI_Main;
//---------------------------------------------------------------------------

//***************************************************************************
// FileDrop
//***************************************************************************

#if wxUSE_DRAG_AND_DROP
class FileDrop : public wxFileDropTarget
{
public:
    FileDrop(Core* C_) {C=C_;};
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);

private:
    Core* C;
};
#else //wxUSE_DRAG_AND_DROP
class FileDrop
{
public:
    FileDrop(Core* C_) {};
    bool OnDropFiles(wxCoord, wxCoord, const wxArrayString&) {return false;};
};
#endif //wxUSE_DRAG_AND_DROP

#endif
