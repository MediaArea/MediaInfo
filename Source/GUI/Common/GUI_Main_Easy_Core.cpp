/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include "GUI/Common/GUI_Main_Easy_Core.h"
#include "Common/Core.h"
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
GUI_Main_Easy_Core::GUI_Main_Easy_Core(Core* Core_)
    : GUI_Main_Common_Core(Core_)
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

