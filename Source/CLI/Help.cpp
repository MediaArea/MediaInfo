// MediaInfo_CLI - A Command Line Interface for MediaInfoLib
// Copyright (C) 2002-2008 Jerome Martinez, Zen@MediaArea.net
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Help.h"
#include "Config.h"
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
int Help()
{
    TEXTOUT("Usage: \"MediaInfo [-Options...] FileName1 [Filename2...]\"");
    TEXTOUT("");
    TEXTOUT("Options:");
    TEXTOUT("--Full, -f  Display full information (all internal tags)");
    TEXTOUT("--Info-Parameters  Display available parameters");
    TEXTOUT("--Inform=...  Specify a template");
    TEXTOUT("             (--Help-Inform for more info)");
    TEXTOUT("--Help, -h  Display this help and exit");
    TEXTOUT("--Language=raw  Display Non-translated words (internal text)");
    TEXTOUT("--Output=HTML  Display with HTML tags for parsing by a browser");
    TEXTOUT("--Version  Display version information and exit");
    TEXTOUT("");
    TEXTOUT("--Help-AnOption  Display more details about \"AnOption\"");

    return -1;
}

//---------------------------------------------------------------------------
int Help_Nothing()
{
    TEXTOUT("Usage: \"MediaInfo [-Options...] FileName1 [Filename2...]\"");
    TEXTOUT("\"MediaInfo --Help\" for displaying more information");

    return -1;
}

//---------------------------------------------------------------------------
int Help_Inform()
{
    TEXTOUT("--Inform=...  Specify a template (BETA)");
    TEXTOUT("Usage: \"MediaInfo --Inform=xxx;Text FileName\"");
    TEXTOUT("");
    TEXTOUT("xxx can be: General, Video, Audio, Text, Chapter, Image, Menu");
    TEXTOUT("Text can be the template text, or a filename");
    TEXTOUT("     Filename must be in the form file://filename");
    TEXTOUT("");
    TEXTOUT("See --Info-Parameters for available parameters in the text");
    TEXTOUT("(Parameters must be surrounded by \"%%\" sign)");
    TEXTOUT("");
    TEXTOUT("Example: \"MediaInfo --Inform=Video;%%AspectRatio%% FileName\"");
    TEXTOUT("Example: \"MediaInfo --Inform=Video;file://Video.txt FileName\"");
    TEXTOUT("and Video.txt contains \"%%AspectRatio%%\"");
    TEXTOUT("for having Video Aspect Ratio.");

    return -1;
}
