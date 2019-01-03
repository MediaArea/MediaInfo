/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
*
*  Use of this source code is governed by a BSD-style license that can
*  be found in the License.html file in the root of the source tree.
*/

#pragma once

//---------------------------------------------------------------------------
#include "App.g.h"

//---------------------------------------------------------------------------
namespace MediaInfo
{
    ref class App sealed
    {
    protected:
        virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;
        virtual void OnFileActivated(Windows::ApplicationModel::Activation::FileActivatedEventArgs^ e) override;

    internal:
        App();

    private:
        void OnSuspending(Platform::Object^ Sender, Windows::ApplicationModel::SuspendingEventArgs^ Event);
        void OnNavigationFailed(Platform::Object^ Sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs^ Event);
    };
}