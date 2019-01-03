/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "ReportPage.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class ReportPage sealed
    {
    public:
        ReportPage();

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
        void OpenFiles_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void OpenFolder_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);

        void UpdateNavigationStack();
        void OnBackRequested();
        void BackRequested(Platform::Object^ Sender, Windows::UI::Core::BackRequestedEventArgs^ Event);
        void Back_Click(Platform::Object^ Sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ Args);
        void ViewItem_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void ExportButton_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void AboutButton_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);

        void NavigateBackForWideState(bool UseTransition);
        bool ShouldGoToWideState();

        void BackButton_Click(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void PageRoot_Loaded(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void PageRoot_Unloaded(Platform::Object^ Sender, Windows::UI::Xaml::RoutedEventArgs^ Event);
        void Window_SizeChanged(Platform::Object^ Sender, Windows::UI::Core::WindowSizeChangedEventArgs^ Event);

        ReportViewModel^ _CurrentReport;
        Windows::Foundation::EventRegistrationToken _SizeChangedEventRegistrationToken;
        Windows::Foundation::EventRegistrationToken _BackRequestedEventRegistrationToken;
    };
}