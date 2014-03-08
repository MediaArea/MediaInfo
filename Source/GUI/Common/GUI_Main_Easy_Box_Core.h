/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

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
    GUI_Main_Easy_Box_Core(Core* Core_, GUI_Main_Easy_Core* Parent_Core, stream_t StreamKind, size_t StreamPos);
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
