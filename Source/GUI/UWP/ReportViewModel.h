/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

#include "MediaInfo/MediaInfo_Internal.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    public ref class ReportViewModel sealed
    {
    public:
        ReportViewModel(Report^ SourceReport);

        property Report^ Source
        {
            Report^ get();
            void set(Report^ Value);
        }

        property uint64 Id
        {
            uint64 get()
            {
                if (!_Source)
                    return 0LL;

                return _Source->Id;
            }
        }

        property Platform::String^ Name
        {
            Platform::String^ get()
            {
                if (!_Source || !_Source->FileName)
                    return L"";

                return _Source->FileName;
            }
        }

        property Platform::String^ ReportText
        {
            Platform::String^ get();
        }

        property Platform::String^ ReportHtml
        {
            Platform::String^ get();
        }

        Platform::String^ Summary(size_t StreamKind, size_t StreamNumber);
        Platform::String^ Inform(Platform::String^ Format, bool Export);
        Platform::String^ Inform_Get(size_t StreamKind, size_t StreamNumber);
        Platform::String^ GetI(size_t StreamKind, size_t StreamNumber, size_t Parameter);
        Platform::String^ GetI(size_t StreamKind, size_t StreamNumber, size_t Parameter,size_t InfoKind);
        Platform::String^ Get(size_t StreamKind, size_t StreamNumber, Platform::String^ Parameter, size_t InfoKind, size_t SearchKind);
        Platform::String^ Get(size_t StreamKind, size_t StreamNumber, Platform::String^ Parameter, size_t InfoKind);
        Platform::String^ Get(size_t StreamKind, size_t StreamNumber, Platform::String^ Parameter);
        size_t Count_Get(size_t StreamKind, size_t StreamNumber);
        size_t Count_Get(size_t StreamKind);

    private:
        Report^ _Source;
        MediaInfoLib::MediaInfo _MI;
    };
}