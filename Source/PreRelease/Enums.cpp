//---------------------------------------------------------------------------
#include <wx/wxprec.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <wx/file.h>
#include <ZenLib/File.h>
#include <ZenLib/ZtringListListF.h>
#include "PreRelease/Enums.h"
using namespace ZenLib;
using namespace std;
#define FILE_MAX 20000
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Open a file
ZenLib::Ztring Enums_Create_Load(Ztring FileName, Ztring &Contents)
{
    wxFile F;
    if (F.Open(FileName.c_str(), wxFile::read)==false)
    {
        Ztring ToReturn=L"Problems to open ";
        ToReturn+=FileName;
        ToReturn+=L"\r\n";
        return ToReturn;
    }

    char C[FILE_MAX];
    size_t Size=F.Read(C, FILE_MAX-1);
    Contents.From_Local(C, Size);
    return L"";
}

//---------------------------------------------------------------------------
// Write a file
ZenLib::Ztring Enums_Create_Save(Ztring FileName, Ztring &Contents)
{
    File F;
    if (F.Create(FileName.c_str(), true)==false)
    {
        Ztring ToReturn=L"Problems to create ";
        ToReturn+=FileName;
        ToReturn+=L"\r\n";
        return ToReturn;
    }

    std::string S1=Contents.To_UTF8();
    size_t Size=F.Write((const int8u*)S1.c_str(), S1.size());
    Contents.From_Number(Size);
    Contents+=L" bytes written";
    return L"";
}

//---------------------------------------------------------------------------
// Open an item
ZenLib::Ztring Enums_Create_Item(const Ztring &Directory, const Ztring &Name, Ztring &Contents)
{
    Contents.clear();
    Ztring Result;

    //Load header
    Ztring Partial;
    Result=Enums_Create_Load(L"../Source/Resource/Text/Enums_.2.txt", Partial);
    if (!Result.empty())
        return Result;
    Partial.FindAndReplace(L"%Name%", Ztring(Name).MakeLowerCase());
    Contents+=Partial;

    //Load line template
    Ztring Line;
    Result=Enums_Create_Load(L"../Source/Resource/Text/Enums_.5.txt", Line);
    if (!Result.empty())
        return Result;

    //Read input file
    ZtringListListF ZLL;
    ZLL.Load(Ztring(L"../Source/Resource/Text/")+Directory+L"/"+Name+L".csv");
    for (size_t Pos=0; Pos<ZLL.size(); Pos++)
    {
        Ztring Line_Temp=Line;
        Line_Temp.FindAndReplace(L"%Name%", Name);
        Ztring &ToAdd=ZLL(Pos, 0);
        ToAdd.FindAndReplace(__T("/"), __T("_"), 0, Ztring_Recursive); //C++ does not accept "/" in names
        ToAdd.FindAndReplace(__T("("), __T("_")); //C++ does not accept "(" in names
        ToAdd.FindAndReplace(__T(")"), __T("_")); //C++ does not accept ")" in names
        ToAdd.FindAndReplace(__T("*"), __T("_")); //C++ does not accept "*" in names
        ToAdd.FindAndReplace(__T("-"), __T("_")); //C++ does not accept "-" in names
        Line_Temp.FindAndReplace(L"%Line%", ToAdd);
        Contents+=Line_Temp;
    }

    //Load footer template
    Result=Enums_Create_Load(L"../Source/Resource/Text/Enums_.8.txt", Partial);
    if (!Result.empty())
        return Result;
    Contents+=Partial;

    return L"";
}

//---------------------------------------------------------------------------
// Main
ZenLib::Ztring Enums_Create()
{
    Ztring Out;
    Ztring Result;
    Ztring Contents;

    //Load header
    Result=Enums_Create_Load(L"../Source/Resource/Text/Enums_.1.txt", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Enums_Create_Item(L"Stream", L"Generic", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Enums_Create_Item(L"Stream", L"General", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Enums_Create_Item(L"Stream", L"Video", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Enums_Create_Item(L"Stream", L"Audio", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Enums_Create_Item(L"Stream", L"Text", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Enums_Create_Item(L"Stream", L"Other", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Enums_Create_Item(L"Stream", L"Image", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Enums_Create_Item(L"Stream", L"Menu", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    //Load footer
    Result=Enums_Create_Load(L"../Source/Resource/Text/Enums_.9.txt", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    //Write file
    Result=Enums_Create_Save(L"../Source/MediaInfo/File__Analyse_Automatic.h", Out);
    if (!Result.empty())
        return Result;

    return Out;

}
//---------------------------------------------------------------------------

