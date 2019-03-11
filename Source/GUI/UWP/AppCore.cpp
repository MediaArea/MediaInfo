/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include <windows.h>

#include "pch.h"
#include "AppCore.h"

#include "ZenLib/FileName.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/File.h"
#include "ZenLib/Dir.h"

//---------------------------------------------------------------------------
using namespace ZenLib;
using namespace MediaInfo;
using namespace MediaInfoLib;

using namespace Platform::Collections;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Security::Cryptography;
using namespace Windows::Security::Cryptography::Core;
using namespace Windows::Storage;
using namespace Windows::Storage::AccessCache;
using namespace Windows::Storage::Streams;
using namespace Concurrency;


//---------------------------------------------------------------------------
// Init
//---------------------------------------------------------------------------
IObservableVector<View^>^ AppCore::_Views=nullptr;

//---------------------------------------------------------------------------
// Properties
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Platform::String^ AppCore::AppVersion::get()
{
    PackageVersion Version=Package::Current->Id->Version;

    Platform::String^ VersionString=Version.Major.ToString();
    if (Version.Minor<10)
        VersionString+=L".0"+Version.Minor.ToString();
    else
        VersionString+=L"."+Version.Minor.ToString();

    if (Version.Build || Version.Revision)
        VersionString+=L"."+Version.Build.ToString();

    if (Version.Revision)
        VersionString+=L"."+Version.Revision.ToString();

    return VersionString;
}

//---------------------------------------------------------------------------
Platform::String^ AppCore::Version::get()
{
    MediaInfoLib::MediaInfo MI;

    ZenLib::Ztring Result=MI.Option(__T("Info_Version"));
    Result.FindAndReplace(__T("MediaInfoLib - v"), __T(""));

    return ref new Platform::String(Result.c_str());
}

//---------------------------------------------------------------------------
IObservableVector<View^>^ AppCore::ViewList::get()
{
    if (_Views)
        return _Views;

    MediaInfoLib::MediaInfo MI;
    _Views=ref new Platform::Collections::Vector<::MediaInfo::View^>();

    _Views->Append(ref new ::MediaInfo::View("Easy", "Easy", "text/plain", View==L"Easy", false));
    _Views->Append(ref new ::MediaInfo::View("Sheet", "Sheet", "text/plain", View==L"Sheet", false));

    ZenLib::ZtringListList ViewsCSV=ZenLib::ZtringListList();
    ViewsCSV.Separator_Set(0, ZenLib::EOL);
    ViewsCSV.Separator_Set(1, __T(","));
    ViewsCSV.Write(MI.Option(__T("Info_OutputFormats_CSV")));

    for (size_t It=0; It<ViewsCSV.size(); It++)
    {
        ZenLib::ZtringList Current=ViewsCSV[It];
        if (Current.size()>=3)
        {
            Platform::String^ Name=ref new Platform::String(Current[0].c_str());
            Platform::String^ Desc=ref new Platform::String(Current[1].c_str());
            Platform::String^ Mime=ref new Platform::String(Current[2].c_str());
            _Views->Append(ref new ::MediaInfo::View(Name, Desc, Mime, View==Name, true));
        }
    }

    return _Views;
}
//---------------------------------------------------------------------------
Platform::String^ AppCore::View::get()
{
    ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
    Platform::String^ Current=safe_cast<Platform::String^>(LocalSettings->Values->Lookup(L"View"));

    if (!Current||!Current->Length())
        Current=L"Easy";

    return Current;
}

//---------------------------------------------------------------------------
void AppCore::View::set(Platform::String^ Value)
{
    ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;

    Platform::String^ Current=safe_cast<Platform::String^>(LocalSettings->Values->Lookup(L"View"));
    if (Current && Current->Length() && Current==Value)
        return;

    LocalSettings->Values->Insert(L"View", Value);

    if (_Views)
    {
        for (::MediaInfo::View^ It:_Views)
            if (It->Name==Value)
                It->Current=true;
            else if(It->Current)
                It->Current=false;
    }

    ViewChangedEvent(Value);
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Platform::String^ AppCore::Get_Mime(Platform::String^ Name)
{
    if (Name==L"Easy" || Name==L"Sheet")
        return L"Text/plain";

    for (::View^ It : ViewList)
    {
        if (It->Name==Name)
            return It->Mime;
    }

    return L"";
}

//---------------------------------------------------------------------------
Platform::String^ AppCore::Get_Stream_Name(size_t StreamKind)
{
    switch (static_cast<stream_t>(StreamKind))
    {
    case Stream_General: return L"General";
    case Stream_Video: return L"Video";
    case Stream_Audio: return L"Audio";
    case Stream_Text: return L"Text";
    case Stream_Other: return L"Other";
    case Stream_Image: return L"Image";
    case Stream_Menu: return L"Menu";
    default: return L"Unknown Stream";
    }
}

//---------------------------------------------------------------------------
size_t AppCore::Get_Stream_Id(Platform::String^ StreamKind)
{
    if (StreamKind==L"General")
        return static_cast<size_t>(Stream_General);
    else if (StreamKind==L"Video")
        return static_cast<size_t>(Stream_Video);
    else if (StreamKind==L"Audio")
        return static_cast<size_t>(Stream_Audio);
    else if (StreamKind==L"Text")
        return static_cast<size_t>(Stream_Text);
    else if (StreamKind==L"Other")
        return static_cast<size_t>(Stream_Other);
    else if (StreamKind==L"Image")
        return static_cast<size_t>(Stream_Image);
    else if (StreamKind==L"Menu")
        return static_cast<size_t>(Stream_Menu);

    return static_cast<size_t>(Stream_Max);
}

//---------------------------------------------------------------------------
Platform::String^ AppCore::Create_Report(Platform::String^ Path)
{
    MediaInfoLib::MediaInfo MI;

    MI.Option(__T("Inform"), __T("MIXML"));
    MI.Option(__T("Input_Compressed"), __T(""));
    MI.Option(__T("Inform_Compress"), __T("zlib+base64"));
    MI.Open(Ztring(Path->Data()));

    Ztring Report=MI.Inform();
    MI.Close();

    return ref new Platform::String(Report.c_str());
}

//---------------------------------------------------------------------------
void AppCore::Add_Item_To_FutureAccessList(IStorageItem^ Item)
{
    if (!Item)
        return;

    IBuffer^ InBuffer=CryptographicBuffer::ConvertStringToBinary(Item->Path, BinaryStringEncoding::Utf16LE);
    HashAlgorithmProvider^ Provider=HashAlgorithmProvider::OpenAlgorithm("SHA256");
    IBuffer^ OutBuffer=Provider->HashData(InBuffer);
    Platform::String^ Hash=CryptographicBuffer::EncodeToBase64String(OutBuffer);
    StorageApplicationPermissions::FutureAccessList->AddOrReplace(Hash, Item);
}