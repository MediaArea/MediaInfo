// Reader_Directory - All info about media files
// Copyright (C) 2002-2010 MediaArea.net SARL, Info@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include "MediaInfo/Setup.h"
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_DIRECTORY_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Reader/Reader_Directory.h"
#if defined(MEDIAINFO_BDMV_YES)
    #include "MediaInfo/Multiple/File_Bdmv.h"
#endif
#include "ZenLib/Dir.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

size_t Reader_Directory::Format_Test(MediaInfo_Internal* MI, const String &File_Name)
{
    #ifdef MEDIAINFO_BDMV_YES
        if (Dir::Exists(File_Name) && File_Name.find(Ztring(1, PathSeparator)+_T("BDMV"))+5==File_Name.size())
            return Bdmv_Format_Test(MI, File_Name);
    #endif //MEDIAINFO_BDMV_YES

    return 0;
}

//---------------------------------------------------------------------------
void Reader_Directory::Directory_Cleanup(ZtringList &List)
{
    #ifdef MEDIAINFO_BDMV_YES
        Bdmv_Directory_Cleanup(List);
    #endif //MEDIAINFO_BDMV_YES
}

//***************************************************************************
// Blu-ray stuff
//***************************************************************************

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_BDMV_YES
int Reader_Directory::Bdmv_Format_Test(MediaInfo_Internal* MI, const String &File_Name)
{
    if (!MI->SelectFromExtension(_T("Bdmv")))
        return 0;

    MI->Open_Buffer_Init(0, File_Name);

    MI->Open_Buffer_Continue(NULL, 0);

    MI->Open_Buffer_Finalize();

    return 1;
}
#endif //MEDIAINFO_BDMV_YES

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_BDMV_YES
void Reader_Directory::Bdmv_Directory_Cleanup(ZtringList &List)
{
    //if there is a BDMV folder, this is blu-ray
    Ztring ToSearch=Ztring(1, PathSeparator)+_T("BDMV")+PathSeparator+_T("index.bdmv"); //"\BDMV\index.bdmv"
    for (size_t File_Pos=0; File_Pos<List.size(); File_Pos++)
    {
        size_t BDMV_Pos=List[File_Pos].find(ToSearch);
        if (BDMV_Pos!=string::npos && BDMV_Pos!=0 && BDMV_Pos+16==List[File_Pos].size())
        {
            //This is a BDMV index, parsing the directory only if index and movie objects are BOTH present
            ToSearch=List[File_Pos];
            ToSearch.resize(ToSearch.size()-10);
            ToSearch+=_T("MovieObject.bdmv");  //"%CompletePath%\BDMV\MovieObject.bdmv"
            if (List.Find(ToSearch)!=string::npos)
            {
                //We want the folder instead of the files
                List[File_Pos].resize(List[File_Pos].size()-11); //only %CompletePath%\BDMV
                ToSearch=List[File_Pos];

                for (size_t Pos=0; Pos<List.size(); Pos++)
                {
                    if (List[Pos].find(ToSearch)==0 && List[Pos]!=ToSearch) //Remove all subdirs of ToSearch but not ToSearch
                    {
                        //Removing the file in the blu-ray directory
                        List.erase(List.begin()+Pos);
                        Pos--;
                    }
                }
            }
        }
    }
}
#endif //MEDIAINFO_BDMV_YES

} //NameSpace

#endif //MEDIAINFO_DIRECTORY_YES

