/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import android.content.res.Resources
import android.os.Bundle
import android.os.Build
import androidx.appcompat.app.AppCompatDelegate
import androidx.preference.PreferenceFragmentCompat
import androidx.preference.SwitchPreferenceCompat
import androidx.preference.DropDownPreference
import com.yariksoffice.lingver.Lingver
import java.util.*

class SettingsFragment : PreferenceFragmentCompat() {
    private var localeDropdown: DropDownPreference? = null
    private var uimodeDropdown: DropDownPreference? = null
    private var systemLanguageSwitch: SwitchPreferenceCompat? = null

    override fun onCreatePreferences(savedInstanceState: Bundle?, rootKey: String?) {
        setPreferencesFromResource(R.xml.fragment_settings, rootKey)

        //val subscribeButton: Preference? = findPreference(getString(R.string.preferences_subscribe_key))
        localeDropdown = findPreference(getString(R.string.preferences_locale_key))
        uimodeDropdown = findPreference(getString(R.string.preferences_uimode_key))
        systemLanguageSwitch = findPreference(getString(R.string.preferences_report_translate_key))

        /*subscribeButton?.setOnPreferenceClickListener  {
            val intent = Intent(activity, SubscribeActivity::class.java)
            startActivity(intent)
            true
        }*/

        if (activity != null) {
            val activityListener = activity as SettingsActivityListener
            val subscriptionManager = activityListener.getSubscriptionManager()

            /*subscriptionManager.ready.observe(this, Observer {
                if (it==true) {
                    subscribeButton?.isVisible = true
                }
            })*/

            subscriptionManager.subscribed.value?.let {
                updateSubscriptionState(it)
            }

            subscriptionManager.subscribed.observe(this) {
                updateSubscriptionState(it)
            }
        }

        localeDropdown?.setOnPreferenceChangeListener { _, newValue ->
            if (newValue is String) {
                val locale: Locale =
                    if (newValue == "system") {
                        if (Build.VERSION.SDK_INT >= 24) {
                            Resources.getSystem().configuration.locales.get(0)
                        } else {
                            @Suppress("DEPRECATION")
                            Resources.getSystem().configuration.locale
                        }
                    } else {
                        val language = newValue.split("-r")
                        if (language.size > 1) {
                            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.BAKLAVA) {
                                Locale.of(language[0], language[1])
                            } else {
                                @Suppress("DEPRECATION")
                                Locale(language[0], language[1])
                            }
                        } else {
                            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.BAKLAVA) {
                                Locale.of(language[0])
                            } else {
                                @Suppress("DEPRECATION")
                                Locale(language[0])
                            }
                        }
                    }

                Locale.setDefault(locale)

                if (Lingver.getInstance().getLocale() != locale) {
                    val settingsActivity = activity
                    if (settingsActivity != null) {
                        Lingver.getInstance().setLocale(settingsActivity, locale)
                        settingsActivity.recreate()
                    }
                }
            }
            true
        }

        uimodeDropdown?.setOnPreferenceChangeListener { _, newValue ->
            if (newValue is String) {
                when (newValue) {
                    "off" -> {
                        if (AppCompatDelegate.getDefaultNightMode() != AppCompatDelegate.MODE_NIGHT_NO) {
                            AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO)
                        }
                    }

                    "on" -> {
                        if (AppCompatDelegate.getDefaultNightMode() != AppCompatDelegate.MODE_NIGHT_YES) {
                            AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES)
                        }
                    }

                    "system" -> {
                        if (AppCompatDelegate.getDefaultNightMode() != AppCompatDelegate.MODE_NIGHT_FOLLOW_SYSTEM) {
                            AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_FOLLOW_SYSTEM)
                        }
                    }
                }
            }
            true
        }
    }

    fun updateSubscriptionState(value: Boolean) {
        if (value) {
            //subscribeButton?.isVisible = false
            uimodeDropdown?.isEnabled = true
            localeDropdown?.isEnabled = true
            systemLanguageSwitch?.isEnabled = true
        }
    }
}
