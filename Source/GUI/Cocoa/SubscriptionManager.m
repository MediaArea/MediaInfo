/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import <Foundation/Foundation.h>

#import "SubscriptionManager.h"

@implementation SubscriptionManager
static SubscriptionManager *sharedInstance = nil;
static NSString *SKU = @"net.mediaarea.mediainfo.mac.nrsubs.inapp1";

static NSString *_subscriptionStateChangedNotification = @"net.mediaarea.mediainfo.mac.notifications.subscriptioStateChanged";
static NSString *_subscriptionDetailsReadyNotification = @"net.mediaarea.mediainfo.mac.notifications.subscriptionDetailsReady";
static NSString *_subscriptionDetailsUnaviableNotification = @"net.mediaarea.mediainfo.mac.notifications.subscriptionDetailsUnaviable";
static NSString *_subscriptionPurchaseSucceededNotification = @"net.mediaarea.mediainfo.mac.notifications.subscriptionPurchaseSucceededNotification";
static NSString *_subscriptionPurchaseDeferredNotification = @"net.mediaarea.mediainfo.mac.notifications.subscriptionPurchaseDeferredNotification";
static NSString *_subscriptionPurchaseFailedNotification = @"net.mediaarea.mediainfo.mac.notifications.subscriptionPurchaseFailedNotification";

+(NSString *)subscriptionStateChangedNotification {
    return _subscriptionStateChangedNotification;
}

+(NSString *)subscriptionDetailsReadyNotification {
    return _subscriptionDetailsReadyNotification;
}

+(NSString *)subscriptionDetailsUnaviableNotification {
    return _subscriptionDetailsUnaviableNotification;
}

+(NSString *)subscriptionPurchaseSucceededNotification {
    return _subscriptionPurchaseSucceededNotification;
}

+(NSString *)subscriptionPurchaseDeferredNotification {
    return _subscriptionPurchaseDeferredNotification;
}

+(NSString *)subscriptionPurchaseFailedNotification {
    return _subscriptionPurchaseFailedNotification;
}

+(instancetype)shared {
    if(!sharedInstance) {
        sharedInstance = [[SubscriptionManager alloc] init];
    }

    return sharedInstance;
}

-(instancetype)init {
    self = [super init];

    _request = nil;
    _subscription = nil;
    _subscriptionEndDate = nil;
    _subscriptionActive = NO;

    [self requestSubscriptionDetails];
    [self parseSubscriptions];

    _shouldNotifyUserForSubscription = NO;
    NSInteger launchCount = [[NSUserDefaults standardUserDefaults]integerForKey:@"launchCount"];
    if(launchCount == 10 && !_subscriptionEndDate) {
        _shouldNotifyUserForSubscription = YES;
    }

    [[NSUserDefaults standardUserDefaults] setInteger:launchCount + 1 forKey:@"launchCount"];

    _shouldNotifyUserForSubscriptionEnd = NO;
    if(_subscriptionEndDate && [_subscriptionEndDate isLessThan:[NSDate date]]) {
    NSDate *subscriptionEndNotificationDate = [[NSUserDefaults standardUserDefaults] objectForKey:@"subscriptionEndNotificationDate"];

        if(!subscriptionEndNotificationDate || [subscriptionEndNotificationDate isLessThan:_subscriptionEndDate]) {
            _shouldNotifyUserForSubscriptionEnd = YES;
        }
    }

    return self;
}

-(void)dealloc {
    if(_request) {
        [_request release];
    }

    if(_subscription) {
        [_subscription release];
    }

    if(_subscriptionEndDate) {
        [_subscriptionEndDate release];
    }

    [super dealloc];
}

-(NSArray *)subscriptions {
    NSMutableSet *toReturn = [[[NSMutableSet alloc] init] autorelease];

    NSArray *local = [[NSUserDefaults standardUserDefaults] arrayForKey:@"subscriptions"];

    if(local) {
        [toReturn addObjectsFromArray:local];
    }

    NSArray *remote = [[NSUbiquitousKeyValueStore defaultStore] arrayForKey:@"subscriptions"];

    if(remote) {
        [toReturn addObjectsFromArray:remote];
    }

    if([[toReturn allObjects] isNotEqualTo:local]) {
        [[NSUserDefaults standardUserDefaults] setObject:[toReturn allObjects] forKey:@"subscriptions"];
    }

    if([[toReturn allObjects] isNotEqualTo:remote]) {
        [[NSUbiquitousKeyValueStore defaultStore] setObject:[toReturn allObjects] forKey:@"subscriptions"];
    }

    return [toReturn allObjects];
}

-(void)requestSubscriptionDetails {
    if(!_request) {
        _request = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObjects:SKU, nil]];
        _request.delegate = self;
        [_request start];
    }
}

