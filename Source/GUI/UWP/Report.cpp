/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "Report.h"

//---------------------------------------------------------------------------
using namespace MediaInfo;

using namespace Windows::Foundation;
using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;

//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Report::Report(uint64 Id, Platform::String^ FileName, Platform::String^ Data, Platform::String^ Version) : _Id(Id), _FileName(FileName), _Data(Data), _Version(Version)
{
}

//---------------------------------------------------------------------------
Report::Report(Report^ Other)
{
    _Id=Other->Id;
    _FileName=ref new Platform::String(Other->FileName->Data());
    _Data=ref new Platform::String(Other->Data->Data());
    _Version=ref new Platform::String(Other->Version->Data());
}