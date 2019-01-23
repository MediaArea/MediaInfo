/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "AboutPage.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class AboutPage sealed
    {
    public:
        AboutPage();

    protected:
        virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
        virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

    private:
        void OnBackRequested();
        void Back_Requested(Platform::Object^ Sender, Windows::UI::Core::BackRequestedEventArgs^ Event);
        void BackButton_Click(Platform::Object^ Sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ Event);

        Windows::Foundation::EventRegistrationToken _BackRequestedEventRegistrationToken;
    };
}