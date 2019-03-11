/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "MainPage.xaml.h"

#include "ZenLib/Dir.h"
#include "ZenLib/FileName.h"
#include "Zenlib/Ztring.h"
#include "ZenLib/ZtringList.h"
#include "ZenLib/ZtringListList.h"

//---------------------------------------------------------------------------
using namespace MediaInfo;
using namespace ZenLib;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::System::Profile;
using namespace Windows::ApplicationModel::Core;
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
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage;
using namespace concurrency;

//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
MainPage::MainPage() : _CurrentReport(ref new ReportViewModel(nullptr)), _CurrentState(WindowState::None), _ViewChangedEventRegistrationToken(), _Resizing(false), _Pointer_Count(0), _Old_Pointer(nullptr)
{
    InitializeComponent();
    CoreApplication::GetCurrentView()->TitleBar->ExtendViewIntoTitleBar=true;
    Window::Current->SetTitleBar(BackgroundElement);

    // Folder picker isn't supported on xBox platforms, see https://docs.microsoft.com/en-us/uwp/extension-sdks/uwp-limitations-on-xbox
    Ztring Platform=Ztring(AnalyticsInfo::VersionInfo->DeviceFamily->Data());
    if (Platform.MakeLowerCase().find(__T("xbox"))!=std::string::npos)
        OpenFolder->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void MainPage::Open_Files(IVectorView<IStorageItem^>^ Files)
{
    if (!Files)
        return;

    Vector<Platform::String^>^ Paths=ref new Vector<Platform::String^>();
    for (IStorageItem^ File : Files)
    {
        if (!File)
            continue;

        // Add file to FUA
        AppCore::Add_Item_To_FutureAccessList(File);
        Paths->Append(File->Path);
    }

    if (Paths->Size)
        Open_Files_Internal(Paths->GetView());
}

//---------------------------------------------------------------------------
void MainPage::Open_Directory(IStorageItem^ Directory)
{
    // Add directory to FUA
    AppCore::Add_Item_To_FutureAccessList(Directory);

    ZtringList Files=Dir::GetAllFileNames(Ztring(Directory->Path->Data()));
    Vector<Platform::String^>^ Paths=ref new Vector<Platform::String^>();
    for (Ztring File:Files)
        Paths->Append(ref new Platform::String(File.c_str()));

    if (Paths->Size)
        Open_Files_Internal(Paths->GetView());
}

//---------------------------------------------------------------------------
void MainPage::OnNavigatedTo(NavigationEventArgs ^ e)
{
    Page::OnNavigatedTo(e);

    _ViewChangedEventRegistrationToken=AppCore::ViewChangedEvent+=ref new ViewChangedEventHandler([this](Platform::String^ New)
    {
        if (_CurrentState==WindowState::Narrow && _CurrentReport && _CurrentReport->Source && New!=L"Sheet")
            this->Frame->Navigate(TypeName(ReportPage::typeid), _CurrentReport->Source, ref new DrillInNavigationTransitionInfo());
        else
            Show_Report();
    });

    MasterListView->ItemsSource=ReportDataSource::GetAllReports();

    if (e->Parameter)
    {
        CurrentReport->Source=safe_cast<Report^>(e->Parameter);
        Show_Report();
    }

    // Don't play a content transition for first item load.
    DisableContentTransitions();
}

//---------------------------------------------------------------------------
void MainPage::OnNavigatedFrom(NavigationEventArgs ^ e)
{
    AppCore::ViewChangedEvent-=_ViewChangedEventRegistrationToken;

    Page::OnNavigatedFrom(e);
}

//---------------------------------------------------------------------------
void MainPage::Open_Files_Internal(Windows::Foundation::Collections::IVectorView<Platform::String^>^ Paths)
{
    WelcomePanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
    WaitPanel->Visibility=Windows::UI::Xaml::Visibility::Visible;
    SpinnerRing->IsActive=true;
    LayoutRoot->AllowDrop=false;

    create_task([Paths]() -> Report^
    {
        Vector<Report^>^ ToAdd=ref new Vector<Report^>();
        for (Platform::String^ Path : Paths)
        {
            SYSTEMTIME SystemTime;
            GetSystemTime(&SystemTime);

            FILETIME FileTime;
            SystemTimeToFileTime(&SystemTime, &FileTime);

            ULARGE_INTEGER Id;
            Id.LowPart=FileTime.dwLowDateTime;
            Id.HighPart=FileTime.dwHighDateTime;

            Platform::String^ Name=ref new Platform::String(FileName::Name_Get(Ztring(Path->Data())).c_str());
            Name+=L".";
            Name+=ref new Platform::String(FileName::Extension_Get(Ztring(Path->Data())).c_str());

            Platform::String^ Version=AppCore::Version;
            Platform::String^ Data=AppCore::Create_Report(Path);

            Report^ NewReport=ref new Report(Id.QuadPart, Name, Data, Version);

            ToAdd->Append(NewReport);
        }

        CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([ToAdd]()
        {
            for (Report^ NewReport : ToAdd)
                ReportDataSource::AddReport(NewReport);
        }));

        return ToAdd->Size?ToAdd->GetAt(ToAdd->Size-1):nullptr;
    }).then([this](task<Report^> Task)
    {
        Report^ NewReport=Task.get();

        CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this, NewReport]()
        {

            if (!CurrentReport->Source && NewReport)
            {
                CurrentReport->Source=NewReport;
                Show_Report();
            }
            WaitPanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
            SpinnerRing->IsActive=false;
            LayoutRoot->AllowDrop=true;
        }));
    });
}

