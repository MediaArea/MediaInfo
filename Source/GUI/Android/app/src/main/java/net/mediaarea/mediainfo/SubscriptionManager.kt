/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicBoolean

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlinx.coroutines.delay
import kotlin.math.pow
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleObserver
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.OnLifecycleEvent
import android.util.Log
import android.app.Activity
import android.app.Application
import com.android.billingclient.api.*

class SubscriptionManager private constructor(private val application: Application) : PurchasesUpdatedListener, BillingClientStateListener, LifecycleObserver {
    val ready = MutableLiveData<Boolean>()
    val subscribed = MutableLiveData<Boolean>()
    val isLifetime = MutableLiveData<Boolean>()
    val details = MutableLiveData<SkuDetails>()
    val lifetimeDetails = MutableLiveData<SkuDetails>()

    private var detailsAvailables = AtomicBoolean(false)
    private var lifetimeDetailsAvailables = AtomicBoolean(false)

    private lateinit var billingClient: BillingClient

    @OnLifecycleEvent(Lifecycle.Event.ON_CREATE)
    fun create() {
        billingClient = BillingClient.newBuilder(application.applicationContext)
                .enablePendingPurchases()
                .setListener(this)
                .build()

        ready.value = false
        subscribed.value = false
        isLifetime.value = false

        billingClient.startConnection(this)
    }

    @OnLifecycleEvent(Lifecycle.Event.ON_DESTROY)
    fun destroy() {
        if (billingClient.isReady) {
            billingClient.endConnection()
        }
    }

    private fun updateState(newState: Boolean) {
        if (ready.value!=newState) {
            ready.value = newState
        }
    }

    private fun updateSubscribedState(newState: Boolean) {
        if (subscribed.value!=newState) {
            subscribed.value = newState
        }
    }

    private fun updateLifetimeState(newState: Boolean) {
        if (isLifetime.value!=newState) {
            isLifetime.value = newState
        }
    }

    private fun isSubscriptionSupported(): Boolean {
        val response = billingClient.isFeatureSupported(BillingClient.FeatureType.SUBSCRIPTIONS)
        return response.responseCode==BillingClient.BillingResponseCode.OK
    }

    fun launchBillingFlow(activity: Activity, params: BillingFlowParams): Int {
        if (!billingClient.isReady) {
            Log.e(LOG_TAG, "BillingClient is not ready to start billing flow")
        }
        val response = billingClient.launchBillingFlow(activity, params)
        Log.i(LOG_TAG, "Launch Billing Flow Response Code: $response")
        return response.responseCode
    }

    override fun onPurchasesUpdated(p0: BillingResult, p1: MutableList<Purchase>?) {
        if (p0.responseCode==BillingClient.BillingResponseCode.OK) {
            handlePurchases(p1)
        } else if (p0.responseCode==BillingClient.BillingResponseCode.DEVELOPER_ERROR) {
            Log.e(LOG_TAG, "Your app's configuration is incorrect. Review in the Google Play Console. Possible causes of this error include: APK is not signed with release key; SKU productId mismatch.")
        }
    }

