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
using namespace ZenLib;
using namespace MediaInfo;
using namespace MediaInfoLib;

//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ReportViewModel::ReportViewModel(Report^ SourceReport)
{
    Source=SourceReport;
}

//---------------------------------------------------------------------------
// Properties
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Report^ ReportViewModel::Source::get()
{
    return _Source;
}

//---------------------------------------------------------------------------
void ReportViewModel::Source::set(Report^ Value)
{
    _Source=Value;

    _MI.Close();

    if (Value && Value->Data)
    {
        _MI.Option(__T("Input_Compressed"), __T("zlib+base64"));

        Ztring Input(Value->Data->Data());

        _MI.Open_Buffer_Init(Input.To_UTF8().length(), 0L);
        _MI.Open_Buffer_Continue((int8u*)Input.To_UTF8().c_str(), Input.To_UTF8().length());
        _MI.Open_Buffer_Finalize();
    }
}

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::ReportText::get()
{
    if (!_Source || !Source->Data)
        return L"";

    return Inform(AppCore::View, true);
}

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::ReportHtml::get()
{
    if (!_Source || !Source->Data)
        return L"";

    if (AppCore::View==L"HTML")
       return Inform(AppCore::View, false);

    ZenLib::Ztring Content=ZenLib::Ztring(Inform(AppCore::View, false)->Data());
    Content.FindAndReplace(__T("\t"), __T("    "), 0, ZenLib::Ztring_Recursive);
    Content.FindAndReplace(__T("<"), __T("&lt;"), 0, ZenLib::Ztring_Recursive);
    Platform::String^ ToReturn=ref new Platform::String();
    ToReturn+=L"<html><head></head><body><pre>";
    ToReturn+=ref new Platform::String(Content.c_str());
    ToReturn+="</pre></body></html>";

    return ToReturn;
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::Summary(size_t StreamKind, size_t StreamNumber)
{
    _MI.Option(__T("Inform"), __T("Summary"));
    Ztring Output=_MI.Get(static_cast<stream_t>(StreamKind), StreamNumber, __T("Inform"));

    return ref new Platform::String(Output.c_str());
}

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::Inform(Platform::String ^ Format, bool Export)
{
    if (Format==L"Easy")
        Format=L"Text";

    _MI.Option(__T("Inform_Compress"), __T(""));
    _MI.Option(__T("Inform"), Ztring().From_Unicode(Format->Data()));

    if (Format=="Text" && !Export)
        _MI.Option(__T("Language"), __T("  Config_Text_ColumnSize;25"));
    else
        _MI.Option(__T("Language"), __T("  Config_Text_ColumnSize;40")); // TODO: reset value

    Ztring Output=_MI.Inform();

    _MI.Option(__T("Language"), __T("  Config_Text_ColumnSize;40")); // TODO: reset value

    return ref new Platform::String(Output.c_str());
}

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::Inform_Get(size_t StreamKind, size_t StreamPos)
{
    _MI.Option(__T("Inform_Compress"), __T(""));
    _MI.Option(__T("Inform"), __T(""));

    Ztring Output=_MI.Get(static_cast<stream_t>(StreamKind), StreamPos, __T("Inform"));

    return ref new Platform::String(Output.c_str());
}

Platform::String^ ReportViewModel::GetI(size_t StreamKind, size_t StreamNumber, size_t Parameter)
{
    return GetI(StreamKind, StreamNumber, Parameter, static_cast<size_t>(MediaInfoLib::Info_Text));
}

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::GetI(size_t StreamKind, size_t StreamNumber, size_t Parameter, size_t InfoKind)
{
    Ztring Output=_MI.Get(static_cast<stream_t>(StreamKind), StreamNumber, Parameter, static_cast<info_t>(InfoKind));

    return ref new Platform::String(Output.c_str());
}

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::Get(size_t StreamKind, size_t StreamNumber, Platform::String^ Parameter, size_t InfoKind, size_t SearchKind)
{
    Ztring Output=_MI.Get(static_cast<stream_t>(StreamKind), StreamNumber, Parameter->Data(), static_cast<info_t>(InfoKind), static_cast<info_t>(SearchKind));

    return ref new Platform::String(Output.c_str());
}

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::Get(size_t StreamKind, size_t StreamNumber, Platform::String^ Parameter, size_t InfoKind)
{
    return Get(StreamKind, StreamNumber, Parameter, InfoKind, static_cast<size_t>(MediaInfoLib::Info_Name));
}

//---------------------------------------------------------------------------
Platform::String^ ReportViewModel::Get(size_t StreamKind, size_t StreamNumber, Platform::String^ Parameter)
{
    return Get(StreamKind, StreamNumber, Parameter, static_cast<size_t>(MediaInfoLib::Info_Text), static_cast<size_t>(MediaInfoLib::Info_Name));
}

//---------------------------------------------------------------------------
size_t ReportViewModel::Count_Get(size_t StreamKind, size_t StreamNumber)
{
    return _MI.Count_Get(static_cast<stream_t>(StreamKind), StreamNumber);
}

//---------------------------------------------------------------------------
size_t ReportViewModel::Count_Get(size_t StreamKind)
{
    return _MI.Count_Get(static_cast<stream_t>(StreamKind), static_cast<size_t>(-1));
}
