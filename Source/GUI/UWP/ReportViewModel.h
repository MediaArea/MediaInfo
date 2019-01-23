/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
namespace MediaInfo
{
    public ref class ReportViewModel sealed
    {
    public:
        ReportViewModel(Report^ Source);

        property Report^ Source
        {
            Report^ get() { return _Source; }
            void set(Report^ Value) { _Source=Value; }
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
            Platform::String^ get()
            {
                if (!_Source || !Source->Data)
                    return L"";

                return AppCore::Convert_Report(_Source->Data, AppCore::View, true);
            }
        }

        property Platform::String^ ReportHtml
        {
            Platform::String^ get();
        }

    private:
        Report^ _Source;
    };
}