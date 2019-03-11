/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "SheetView.xaml.h"

#include "MediaInfo/MediaInfo_Internal.h"
#include "ZenLib/Ztring.h"
#include "ZenLib/ZtringList.h"
#include "ZenLib/ZtringListList.h"

//---------------------------------------------------------------------------
using namespace MediaInfoLib;
using namespace MediaInfo;
using namespace ZenLib;

using namespace concurrency;
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
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Storage;
using namespace Windows::Storage::AccessCache;
using namespace Windows::Storage::Streams;


//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
SheetView::SheetView(ReportViewModel^ CurrentReport) : _CurrentReport(CurrentReport), _Sheets(ref new Vector<Sheet^>()),
                                                       _Reports(ref new Vector<IVectorView<Platform::String^>^>()),
                                                       _ReportVectorChangedToken(EventRegistrationToken())
{
    InitializeComponent();

    int StreamPanel_Size=320;
    ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
    if (LocalSettings->Values->HasKey(L"SheetPanel_Size"))
    {
        StreamPanel_Size=static_cast<uint32>(LocalSettings->Values->Lookup(L"SheetPanel_Size"));
    }
    StreamRow->Height=StreamPanel_Size;

    Get_Saved_Sheets();

    _ReportVectorChangedToken=ReportDataSource::GetAllReports()->VectorChanged+=ref new VectorChangedEventHandler<Report^>(this, &SheetView::ReportDataSource_VectorChanged);
}

