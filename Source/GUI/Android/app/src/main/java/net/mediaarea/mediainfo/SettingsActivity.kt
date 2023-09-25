/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.content.Context
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle

import net.mediaarea.mediainfo.databinding.ActivitySettingsBinding

class SettingsActivity : AppCompatActivity(), SettingsActivityListener {
    private lateinit var activitySettingsBinding: ActivitySettingsBinding
    override fun getSubscriptionManager(): SubscriptionManager {
        return SubscriptionManager.getInstance(application)
    }

    override fun getContext(): Context {
        return this
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        activitySettingsBinding = ActivitySettingsBinding.inflate(layoutInflater)
        setContentView(activitySettingsBinding.root)


        setSupportActionBar(activitySettingsBinding.toolbar)

        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        supportFragmentManager
                .beginTransaction()
                .replace(R.id.settings_container, SettingsFragment())
                .commit()

    }
}
