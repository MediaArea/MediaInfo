//---------------------------------------------------------------------------
#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <wx/filename.h>
#include <wx/datetime.h>
#include <wx/file.h>
#include <ZenLib/Ztring.h>
#include "PreRelease/OldFiles.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

Ztring Version; //X.X.X.X
Ztring Version_Short; //X.X
Ztring ToShow; //What to show to the user
char C[1000000];
wxFile F;
wxFileName FN;
int I;
Ztring Z;
wxDateTime Access;

int Test_Version(char* FileName_, char* Begin, char* End)
{
    wxString FileName=Ztring().From_Local(FileName_).c_str();
    //Opening File
    F.Open(FileName);
    I=F.Read(C, 1000000);
    if (!I)
    {
        ToShow+=__T("Error opening ")+FileName;
        return -1;
    }

    //Getting information
    C[I]=0;
    Z.From_Local(C);
    Z=Z.SubString(Ztring().From_Local(Begin), Ztring().From_Local(End));

    //deleting extra bytes
    if (Z[Z.size()-1]=='\n')
        Z.resize(Z.size()-1);
    if (Z[Z.size()-1]=='\r')
        Z.resize(Z.size()-1);

    //Testing validity
    if (Z.size()!=3 && Z.size()!=7) //non long, no short
    {
        ToShow+=__T("Error reading ")+FileName;
        return -2;
    }

    //Reformtation information
    Z.FindAndReplace(__T(","), __T("."), 0, Ztring_Recursive);

    if (Z!=Version && Z!=Version_Short)
    {
        ToShow+=FileName;
        ToShow+=__T(" is not good : version is marked ");
        ToShow+=Z;
        ToShow+=__T("\r\n");
    }
    return 0;
}
//---------------------------------------------------------------------------

int Test_Date(wxString FileName)
{
    FN.Assign(FileName);
    if (!FN.FileExists())
    {
        ToShow+=FileName;
        ToShow+=__T(" does not exist");
        return -1;
    }
    if (!FN.GetTimes(&Access, NULL, NULL))
    {
        ToShow+=__T("Error getting date of ");
        ToShow+=FileName;
        return -1;
    }
    wxTimeSpan TS=wxDateTime::Now()-Access;
    if (TS.GetWeeks()>0 || TS.GetDays()>0 || TS.GetHours()>0 || TS.GetMinutes()>0)
    {
        ToShow+=FileName;
        ToShow+=__T(" is old : was compiled");
        ToShow+=TS.Format().c_str();
        ToShow+=__T(" ago\r\n");
    }
    return 0;
}
//---------------------------------------------------------------------------

Ztring OldFiles_Test ()
{
    //Checking version in Info_Version
    ToShow+=__T("Version checked : ");
    F.Open(__T("../Source/MediaInfo/MediaInfo_Config.cpp"));
    I=F.Read(C, 1000000);
    if (!I)
    {
        ToShow+=__T("Error opening ../Source/MediaInfo/MediaInfo_Config.cpp");
        return ToShow;
    }
    C[I]=0;
    Z.From_Local(C);
    Version=Z.SubString(__T("MediaInfoLib - v"), __T("\")"));
    if (Version.size()!=7)
    {
        ToShow+=__T("Error reading ../Source/MediaInfo/MediaInfo.cpp : \"MediaInfoLib - vX.X.X.X - \" not found");
        return ToShow;
    }
    Version_Short=Version; Version_Short.resize(3);
    ToShow+=Version+__T("\r\n");

    //Checking version in MediaInfo.h
    if (Test_Version("../Source/MediaInfo/MediaInfo.h", "@version ", "\n")) return ToShow;
    if (Test_Version("../Source/MediaInfo/MediaInfoList.h", "@version ", "\n")) return ToShow;
    if (Test_Version("../Project/MSVC/Dll/MediaInfo.rc", " FILEVERSION ", "\n")) return ToShow;
    if (Test_Version("../Project/MSVC/Dll/MediaInfo.rc", " PRODUCTVERSION ", "\n")) return ToShow;
    if (Test_Version("../Project/MSVC/Dll/MediaInfo.rc", "            VALUE \"FileVersion\", \"", "\"")) return ToShow;
    if (Test_Version("../Project/MSVC/Dll/MediaInfo.rc", "            VALUE \"ProductVersion\", \"", "\"")) return ToShow;
    if (Test_Version("../History.txt", "Version ", " ")) return ToShow;
    if (Test_Date(__T("MSVC/Library/MediaInfo.lib"))) return ToShow;
    if (Test_Date(__T("MSVC/Dll/MediaInfo.dll"))) return ToShow;

    return ToShow;
}
//---------------------------------------------------------------------------
