/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo.tv

import android.app.Application
import net.mediaarea.mediainfo.tv.feature.settings.SettingsRepository

class MediaInfoApplication : Application() {
    val settingsRepository by lazy { SettingsRepository(applicationContext) }
    val factory by lazy { ViewModelFactory(this, settingsRepository) }
}
