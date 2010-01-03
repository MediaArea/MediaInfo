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
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "CommandLine_Parser.h"
#include "Help.h"
//---------------------------------------------------------------------------

//Parse Command Line
#define LAUNCH(_METHOD) \
    { \
        int Return=_METHOD(MI, Argument); \
        if (Return<0) \
            return Return; \
    } \

#define OPTION(_TEXT, _TOLAUNCH) \
    else if (Argument.find(_T(_TEXT))==0)        LAUNCH(_TOLAUNCH) \

#define OPTION2(_TEXT, _TOLAUNCH) \
    else if (Argument.find(_T(_TEXT))==0)        _TOLAUNCH(); \


//***************************************************************************
// Defaults
//***************************************************************************

ZenLib::Ztring LogFile_FileName;

//***************************************************************************
// Main
//***************************************************************************

int Parse(Core &MI, MediaInfoNameSpace::String &Argument)
{
    if (0);
    OPTION("--full",                                        Full)
    OPTION("-f",                                            Full)
    OPTION("--help-",                                       Help_xxx)
    OPTION("--help",                                        Help)
    OPTION("-h",                                            Help)
    OPTION("--info-parameters",                             Info_Parameters)
    OPTION("--inform=",                                     Inform)
    OPTION("--language",                                    Language)
    OPTION("--output=html",                                 Output_HTML)
    OPTION("--output=HTML",                                 Output_HTML)
    OPTION("--output=xml",                                  Output_XML)
    OPTION("--output=XML",                                  Output_XML)
    OPTION("--logfile",                                     LogFile)
    OPTION("--version",                                     Version)
    //Obsolete
    OPTION("-lang=raw",                                     Language)
    //Default
    OPTION("--",                                            Default)
    else
        return 1;

    return 0;
}

//---------------------------------------------------------------------------
CL_OPTION(Full)
{
    MI.Menu_Debug_Complete(1);

    return 0;
}

//---------------------------------------------------------------------------
CL_OPTION(Help)
{
    MI.Menu_Help_Version();

    TEXTOUT("MediaInfo Command line, ");
    STRINGOUT(MI.Text_Get());
    return Help();
}

//---------------------------------------------------------------------------
CL_OPTION(Help_xxx)
{
    if (0);
    OPTION2("--help-inform",                                Help_Inform)
    else
        TEXTOUT("No help available yet");

    return -1;
}

//---------------------------------------------------------------------------
CL_OPTION(Info_Parameters)
{
    MI.Menu_Help_Info_Parameters();

    STRINGOUT(MI.Text_Get());

    return -1;
}

//---------------------------------------------------------------------------
CL_OPTION(Inform)
{
    //Form : --Inform=Text
    size_t Egal_Pos=Argument.find(_T('='));
    if (Egal_Pos==String::npos)
        return Help_Inform();

    MI.Menu_Option_Preferences_Inform(Argument.substr(Egal_Pos+1));

    return 0;
}

//---------------------------------------------------------------------------
CL_OPTION(Language)
{
    size_t Egal_Pos=Argument.find(_T('='));
    if (Egal_Pos!=String::npos)
        MI.Menu_Language(Argument.substr(Egal_Pos+1));

    return 0;
}

//---------------------------------------------------------------------------
CL_OPTION(Output_HTML)
{
    MI.Menu_View_HTML();

    return 0;
}

//---------------------------------------------------------------------------
CL_OPTION(Output_XML)
{
    MI.Menu_View_XML();
    return 0;
}

//---------------------------------------------------------------------------
CL_OPTION(Version)
{
    MI.Menu_Help_Version();

    TEXTOUT("MediaInfo Command line, ");
    STRINGOUT(MI.Text_Get());

    return -1;
}

//---------------------------------------------------------------------------
CL_OPTION(LogFile)
{
    //Form : --LogFile=Text
    LogFile_FileName.assign(Argument, 10, std::string::npos);

    return 0;
}

//---------------------------------------------------------------------------
CL_OPTION(Default)
{
    //Form : --Option=Text
    size_t Egal_Pos=Argument.find(_T('='));
    if (Egal_Pos<2)
        return 0;
    MediaInfoNameSpace::String Option(Argument, 2, Egal_Pos-2);
    MediaInfoNameSpace::String Value;
    if (Egal_Pos!=std::string::npos)
        Value.assign(Argument, Egal_Pos+1, std::string::npos);
    else
        Value=_T('1');

    MI.Menu_Option_Preferences_Option(Option, Value);

    return 0;
}

void LogFile_Action(ZenLib::Ztring Inform)
{
    if (LogFile_FileName.empty())
        return;

    std::string Inform_Ansi=Inform.To_UTF8();
    std::fstream File(LogFile_FileName.To_Local().c_str(), std::ios_base::out|std::ios_base::binary|std::ios_base::trunc);
    File.write(Inform_Ansi.c_str(), Inform_Ansi.size());
}

