/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//---------------------------------------------------------------------------
#include <vector>
#include <algorithm>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "Common/Core.h"
#include "CommandLine_Parser.h"
#include "Help.h"
#if defined(_MSC_VER) && defined(UNICODE)
    #include "io.h"
    #include "fcntl.h"
#endif
using namespace std;
using namespace MediaInfoNameSpace;
#include "MediaInfo/MediaInfo_Events.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(_MSC_VER) && defined(UNICODE)
    extern bool CLI_Option_Bom;
#endif //defined(_MSC_VER) && defined(UNICODE)
//---------------------------------------------------------------------------

//****************************************************************************
// Event to manage
//****************************************************************************

void Log_0 (struct MediaInfo_Event_Log_0* Event, struct UserHandle_struct* UserHandler)
{
    String MessageString;

    if (Event->Type>=0xC0)
        MessageString+=__T("E: ");

    #if defined(UNICODE) || defined (_UNICODE)
        MessageString+=Event->MessageStringU;
    #else //defined(UNICODE) || defined (_UNICODE)
        MessageString+=Event->MessageStringA;
    #endif //defined(UNICODE) || defined (_UNICODE)

    //Special cases
    switch (Event->MessageCode)
    {
        case 0xF1010101 : MessageString+=__T(" If you want to use such protocols, compile libcurl with SSL/SSH support"); break;
        case 0xF1010102 :
        case 0xF1010103 : MessageString+=__T(" If you are in a secure environment, do \"ssh %YourServerName%\" in order to add the fingerprint to the known_hosts file. If you want to ignore security issues, use --Ssh_IgnoreSecurity option"); break;
        case 0xF1010104 : MessageString+=__T(" If you want to ignore security issues, use --Ssl_IgnoreSecurity option."); break;
        default         : ;
    }

    if (Event->Type>=0x80)
        STRINGERR(MessageString);
    else
        STRINGOUT(MessageString);
}

//****************************************************************************
// The callback function
//****************************************************************************

void __stdcall Event_CallBackFunction(unsigned char* Data_Content, size_t Data_Size, void* UserHandler_Void)
{
    //*integrity tests
    if (Data_Size<4)
        return; //There is a problem

    //*Retrieving UserHandler
    struct UserHandle_struct*           UserHandler=(struct UserHandle_struct*)UserHandler_Void;
    struct MediaInfo_Event_Generic*     Event_Generic=(struct MediaInfo_Event_Generic*) Data_Content;


    //*Retrieving EventID
    //MediaInfo_int8u     ParserID    =(MediaInfo_int8u) ((Event_Generic->EventCode&0xFF000000)>>24);
    MediaInfo_int16u    EventID     =(MediaInfo_int16u)((Event_Generic->EventCode&0x00FFFF00)>>8 );
    MediaInfo_int8u     EventVersion=(MediaInfo_int8u) ( Event_Generic->EventCode&0x000000FF     );

    //*Global to all parsers
    switch (EventID)
    {
        case MediaInfo_Event_Log                                                    : if (EventVersion==0 && Data_Size>=sizeof(struct MediaInfo_Event_Log_0)) Log_0((struct MediaInfo_Event_Log_0*)Data_Content, UserHandler); break;
        default                                                                     : ;
    }
}

//***************************************************************************
// Main
//***************************************************************************

int main(int argc, char* argv_ansi[])
{
    //Localisation
    setlocale(LC_ALL, "");
    MediaInfo::Option_Static(__T("CharSet"), __T(""));

    //Initialize terminal (to fix Unicode output on Win32)
    #if defined(_MSC_VER) && defined(UNICODE)
        _setmode(_fileno(stdout), _O_U8TEXT);
        _setmode(_fileno(stderr), _O_U8TEXT);
        CLI_Option_Bom=false;
    #endif
    MediaInfo::Option_Static(__T("LineSeparator"), __T("\n")); //Using sdtout

    //Configure MediaInfo core
    Core MI;
    MI.Menu_View_Text(); //Default to text with CLI.

    //Retrieve command line (mainly for Unicode)
    GETCOMMANDLINE();

    //Parse command line
    vector<String> List;
    for (int Pos=1; Pos<argc; Pos++)
    {
        //First part of argument (before "=") should be case insensitive
        String Argument(argv[Pos]);
        size_t Egal_Pos=Argument.find(__T('='));
        if (Egal_Pos==string::npos)
            Egal_Pos=Argument.size();
        transform(Argument.begin(), Argument.begin()+Egal_Pos, Argument.begin(), (int(*)(int))tolower); //(int(*)(int)) is a patch for unix
        int Return=Parse (MI, Argument);
        if (Return<0)
            return Return; //no more tasks to do
        if (Return>0)
            List.push_back(argv[Pos]); //Append the filename to the list of filenames to parse
    }

    //If no filenames (and no options)
    if (List.empty())
        return Help_Nothing();

    //Callback for error handling
    CallBack_Set(MI, (void*)Event_CallBackFunction);

    //Parse files
    MI.Menu_File_Open_Files_Begin();
    size_t Files_Count=0;
    for (size_t Pos=0; Pos<List.size(); Pos++)
        Files_Count+=MI.Menu_File_Open_Files_Continue(List[Pos]);

    //Output
    STRINGOUT(MI.Inform_Get());

    //Output, in a file if needed
    LogFile_Action(MI.Inform_Get());

    return Files_Count?0:1;
}
//---------------------------------------------------------------------------

