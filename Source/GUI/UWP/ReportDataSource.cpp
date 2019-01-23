/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "ReportDataSource.h"


//---------------------------------------------------------------------------
using namespace MediaInfo;

using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

//---------------------------------------------------------------------------
IObservableVector<Report^>^ ReportDataSource::_Reports=ref new Vector<Report^>();