//---------------------------------------------------------------------------
void MainPage::On_File_DragOver(Object^, DragEventArgs^ Event)
{
    Event->AcceptedOperation=DataPackageOperation::Copy;

    if (Event->DragUIOverride)
    {
        Event->DragUIOverride->Caption=L"Add file";
        Event->DragUIOverride->IsContentVisible=true;
    }

    WelcomePanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
    AddFilePanel->Visibility=Windows::UI::Xaml::Visibility::Visible;
}

//---------------------------------------------------------------------------
void MainPage::On_File_DragLeave(Object^, DragEventArgs^)
{
    AddFilePanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
    Update_Ui();
}

//---------------------------------------------------------------------------
void MainPage::On_File_Drop(Object^, DragEventArgs^ Event)
{
    if (Event->DataView->Contains(StandardDataFormats::StorageItems))
    {
        create_task(Event->DataView->GetStorageItemsAsync()).then([this](IVectorView<IStorageItem^>^ Items)
        {
            if (Items->Size)
                Open_Files(Items);
        });
    }

    AddFilePanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
}

//---------------------------------------------------------------------------
void MainPage::LayoutRoot_Loaded(Object^, RoutedEventArgs^)
{
    Update_Ui();
}

//---------------------------------------------------------------------------
void MainPage::OpenFiles_Click(Object^, RoutedEventArgs^) {
    // Verify that we are currently snapped and fail silently if we can't unsnap
    if (ApplicationView::Value==ApplicationViewState::Snapped && !ApplicationView::TryUnsnap())
        return;

    FileOpenPicker^ Picker=ref new FileOpenPicker();
    Picker->ViewMode=PickerViewMode::Thumbnail;
    Picker->SuggestedStartLocation=PickerLocationId::VideosLibrary;
    Picker->FileTypeFilter->Append("*");

    create_task(Picker->PickMultipleFilesAsync()).then([this](IVectorView<StorageFile^>^ Files)
    {
        Open_Files(safe_cast<IVectorView<IStorageItem^>^>(Files));
    });
}

