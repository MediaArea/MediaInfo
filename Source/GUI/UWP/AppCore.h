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
    ref class View sealed
    {
    public:
        View(Platform::String^ Name, Platform::String^ Desc, Platform::String^ Mime) : _Name(Name), _Desc(Desc), _Mime(Mime) {};

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

    private:
        Platform::String^ _Name;
        Platform::String^ _Desc;
        Platform::String^ _Mime;
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

        static property Windows::Foundation::Collections::IVectorView<View^>^ ViewList
        {
            Windows::Foundation::Collections::IVectorView<View^>^ get();
        }

        static property Platform::String^ View
        {
            Platform::String^ get();
            void set(Platform::String^ Value);
        }

        static Platform::String^ Get_Mime(Platform::String^ Name);
        static Platform::String^ Create_Report(Platform::String^ Path);
        static Platform::String^ Convert_Report(Platform::String^ Report, Platform::String^ Format, bool Export);
        static void Add_Item_To_FutureAccessList(Windows::Storage::IStorageItem^ Item);
    };
}