/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.net.Uri
import android.content.Intent
import net.mediaarea.mediainfo.databinding.ActivityAboutBinding

class AboutActivity : AppCompatActivity() {
    private lateinit var activityAboutBinding: ActivityAboutBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        activityAboutBinding = ActivityAboutBinding.inflate(layoutInflater)
        setContentView(activityAboutBinding.root)

        setSupportActionBar(activityAboutBinding.toolbar)

        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        activityAboutBinding.aboutAboutTextview.text = activityAboutBinding.aboutAboutTextview.text.toString()
                .replace("%MI_VERSION%", getString(R.string.app_version))
                .replace("%MIL_VERSION%", Core.version)

        activityAboutBinding.websiteBtn.setOnClickListener {
            val intent = Intent(Intent.ACTION_VIEW)
            intent.data = Uri.parse(getString(R.string.website_url))
            startActivity(intent)
        }

        activityAboutBinding.emailBtn.setOnClickListener {
            val intent = Intent(Intent.ACTION_VIEW)
            intent.data = Uri.parse(getString(R.string.email_url))
            startActivity(intent)
        }
    }
}