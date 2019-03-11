/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include <collection.h>

#include "pch.h"
#include "DataGrid.xaml.h"

#include "ZenLib/Ztring.h"

//---------------------------------------------------------------------------
using namespace ZenLib;
using namespace MediaInfo;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Shapes;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;

//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
DataGrid::DataGrid() : _SortMode(ref new Sort(SortMode::None, 0)), _Rows(ref new Vector<IVectorView<Platform::String^>^>()),
                       _SelectedRow(nullptr), _SortedRows(ref new Vector<IVectorView<Platform::String^>^>()),
                       _OriginalRows(nullptr), _Columns(ref new Vector<ColumnDefinition^>()),
                       _ColumnsTitles(ref new Vector<Platform::String^>),
                       _RowsBackgrounds(ref new Vector<Rectangle^>()),
                       _RowsVectorChangedToken(EventRegistrationToken())
{
    InitializeComponent();
}

//---------------------------------------------------------------------------
// Properties
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void DataGrid::Rows::set(IObservableVector<IVectorView<Platform::String^>^>^ Value)
{
    if (_OriginalRows)
        _OriginalRows->VectorChanged-=_RowsVectorChangedToken;

    _OriginalRows=Value;
    _Rows->Clear();
    _SortedRows->Clear();

    // Copy Rows
    for (IVectorView<Platform::String^>^ Row : Value)
    {
        _Rows->Append(Row);
        _SortedRows->Append(Row);
    }

    // Observe original list
    _RowsVectorChangedToken=Value->VectorChanged+=ref new VectorChangedEventHandler<IVectorView<Platform::String^>^>([this](IObservableVector<IVectorView<Platform::String^>^>^ Sender, IVectorChangedEventArgs^ Event)
    {
        switch (Event->CollectionChange)
        {
        case CollectionChange::ItemInserted:
            if (Event->Index<=_Rows->Size)
            {
                _Rows->InsertAt(Event->Index, Sender->GetAt(Event->Index));
                _SortedRows->Append(Sender->GetAt(Event->Index));
                Display();
            }
            break;
        case CollectionChange::ItemRemoved:
            if (Event->Index<_Rows->Size)
            {
                unsigned int Index=0;
                if (_SortedRows->IndexOf(_Rows->GetAt(Event->Index), &Index))
                    _SortedRows->RemoveAt(Index);

                _Rows->RemoveAt(Event->Index);
            }
            Display();
            break;
        case CollectionChange::Reset:
            _Rows->Clear();
            _SortedRows->Clear();
            // Copy Rows
            for (IVectorView<Platform::String^>^ Row : Sender)
            {
                _Rows->Append(Row);
                _SortedRows->Append(Row);
            }
            Display();
            break;
        }
    });

    Display();
}

