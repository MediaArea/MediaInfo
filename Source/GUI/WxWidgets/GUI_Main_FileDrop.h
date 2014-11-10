/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
