// MediaInfo_CLI - A Command Line Interface for MediaInfoLib
// Copyright (C) 2002-2009 Jerome Martinez, Zen@MediaArea.net
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
#include <iostream>
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
using namespace std;
using namespace MediaInfoLib;
//---------------------------------------------------------------------------

//***************************************************************************
// Main
//***************************************************************************

int main(int argc, char* argv_ansi[])
{
    //Localisation
    setlocale(LC_ALL, """""");

    //Configure MediaInfo core
    Core MI;
    MI.Menu_View_Text(); //Default to text with CLI.

    //Retrieve command line (mainly for Unicode) and parse it
    GETCOMMANDLINE();
    vector<String> List;
    for (int Pos=1; Pos<argc; Pos++)
    {
        //First part of argument (before "=") should be case insensitive
        String Argument(argv[Pos]);
        size_t Egal_Pos=Argument.find(_T('='));
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

    //Parse files
    MI.Menu_File_Open_Files_Begin();
    size_t Files_Count=0;
    for (size_t Pos=0; Pos<List.size(); Pos++)
        Files_Count+=MI.Menu_File_Open_Files_Continue(List[Pos]);

    //Output
    STRINGOUT(MI.Inform_Get());

    //Output, in a file if needed
    LogFile_Action(MI.Inform_Get());
    
    return Files_Count;
}
//---------------------------------------------------------------------------

