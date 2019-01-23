/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
namespace MediaInfo
{
    public ref class Report sealed
    {
    public:
        Report(uint64 Id, Platform::String^ FileName, Platform::String^ Data, Platform::String^ Version);
        Report(Report^ Other);

        property uint64 Id {
            uint64 get() { return _Id; }
            void set(uint64 Value) { _Id=Value; }
        }

        property Platform::String^ FileName {
            Platform::String^ get() { return _FileName; }
            void set(Platform::String^ Value) { _FileName=Value; }
        }

        property Platform::String^ Data {
            Platform::String^ get() { return _Data; }
            void set(Platform::String^ Value) { _Data=Value; }
        }

        property Platform::String^ Version {
            Platform::String^ get() { return _Version; }
            void set(Platform::String^ Value) { _Version=Value; }
        }

    internal:
        uint64 _Id;
        Platform::String^ _FileName;
        Platform::String^ _Data;
        Platform::String^ _Version;
    };
}