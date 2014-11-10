/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

// ----------------------------------------------------------------------------
#include "PreRelease/WxWidgets/App.h"
#include "PreRelease/WxWidgets/GUI_Main.h"

// ----------------------------------------------------------------------------
bool App::OnInit()
{
    GUI_Main *frame = new GUI_Main(wxPoint(50, 50), wxSize(450, 340));
    frame->Show(TRUE);
    return TRUE;
}
