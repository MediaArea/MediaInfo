/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.app.Application
import com.google.android.material.color.DynamicColors

import com.yariksoffice.lingver.Lingver
import com.yariksoffice.lingver.store.InMemoryLocaleStore


class MediaInfoApplication : Application() {
    override fun onCreate() {
        DynamicColors.applyToActivitiesIfAvailable(this)

        super.onCreate()

        Lingver.init(this, InMemoryLocaleStore())
    }

    override fun onTerminate() {
        SubscriptionManager.getInstance(this).terminate()

        super.onTerminate()
    }
}