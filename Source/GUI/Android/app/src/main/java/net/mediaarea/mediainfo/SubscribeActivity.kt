/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.Observer
import com.android.billingclient.api.SkuDetails
import com.android.billingclient.api.BillingClient
import com.android.billingclient.api.BillingFlowParams
import android.app.Activity
import android.os.Bundle
import android.view.Gravity
import android.view.MenuItem
import android.view.View

import kotlinx.android.synthetic.main.activity_subscribe.*

class SubscribeActivity : AppCompatActivity() {
    private lateinit var subscriptionManager: SubscriptionManager
    private lateinit var subscriptionDetails: SkuDetails
    private lateinit var lifetimeSubscriptionDetails: SkuDetails

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_subscribe)

        subscriptionManager = SubscriptionManager.getInstance(application)

        setSupportActionBar(toolbar)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        subscriptionManager.details.observe (this, Observer {
            subscriptionDetails = it

            subscribe_button.isEnabled = true
            subscribe_button.text = subscribe_button.text.toString()
                    .replace("%PRICE%", subscriptionDetails.price)
            subscription_detail_text.visibility= View.VISIBLE
            subscription_detail_text.gravity=Gravity.CENTER_HORIZONTAL
        })

        subscriptionManager.lifetimeDetails.observe (this, Observer {
            lifetimeSubscriptionDetails = it

            lifetime_subscribe_button.isEnabled = true
            lifetime_subscribe_button.text = lifetime_subscribe_button.text.toString()
                    .replace("%PRICE%", lifetimeSubscriptionDetails.price)
        })

        subscribe_button.setOnClickListener {
            if (::subscriptionDetails.isInitialized) {
                val request = BillingFlowParams.newBuilder()
                    .setSkuDetails(subscriptionDetails)
                    .build()
                if (subscriptionManager.launchBillingFlow(this, request) == BillingClient.BillingResponseCode.OK) {
                    finish()
                }
            }
        }

        lifetime_subscribe_button.setOnClickListener {
            if (::lifetimeSubscriptionDetails.isInitialized) {
                val request = BillingFlowParams.newBuilder()
                    .setSkuDetails(lifetimeSubscriptionDetails)
                    .build()
                if (subscriptionManager.launchBillingFlow(this, request) == BillingClient.BillingResponseCode.OK) {
                    setResult(Activity.RESULT_OK)
                    finish()
                }
            }
        }
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if (item.itemId == android.R.id.home) {
            setResult(Activity.RESULT_CANCELED)
            finish()
        }

        return super.onOptionsItemSelected(item)
    }
}
