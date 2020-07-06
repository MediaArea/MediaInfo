/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#ifndef SubscriptionManager_h
#define SubscriptionManager_h

#import <Cocoa/Cocoa.h>
#import <StoreKit/StoreKit.h>

@interface SubscriptionManager : NSObject <SKProductsRequestDelegate> {
    SKProductsRequest *_request;
};
+(instancetype)shared;

-(void)requestSubscriptionDetails;
-(void)purchaseSubscription:(SKProduct *)product;
-(void)restoreSubscriptions;
-(void)parseSubscriptions;
-(void)purchaseSucceeded:(SKPaymentTransaction *)transaction;
-(void)purchaseDeferred:(SKPaymentTransaction *)transaction;
-(void)purchaseFailed:(SKPaymentTransaction *)transaction;
-(void)purchaseRestored:(SKPaymentTransaction *)transaction;
-(void)userNotifiedForSubscriptionEnd;

@property (readonly) BOOL isLifetime;
@property (readonly, strong) NSDate *subscriptionEndDate;
@property (readonly, strong) SKProduct *subscription;
@property (readonly, strong) SKProduct *lifetimeSubscription;
@property (readonly) NSArray *subscriptions;
@property (readonly) BOOL subscriptionActive;
@property (readonly) BOOL shouldNotifyUserForSubscription;
@property (readonly) BOOL shouldNotifyUserForSubscriptionEnd;

@property (class, readonly) NSNotificationName subscriptionStateChangedNotification;
@property (class, readonly) NSNotificationName subscriptionDetailsReadyNotification;
@property (class, readonly) NSNotificationName subscriptionDetailsUnaviableNotification;
@property (class, readonly) NSNotificationName subscriptionPurchaseSucceededNotification;
@property (class, readonly) NSNotificationName subscriptionPurchaseDeferredNotification;
@property (class, readonly) NSNotificationName subscriptionPurchaseFailedNotification;
@end

#endif /* SubscriptionManager_h */
