/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

//---------------------------------------------------------------------------
#include "pch.h"
#include "AboutPage.xaml.h"

#include "ZenLib/Ztring.h"

//---------------------------------------------------------------------------
using namespace MediaInfo;
using namespace ZenLib;

using namespace Platform;
using namespace Windows::System;
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
using namespace Windows::UI::Xaml::Media::Animation;

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
AboutPage::AboutPage()
{
    InitializeComponent();
    CoreApplication::GetCurrentView()->TitleBar->ExtendViewIntoTitleBar=true;
    Window::Current->SetTitleBar(BackgroundElement);

    Ztring AboutText=Ztring(AboutTextBlock->Text->Data());
    AboutText.FindAndReplace(__T("MI_VERSION"), AppCore::AppVersion->Data());
    AboutText.FindAndReplace(__T("MIL_VERSION"), AppCore::Version->Data());
    AboutTextBlock->Text=ref new Platform::String(AboutText.c_str());
}

//---------------------------------------------------------------------------
void AboutPage::OnNavigatedTo(NavigationEventArgs^ e)
{
    Page::OnNavigatedTo(e);

    // Register for hardware and software back request from the system
    SystemNavigationManager^ systemNavigationManager=SystemNavigationManager::GetForCurrentView();
    _BackRequestedEventRegistrationToken=systemNavigationManager->BackRequested+=ref new EventHandler<BackRequestedEventArgs^>(this, &AboutPage::Back_Requested);
}

//---------------------------------------------------------------------------
void AboutPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
    Page::OnNavigatedFrom(e);

    SystemNavigationManager^ systemNavigationManager=SystemNavigationManager::GetForCurrentView();
    systemNavigationManager->BackRequested-=_BackRequestedEventRegistrationToken;
    systemNavigationManager->AppViewBackButtonVisibility=AppViewBackButtonVisibility::Collapsed;
}

//---------------------------------------------------------------------------
void AboutPage::OnBackRequested()
{
    Frame->GoBack(ref new DrillInNavigationTransitionInfo());
}

//---------------------------------------------------------------------------
void AboutPage::Back_Requested(Object^, BackRequestedEventArgs^ Event)
{
    // Mark event as handled so we don't get bounced out of the app.
    Event->Handled=true;

    OnBackRequested();
}

//---------------------------------------------------------------------------
void AboutPage::BackButton_Click(Platform::Object^, Windows::UI::Xaml::Controls::ItemClickEventArgs^)
{
    OnBackRequested();
}