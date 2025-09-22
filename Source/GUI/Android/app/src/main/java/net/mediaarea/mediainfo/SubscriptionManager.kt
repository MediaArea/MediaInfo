/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

package net.mediaarea.mediainfo

import java.util.concurrent.atomic.AtomicBoolean

import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.DefaultLifecycleObserver
import android.util.Log
import android.app.Activity
import android.app.Application
import com.android.billingclient.api.*

class SubscriptionManager private constructor(private val application: Application) : PurchasesUpdatedListener, BillingClientStateListener {
    val ready = MutableLiveData<Boolean>()
    val subscribed = MutableLiveData<Boolean>()
    val isLifetime = MutableLiveData<Boolean>()
    val offer = MutableLiveData<ProductDetails.SubscriptionOfferDetails>()
    val details = MutableLiveData<ProductDetails>()
    val lifetimeDetails = MutableLiveData<ProductDetails>()

    private var detailsAvailable = AtomicBoolean(false)
    private var lifetimeDetailsAvailable = AtomicBoolean(false)

    private lateinit var billingClient: BillingClient

    init {
        val params = PendingPurchasesParams
            .newBuilder()
            .enablePrepaidPlans()
            .enableOneTimeProducts()
            .build()
        billingClient = BillingClient.newBuilder(application.applicationContext)
                .enablePendingPurchases(params)
                .setListener(this)
                .build()

        updateState(false)
        updateSubscribedState(false)
        updateLifetimeState(false)

        billingClient.startConnection(this)
    }

    fun terminate() {
        if (billingClient.isReady) {
            billingClient.endConnection()
        }
    }

    private fun updateState(newState: Boolean) {
        if (ready.value!=newState) {
            ready.postValue(newState)
        }
    }

    private fun updateSubscribedState(newState: Boolean) {
        if (subscribed.value!=newState) {
            subscribed.postValue(newState)
        }
    }

    private fun updateLifetimeState(newState: Boolean) {
        if (isLifetime.value!=newState) {
            isLifetime.postValue(newState)
        }
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
            val subsPurchaseParams = QueryPurchasesParams
                .newBuilder()
                .setProductType(BillingClient.ProductType.SUBS)
                .build()

            billingClient.queryPurchasesAsync(subsPurchaseParams) { result: BillingResult, purchaseList: MutableList<Purchase> ->
                if (result.responseCode == BillingClient.BillingResponseCode.OK) {
                    handlePurchases(purchaseList)
                }
            }

            val inAppPurchaseParams = QueryPurchasesParams
                .newBuilder()
                .setProductType(BillingClient.ProductType.INAPP)
                .build()

            billingClient.queryPurchasesAsync(inAppPurchaseParams) { result: BillingResult, purchaseList: MutableList<Purchase> ->
                if (result.responseCode == BillingClient.BillingResponseCode.OK) {
                    handlePurchases(purchaseList)
                }
            }

            val subsProductParams = QueryProductDetailsParams
                .newBuilder()
                .setProductList(
                    listOf(
                        QueryProductDetailsParams.Product
                            .newBuilder()
                            .setProductId(application.getString(R.string.subscription_sku))
                            .setProductType(BillingClient.ProductType.SUBS)
                            .build()
                    )
                )
                .build()

            billingClient.queryProductDetailsAsync(subsProductParams) { billingResult: BillingResult, productDetailsResult: QueryProductDetailsResult ->
                if (billingResult.responseCode == BillingClient.BillingResponseCode.OK) {
                    handleProductDetails(productDetailsResult)
                }
            }

            val inAppProductParams = QueryProductDetailsParams
                .newBuilder()
                .setProductList(
                    listOf(
                        QueryProductDetailsParams.Product
                            .newBuilder()
                            .setProductId(application.getString(R.string.lifetime_subscription_sku))
                            .setProductType(BillingClient.ProductType.INAPP)
                            .build()
                    )
                )
                .build()

            billingClient.queryProductDetailsAsync(inAppProductParams) { billingResult: BillingResult, productDetailsResult: QueryProductDetailsResult ->
                if (billingResult.responseCode == BillingClient.BillingResponseCode.OK) {
                    handleProductDetails(productDetailsResult)
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
    }

    private fun handleProductDetails(productDetails: QueryProductDetailsResult) {
            productDetails.productDetailsList.forEach {
                when (it.productId) {
                    application.getString(R.string.subscription_sku) -> {
                        details.postValue(it)
                        it.subscriptionOfferDetails?.first().let {
                            offer.postValue(it)
                            detailsAvailable.set(true)
                        }
                    }

                    application.getString(R.string.lifetime_subscription_sku) -> {
                        lifetimeDetails.postValue(it)
                        lifetimeDetailsAvailable.set(true)
                    }
                }
            }

        if (detailsAvailable.get() && lifetimeDetailsAvailable.get()) {
            updateState(true)
        }
    }

    private fun handlePurchases(purchasesList: List<Purchase>?) {
        if (purchasesList == null)
            return

        purchasesList.forEach { purchase ->
            if (!purchase.isAcknowledged) {
                val purchaseParams = AcknowledgePurchaseParams.newBuilder()
                        .setPurchaseToken(purchase.purchaseToken)
                        .build()

                billingClient.acknowledgePurchase(purchaseParams) {
                }
            }

            if (purchase.purchaseState == Purchase.PurchaseState.PENDING)
                return

            purchase.products.forEach { product ->
                when (product) {
                    application.getString(R.string.subscription_sku) -> {
                        updateSubscribedState(true)
                    }
                    application.getString(R.string.lifetime_subscription_sku) -> {
                        updateLifetimeState(true)
                        updateSubscribedState(true)
                    }
                }
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
