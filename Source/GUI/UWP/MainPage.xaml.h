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

    private:
        void Open_Files_Internal(Windows::Foundation::Collections::IVectorView<Platform::String^>^ Paths);
        void On_File_DragOver(Platform::Object^, Windows::UI::Xaml::DragEventArgs^ Event);
        void On_File_DragLeave(Platform::Object^, Windows::UI::Xaml::DragEventArgs^);
        void On_File_Drop(Platform::Object^, Windows::UI::Xaml::DragEventArgs^ Event);
        void LayoutRoot_Loaded(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void OpenFiles_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void OpenFolder_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void ExportButton_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void AboutButton_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void Show_Report();
        void ViewItem_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void MasterListView_ItemClick(Platform::Object^ Sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ Event);
        void MasterListView_ContainerContentChanging(Windows::UI::Xaml::Controls::ListViewBase^ Sender, Windows::UI::Xaml::Controls::ContainerContentChangingEventArgs^ Args);
        void ListViewItem_Delete_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void AdaptiveStates_CurrentStateChanged(Platform::Object^ Sender, Windows::UI::Xaml::VisualStateChangedEventArgs^ Event);
        void UpdateForVisualState(Windows::UI::Xaml::VisualState^ NewState, Windows::UI::Xaml::VisualState^ OldState=nullptr);
        void EnableContentTransitions();
        void DisableContentTransitions();
        void Update_Ui();

        ReportViewModel^ _CurrentReport;
    };
}