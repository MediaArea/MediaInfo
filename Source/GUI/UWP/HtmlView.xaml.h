/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "HtmlView.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class HtmlView sealed
    {
        public:
        HtmlView(ReportViewModel^ CurrentReport);

        private:
            ReportViewModel^ _CurrentReport;
    };
}
