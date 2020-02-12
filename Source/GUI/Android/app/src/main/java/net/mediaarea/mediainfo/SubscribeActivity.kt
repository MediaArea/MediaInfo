/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import androidx.appcompat.app.AppCompatActivity
import com.android.billingclient.api.BillingClient
import com.android.billingclient.api.BillingFlowParams
import android.os.Bundle
import android.view.Gravity
import android.view.MenuItem
import android.view.View

import kotlinx.android.synthetic.main.activity_subscribe.*

class SubscribeActivity : AppCompatActivity() {
    private lateinit var subscriptionManager: SubscriptionManager

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_subscribe)

        subscriptionManager = SubscriptionManager.getInstance(application)

        setSupportActionBar(toolbar)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        subscriptionManager.querySkuDetailsAsync { responseCode, skuDetailsList ->
            if (responseCode==BillingClient.BillingResponse.OK)
            {
                skuDetailsList?.forEach {
                    if (it.sku==getString(R.string.subscription_sku)) {
                        subscription_detail_text.text = subscription_detail_text.text.toString()
                                .replace("%PRICE%", it.price)
                        subscription_detail_text.visibility= View.VISIBLE
                        subscription_detail_text.gravity=Gravity.CENTER_HORIZONTAL
                    }
                }
            }
        }

        subscribe_button.setOnClickListener {
            val request = BillingFlowParams.newBuilder()
                    .setSku(getString(R.string.subscription_sku))
                    .setType(BillingClient.SkuType.SUBS)
                    .build()
            if (subscriptionManager.launchBillingFlow(this, request)==BillingClient.BillingResponse.OK) {
                finish()
            }
        }
    }

    override fun onOptionsItemSelected(item: MenuItem?): Boolean {
        if (item?.itemId == android.R.id.home) {
            finish()
        }

        return super.onOptionsItemSelected(item)
    }
}
