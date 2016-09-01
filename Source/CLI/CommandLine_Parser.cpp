/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <string>
#include <vector>
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
        if (Return==MI_STOP||Return==MI_ERROR) \
            return Return; \
    } \

#define OPTION(_TEXT, _TOLAUNCH) \
    else if (Argument.find(__T(_TEXT))==0)        LAUNCH(_TOLAUNCH) \

#define OPTION2(_TEXT, _TOLAUNCH) \
    else if (Argument.find(__T(_TEXT))==0)        _TOLAUNCH(); \


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
    OPTION("--language",                                    Language)
    OPTION("--output=",                                     Output)
    OPTION("--logfile",                                     LogFile)
    OPTION("--bom",                                         Bom)
    OPTION("--version",                                     Version)
    //Obsolete
    OPTION("-lang=raw",                                     Language)
    //Default
    OPTION("--",                                            Default)
    else
        return MI_ADD;

    return MI_OK;
}

//---------------------------------------------------------------------------
CL_OPTION(Full)
{
    MI.Menu_Debug_Complete(1);

    return MI_OK;
}

//---------------------------------------------------------------------------
CL_OPTION(Help)
{
    MI.Menu_Help_Version();

    TEXTOUT("MediaInfo Command line, ");
    STRINGOUT(MI.Text_Get());
    Help();
    return MI_STOP;
}

//---------------------------------------------------------------------------
CL_OPTION(Help_xxx)
{
    if (0);
    OPTION2("--help-output",                                Help_Output)
    OPTION2("--help-inform",                                Help_Output)
    else
        TEXTOUT("No help available yet");

    return MI_STOP;
}

//---------------------------------------------------------------------------
CL_OPTION(Info_Parameters)
{
    MI.Menu_Help_Info_Parameters();

    STRINGOUT(MI.Text_Get());

    return MI_STOP;
}

//---------------------------------------------------------------------------
CL_OPTION(Inform)
{
    //Form : --Inform=Text
    size_t Egal_Pos=Argument.find(__T('='));
    if (Egal_Pos==String::npos)
        return Help_Output();

    MI.Menu_Option_Preferences_Inform(Argument.substr(Egal_Pos+1));

    return MI_OK;
}

//---------------------------------------------------------------------------
CL_OPTION(Language)
{
    size_t Egal_Pos=Argument.find(__T('='));
    if (Egal_Pos!=String::npos)
        MI.Menu_Language(Argument.substr(Egal_Pos+1));

    return MI_OK;
}

//---------------------------------------------------------------------------
CL_OPTION(Output)
{
    //Form : --Inform=Text
    size_t Egal_Pos=Argument.find(__T('='));
    if (Egal_Pos==String::npos)
        return Help_Output();

    MI.Menu_Option_Preferences_Inform(Argument.substr(Egal_Pos+1));

    return MI_OK;
}

//---------------------------------------------------------------------------
#if defined(_MSC_VER) && defined(UNICODE)
    bool CLI_Option_Bom;
#endif //defined(_MSC_VER) && defined(UNICODE)
CL_OPTION(Bom)
{
    #if defined(_MSC_VER) && defined(UNICODE)
        fwprintf(stdout, L"\uFEFF");
        fwprintf(stderr, L"\uFEFF");
        CLI_Option_Bom=true;
    #endif //defined(_MSC_VER) && defined(UNICODE)

    return MI_OK;
}

//---------------------------------------------------------------------------
CL_OPTION(Version)
{
    MI.Menu_Help_Version();

    TEXTOUT("MediaInfo Command line, ");
    STRINGOUT(MI.Text_Get());

    return MI_STOP;
}

//---------------------------------------------------------------------------
CL_OPTION(LogFile)
{
    //Form : --LogFile=Text
    LogFile_FileName.assign(Argument, 10, std::string::npos);

    return MI_OK;
}

//---------------------------------------------------------------------------
CL_OPTION(Default)
{
    //Form : --Option=Text
    size_t Egal_Pos=Argument.find(__T('='));
    if (Egal_Pos<2)
        return MI_OK;
    MediaInfoNameSpace::String Option(Argument, 2, Egal_Pos-2);
    MediaInfoNameSpace::String Value;
    if (Egal_Pos!=std::string::npos)
        Value.assign(Argument, Egal_Pos+1, std::string::npos);
    else
        Value=__T('1');

    String Result=MI.Menu_Option_Preferences_Option(Option, Value);
    if (!Result.empty())
    {
        STRINGOUT(Result);
        return MI_ERROR;
    }

    return MI_OK;
}

void LogFile_Action(ZenLib::Ztring Inform)
{
    if (LogFile_FileName.empty())
        return;

    std::string Inform_Ansi=Inform.To_UTF8();
    std::fstream File(LogFile_FileName.To_Local().c_str(), std::ios_base::out|std::ios_base::trunc);
    #if defined(_MSC_VER) && defined(UNICODE)
        if (CLI_Option_Bom)
            File.write("\xEF\xBB\xBF", 3);
    #endif //defined(_MSC_VER) && defined(UNICODE)
    File.write(Inform_Ansi.c_str(), Inform_Ansi.size());
}
void CallBack_Set(Core &MI, void* Event_CallBackFunction)
{
    //CallBack configuration
    MI.Menu_Option_Preferences_Option(__T("Event_CallBackFunction"), __T("CallBack=memory://")+ZenLib::Ztring::ToZtring((size_t)Event_CallBackFunction));
}

