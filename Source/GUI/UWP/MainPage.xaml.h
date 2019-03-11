/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "MainPage.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    //---------------------------------------------------------------------------
    [Windows::Foundation::Metadata::WebHostHidden]
    public enum class WindowState
    {
        None,
        Wide,
        Narrow
    };

    //---------------------------------------------------------------------------
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class MainPage sealed
    {
    public:
        MainPage();

         property ReportViewModel^ CurrentReport
         {
             ReportViewModel^ get() { return _CurrentReport; }
         }

         void Open_Files(Windows::Foundation::Collections::IVectorView<Windows::Storage::IStorageItem^>^ Files);
         void Open_Directory(Windows::Storage::IStorageItem^ Directory);

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        void Open_Files_Internal(Windows::Foundation::Collections::IVectorView<Platform::String^>^ Paths);
        void On_File_DragOver(Platform::Object^, Windows::UI::Xaml::DragEventArgs^ Event);
        void On_File_DragLeave(Platform::Object^, Windows::UI::Xaml::DragEventArgs^);
        void On_File_Drop(Platform::Object^, Windows::UI::Xaml::DragEventArgs^ Event);
        void LayoutRoot_Loaded(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void LayoutRoot_SizeChanged(Platform::Object^ Sender, Windows::UI::Xaml::SizeChangedEventArgs^ Event);
        void OpenFiles_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void OpenFolder_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void ExportButton_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void AboutButton_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void Show_Report();
        void MasterListView_ItemClick(Platform::Object^ Sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ Event);
        void MasterListView_ContainerContentChanging(Windows::UI::Xaml::Controls::ListViewBase^ Sender, Windows::UI::Xaml::Controls::ContainerContentChangingEventArgs^ Args);
        void ListViewItem_Delete_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        //void AdaptiveStates_CurrentStateChanged(Platform::Object^ Sender, Windows::UI::Xaml::VisualStateChangedEventArgs^ Event);
        void ResizePanel_PointerEntered(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        void ResizePanel_PointerPressed(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        void ResizePanel_PointerReleased(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        void ResizePanel_PointerMoved(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        void ResizePanel_PointerExited(Platform::Object^ Sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ Event);
        //void UpdateForVisualState(Windows::UI::Xaml::VisualState^ NewState, Windows::UI::Xaml::VisualState^ OldState=nullptr);
        void EnableContentTransitions();
        void DisableContentTransitions();
        void Update_Ui();

        ReportViewModel^ _CurrentReport;

        WindowState _CurrentState;
        Windows::Foundation::EventRegistrationToken _ViewChangedEventRegistrationToken;

        bool _Resizing;
        uint16 _Pointer_Count;
        Windows::UI::Core::CoreCursor^ _Old_Pointer;
        void ViewListMenu_Opening(Platform::Object^ sender, Platform::Object^ e);
        void ViewListMenu_Closing(Windows::UI::Xaml::Controls::Primitives::FlyoutBase^ sender, Windows::UI::Xaml::Controls::Primitives::FlyoutBaseClosingEventArgs^ args);
    };
}