/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "EasyView.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    ref class Box sealed
    {
    internal:
         Box(Platform::String^ Kind, Platform::String^ Title, Platform::Collections::Vector<Platform::String^>^ Content) : _Kind(Kind), _Title(Title), _Content(Content) {};

        property Platform::String^ Kind
        {
            Platform::String^ get() { return _Kind; }
        };

        property Platform::String^ Title
        {
            Platform::String^ get() { return _Title; }
        };

        property Windows::Foundation::Collections::IVectorView<Platform::String^>^ Content
        {
            Windows::Foundation::Collections::IVectorView<Platform::String^>^ get() { return _Content->GetView(); }
        };

    private:
        Platform::String^ _Kind;
        Platform::String^ _Title;
        Platform::Collections::Vector<Platform::String^>^ _Content;
    };

    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class EasyView sealed
    {
    public:
        EasyView(ReportViewModel^ CurrentReport);

    private:
        Platform::Collections::VectorView<Box^>^ Create_Boxes();
        Box^ Create_Box(size_t StreamKind, int StreamPos);
        size_t Lines_Count_Get(size_t StreamKind);
        size_t Boxes_Count_Get(size_t StreamKind);
        Platform::String^ Tags_Get_General();

        ReportViewModel^ _CurrentReport;
    };
}
