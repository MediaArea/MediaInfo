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

import net.mediaarea.mediainfo.databinding.ActivitySubscribeBinding

class SubscribeActivity : AppCompatActivity() {
    private lateinit var activitySubscribeBinding: ActivitySubscribeBinding
    private lateinit var subscriptionManager: SubscriptionManager
    private lateinit var subscriptionDetails: SkuDetails
    private lateinit var lifetimeSubscriptionDetails: SkuDetails

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)


        activitySubscribeBinding = ActivitySubscribeBinding.inflate(layoutInflater)
        setContentView(activitySubscribeBinding.root)

        subscriptionManager = SubscriptionManager.getInstance(application)

        setSupportActionBar(activitySubscribeBinding.toolbar)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        subscriptionManager.details.observe (this, Observer {
            subscriptionDetails = it

            activitySubscribeBinding.subscribeButton.isEnabled = true
            activitySubscribeBinding.subscribeButton.text = activitySubscribeBinding.subscribeButton.text.toString()
                    .replace("%PRICE%", subscriptionDetails.price)
            activitySubscribeBinding.subscriptionDetailText.visibility = View.VISIBLE
            activitySubscribeBinding.subscriptionDetailText.gravity = Gravity.CENTER_HORIZONTAL

            activitySubscribeBinding.subscriptionConditionText.text = activitySubscribeBinding.subscriptionConditionText.text.toString()
                    .replace("%PRICE%", subscriptionDetails.price)
            activitySubscribeBinding.subscriptionConditionText.visibility = View.VISIBLE
            activitySubscribeBinding.subscriptionConditionText.gravity = Gravity.CENTER_HORIZONTAL
        })

        subscriptionManager.lifetimeDetails.observe (this, Observer {
            lifetimeSubscriptionDetails = it

            activitySubscribeBinding.lifetimeSubscribeButton.isEnabled = true
            activitySubscribeBinding.lifetimeSubscribeButton.text = activitySubscribeBinding.lifetimeSubscribeButton.text.toString()
                    .replace("%PRICE%", lifetimeSubscriptionDetails.price)
        })

        activitySubscribeBinding.subscribeButton.setOnClickListener {
            if (::subscriptionDetails.isInitialized) {
                val request = BillingFlowParams.newBuilder()
                    .setSkuDetails(subscriptionDetails)
                    .build()
                if (subscriptionManager.launchBillingFlow(this, request) == BillingClient.BillingResponseCode.OK) {
                    finish()
                }
            }
        }

        activitySubscribeBinding.lifetimeSubscribeButton.setOnClickListener {
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
