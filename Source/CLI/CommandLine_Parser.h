/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#ifndef CommandLine_ParserH
#define CommandLine_ParserH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Core.h"
#include "Config.h"
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************

int Parse(Core &I, MediaInfoNameSpace::String &Argument);


//---------------------------------------------------------------------------
#define CL_METHOD(_NAME) \
    int _NAME(Core &MI, const MediaInfoNameSpace::String &Argument)

#define CL_OPTION(_NAME) \
    int _NAME(Core &MI, const MediaInfoNameSpace::String &Argument)

CL_OPTION(Full);
CL_OPTION(Help);
CL_OPTION(Help_xxx);
CL_OPTION(Info_Parameters);
CL_OPTION(Info_CanHandleUrls);
CL_OPTION(Inform);
CL_OPTION(Language);
CL_OPTION(Output);
CL_OPTION(LogFile);
CL_OPTION(Bom);
CL_OPTION(Version);
CL_OPTION(Default);

//***************************************************************************
// Options which need actions
//***************************************************************************

void LogFile_Action(ZenLib::Ztring Inform);
void CallBack_Set(Core &MI, void* Event_CallBackFunction);

#endif