//---------------------------------------------------------------------------
void MainPage::OpenFolder_Click(Object^, RoutedEventArgs^) {
    // Verify that we are currently snapped and fail silently if we can't unsnap
    if (ApplicationView::Value==ApplicationViewState::Snapped && !ApplicationView::TryUnsnap())
        return;

    FolderPicker^ Picker=ref new FolderPicker();
    Picker->ViewMode=PickerViewMode::Thumbnail;
    Picker->SuggestedStartLocation=PickerLocationId::VideosLibrary;
    Picker->FileTypeFilter->Append("*"); // Ignored, but picker fail with exception if not present

    create_task(Picker->PickSingleFolderAsync()).then([this](StorageFolder^ Directory)
    {
        if (Directory)
            Open_Directory(Directory);
    });
}

//---------------------------------------------------------------------------
void MainPage::ExportButton_Click(Object^, RoutedEventArgs^)
{
    // Verify that we are currently snapped and fail silently if we can't unsnap
    if (ApplicationView::Value==ApplicationViewState::Snapped && !ApplicationView::TryUnsnap())
        return;

    if (!CurrentReport)
        return;

    Platform::String^ Content=CurrentReport->ReportText;
    Platform::String^ Mime=AppCore::Get_Mime(AppCore::View);
    Platform::String^ Type="Text file";
    Platform::String^ Extension=".txt";

    if (Mime==L"text/xml")
    {
        Type="XML file";
        Extension=".xml";
    }
    else if (Mime==L"text/html")
    {
        Type="HTML file";
        Extension=".html";
    }
    else if (Mime==L"text/json")
    {
        Type="JSON file";
        Extension=".json";
    }

    FileSavePicker^ Picker=ref new FileSavePicker();
    Picker->SuggestedStartLocation=PickerLocationId::DocumentsLibrary;
    Picker->SuggestedFileName=CurrentReport->Name+Extension;
    Picker->FileTypeChoices->Insert(Type, ref new Vector<Platform::String^>(1, Extension));

    create_task(Picker->PickSaveFileAsync()).then([Content](StorageFile^ File)
    {
        if (!File)
            return;

       CachedFileManager::DeferUpdates(File);

        create_task(FileIO::WriteTextAsync(File, Content)).then([File]()
        {
            create_task(CachedFileManager::CompleteUpdatesAsync(File));
        });
    });
}

//---------------------------------------------------------------------------
void MainPage::AboutButton_Click(Object^, RoutedEventArgs^)
{
    Frame->Navigate(TypeName(AboutPage::typeid), nullptr, ref new DrillInNavigationTransitionInfo());
}

//---------------------------------------------------------------------------
void MainPage::Show_Report()
{
    MasterListView->SelectedItem=CurrentReport->Source;

    if (AppCore::View==L"Easy")
    {
        DetailContentPresenter->Content=ref new EasyView(_CurrentReport);
        TitleText->Text=L"MediaInfo - "+CurrentReport->Name;
    }
    else if (AppCore::View==L"Sheet")
    {
        DetailContentPresenter->Content=ref new SheetView(_CurrentReport);
        TitleText->Text=L"MediaInfo";
    }
    else
    {
        DetailContentPresenter->Content=ref new HtmlView(_CurrentReport);
        TitleText->Text=L"MediaInfo - "+CurrentReport->Name;
    }

    ViewListButton->IsEnabled=true;
    ExportButton->IsEnabled=true;

    EnableContentTransitions();
    Update_Ui();
}

//---------------------------------------------------------------------------
void MainPage::MasterListView_ItemClick(Object^, ItemClickEventArgs^ Event)
{
    Report^ Item=safe_cast<Report^>(Event->ClickedItem);
    CurrentReport->Source=Item;

    if (_CurrentState==WindowState::Narrow)
        Frame->Navigate(TypeName(ReportPage::typeid), CurrentReport->Source, ref new DrillInNavigationTransitionInfo());
    else
        Show_Report();
}

//---------------------------------------------------------------------------
void MainPage::MasterListView_ContainerContentChanging(ListViewBase^, ContainerContentChangingEventArgs^)
{
    Update_Ui();
}

