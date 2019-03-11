/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "SheetEditDialog.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class SheetEditDialog sealed
    {
    public:
        SheetEditDialog(Platform::String^ Title, Sheet^ Content);
    private:
        void Add_Field(Platform::String^ Name, size_t Width, size_t StreamKind, size_t StreamPos, Platform::String^ Parameter);
        void ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ Sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ Event);
        void ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ Sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ Event);

        Sheet^ _Sheet;
    };
}
