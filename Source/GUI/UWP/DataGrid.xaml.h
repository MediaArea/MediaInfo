/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "DataGrid.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
//---------------------------------------------------------------------------
    ref class DataGrid;

//---------------------------------------------------------------------------
    public enum class SortMode
    {
        None,
        Ascending,
        Descending
    };

//---------------------------------------------------------------------------
    public ref class Sort sealed
    {
    public:
        Sort(SortMode Mode, unsigned int Column) : _Mode(Mode), _Column(Column) {}
        property SortMode Mode
        {
            SortMode get() { return _Mode; }
            void set(SortMode Value) { _Mode=Value; }
        }
        property unsigned int Column
        {
            unsigned int get() { return _Column; }
            void set(unsigned int Value) { _Column=Value; }
        }
    private:
        SortMode _Mode;
        unsigned int _Column;
    };

//---------------------------------------------------------------------------
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class RightClickedEventArgs sealed
    {
    public:
        RightClickedEventArgs(unsigned int Pos, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ Event) : _Pos(Pos), _Event(Event) {}
        property unsigned int Pos
        {
            unsigned int get() { return _Pos; };
        }
        property Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ Event
        {
            Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ get() { return _Event; }
        }
    private:
        unsigned int _Pos;
        Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ _Event;
    };

//---------------------------------------------------------------------------
    [Windows::Foundation::Metadata::WebHostHidden]
    public delegate void SelectionChangedEventHandler(DataGrid^ Sender, unsigned int Pos);
    public delegate void RightClickedEventHandler(DataGrid^ Sender, RightClickedEventArgs^ Event);

//---------------------------------------------------------------------------
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class DataGrid sealed
    {
    public:
        DataGrid();
        void Reset(); // Delete cell content and rows source
        void Clear(); // Delete only cell content
        void Add_Column(Windows::UI::Xaml::GridLength Width, Platform::String^ Header);
        void Set_Content(unsigned int Row, unsigned int Column, Platform::String^ Content);
        void Select(Windows::Foundation::Collections::IVectorView<Platform::String^>^ Row);

        property Windows::Foundation::Collections::IObservableVector<Windows::Foundation::Collections::IVectorView<Platform::String^>^>^ Rows
        {
            Windows::Foundation::Collections::IObservableVector<Windows::Foundation::Collections::IVectorView<Platform::String^>^>^ get() { return _Rows; }
            void set(Windows::Foundation::Collections::IObservableVector<Windows::Foundation::Collections::IVectorView<Platform::String^>^>^ Value);
        }

        property unsigned int SelectedRow {
            unsigned int get();
        }

        event SelectionChangedEventHandler^ SelectionChangedEvent;
        event RightClickedEventHandler^ RightClickedEvent;
    private:
        void Display();
        void Sort_Rows();
        Windows::UI::Xaml::Controls::RelativePanel^ Add_Cell(unsigned int Row, unsigned int Column, Platform::String^ Content);
        void Cell_Clicked(Platform::Object^ Sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ Event);
        void Cell_RightClicked(Platform::Object^ Sender, Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^ Event);
        void ResizeHandle_PointerEntered(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        void ResizeHandle_PointerPressed(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        void ResizeHandle_PointerReleased(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        void ResizeHandle_PointerMoved(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        void ResizeHandle_PointerExited(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);


        Sort^ _SortMode;
        Platform::Collections::Vector<Windows::UI::Xaml::Controls::ColumnDefinition^>^ _Columns;
        Platform::Collections::Vector<Platform::String^>^ _ColumnsTitles;
        Platform::Collections::Vector<Windows::UI::Xaml::Shapes::Rectangle^>^ _RowsBackgrounds;
        Windows::Foundation::Collections::IVectorView<Platform::String^>^ _SelectedRow;
        Platform::Collections::Vector<Windows::Foundation::Collections::IVectorView<Platform::String^>^>^ _Rows;
        Platform::Collections::Vector<Windows::Foundation::Collections::IVectorView<Platform::String^>^>^ _SortedRows;
        Windows::Foundation::Collections::IObservableVector<Windows::Foundation::Collections::IVectorView<Platform::String^>^>^ _OriginalRows; // keep this reference to unregister the VectorChanged token later
        bool _Resizing;
        uint16 _Pointer_Count;
        Windows::UI::Core::CoreCursor^ _Old_Pointer;
        Windows::Foundation::EventRegistrationToken _RowsVectorChangedToken;
    };
}