//---------------------------------------------------------------------------
void MainPage::ListViewItem_Delete_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^)
{
    Button^ Btn=safe_cast<Button^>(Sender);
    uint64 Id=safe_cast<uint64>(Btn->Tag);

    if (_CurrentReport && _CurrentReport->Source && _CurrentReport->Source->Id==Id)
    {
        _CurrentReport->Source=nullptr;
        ViewListButton->IsEnabled=false;
        ExportButton->IsEnabled=false;
        TitleText->Text=L"MediaInfo";

        DetailContentPresenter->Content=nullptr;
    }

    ReportDataSource::DeleteReport(ReportDataSource::GetReportById(Id));
}

//---------------------------------------------------------------------------
void MainPage::ResizePanel_PointerEntered(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (Window::Current->CoreWindow->PointerCursor->Type!=CoreCursorType::SizeWestEast)
    {
        _Old_Pointer=Window::Current->CoreWindow->PointerCursor;
        Window::Current->CoreWindow->PointerCursor=ref new CoreCursor(CoreCursorType::SizeWestEast, 0);
    }
}

//---------------------------------------------------------------------------
void MainPage::ResizePanel_PointerPressed(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    _Resizing=true;
    _Pointer_Count++;

    ResizePanel->CapturePointer(Event->Pointer);
}

//---------------------------------------------------------------------------
void MainPage::ResizePanel_PointerReleased(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    ResizePanel->ReleasePointerCapture(Event->Pointer);

    if (--_Pointer_Count==0)
    {
        _Resizing=false;

        ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
        LocalSettings->Values->Insert(L"FilePanel_Size", static_cast<uint32>(round(MasterColumn->Width.Value)));

        if (_Old_Pointer && Window::Current->CoreWindow->PointerCursor->Type==CoreCursorType::SizeWestEast)
        {
            Window::Current->CoreWindow->PointerCursor=_Old_Pointer;
            _Old_Pointer=nullptr;
        }
    }
}

//---------------------------------------------------------------------------
void MainPage::ResizePanel_PointerMoved(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (_Resizing==true)
    {
        double Width=Event->GetCurrentPoint(MasterListView)->Position.X;
        if (Width<10)
            Width=10;
        else if (Width>Window::Current->Bounds.Width-21) // 10px minimal control size + separator column size
            Width=Window::Current->Bounds.Width-21;

        MasterColumn->Width=Width;
    }
}

//---------------------------------------------------------------------------
void MainPage::ResizePanel_PointerExited(Object^ Sender, PointerRoutedEventArgs^ Event)
{
    if (_Resizing==false && _Old_Pointer && Window::Current->CoreWindow->PointerCursor->Type==CoreCursorType::SizeWestEast)
    {
        Window::Current->CoreWindow->PointerCursor=_Old_Pointer;
        _Old_Pointer=nullptr;
    }
}

//---------------------------------------------------------------------------
void MainPage::EnableContentTransitions()
{
    DetailContentPresenter->ContentTransitions->Clear();
    DetailContentPresenter->ContentTransitions->Append(ref new EntranceThemeTransition());
}

//---------------------------------------------------------------------------
void MainPage::DisableContentTransitions()
{
    if (DetailContentPresenter)
        DetailContentPresenter->ContentTransitions->Clear();
}

