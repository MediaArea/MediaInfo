/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
    GUI_Main_Easy_Core(Core* Core_);
    virtual ~GUI_Main_Easy_Core() {};

    //Actions
    size_t Boxes_Count_Get(size_t StreamKind);
};

#endif
