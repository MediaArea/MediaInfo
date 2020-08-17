/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.content.Intent
import android.content.res.Resources
import android.os.Bundle
import android.os.Build
import androidx.appcompat.app.AppCompatDelegate
import androidx.preference.PreferenceFragmentCompat
import androidx.preference.SwitchPreferenceCompat
import androidx.lifecycle.Observer
import androidx.preference.DropDownPreference
import androidx.preference.Preference
import com.yariksoffice.lingver.Lingver
import java.util.*

class SettingsFragment : PreferenceFragmentCompat() {
    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.fragment_settings, rootKey)

        //val subscribeButton: Preference? = findPreference(getString(R.string.preferences_subscribe_key))
        val localeDropdown: DropDownPreference? = findPreference(getString(R.string.preferences_locale_key))
        val uimodeSwitch: SwitchPreferenceCompat? = findPreference(getString(R.string.preferences_uimode_key))
        val systemLanguageSwitch: SwitchPreferenceCompat? = findPreference(getString(R.string.preferences_report_translate_key))

        /*subscribeButton?.setOnPreferenceClickListener  {
            val intent = Intent(activity, SubscribeActivity::class.java)
            startActivity(intent)
            true
        }*/

        if (activity != null) {
            val activityListener = activity as SettingsActivityListener
            val subscriptionManager = activityListener.getSubscriptionManager()

            lifecycle.addObserver(subscriptionManager)

            /*subscriptionManager.ready.observe(this, Observer {
                if (it==true) {
                    subscribeButton?.isVisible = true
                }
            })*/

            subscriptionManager.subscribed.observe(this, Observer {
                if (it==true) {
                    //subscribeButton?.isVisible = false
                    uimodeSwitch?.isEnabled = true
                    localeDropdown?.isEnabled = true
                    systemLanguageSwitch?.isEnabled = true
                }
            })
        }

        localeDropdown?.setOnPreferenceChangeListener { _, newValue -> Boolean
            if (newValue is String) {
                val locale: Locale =
                    if (newValue == "system") {
                        if (Build.VERSION.SDK_INT>=24) {
                            Resources.getSystem().configuration.locales.get(0)
                        }
                        else {
                            @Suppress("DEPRECATION")
                            Resources.getSystem().configuration.locale
                        }
                    }
                    else {
                        val language = newValue.split("-r")
                        if (language.size > 1) {
                            Locale(language[0], language[1])
                        }
                        else {
                            Locale(language[0])
                        }
                    }

                Locale.setDefault(locale)

                if (Lingver.getInstance().getLocale() != locale) {
                    val settingsActivity = activity
                    if(settingsActivity != null) {
                        Lingver.getInstance().setLocale(settingsActivity, locale)
                        settingsActivity.recreate()
                    }
                }
            }
            true
        }

        uimodeSwitch?.setOnPreferenceChangeListener { _, newValue -> Boolean
            when (newValue) {
                false -> {
                    if (AppCompatDelegate.getDefaultNightMode() != AppCompatDelegate.MODE_NIGHT_NO) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO)
                    }
                }
                true -> {
                    if (AppCompatDelegate.getDefaultNightMode() != AppCompatDelegate.MODE_NIGHT_YES) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES)
                    }
                }
            }
            true
        }
    }
}