//---------------------------------------------------------------------------
void MediaInfo::MainPage::Update_Ui()
{
    if (MasterListView->Items->Size)
    {
        WelcomePanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
        DetailContentPresenter->BorderBrush->Opacity=255;
        ResizePanel->Visibility=Windows::UI::Xaml::Visibility::Visible;
    }
    else
    {
        WelcomePanel->Visibility=Windows::UI::Xaml::Visibility::Visible;
        DetailContentPresenter->BorderBrush->Opacity=0;
        ResizePanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
        DetailContentPresenter->Content=nullptr;
    }

    if (AppCore::View==L"Sheet")
    {
        MasterColumn->Width=0;
        SeparatorColumn->Width=0;
        DetailColumn->Width=GridLengthHelper::FromValueAndType(1, GridUnitType::Star);

        SectionSeparator->Visibility=Windows::UI::Xaml::Visibility::Visible;
        ViewListButton->Visibility=Windows::UI::Xaml::Visibility::Visible;
        ExportButton->Visibility=Windows::UI::Xaml::Visibility::Visible;

        return;
    }

    if (_CurrentState==WindowState::Narrow)
    {
        MasterColumn->Width=GridLengthHelper::FromValueAndType(1, GridUnitType::Star);
        SeparatorColumn->Width=0;
        DetailColumn->Width=0;

        MasterListView->SelectionMode=ListViewSelectionMode::None;

        SectionSeparator->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
        ViewListButton->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
        ExportButton->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
    }
    else
    {
        int FilePanel_Size=320;
        ApplicationDataContainer^ LocalSettings=ApplicationData::Current->LocalSettings;
        if (LocalSettings->Values->HasKey(L"FilePanel_Size"))
        {
            uint32 SavedSize=static_cast<uint32>(LocalSettings->Values->Lookup(L"FilePanel_Size"));
            if (SavedSize>0&&SavedSize<Window::Current->Bounds.Width-21)
                FilePanel_Size=SavedSize;
        }
        MasterColumn->Width=FilePanel_Size;
        SeparatorColumn->Width=11;
        DetailColumn->Width=GridLengthHelper::FromValueAndType(1, GridUnitType::Star);;

        MasterListView->SelectionMode=ListViewSelectionMode::Single;

        SectionSeparator->Visibility=Windows::UI::Xaml::Visibility::Visible;
        ViewListButton->Visibility=Windows::UI::Xaml::Visibility::Visible;
        ExportButton->Visibility=Windows::UI::Xaml::Visibility::Visible;
    }
}

//---------------------------------------------------------------------------
void MainPage::LayoutRoot_SizeChanged(Object^ Sender, SizeChangedEventArgs^ Event)
{
//    bool IsNarrow=Window::Current->Bounds.Width<720.0;
    WindowState NewState=Window::Current->Bounds.Width<720.0?WindowState::Narrow:WindowState::Wide;
    if (Window::Current->Bounds.Width<=MasterColumn->ActualWidth && NewState!=WindowState::Narrow)
        MasterColumn->Width=320; // Reset to default

    if (NewState!=_CurrentState)
    {
        WindowState OldState=_CurrentState;
        _CurrentState=NewState;
        if (OldState==WindowState::Wide && NewState==WindowState::Narrow && _CurrentReport && _CurrentReport->Source && AppCore::View!=L"Sheet")
            Frame->Navigate(TypeName(ReportPage::typeid), CurrentReport->Source, ref new SuppressNavigationTransitionInfo());
        else
            Update_Ui();
    }
}

//---------------------------------------------------------------------------
void MainPage::ViewListMenu_Opening(Platform::Object^, Platform::Object^)
{
    // Populate views list menu
    for (::MediaInfo::View^ It:AppCore::ViewList)
    {
        MenuFlyoutItem^ Item=ref new MenuFlyoutItem();
        Item->Tag=It->Name;
        Item->Text=It->Desc;
        if (It->Current)
        {
            FontIcon^ Radio=ref new FontIcon();
            Radio->FontFamily=ref new Windows::UI::Xaml::Media::FontFamily(L"Segoe MDL2 Assets");
            Radio->Glyph=L"\uF137";

            Item->Icon=Radio;
        }

        Item->Click+=ref new RoutedEventHandler([this](Object^ Sender, RoutedEventArgs^)
        {
            AppCore::View=safe_cast<Platform::String^>((safe_cast<MenuFlyoutItemBase^>(Sender))->Tag);
        });
        ViewListMenu->Items->Append(Item);
    }
}

//---------------------------------------------------------------------------
void MainPage::ViewListMenu_Closing(FlyoutBase^, FlyoutBaseClosingEventArgs^)
{
    ViewListMenu->Items->Clear();
}
