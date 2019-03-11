/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "AppCore.h"

//---------------------------------------------------------------------------
using namespace MediaInfo;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
App::App()
{
    InitializeComponent();

    Suspending+=ref new SuspendingEventHandler(this, &App::OnSuspending);
}

//---------------------------------------------------------------------------
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ Event)
{
    Frame^ RootFrame=dynamic_cast<Frame^>(Window::Current->Content);

    if (!RootFrame)
    {
        RootFrame=ref new Frame();
        RootFrame->NavigationFailed+=ref new NavigationFailedEventHandler(this, &App::OnNavigationFailed);

        if (!Event->PrelaunchActivated)
            Window::Current->Content=RootFrame;
    }

    if (!Event->PrelaunchActivated)
    {
        if (!RootFrame->Content)
            RootFrame->Navigate(TypeName(MainPage::typeid), Event->Arguments);

        Window::Current->Activate();
    }

    MainPage^ Page=dynamic_cast<MainPage^>(RootFrame->Content);
}

//---------------------------------------------------------------------------
void App::OnSuspending(Object^, SuspendingEventArgs^)
{
}

//---------------------------------------------------------------------------
void App::OnNavigationFailed(Platform::Object^, NavigationFailedEventArgs^ Event)
{
    throw ref new FailureException("Failed to load Page "+Event->SourcePageType.Name);
}

//---------------------------------------------------------------------------
void App::OnFileActivated(FileActivatedEventArgs^ Event)
{
    if (!Event->Files->Size)
        return;

    Frame^ RootFrame=dynamic_cast<Frame^>(Window::Current->Content);

    if (!RootFrame)
    {
        RootFrame=ref new Frame();
        RootFrame->NavigationFailed+=ref new NavigationFailedEventHandler(this, &App::OnNavigationFailed);
        Window::Current->Content=RootFrame;

        if (!RootFrame->Content)
            RootFrame->Navigate(TypeName(MainPage::typeid), nullptr);
    }
    else if (!RootFrame->Content || TypeName(RootFrame->Content->GetType()).Name!=TypeName(MainPage::typeid).Name)
    {
        RootFrame->Navigate(TypeName(MainPage::typeid), nullptr);


    }

    Window::Current->Activate();

    MainPage^ Page=dynamic_cast<MainPage^>(RootFrame->Content);
    Page->Open_Files(Event->Files);
}