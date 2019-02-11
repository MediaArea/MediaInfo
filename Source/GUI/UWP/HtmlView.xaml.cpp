/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "HtmlView.xaml.h"

//---------------------------------------------------------------------------
using namespace MediaInfo;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
HtmlView::HtmlView(ReportViewModel^ CurrentReport) : _CurrentReport(CurrentReport)
{
    InitializeComponent();

    HtmlControl->NavigateToString(CurrentReport->ReportHtml);
}