//---------------------------------------------------------------------------
unsigned int DataGrid::SelectedRow::get()
{
    unsigned int Pos=static_cast<unsigned int>(-1);
    if (_SelectedRow)
        _Rows->IndexOf(_SelectedRow, &Pos);

    return Pos;
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void DataGrid::Reset()
{
    if (_OriginalRows)
    {
        _OriginalRows->VectorChanged-=_RowsVectorChangedToken;
        _OriginalRows=nullptr;
    }

    _SortMode=ref new Sort(SortMode::None, 0);

    _RowsBackgrounds->Clear();
    _SelectedRow=nullptr;

    _Columns->Clear();
    _ColumnsTitles->Clear();

    _Rows->Clear();
    _SortedRows->Clear();

    RootGrid->Children->Clear();
    RootGrid->ColumnDefinitions->Clear();

    //Keep only header row
    for (unsigned int Pos=RootGrid->RowDefinitions->Size-1; Pos>1; Pos--)
        RootGrid->RowDefinitions->RemoveAt(Pos);
}

void DataGrid::Clear()
{
    RootGrid->Children->Clear();
    RootGrid->ColumnDefinitions->Clear();

    _RowsBackgrounds->Clear();

    //Keep only header row
    if (RootGrid->RowDefinitions->Size)
    {
        RowDefinition^ HeaderRow=RootGrid->RowDefinitions->GetAt(0);
        RootGrid->RowDefinitions->Clear();
        RootGrid->RowDefinitions->Append(HeaderRow);
    }

}

//---------------------------------------------------------------------------
void DataGrid::Display()
{
    Clear();
    Sort_Rows();

    Rectangle^ HeaderBackground=ref new Rectangle();
    HeaderBackground->Fill=ref new SolidColorBrush(Windows::UI::Colors::Silver);
    HeaderBackground->HorizontalAlignment=Windows::UI::Xaml::HorizontalAlignment::Stretch;
    HeaderBackground->VerticalAlignment=Windows::UI::Xaml::VerticalAlignment::Stretch;
    RootGrid->Children->Append(HeaderBackground);
    RootGrid->SetRow(HeaderBackground, 0);
    RootGrid->SetColumn(HeaderBackground, 0);
    RootGrid->SetColumnSpan(HeaderBackground, _Columns->Size);
    _RowsBackgrounds->Append(HeaderBackground);
    for (unsigned int Pos=0; Pos<_Columns->Size; Pos++)
    {
        RootGrid->ColumnDefinitions->Append(_Columns->GetAt(Pos));
        Platform::String^ Title=L"";
        if (Pos<_ColumnsTitles->Size)
            Title=_ColumnsTitles->GetAt(Pos);

        if (Pos==_SortMode->Column)
        {
            switch (_SortMode->Mode)
            {
            case SortMode::Ascending:
                Title+=L"  ˄";
                break;
            case SortMode::Descending:
                Title+=L"  ˅";
                break;
            }
        }

        RelativePanel^ HeaderPanel=Add_Cell(0, RootGrid->ColumnDefinitions->Size-1, Title);
    }

    for (unsigned int Pos=0; Pos<_SortedRows->Size; Pos++)
    {
        RowDefinition^ NewRow=ref new RowDefinition();
        NewRow->Height=GridLengthHelper::Auto;
        RootGrid->RowDefinitions->Append(NewRow);

        Rectangle^ Background=ref new Rectangle();
        if (_SortedRows->GetAt(Pos)==_SelectedRow)
            Background->Fill=ref new SolidColorBrush(Windows::UI::Colors::LightBlue);
        else if ((Pos+1)%2==0)
            Background->Fill=ref new SolidColorBrush(Windows::UI::Colors::WhiteSmoke);
        else
            Background->Fill=ref new SolidColorBrush(Windows::UI::Colors::White);

        Background->HorizontalAlignment=Windows::UI::Xaml::HorizontalAlignment::Stretch;
        Background->VerticalAlignment=Windows::UI::Xaml::VerticalAlignment::Stretch;
        RootGrid->Children->Append(Background);
        RootGrid->SetRow(Background, RootGrid->RowDefinitions->Size-1);
        RootGrid->SetColumn(Background, 0);
        RootGrid->SetColumnSpan(Background, RootGrid->ColumnDefinitions->Size);
        _RowsBackgrounds->Append(Background);

        for (unsigned int Pos2=0; Pos2<RootGrid->ColumnDefinitions->Size; Pos2++)
        {
            Platform::String^ CellContent=L"";
            if (Pos2<_SortedRows->GetAt(Pos)->Size)
                CellContent=_SortedRows->GetAt(Pos)->GetAt(Pos2);

            Add_Cell(RootGrid->RowDefinitions->Size-1, Pos2, CellContent);
        }
    }
}

//---------------------------------------------------------------------------
void DataGrid::Sort_Rows()
{
    if (_SortMode->Mode==SortMode::None)
        return;

    std::stable_sort(begin(_SortedRows), end(_SortedRows), [this](IVectorView<Platform::String^>^ Left, IVectorView<Platform::String^>^ Right)
    {
        Ztring LeftStr(this->_SortMode->Column<Left->Size?Left->GetAt(this->_SortMode->Column)->Data():__T(""));
        Ztring RightStr(this->_SortMode->Column<Right->Size?Right->GetAt(this->_SortMode->Column)->Data():__T(""));

        return LeftStr.Compare(RightStr, this->_SortMode->Mode==SortMode::Descending?__T(">"):__T("<"));
    });
}

void DataGrid::Add_Column(GridLength Width, Platform::String^ Title)
{
    ColumnDefinition^ NewColumn=ref new ColumnDefinition();
    NewColumn->Width=Width;

    _Columns->Append(NewColumn);
    _ColumnsTitles->Append(Title);
}

//---------------------------------------------------------------------------
void DataGrid::Set_Content(unsigned int Row, unsigned int Column, Platform::String^ Content)
{
    if (RootGrid->RowDefinitions->Size<=Row || RootGrid->ColumnDefinitions->Size<=Column)
        return;

    Object^ PanelObj=RootGrid->FindName(Column+L";"+Row);
    if (!PanelObj || PanelObj->GetType()->FullName!=TypeName(RelativePanel::typeid).Name)
        return;

    RelativePanel^ CellPanel=safe_cast<RelativePanel^>(PanelObj);

    Object^ ContentObj=CellPanel->FindName("CellContent");
    if (!ContentObj||ContentObj->GetType()->FullName!=TypeName(TextBlock::typeid).Name)
        return;

    TextBlock^ ContentBlock=safe_cast<TextBlock^>(ContentObj);
    ContentBlock->Text=Content;
}

//---------------------------------------------------------------------------
void DataGrid::Select(IVectorView<Platform::String^>^ Row)
{
    unsigned int Pos;
    if(!_SortedRows->IndexOf(Row, &Pos))
        return;

    if (_SelectedRow)
    {
        unsigned int SelectedRowIndex=0;
        if (!_SortedRows->IndexOf(_SelectedRow, &SelectedRowIndex))
            return;

        if (SelectedRowIndex%2==0)
            _RowsBackgrounds->GetAt(SelectedRowIndex+1)->Fill=ref new SolidColorBrush(Windows::UI::Colors::WhiteSmoke);
        else
            _RowsBackgrounds->GetAt(SelectedRowIndex+1)->Fill=ref new SolidColorBrush(Windows::UI::Colors::White);
    }

    if (Pos+1<_RowsBackgrounds->Size)
        _RowsBackgrounds->GetAt(Pos+1)->Fill=ref new SolidColorBrush(Windows::UI::Colors::LightBlue);

    _SelectedRow=Row;

    unsigned int OriginalPos=0;
    if (!_Rows->IndexOf(Row, &OriginalPos))
        return;

    SelectionChangedEvent(this, OriginalPos);
}

//---------------------------------------------------------------------------
RelativePanel^ DataGrid::Add_Cell(unsigned int Row, unsigned int Column, Platform::String^ Content)
{
    RelativePanel^ Panel=ref new RelativePanel();
    Panel->Name=Column+L";"+Row;

    TextBlock^ Block=ref new TextBlock;
    Block->Name=L"CellContent";
    Block->Margin=Thickness(2, 2, 2, 2);
    Block->TextAlignment=TextAlignment::Center;
    Block->TextTrimming=TextTrimming::CharacterEllipsis;
    Block->VerticalAlignment=Windows::UI::Xaml::VerticalAlignment::Center;

    Panel->Children->Append(Block);

     if (Column==0)
    {
        Rectangle^ LeftBorder=ref new Rectangle();
        LeftBorder->Name=L"LeftBorder";
        LeftBorder->Fill=ref new SolidColorBrush(Windows::UI::Colors::LightGray);
        LeftBorder->Width=1;

        Panel->Children->Append(LeftBorder);
        Panel->SetAlignLeftWithPanel(LeftBorder, true);
        Panel->SetAlignBottomWithPanel(LeftBorder, true);
        Panel->SetAlignTopWithPanel(LeftBorder, true);
        Panel->SetAlignLeftWith(Block, LeftBorder);
    }
    else
    {
        Panel->SetAlignLeftWithPanel(Block, true);
    }

    if (Row==0)
    {
        Rectangle^ TopBorder=ref new Rectangle();
        TopBorder->Name=L"TopBorder";
        TopBorder->Fill=ref new SolidColorBrush(Windows::UI::Colors::LightGray);
        TopBorder->Height=1;

        Panel->Children->Append(TopBorder);
        Panel->SetAlignRightWithPanel(TopBorder, true);
        Panel->SetAlignLeftWithPanel(TopBorder, true);
        Panel->SetAlignTopWithPanel(TopBorder, true);
        Panel->SetAlignTopWith(Block, TopBorder);
    }
    else
    {
        Panel->SetAlignTopWithPanel(Block, true);
    }

    Rectangle^ RightBorder=ref new Rectangle();
    RightBorder->Name=L"RightBorder";
    RightBorder->Fill=ref new SolidColorBrush(Windows::UI::Colors::LightGray);
    RightBorder->Width=1;

    if (Row==0) // Make column resizable
    {
        RightBorder->PointerPressed+=ref new PointerEventHandler(this, &DataGrid::ResizeHandle_PointerPressed);
        RightBorder->PointerReleased+=ref new PointerEventHandler(this, &DataGrid::ResizeHandle_PointerReleased);
        RightBorder->PointerMoved+=ref new PointerEventHandler(this, &DataGrid::ResizeHandle_PointerMoved);
        RightBorder->PointerEntered+=ref new PointerEventHandler(this, &DataGrid::ResizeHandle_PointerEntered);
        RightBorder->PointerExited+=ref new PointerEventHandler(this, &DataGrid::ResizeHandle_PointerExited);
    }

    Panel->Children->Append(RightBorder);
    Panel->SetAlignRightWithPanel(RightBorder, true);
    Panel->SetAlignBottomWithPanel(RightBorder, true);
    Panel->SetAlignTopWithPanel(RightBorder, true);
    Panel->SetAlignRightWith(Block, RightBorder);

    Rectangle^ BottomBorder=ref new Rectangle();
    BottomBorder->Name=L"BottomBorder";
    BottomBorder->Fill=ref new SolidColorBrush(Windows::UI::Colors::LightGray);
    BottomBorder->Height=1;

    Panel->Children->Append(BottomBorder);
    Panel->SetAlignRightWithPanel(BottomBorder, true);
    Panel->SetAlignLeftWithPanel(BottomBorder, true);
    Panel->SetAlignBottomWithPanel(BottomBorder, true);
    Panel->SetAlignBottomWith(Block, BottomBorder);

    Panel->Tapped+=ref new TappedEventHandler(this, &DataGrid::Cell_Clicked);
    Panel->RightTapped+=ref new RightTappedEventHandler(this, &DataGrid::Cell_RightClicked);

    RootGrid->Children->Append(Panel);
    RootGrid->SetRow(Panel, Row);
    RootGrid->SetColumn(Panel, Column);

    Block->Text=Content;

    return Panel;
}

//---------------------------------------------------------------------------
void DataGrid::Cell_Clicked(Object^ Sender, TappedRoutedEventArgs^ Event)
{
    unsigned int Row=RootGrid->GetRow(safe_cast<FrameworkElement^>(Sender));
    if (Row==0) // Sorting
    {
        unsigned int Column=RootGrid->GetColumn(safe_cast<FrameworkElement^>(Sender));
        if (Column>=RootGrid->ColumnDefinitions->Size)
            return;

        if (_SortMode->Mode==SortMode::None)
        {
            _SortMode->Mode=SortMode::Descending;
            _SortMode->Column=Column;
        }
        else
        {
            if (_SortMode->Column==Column)
            {
                _SortMode->Mode=_SortMode->Mode==SortMode::Descending?SortMode::Ascending:SortMode::Descending;
            }
            else
            {
                _SortMode->Mode=SortMode::Descending;
                _SortMode->Column=Column;
            }
        }

        Display();
        return;
    }

    if (_SelectedRow)
    {
        unsigned int SelectedRowIndex=0;
        if (!_SortedRows->IndexOf(_SelectedRow, &SelectedRowIndex))
            return;

        if (SelectedRowIndex%2==0)
            _RowsBackgrounds->GetAt(SelectedRowIndex+1)->Fill=ref new SolidColorBrush(Windows::UI::Colors::WhiteSmoke);
        else
            _RowsBackgrounds->GetAt(SelectedRowIndex+1)->Fill=ref new SolidColorBrush(Windows::UI::Colors::White);
    }

    if (Row<_RowsBackgrounds->Size)
        _RowsBackgrounds->GetAt(Row)->Fill=ref new SolidColorBrush(Windows::UI::Colors::LightBlue);

    Row--;
    if (Row>=_SortedRows->Size)
        return;

    _SelectedRow=_SortedRows->GetAt(Row);

    unsigned int OriginalPos=0;
    if(!_Rows->IndexOf(_SelectedRow, &OriginalPos))
        return;

    SelectionChangedEvent(this, OriginalPos);
}

//---------------------------------------------------------------------------
void DataGrid::Cell_RightClicked(Object^ Sender, RightTappedRoutedEventArgs^ Event)
{
    unsigned int Row=RootGrid->GetRow(safe_cast<FrameworkElement^>(Sender));
    if (Row==0)
        return;

    Row--;
    if (Row>=_SortedRows->Size)
        return;

    unsigned int OriginalPos=0;
    if (!_Rows->IndexOf(_SortedRows->GetAt(Row), &OriginalPos))
        return;

    RightClickedEvent(this, ref new RightClickedEventArgs(OriginalPos, Event));
}

//---------------------------------------------------------------------------
void DataGrid::ResizeHandle_PointerEntered(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (Window::Current->CoreWindow->PointerCursor->Type!=CoreCursorType::SizeWestEast)
    {
        _Old_Pointer=Window::Current->CoreWindow->PointerCursor;
        Window::Current->CoreWindow->PointerCursor=ref new CoreCursor(CoreCursorType::SizeWestEast, 0);
    }
}

//---------------------------------------------------------------------------
void DataGrid::ResizeHandle_PointerPressed(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    UIElement^ Handle=safe_cast<UIElement^>(Sender);

    _Resizing=true;
    _Pointer_Count++;

    Handle->CapturePointer(Event->Pointer);
}

//---------------------------------------------------------------------------
void DataGrid::ResizeHandle_PointerReleased(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    UIElement^ Handle=safe_cast<UIElement^>(Sender);
    Handle->ReleasePointerCapture(Event->Pointer);

    if (--_Pointer_Count==0)
    {
        _Resizing=false;
        if (_Old_Pointer && Window::Current->CoreWindow->PointerCursor->Type==CoreCursorType::SizeWestEast)
        {
            Window::Current->CoreWindow->PointerCursor=_Old_Pointer;
            _Old_Pointer=nullptr;
        }
    }
}

//---------------------------------------------------------------------------
void DataGrid::ResizeHandle_PointerMoved(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (_Resizing==true)
    {
        FrameworkElement^ Handle=safe_cast<FrameworkElement^>(Sender);
        unsigned int Col=RootGrid->GetColumn(safe_cast<FrameworkElement^>(Handle->Parent));

        double Width=Event->GetCurrentPoint(safe_cast<FrameworkElement^>(Handle->Parent))->Position.X;
        if (Width<10)
            Width=10;

        RootGrid->ColumnDefinitions->GetAt(Col)->Width=Width;
        _Columns->GetAt(Col)->Width=Width;
    }
}

//---------------------------------------------------------------------------
void DataGrid::ResizeHandle_PointerExited(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (_Resizing==false && _Old_Pointer && Window::Current->CoreWindow->PointerCursor->Type==CoreCursorType::SizeWestEast)
    {
        Window::Current->CoreWindow->PointerCursor=_Old_Pointer;
        _Old_Pointer=nullptr;
    }
}
