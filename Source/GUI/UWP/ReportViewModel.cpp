/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "ReportViewModel.h"

#include "ZenLib/Ztring.h"

//---------------------------------------------------------------------------
using namespace MediaInfo;

//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ReportViewModel::ReportViewModel(Report^ Source) : _Source(Source)
{
}

//---------------------------------------------------------------------------
// Properties
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::ReportHtml::get()
{
    if (!_Source || !Source->Data)
        return L"";

    if (AppCore::View==L"HTML")
       return AppCore::Convert_Report(_Source->Data, AppCore::View, false);

    ZenLib::Ztring Content=ZenLib::Ztring(AppCore::Convert_Report(_Source->Data, AppCore::View, false)->Data());
    Content.FindAndReplace(__T("\t"), __T("    "), 0, ZenLib::Ztring_Recursive);
    Content.FindAndReplace(__T("<"), __T("&lt;"), 0, ZenLib::Ztring_Recursive);
    Platform::String^ ToReturn=ref new Platform::String();
    ToReturn+=L"<html><head></head><body><pre>";
    ToReturn+=ref new Platform::String(Content.c_str());
    ToReturn+="</pre></body></html>";

    return ToReturn;
}