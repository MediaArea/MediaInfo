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
#include "PreRelease/Resources.h"
using namespace ZenLib;
using namespace std;
#define FILE_MAX 20000
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Open a file
ZenLib::Ztring Resources_Create_Load(Ztring FileName, Ztring &Contents)
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
ZenLib::Ztring Resources_Create_Save(Ztring FileName, Ztring &Contents)
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
ZenLib::Ztring Resources_Create_Item(const Ztring &Directory, const Ztring &Name, const Ztring &Class, Ztring &Contents)
{
    Contents.clear();
    Ztring Result;

    //Load header
    Ztring Partial;
    Result=Resources_Create_Load(L"../Source/Resource/Text/_.2.txt", Partial);
    if (!Result.empty())
        return Result;
    Partial.FindAndReplace(L"%Name%", Ztring(L"MediaInfo_Config_")+Name);
    Partial.FindAndReplace(L"%Class%", Class);
    Contents+=Partial;

    //Load line template
    Ztring Line;
    Result=Resources_Create_Load(L"../Source/Resource/Text/_.5.txt", Line);
    if (!Result.empty())
        return Result;

    //Read input file
    ZtringListListF ZLL;
    ZLL.Load(Ztring(L"../Source/Resource/Text/")+Directory+L"/"+Name+L".csv");
    for (size_t Pos=0; Pos<ZLL.size(); Pos++)
    {
        Ztring Line_Temp=Line;
        Line_Temp.FindAndReplace(L"%Line%", ZLL.Read(Pos));
        Contents+=Line_Temp;
    }

    //Load footer template
    Result=Resources_Create_Load(L"../Source/Resource/Text/_.8.txt", Partial);
    if (!Result.empty())
        return Result;
    Contents+=Partial;

    return L"";
}

//---------------------------------------------------------------------------
// Main
ZenLib::Ztring Resources_Create()
{
    Ztring Out;
    Ztring Result;
    Ztring Contents;

    //Load header
    Result=Resources_Create_Load(L"../Source/Resource/Text/_.1.txt", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    //Load datas
    Result=Resources_Create_Item(L"Language", L"DefaultLanguage", L"Translation", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"Format", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_General_Mpeg4", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Video_Matroska", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Video_Mpeg4", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Video_Real", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Video_Riff", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Audio_Matroska", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Audio_Mpeg4", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Audio_Real", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Audio_Riff", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Text_Matroska", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Text_Mpeg4", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Text_Riff", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"CodecID_Other_Mpeg4", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"Codec", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"Stream", L"Generic", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"Stream", L"General", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"Stream", L"Video", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"Stream", L"Audio", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"Stream", L"Text", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"Stream", L"Other", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"Stream", L"Image", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"Stream", L"Menu", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"Iso639_1", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"Iso639_2", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"Library_DivX", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"Library_XviD", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"Library_MainConcept_Avc", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"DataBase", L"Library_VorbisCom", L"InfoMap", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    Result=Resources_Create_Item(L"MediaInfo", L"Summary", L"ZtringListList", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    //Load footer
    Result=Resources_Create_Load(L"../Source/Resource/Text/_.9.txt", Contents);
    if (!Result.empty())
        return Result;
    Out+=Contents;

    //Write file
    Result=Resources_Create_Save(L"../Source/MediaInfo/MediaInfo_Config_Automatic.cpp", Out);
    if (!Result.empty())
        return Result;

    return Out;

}
//---------------------------------------------------------------------------

