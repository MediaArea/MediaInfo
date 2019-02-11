/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
namespace MediaInfo
{
//---------------------------------------------------------------------------
    public ref class Field sealed
    {
    public:
        Field(Platform::String^ Title, uint32 StreamKind, uint32 StreamPos, Platform::String^ Parameter, uint32 Length) : _Title(Title), _StreamKind(StreamKind), _StreamPos(StreamPos), _Parameter(Parameter), _Length(Length) {};

        property Platform::String^ Title
        {
            Platform::String^ get() { return _Title; }
            void set(Platform::String^ Value) { _Title=Value; }
        };

        property uint32 StreamKind
        {
            uint32 get() { return _StreamKind; }
            void set(uint32 Value) { _StreamKind=Value; }
        };

        property uint32 StreamPos
        {
            uint32 get() { return _StreamPos; }
            void set(uint32 Value) { _StreamPos=Value; }
        };

        property Platform::String^ Parameter
        {
            Platform::String^ get() { return _Parameter; }
            void set(Platform::String^ Value) { _Parameter=Value; }
        };

        property uint32 Length
        {
            uint32 get() { return _Length; }
            void set(uint32 Value) { _Length=Value; }
        };

    private:
        Platform::String^ _Title;
        uint32 _StreamKind;
        uint32 _StreamPos;
        Platform::String^ _Parameter;
        uint32 _Length;
    };

//---------------------------------------------------------------------------
    public ref class Sheet sealed
    {
    public:
        Sheet(Platform::String^ Name, Windows::Foundation::Collections::IObservableVector<Field^>^ Fields) : _Name(Name), _Fields(Fields) {};

        property Platform::String^ Name
        {
            Platform::String^ get() { return _Name; }
            void set(Platform::String^ Value) { _Name=Value; }
        };

        property  Windows::Foundation::Collections::IObservableVector<Field^>^ Fields
        {
            Windows::Foundation::Collections::IObservableVector<Field^>^ get() { return _Fields; }
            void set(Windows::Foundation::Collections::IObservableVector<Field^>^ Value) { _Fields=Value; }
        };

    private:
        Platform::String^ _Name;
        Windows::Foundation::Collections::IObservableVector<Field^>^ _Fields;
    };
}
