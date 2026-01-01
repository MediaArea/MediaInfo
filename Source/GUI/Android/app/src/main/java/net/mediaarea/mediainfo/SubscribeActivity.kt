/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.Observer
import com.android.billingclient.api.BillingClient
import com.android.billingclient.api.ProductDetails
import com.android.billingclient.api.BillingFlowParams
import com.android.billingclient.api.BillingFlowParams.ProductDetailsParams
import android.os.Bundle
import android.view.Gravity
import android.view.MenuItem
import android.view.View
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.updatePadding

import net.mediaarea.mediainfo.databinding.ActivitySubscribeBinding

class SubscribeActivity : AppCompatActivity() {
    private lateinit var activitySubscribeBinding: ActivitySubscribeBinding
    private lateinit var subscriptionManager: SubscriptionManager
    private lateinit var subscriptionOffer: ProductDetails.SubscriptionOfferDetails
    private lateinit var subscriptionDetails: ProductDetails
    private lateinit var lifetimeSubscriptionDetails: ProductDetails

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)


        activitySubscribeBinding = ActivitySubscribeBinding.inflate(layoutInflater)
        setContentView(activitySubscribeBinding.root)

        ViewCompat.setOnApplyWindowInsetsListener(activitySubscribeBinding.appBar) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                left = bars.left,
                top = bars.top,
                right = bars.right,
            )
            WindowInsetsCompat.CONSUMED
        }
        ViewCompat.setOnApplyWindowInsetsListener(activitySubscribeBinding.scrollView) { v: View, insets: WindowInsetsCompat ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars()
                    or WindowInsetsCompat.Type.displayCutout())
            v.updatePadding(
                bottom = bars.bottom,
            )
            WindowInsetsCompat.CONSUMED
        }

        subscriptionManager = SubscriptionManager.getInstance(application)

        setSupportActionBar(activitySubscribeBinding.toolbar)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        subscriptionManager.details.value?.let {
            subscriptionDetails = it
            updateSubscriptionDetails()
        }

        subscriptionManager.details.observe (this, Observer {
            subscriptionDetails = it
            updateSubscriptionDetails()
        })

        subscriptionManager.lifetimeDetails.value?.let {
            lifetimeSubscriptionDetails = it
            updateLifetimeSubscriptionDetails()
        }

        subscriptionManager.lifetimeDetails.observe (this, Observer {
            lifetimeSubscriptionDetails = it
            updateLifetimeSubscriptionDetails()
        })

        subscriptionManager.offer.value?.let {
            subscriptionOffer = it
        }

        subscriptionManager.offer.observe (this, Observer {
            subscriptionOffer = it
        })

        activitySubscribeBinding.subscribeButton.setOnClickListener {
            if (::subscriptionDetails.isInitialized && ::subscriptionOffer.isInitialized) {
                val request = BillingFlowParams.newBuilder()
                    .setProductDetailsParamsList(
                        listOf(
                            ProductDetailsParams.newBuilder()
                                .setProductDetails(subscriptionDetails)
                                .setOfferToken(subscriptionOffer.offerToken)
                                .build()
                        )
                    )
                    .build()

                if (subscriptionManager.launchBillingFlow(this, request) == BillingClient.BillingResponseCode.OK) {
                    setResult(RESULT_OK)
                    finish()
                }
            }
        }

        activitySubscribeBinding.lifetimeSubscribeButton.setOnClickListener {
            if (::lifetimeSubscriptionDetails.isInitialized) {

                val request = BillingFlowParams.newBuilder()
                    .setProductDetailsParamsList(listOf(
                        ProductDetailsParams.newBuilder()
                            .setProductDetails(lifetimeSubscriptionDetails)
                            .build()
                    ))
                    .build()
                if (subscriptionManager.launchBillingFlow(this, request) == BillingClient.BillingResponseCode.OK) {
                    setResult(RESULT_OK)
                    finish()
                }
            }
        }
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if (item.itemId == android.R.id.home) {
            setResult(RESULT_CANCELED)
            finish()
        }

        return super.onOptionsItemSelected(item)
    }

    fun updateSubscriptionDetails() {
        val price = subscriptionDetails.subscriptionOfferDetails?.firstOrNull().let { details ->
            details?.pricingPhases?.pricingPhaseList?.find { phase ->
                phase.priceAmountMicros > 0
            }.let { pricing ->
                pricing?.formattedPrice
            }
        }

        if (price != null) {
            activitySubscribeBinding.subscribeButton.isEnabled = true
            activitySubscribeBinding.subscribeButton.text =
                activitySubscribeBinding.subscribeButton.text.toString()
                    .replace("%PRICE%", price)
            activitySubscribeBinding.subscriptionDetailText.visibility = View.VISIBLE
            activitySubscribeBinding.subscriptionDetailText.gravity = Gravity.CENTER_HORIZONTAL

            activitySubscribeBinding.subscriptionConditionText.text =
                activitySubscribeBinding.subscriptionConditionText.text.toString()
                    .replace("%PRICE%", price)
            activitySubscribeBinding.subscriptionConditionText.visibility = View.VISIBLE
            activitySubscribeBinding.subscriptionConditionText.gravity =
                Gravity.CENTER_HORIZONTAL
        }
    }

    fun updateLifetimeSubscriptionDetails() {
        val price = lifetimeSubscriptionDetails.oneTimePurchaseOfferDetails?.formattedPrice
        if (price != null) {
            activitySubscribeBinding.lifetimeSubscribeButton.isEnabled = true
            activitySubscribeBinding.lifetimeSubscribeButton.text = activitySubscribeBinding.lifetimeSubscribeButton.text.toString()
                .replace("%PRICE%", price)
        }
    }
}