-(void)purchaseSubscription {
    if(!_subscription) {
        [[NSNotificationCenter defaultCenter] postNotificationName:_subscriptionPurchaseFailedNotification object:self userInfo:nil];

        return;
    }

    SKPayment *payment = [SKPayment paymentWithProduct:_subscription];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

-(void)restoreSubscriptions {
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

-(void)parseSubscriptions {
    NSDate *end = [NSDate dateWithTimeIntervalSince1970:0];

    NSArray *descriptors = [NSArray arrayWithObject:[NSSortDescriptor sortDescriptorWithKey:@"date" ascending:YES]];
    for(NSDictionary *subscription in [[self subscriptions] sortedArrayUsingDescriptors:descriptors]) {
        if([subscription[@"product"] isEqualToString:SKU]) {
            NSDate *new = subscription[@"date"];
            NSInteger quantity = [subscription[@"quantity"] integerValue];

            if([new isGreaterThan:end]) {
                end = [[NSCalendar currentCalendar] dateByAddingUnit:NSCalendarUnitYear value:quantity toDate:new options:nil];
            }
            else {
                end = [[NSCalendar currentCalendar] dateByAddingUnit:NSCalendarUnitYear value:quantity toDate:end options:nil];
            }
        }
    }

    if([end isGreaterThan:[NSDate dateWithTimeIntervalSince1970:0]]) {
        _subscriptionEndDate = end;

        if([end isGreaterThanOrEqualTo:[NSDate date]]) {
            BOOL oldSubscriptionState = _subscriptionActive;
            _subscriptionActive = YES;

            if(!oldSubscriptionState) {
                [[NSNotificationCenter defaultCenter] postNotificationName:_subscriptionStateChangedNotification object:self userInfo:nil];
            }
        }
    }
}

-(void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    for(SKProduct* product in response.products) {
        if([product.productIdentifier isEqualToString:SKU]) {
            if(_subscription) {
                [_subscription release];
                _subscription = nil;
            }
            _subscription = [product retain];

            [[NSNotificationCenter defaultCenter] postNotificationName:_subscriptionDetailsReadyNotification object:self userInfo:nil];
        }
    }
    [_request release];
    _request = nil;
}

-(void)request:(SKRequest *)request didFailWithError:(NSError *)error {
    [[NSNotificationCenter defaultCenter] postNotificationName:_subscriptionDetailsUnaviableNotification object:self userInfo:nil];

    [_request release];
    _request = nil;
}

-(void)purchaseSucceeded:(SKPaymentTransaction *)transaction {
    if(transaction && [transaction.payment.productIdentifier isEqualToString:SKU]) {
        NSDictionary *new = [[NSDictionary alloc] initWithObjectsAndKeys:transaction.transactionIdentifier, @"id", SKU, @"product",[NSNumber numberWithInteger: transaction.payment.quantity], @"quantity", transaction.transactionDate, @"date", nil];

        NSMutableSet *saved = [[[NSMutableSet alloc] initWithArray: [self subscriptions]] autorelease];
        [saved addObject:new];

        [[NSUserDefaults standardUserDefaults] setObject:[saved allObjects] forKey:@"subscriptions"];
        [[NSUbiquitousKeyValueStore defaultStore] setObject:[saved allObjects] forKey:@"subscriptions"];

        [[NSNotificationCenter defaultCenter]    postNotificationName:_subscriptionPurchaseSucceededNotification object:self userInfo:nil];

        [self parseSubscriptions];
    }
}

-(void)purchaseDeferred:(SKPaymentTransaction *)transaction {
    [[NSNotificationCenter defaultCenter] postNotificationName:_subscriptionPurchaseDeferredNotification object:self userInfo:nil];
}

-(void)purchaseFailed:(SKPaymentTransaction *)transaction {
    [[NSNotificationCenter defaultCenter] postNotificationName:_subscriptionPurchaseFailedNotification object:self userInfo:nil];
}

-(void)purchaseRestored:(SKPaymentTransaction *)transaction {
    SKPaymentTransaction *originalTransaction = transaction.originalTransaction;
    if(originalTransaction && [originalTransaction.payment.productIdentifier isEqualToString:SKU]) {
        NSDictionary *new = [[NSDictionary alloc] initWithObjectsAndKeys:originalTransaction.transactionIdentifier, @"id", SKU, @"product",[NSNumber numberWithInteger: originalTransaction.payment.quantity], @"quantity", originalTransaction.transactionDate, @"date", nil];

        NSMutableSet *saved = [[[NSMutableSet alloc] initWithArray: [self subscriptions]] autorelease];
        [saved addObject:new];

        [[NSUserDefaults standardUserDefaults] setObject:[saved allObjects] forKey:@"subscriptions"];
        [[NSUbiquitousKeyValueStore defaultStore] setObject:[saved allObjects] forKey:@"subscriptions"];

        [self parseSubscriptions];
    }
}

- (void)userNotifiedForSubscriptionEnd {
    [[NSUserDefaults standardUserDefaults] setObject:[NSDate date] forKey:@"subscriptionEndNotificationDate"];
}
@end
