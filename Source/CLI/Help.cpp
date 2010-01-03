// MediaInfo_CLI - A Command Line Interface for MediaInfoLib
// Copyright (C) 2002-2010 MediaArea.net SARL, Info@MediaArea.net
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
    TEXTOUT("--Help, -h         Display this help and exit");
    TEXTOUT("--Help-Inform      Display help for Inform= option");
    TEXTOUT("--Help-AnOption    Display help for \"AnOption\"");
    TEXTOUT("--Version          Display MediaInfo version and exit");
    TEXTOUT("");
    TEXTOUT("--Full , -f        Full information Display (all internal tags)");
    TEXTOUT("--Output=HTML      Full information Display with HTML tags ");
    TEXTOUT("--Output=XML       Full information Display with XML tags ");
    TEXTOUT("--Inform=...       Template defined information Display");
    TEXTOUT("--Info-Parameters  Display list of Inform= parameters");
    TEXTOUT("");
    TEXTOUT("--Language=raw     Display non-translated unique identifiers (internal text)");
    TEXTOUT("--LogFile=...      Save the output in the specified file");

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
    TEXTOUT("Usage: \"MediaInfo --Inform=[xxx;]Text FileName\"");
    TEXTOUT("");
    TEXTOUT("xxx can be: General, Video, Audio, Text, Chapter, Image, Menu");
    TEXTOUT("Text can be the template text, or a filename");
    TEXTOUT("     Filename must be in the form file://filename");
    TEXTOUT("");
    TEXTOUT("See --Info-Parameters for available parameters in the text");
    TEXTOUT("(Parameters must be surrounded by \"%\" sign)");
    TEXTOUT("");
    TEXTOUT("Example: \"MediaInfo --Inform=Video;%AspectRatio% FileName\"");
    TEXTOUT("");
    TEXTOUT("Example: \"MediaInfo --Inform=Video;file://Video.txt FileName\"");
    TEXTOUT("and Video.txt contains ");
    TEXTOUT("\"%DisplayAspectRatio%\"        for Video Aspect Ratio.");
    TEXTOUT("");
    TEXTOUT("Example: \"MediaInfo --Inform=file://Text.txt FileName\"");
    TEXTOUT("and Text.txt contains");
    TEXTOUT("\"Video;%DisplayAspectRatio%\"  for Video Aspect Ratio.");
    TEXTOUT("\"Audio;%Format%\"              for Audio Format.");

    return -1;
}
