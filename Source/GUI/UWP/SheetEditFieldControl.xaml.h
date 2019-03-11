/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "SheetEditFieldControl.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class SheetEditFieldControl sealed
    {
    public:
        SheetEditFieldControl(Platform::String^ Name, size_t Width, size_t StreamKind, size_t StreamPos, Platform::String^ Parameter);
        Platform::String^ Get_Name();
        size_t Get_Width();
        size_t Get_StreamKind();
        size_t Get_StreamPos();
        Platform::String^ Get_Parameter();

    private:
        void Get_Streams();
        void Get_Parameters();

        static Platform::Collections::Map<size_t, Platform::String^>^ _Streams;
        static Platform::Collections::Map<size_t, Windows::Foundation::Collections::IMapView<Platform::String^, Platform::String^>^>^ _Parameters;
        void StreamBox_SelectionChanged(Platform::Object^ Sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ Event);
        void ParameterBox_SelectionChanged(Platform::Object^ Sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ Event);
        void NumText_TextChanging(Windows::UI::Xaml::Controls::TextBox^ Sender, Windows::UI::Xaml::Controls::TextBoxTextChangingEventArgs^ Event);
    };
}
