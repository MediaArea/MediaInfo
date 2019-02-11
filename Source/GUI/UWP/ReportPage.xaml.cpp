/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "ReportPage.xaml.h"

#include "ZenLib/Dir.h"
#include "ZenLib/FileName.h"
#include "Zenlib/Ztring.h"
#include "ZenLib/ZtringList.h"

//---------------------------------------------------------------------------
using namespace ZenLib;
using namespace MediaInfo;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Media::Animation;
using namespace Windows::UI::ViewManagement;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage;
using namespace concurrency;


//---------------------------------------------------------------------------
// Constructor/Destructor
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ReportPage::ReportPage() : _ViewChangedEventRegistrationToken(), _BackRequestedEventRegistrationToken(), _SizeChangedEventRegistrationToken(), _CurrentReport(ref new ReportViewModel(nullptr))
{
    InitializeComponent();
    CoreApplication::GetCurrentView()->TitleBar->ExtendViewIntoTitleBar=true;
    Window::Current->SetTitleBar(BackgroundElement);
}

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void ReportPage::Open_Files(IVectorView<IStorageItem^>^ Files)
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
void ReportPage::Open_Files_Internal(Windows::Foundation::Collections::IVectorView<Platform::String^>^ Paths)
{
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
            for (Report^ NewReport:ToAdd)
                ReportDataSource::AddReport(NewReport);
        }));

        return ToAdd->Size?ToAdd->GetAt(ToAdd->Size-1):nullptr;
    }).then([this](task<Report^> Task)
    {
        Report^ NewReport=Task.get();

        CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this, NewReport]()
        {
            if (NewReport)
            {
                CurrentReport->Source=NewReport;
                TitleText->Text+=_CurrentReport->Name;

                Show_Report();
            }
            WaitPanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
            SpinnerRing->IsActive=false;
            LayoutRoot->AllowDrop=true;
        }));
    });
}

//---------------------------------------------------------------------------
void ReportPage::Open_Directory(IStorageItem^ Directory)
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
void ReportPage::On_File_DragOver(Object^, DragEventArgs^ Event)
{
    Event->AcceptedOperation=DataPackageOperation::Copy;

    if (Event->DragUIOverride)
    {
        Event->DragUIOverride->Caption=L"Add file";
        Event->DragUIOverride->IsContentVisible=true;
    }

    AddFilePanel->Visibility=Windows::UI::Xaml::Visibility::Visible;
}

//---------------------------------------------------------------------------
void ReportPage::On_File_DragLeave(Object^, DragEventArgs^)
{
    AddFilePanel->Visibility=Windows::UI::Xaml::Visibility::Collapsed;
}