//---------------------------------------------------------------------------
SheetView::~SheetView()
{
    ReportDataSource::GetAllReports()->VectorChanged-=_ReportVectorChangedToken;
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void SheetView::ReportDataSource_VectorChanged(IObservableVector<Report^>^ Sender, IVectorChangedEventArgs^ Event)
{
    unsigned int Pos=0;
    switch (Event->CollectionChange)
    {
    case CollectionChange::ItemInserted:
        if (Event->Index<=_Reports->Size)
            _Reports->InsertAt(Event->Index, Get_Values(ref new ReportViewModel(ReportDataSource::GetReportAt(Event->Index))));
        break;
    case CollectionChange::ItemRemoved:
        if (Event->Index<_Reports->Size)
        {
            if (Event->Index==MainGrid->SelectedRow)
                _CurrentReport->Source=ReportDataSource::GetAllReports()->Size?ReportDataSource::GetReportAt(0):nullptr;
            _Reports->RemoveAt(Event->Index);
        }
        break;
    case CollectionChange::Reset:
        _Reports->Clear();
        for (Report^ Cur:ReportDataSource::GetAllReports())
            _Reports->Append(Get_Values(ref new ReportViewModel(Cur)));
        break;
    }

    if (_CurrentReport && _CurrentReport->Source)
    {
        unsigned int ReportPos=0;
        if (ReportDataSource::GetAllReports()->IndexOf(_CurrentReport->Source, &ReportPos))
            MainGrid->Select(_Reports->GetAt(ReportPos));
    }
}

//---------------------------------------------------------------------------
void SheetView::Configure()
{
    MainGrid->Reset();

    _Reports->Clear();
    for (Report^ Cur : ReportDataSource::GetAllReports())
        _Reports->Append(Get_Values(ref new ReportViewModel(Cur)));

    for (Field^ Cur : _CurrentSheet->Fields)
    {
        GridLength Width=GridLengthHelper::FromValueAndType(Cur->Length?Cur->Length:1, GridUnitType::Star);
        MainGrid->Add_Column(Width, Cur->Title);
    }
    MainGrid->Rows=_Reports;

        if (_CurrentReport && _CurrentReport->Source)
        {
            unsigned int ReportPos=0;
            if (ReportDataSource::GetAllReports()->IndexOf(_CurrentReport->Source, &ReportPos))
                MainGrid->Select(_Reports->GetAt(ReportPos));
        }
}

//---------------------------------------------------------------------------
void SheetView::Save_Sheets()
{
      ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
      ApplicationDataCompositeValue^ Composite=ref new ApplicationDataCompositeValue();

      for (Sheet^ Cur : _Sheets)
      {
          Platform::String^ Name=Cur->Name;

          ZtringListList Fields;
          for (Field^ Cur2 : Cur->Fields)
          {
              ZtringList Z1;
              Z1.Insert(Cur2->Title->Data(), 0);
              Z1.Insert(Ztring().From_Number(Cur2->StreamKind), 1);
              Z1.Insert(Ztring().From_Number(Cur2->StreamPos), 2);
              Z1.Insert(Cur2->Parameter->Data(), 3);
              Z1.Insert(Ztring().From_Number(Cur2->Length), 4);

              Fields.Insert(Z1, Fields.size());
          }

          Composite->Insert(Cur->Name, ref new Platform::String(Fields.Read().c_str()));
      }

      LocalSettings->Values->Insert(L"Sheets", Composite);
}

//---------------------------------------------------------------------------
void SheetView::Get_Saved_Sheets()
{
    ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
    Object^ Obj=LocalSettings->Values->Lookup(L"Sheets");

    if (!Obj)
    {
        // Default sheet
        Vector<Field^>^ Fields=ref new Vector<Field^>();
        Fields->Append(ref new Field("File Name", 0, 0, "FileNameExtension", 3));
        Fields->Append(ref new Field("Format", 0, 0, "Format", 1));
        Fields->Append(ref new Field("Video Formats", 0, 0, "Video_Format_List", 1));
        Fields->Append(ref new Field("Audio Formats", 0, 0, "Audio_Format_List", 1));
        Fields->Append(ref new Field("Text Formats", 0, 0, "Text_Format_List", 1));
        _Sheets->Append(ref new Sheet("Default", Fields));
    }
    else {
        ApplicationDataCompositeValue^ Composite=safe_cast<ApplicationDataCompositeValue^>(Obj);
        for (IKeyValuePair<Platform::String^, Platform::Object^>^ Cur : Composite)
        {
            Vector<Field^>^ Fields=ref new Vector<Field^>();

            for (ZtringList CurrentField : ZtringListList((safe_cast<Platform::String^>(Cur->Value))->Data()))
            {
                if (CurrentField.size()<4)
                    continue;

                if (CurrentField.size()==4)
                    Fields->Append(ref new Field(ref new Platform::String(CurrentField[0].c_str()), CurrentField[1].To_int32u(), 0, ref new Platform::String(CurrentField[2].c_str()), CurrentField[3].To_int32u()));
                else // Size >=5
                    Fields->Append(ref new Field(ref new Platform::String(CurrentField[0].c_str()), CurrentField[1].To_int32u(), CurrentField[2].To_int32u(), ref new Platform::String(CurrentField[3].c_str()), CurrentField[4].To_int32u()));
            }
            _Sheets->Append(ref new Sheet(Cur->Key, Fields));
        }
    }

    FormatBox->ItemsSource=_Sheets;
    FormatBox->SelectedIndex=0;
}

//---------------------------------------------------------------------------
void SheetView::Get_Streams()
{
    if (!_CurrentReport || !_CurrentReport->Source)
        return;

    StreamBox->Items->Clear();
    StreamBlock->Text=L"";

    TextBlock^ SummaryContent=ref new TextBlock();
    SummaryContent->Text=L"Summary";

    ComboBoxItem^ SummaryItem=ref new ComboBoxItem();
    Array<size_t>^ SummaryPos=ref new Array<size_t>(2);
    SummaryPos[0]=static_cast<size_t>(-1);
    SummaryPos[1]=static_cast<size_t>(-1);
    SummaryItem->Tag=SummaryPos;
    SummaryItem->Content=SummaryContent;

    StreamBox->Items->Append(SummaryItem);

    for (size_t StreamKind=0; StreamKind<static_cast<size_t>(Stream_Max); StreamKind++)
    {
        size_t StreamCount=_CurrentReport->Count_Get(StreamKind);
        for (size_t StreamPos=0; StreamPos<StreamCount; StreamPos++)
        {
            TextBlock^ Content=ref new TextBlock();
            Content->Text=AppCore::Get_Stream_Name(StreamKind)+": "+_CurrentReport->Summary(StreamKind, StreamPos);

            ComboBoxItem^ Item=ref new ComboBoxItem();
            Array<size_t>^ Pos=ref new Array<size_t>(2);
            Pos[0]=StreamKind;
            Pos[1]=StreamPos;
            Item->Tag=Pos;
            Item->Content=Content;

            StreamBox->Items->Append(Item);
        }
    }

    StreamBox->SelectedIndex=0;
}

//---------------------------------------------------------------------------
IVectorView<Platform::String^>^ SheetView::Get_Values(ReportViewModel ^ Source)
{
    Vector<Platform::String^>^ Fields=ref new Vector<Platform::String^>;
    for (Field^ Cur : _CurrentSheet->Fields)
        Fields->Append(Source->Get(Cur->StreamKind, Cur->StreamPos, Cur->Parameter));

    return Fields->GetView();
}

//---------------------------------------------------------------------------
void SheetView::StreamBox_SelectionChanged(Object^ Sender, SelectionChangedEventArgs^ Event)
{
    if (!_CurrentReport || !_CurrentReport->Source)
        return;

    Object^ Obj=StreamBox->SelectedItem;
    if (!Obj || Obj->GetType()->FullName!=TypeName(ComboBoxItem::typeid).Name)
        return;

    ComboBoxItem^ Item=safe_cast<ComboBoxItem^>(Obj);
    Array<size_t>^ Pos=safe_cast<Array<size_t>^>(Item->Tag);

    if (Pos->get(0)==static_cast<size_t>(-1))
    {
        Platform::String^ Summary=L"";
        for (size_t StreamKind=0; StreamKind<static_cast<size_t>(Stream_Max); StreamKind++)
        {
            for (size_t StreamPos=0; StreamPos<_CurrentReport->Count_Get(StreamKind); StreamPos++)
            {
                Summary+=_CurrentReport->Summary(StreamKind, StreamPos)+"\r\n";
            }
        }

        StreamBlock->Text=Summary;
    }
    else
    {
        StreamBlock->Text=_CurrentReport->Inform_Get(Pos[0], Pos[1]);
    }
}

//---------------------------------------------------------------------------
void SheetView::FormatBox_SelectionChanged(Object^ Sender, SelectionChangedEventArgs^ Event)
{
    if (FormatBox->SelectedIndex==0)
        DeleteSheet->IsEnabled=false;
    else
        DeleteSheet->IsEnabled=true;

    if (!FormatBox->SelectedItem)
        return;

    _CurrentSheet=safe_cast<Sheet^>(FormatBox->SelectedItem);

    Configure();
}

//---------------------------------------------------------------------------
void SheetView::EditSheet_Click(Object^ Sender, RoutedEventArgs^ Event)
{
    if (!FormatBox->SelectedItem)
        return;

    SheetEditDialog^ Dialog=ref new SheetEditDialog(L"Edit Sheet", safe_cast<Sheet^>(FormatBox->SelectedItem));
    create_task(Dialog->ShowAsync()).then([this](ContentDialogResult Result) {
        if (Result==ContentDialogResult::Primary)
        {
            Save_Sheets();
            unsigned int Index=FormatBox->SelectedIndex;
            Configure();
        }
    });
}

//---------------------------------------------------------------------------
void SheetView::AddSheet_Click(Object^ Sender, RoutedEventArgs^ Event)
{
    Sheet^ NewSheet=ref new Sheet(L"New Sheet", ref new Vector<Field^>());
    SheetEditDialog^ Dialog=ref new SheetEditDialog(L"New Sheet", NewSheet);
    create_task(Dialog->ShowAsync()).then([this, NewSheet](ContentDialogResult Result) {
        if (Result==ContentDialogResult::Primary)
        {
            _Sheets->Append(NewSheet);
            Save_Sheets();
        }
    });
}

//---------------------------------------------------------------------------
void SheetView::DeleteSheet_Click(Object^ Sender, RoutedEventArgs^ Event)
{
    if (!FormatBox->SelectedItem || FormatBox->SelectedIndex==0)
        return;

    unsigned int Index=0;
    if (_Sheets->IndexOf(safe_cast<Sheet^>(FormatBox->SelectedItem), &Index))
    {
        FormatBox->SelectedIndex=0;
        _Sheets->RemoveAt(Index);
        Save_Sheets();
    }
}

//---------------------------------------------------------------------------
void SheetView::MainGrid_SelectionChangedEvent(DataGrid^ Sender, uint32 Pos)
{
    if (Pos>=ReportDataSource::GetAllReports()->Size)
        return;

    _CurrentReport->Source=ReportDataSource::GetReportAt(Pos);
    Get_Streams();
}

//---------------------------------------------------------------------------
void SheetView::MainGrid_RightClickedEvent(DataGrid^ Sender, RightClickedEventArgs^ Event)
{
    CloseMenuItem->Tag=Event->Pos;
    CloseMenuFlyout->ShowAt(Sender, Event->Event->GetPosition(Sender));
}

//---------------------------------------------------------------------------
void SheetView::ResizePanel_PointerEntered(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (Window::Current->CoreWindow->PointerCursor->Type!=CoreCursorType::SizeNorthSouth)
    {
        _Old_Pointer=Window::Current->CoreWindow->PointerCursor;
        Window::Current->CoreWindow->PointerCursor=ref new CoreCursor(CoreCursorType::SizeNorthSouth, 0);
    }
}

//---------------------------------------------------------------------------
void SheetView::ResizePanel_PointerPressed(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    _Resizing=true;
    _Pointer_Count++;

    ResizePanel->CapturePointer(Event->Pointer);
}

//---------------------------------------------------------------------------
void SheetView::ResizePanel_PointerReleased(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    ResizePanel->ReleasePointerCapture(Event->Pointer);

    if (--_Pointer_Count==0)
    {
        _Resizing=false;

        ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
        LocalSettings->Values->Insert(L"SheetPanel_Size", static_cast<uint32>(round(StreamRow->Height.Value)));

        if (_Old_Pointer && Window::Current->CoreWindow->PointerCursor->Type==CoreCursorType::SizeNorthSouth)
        {
            Window::Current->CoreWindow->PointerCursor=_Old_Pointer;
            _Old_Pointer=nullptr;
        }
    }
}

//---------------------------------------------------------------------------
void SheetView::ResizePanel_PointerMoved(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (_Resizing==true)
    {
        double Height=StreamRow->ActualHeight-Event->GetCurrentPoint(StreamContainer)->Position.Y;
        if (Height>0)
            StreamRow->Height=Height;
    }
}

//---------------------------------------------------------------------------
void SheetView::ResizePanel_PointerExited(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (_Resizing==false && _Old_Pointer && Window::Current->CoreWindow->PointerCursor->Type==CoreCursorType::SizeNorthSouth)
    {
        Window::Current->CoreWindow->PointerCursor=_Old_Pointer;
        _Old_Pointer=nullptr;
    }
}

//---------------------------------------------------------------------------
void SheetView::CloseMenuItem_Click(MenuFlyoutItem^ Sender, RoutedEventArgs^ Event)
{
    unsigned int Pos=static_cast<unsigned int>(Sender->Tag);
    Windows::Foundation::Collections::IObservableVector<Report^>^ Reports=ReportDataSource::GetAllReports();

    if (Reports->Size<=Pos)
        return;

    if (_CurrentReport->Source==Reports->GetAt(Pos))
        _CurrentReport->Source=nullptr;

    Reports->RemoveAt(Pos);
    Configure();
}
