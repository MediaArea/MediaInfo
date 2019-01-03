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
IVectorView<View^>^ AppCore::ViewList::get()
{
    MediaInfoLib::MediaInfo MI;
    Vector<::MediaInfo::View^>^ ToReturn=ref new Platform::Collections::Vector<::MediaInfo::View^>();

    ZenLib::ZtringListList ViewsCSV=ZenLib::ZtringListList();
    ViewsCSV.Separator_Set(0, ZenLib::EOL);
    ViewsCSV.Separator_Set(1, __T(","));
    ViewsCSV.Write(MI.Option(__T("Info_OutputFormats_CSV")));

    for (size_t It=0; It<ViewsCSV.size(); It++)
    {
        ZenLib::ZtringList Current=ViewsCSV[It];
        if (Current.size()>=3)
            ToReturn->Append(ref new ::MediaInfo::View(ref new Platform::String(Current[0].c_str()), ref new Platform::String(Current[1].c_str()), ref new Platform::String(Current[2].c_str())));
    }

    return ToReturn->GetView();
}

//---------------------------------------------------------------------------
Platform::String^ AppCore::View::get()
{
    ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
    Platform::String^ CurrentView=safe_cast<Platform::String^>(LocalSettings->Values->Lookup(L"View"));

    if (!CurrentView->Length())
        CurrentView=L"HTML";

    return CurrentView;
}

void AppCore::View::set(Platform::String^ Value)
{
    ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
    LocalSettings->Values->Insert(L"View", Value);
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Platform::String^ AppCore::Get_Mime(Platform::String^ Name)
{
    for (::View^ It : ViewList)
    {
        if (It->Name==Name)
            return It->Mime;
    }

    return L"";
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
Platform::String^ AppCore::Convert_Report(Platform::String^ Report, Platform::String^ Format, bool Export)
{
    MediaInfoLib::MediaInfo MI;

    MI.Option(__T("Inform"), Ztring().From_Unicode(Format->Data()));
    MI.Option(__T("Inform_Compress"), __T(""));
    MI.Option(__T("Input_Compressed"), __T("zlib+base64"));

    if (Format=="Text" && !Export)
        MI.Option(__T("Language"), __T("  Config_Text_ColumnSize;25"));

    Ztring Input(Report->Data());

    MI.Open_Buffer_Init(Input.To_UTF8().length(), 0L);
    MI.Open_Buffer_Continue((int8u*)Input.To_UTF8().c_str(), Input.To_UTF8().length());
    MI.Open_Buffer_Finalize();

    Ztring Output=MI.Inform();
    MI.Close();

    return ref new Platform::String(Output.c_str());
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