//---------------------------------------------------------------------------
void ReportPage::On_File_Drop(Object^, DragEventArgs^ Event)
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
void ReportPage::OpenFiles_Click(Object^, RoutedEventArgs^) {
    // Verify that we are currently snapped and fail silently if we can't unsnap
    if (ApplicationView::Value == ApplicationViewState::Snapped && !ApplicationView::TryUnsnap())
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
void ReportPage::OpenFolder_Click(Object^, RoutedEventArgs^) {
    // Verify that we are currently snapped and fail silently if we can't unsnap
    if (ApplicationView::Value == ApplicationViewState::Snapped && !ApplicationView::TryUnsnap())
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
void ReportPage::OnNavigatedTo(NavigationEventArgs^ e)
{
    Page::OnNavigatedTo(e);

    if (e->Parameter)
        CurrentReport->Source=safe_cast<Report^>(e->Parameter);

    _ViewChangedEventRegistrationToken=AppCore::ViewChangedEvent+=ref new ViewChangedEventHandler([this](Platform::String^ New)
    {
        Show_Report();
    });


    _SizeChangedEventRegistrationToken=Window::Current->SizeChanged+=ref new WindowSizeChangedEventHandler(this, &ReportPage::Window_SizeChanged);

    // Register for hardware and software back request from the system
    SystemNavigationManager^ systemNavigationManager=SystemNavigationManager::GetForCurrentView();
    _BackRequestedEventRegistrationToken=systemNavigationManager->BackRequested+=ref new EventHandler<BackRequestedEventArgs^>(this, &ReportPage::BackRequested);
}

//---------------------------------------------------------------------------
void ReportPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
    if(e->SourcePageType.Name==TypeName(AboutPage::typeid).Name)
        UpdateNavigationStack();

    AppCore::ViewChangedEvent-=_ViewChangedEventRegistrationToken;
    Window::Current->SizeChanged-=_SizeChangedEventRegistrationToken;

    SystemNavigationManager^ systemNavigationManager=SystemNavigationManager::GetForCurrentView();
    systemNavigationManager->BackRequested-=_BackRequestedEventRegistrationToken;
    systemNavigationManager->AppViewBackButtonVisibility=AppViewBackButtonVisibility::Collapsed;

    Page::OnNavigatedFrom(e);
}

//---------------------------------------------------------------------------
void ReportPage::PageRoot_Loaded(Object^, RoutedEventArgs^)
{
    // We shouldn't see this page since we are in "wide master-detail" mode.
    // Play a transition as we are likely navigating back from a separate page.
    if (ShouldGoToWideState())
        NavigateBackForWideState(true);
    else
        Show_Report();
}

//---------------------------------------------------------------------------
void ReportPage::PageRoot_Unloaded(Object^, RoutedEventArgs^)
{
}

//---------------------------------------------------------------------------
void ReportPage::BackButton_Click(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^)
{
    OnBackRequested();
}

//---------------------------------------------------------------------------
void ReportPage::BackRequested(Object^, BackRequestedEventArgs^ Event)
{
    // Mark event as handled so we don't get bounced out of the app.
    Event->Handled=true;

    OnBackRequested();
}

//---------------------------------------------------------------------------
void ReportPage::UpdateNavigationStack()
{
    IVector<PageStackEntry^>^ BackStack=Frame->BackStack;
    unsigned int BackStackCount=BackStack->Size;

    if (BackStackCount>0)
    {
        PageStackEntry^ MasterPageEntry=BackStack->GetAt(BackStackCount-1);
        BackStack->RemoveAt(BackStackCount-1);

        // Doctor the navigation parameter for the master page so it
        // will show the correct item in the side-by-side view.
        PageStackEntry^ ModifiedEntry=ref new PageStackEntry(
            MasterPageEntry->SourcePageType,
           CurrentReport->Source,
            MasterPageEntry->NavigationTransitionInfo
        );
        BackStack->Append(ModifiedEntry);
    }
}

//---------------------------------------------------------------------------
void ReportPage::OnBackRequested()
{
    UpdateNavigationStack();

    Frame->GoBack(ref new DrillInNavigationTransitionInfo());
}

//---------------------------------------------------------------------------
void ReportPage::Window_SizeChanged(Object^, WindowSizeChangedEventArgs^)
{
    if (ShouldGoToWideState())
    {
        // Make sure we are no longer listening to window change events.
        Window::Current->SizeChanged-=_SizeChangedEventRegistrationToken;

        // We shouldn't see this page since we are in "wide master-detail" mode.
        NavigateBackForWideState(false);
    }
}

//---------------------------------------------------------------------------
bool ReportPage::ShouldGoToWideState()
{
    return (Window::Current->Bounds.Width>=720.0);
}

//---------------------------------------------------------------------------
void ReportPage::NavigateBackForWideState(bool UseTransition)
{
    UpdateNavigationStack();

    if (UseTransition)
        Frame->GoBack(ref new EntranceNavigationTransitionInfo());
    else
        Frame->GoBack(ref new SuppressNavigationTransitionInfo());
}

//---------------------------------------------------------------------------
void ReportPage::Back_Click(Object^, ItemClickEventArgs^)
{
    OnBackRequested();
}

//---------------------------------------------------------------------------
void ReportPage::Show_Report()
{
    if (ShouldGoToWideState())
        NavigateBackForWideState(true);

    if (AppCore::View==L"Easy")
    {
        DetailContentPresenter->Content=ref new EasyView(_CurrentReport);
        TitleText->Text=L"MediaInfo - "+CurrentReport->Name;
    }
    else if (AppCore::View==L"Sheet")
    {
        NavigateBackForWideState(true);
    }
    else
    {
        DetailContentPresenter->Content=ref new HtmlView(_CurrentReport);
        TitleText->Text=L"MediaInfo - "+CurrentReport->Name;
    }

        DetailContentPresenter->ContentTransitions->Clear();
        DetailContentPresenter->ContentTransitions->Append(ref new EntranceThemeTransition());
}

//---------------------------------------------------------------------------
void ReportPage::ExportButton_Click(Object^, RoutedEventArgs^)
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

    if (Mime == L"text/xml")
    {
        Type="XML file";
        Extension=".xml";
    }
    else if (Mime == L"text/html")
    {
        Type="HTML file";
        Extension=".html";
    }
    else if (Mime == L"text/json")
    {
        Type="JSON file";
        Extension=".json";
    }

    FileSavePicker^ Picker=ref new FileSavePicker();
    Picker->SuggestedStartLocation=PickerLocationId::DocumentsLibrary;
    Picker->SuggestedFileName=CurrentReport->Name + Extension;
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
void ReportPage::AboutButton_Click(Object^, RoutedEventArgs^)
{
    Frame->Navigate(TypeName(AboutPage::typeid), nullptr);
}

//---------------------------------------------------------------------------
void ReportPage::ViewListMenu_Opening(Platform::Object^, Platform::Object^)
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
void ReportPage::ViewListMenu_Closing(FlyoutBase^, FlyoutBaseClosingEventArgs^)
{
    ViewListMenu->Items->Clear();
}