    override fun onBillingSetupFinished(p0: BillingResult) {
        if (p0.responseCode==BillingClient.BillingResponseCode.OK) {
            RetryPolicies.resetConnectionRetryPolicyCounter()

            fun updatePurchasesTask() {
                billingClient.queryPurchasesAsync(BillingClient.SkuType.SUBS) { result: BillingResult, purchaseList: MutableList<Purchase> ->
                    if (result.responseCode == BillingClient.BillingResponseCode.OK) {
                        handlePurchases(purchaseList)
                    }
                }
                billingClient.queryPurchasesAsync(BillingClient.SkuType.INAPP) { result: BillingResult, purchaseList: MutableList<Purchase> ->
                    if (result.responseCode == BillingClient.BillingResponseCode.OK) {
                        handlePurchases(purchaseList)
                    }
                }
            }
            RetryPolicies.taskExecutionRetryPolicy(billingClient, this) { updatePurchasesTask() }

            if (isSubscriptionSupported()) {
                val params = SkuDetailsParams
                        .newBuilder()
                        .setSkusList(listOf(application.getString(R.string.subscription_sku)))
                        .setType(BillingClient.SkuType.SUBS)
                        .build()

                billingClient.querySkuDetailsAsync(params) { result: BillingResult, list: List<SkuDetails>? ->
                    if (result.responseCode == BillingClient.BillingResponseCode.OK) {
                        list?.forEach {
                            if (it.sku == application.getString(R.string.subscription_sku)) {
                                details.postValue(it)
                                detailsAvailables.set(true)
                            }
                        }

                        if (detailsAvailables.get() && lifetimeDetailsAvailables.get()) {
                            ready.postValue(true)
                        }
                    }
                }
            }

            val params = SkuDetailsParams
               .newBuilder()
               .setSkusList(listOf(application.getString(R.string.lifetime_subscription_sku)))
               .setType(BillingClient.SkuType.INAPP)
               .build()

            billingClient.querySkuDetailsAsync(params) { result: BillingResult, list: MutableList<SkuDetails>? ->
                if (result.responseCode == BillingClient.BillingResponseCode.OK) {
                    list?.forEach {
                        if (it.sku == application.getString(R.string.lifetime_subscription_sku)) {
                            lifetimeDetails.postValue(it)
                            lifetimeDetailsAvailables.set(true)
                        }
                    }

                    if (detailsAvailables.get() && lifetimeDetailsAvailables.get()) {
                        ready.postValue(true)
                    }
                }
            }
        } else  {
            updateState(false)
            Log.d(LOG_TAG, "onBillingSetupFinished with failure response code: ${p0.responseCode}")
        }
    }

    // Disconnected from the Play BillingClient
    override fun onBillingServiceDisconnected() {
        updateState(false)
        RetryPolicies.connectionRetryPolicy { billingClient.startConnection(this) }
    }

    private fun handlePurchases(purchasesList: List<Purchase>?) {
        if (purchasesList == null)
            return

        if (purchasesList.isNotEmpty()) {
            purchasesList.forEach { purchase ->
                if (!purchase.isAcknowledged) {
                    val purchaseParams = AcknowledgePurchaseParams.newBuilder()
                            .setPurchaseToken(purchase.purchaseToken)
                            .build()

                    billingClient.acknowledgePurchase(purchaseParams)  {
                    }
                }

                if (purchase.purchaseState == Purchase.PurchaseState.PENDING)
                    return

                purchase.skus.forEach { sku ->
                    when (sku) {
                        application.getString(R.string.subscription_sku) -> {
                            subscribed.postValue(true)
                        }
                        application.getString(R.string.lifetime_subscription_sku) -> {
                            isLifetime.postValue(true)
                            subscribed.postValue(true)
                        }
                    }
                }
            }
        }
    }

    // Retries handler
    private object RetryPolicies {
        private const val maxRetry = 5
        private const val taskDelay = 2000L
        private const val baseDelayMillis = 500
        private var retryCounter = AtomicInteger(1)

        fun resetConnectionRetryPolicyCounter() {
            retryCounter.set(1)
        }

        fun connectionRetryPolicy(block: () -> Unit) {
            val scope = CoroutineScope(Job() + Dispatchers.Main)
            scope.launch {
                val counter = retryCounter.getAndIncrement()
                if (counter < maxRetry) {
                    val waitTime: Long = (2f.pow(counter) * baseDelayMillis).toLong()
                    delay(waitTime)
                    block()
                }
            }
        }

        fun taskExecutionRetryPolicy(billingClient: BillingClient, listener: SubscriptionManager, task: () -> Unit) {
            val scope = CoroutineScope(Job() + Dispatchers.Main)
            scope.launch {
                if (!billingClient.isReady) {
                    billingClient.startConnection(listener)
                    delay(taskDelay)
                }
                task()
            }
        }
    }

    companion object {
        private const val LOG_TAG = "MediaInfo/Billing"

        @Volatile
        private var INSTANCE: SubscriptionManager? = null
        fun getInstance(app: Application): SubscriptionManager = INSTANCE?: synchronized(this) {
            INSTANCE ?: SubscriptionManager(app).also { INSTANCE = it }
        }
    }
}
