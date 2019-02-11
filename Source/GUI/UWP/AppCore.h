/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo.h"
#include "ZenLib/ZtringListList.h"
#include "ZenLib/Ztring.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
//---------------------------------------------------------------------------
    ref class AppCore;

//---------------------------------------------------------------------------
    delegate void ViewChangedEventHandler(Platform::String^ New);

//---------------------------------------------------------------------------
    public ref class View sealed
    {
    public:
        View(Platform::String^ Name, Platform::String^ Desc, Platform::String^ Mime, bool Current, bool Exportable) : _Name(Name), _Desc(Desc), _Mime(Mime), _Current(Current), _Exportable(Exportable) {};

        property Platform::String^ Name
        {
            Platform::String^ get() { return _Name; }
        };

        property Platform::String^ Desc
        {
            Platform::String^ get() { return _Desc; }
        };

        property Platform::String^ Mime
        {
            Platform::String^ get() { return _Mime; }
        };

        property bool Current
        {
            bool get() { return _Current; }
            void set(bool Value) { _Current=Value; }
        };

        property bool Exportable
        {
            bool get() { return _Exportable; }
        };

    private:
        Platform::String^ _Name;
        Platform::String^ _Desc;
        Platform::String^ _Mime;
        bool _Current;
        bool _Exportable;
    };

//---------------------------------------------------------------------------
    ref class AppCore sealed
    {
    public:
        static property Platform::String^ AppVersion
        {
            Platform::String^ get();
        };

        static property Platform::String^ Version
        {
            Platform::String^ get();
        }

        static property Windows::Foundation::Collections::IObservableVector<View^>^ ViewList
        {
            Windows::Foundation::Collections::IObservableVector<View^>^ get();
        }

        static property Platform::String^ View
        {
            Platform::String^ get();
            void set(Platform::String^ Value);
        }

        static event ViewChangedEventHandler^ ViewChangedEvent;

        static Platform::String^ Get_Mime(Platform::String^ Name);
        static Platform::String^ Get_Stream_Name(size_t StreamKind);
        static size_t Get_Stream_Id(Platform::String^ StreamKind);
        static Platform::String^ Create_Report(Platform::String^ Path);
        static void Add_Item_To_FutureAccessList(Windows::Storage::IStorageItem^ Item);

    private:
        static Windows::Foundation::Collections::IObservableVector<::MediaInfo::View^>^ _Views;
    };
}