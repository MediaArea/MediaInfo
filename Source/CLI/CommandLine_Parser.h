// MediaInfo_CLI - A Command Line Interface for MediaInfoLib
// Copyright (C) 2002-2011 MediaArea.net SARL, Info@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
CL_OPTION(Inform);
CL_OPTION(Language);
CL_OPTION(Output_HTML);
CL_OPTION(Output_XML);
CL_OPTION(LogFile);
CL_OPTION(Version);
CL_OPTION(Default);

//***************************************************************************
// Options which need actions
//***************************************************************************

void LogFile_Action(ZenLib::Ztring Inform);

#endif
