/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import "SubscribeWindowController.h"
#import "SubscriptionManager.h"

@implementation SubscribeWindowController
static SubscribeWindowController *subscribeCtrl = nil;

+(instancetype)controller
{
    if (!subscribeCtrl) {
        subscribeCtrl = [[SubscribeWindowController alloc] initWithWindowNibName:@"Subscribe"];
    }

    return subscribeCtrl;
}

-(void)dealloc {
    for(id observer in observers) {
        [[NSNotificationCenter defaultCenter] removeObserver:observer];
    }
    [observers release];

    [super dealloc];
}

-(void)show {
    [[self window] makeKeyAndOrderFront:nil];
}

-(void)showProgress {
    _loadingLayer.wantsLayer=YES;
    [_loadingLayer setFillColor:NSColor.controlBackgroundColor];
    [_loadingMessage setStringValue:@"Feching subscription details..."];
    [_retryButton setHidden:YES];
    [_loadingIndicator setHidden:false];
    [_loadingIndicator startAnimation:self];
}

-(void)hideProgress {
    [_loadingIndicator stopAnimation:self];
    [_loadingLayer setHidden:YES];
}

- (void)windowDidLoad {
    [super windowDidLoad];

    [self showProgress];

    [observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:[SubscriptionManager subscriptionStateChangedNotification] object:[SubscriptionManager shared] queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notification) {
        [self close];
    }]];

    [observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:[SubscriptionManager subscriptionDetailsReadyNotification] object:[SubscriptionManager shared] queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notification) {
        [self updateSubscriptionDetails];
    }]];

    [observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:[SubscriptionManager subscriptionDetailsUnaviableNotification] object:[SubscriptionManager shared] queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notification) {
        [self updateSubscriptionDetails];
    }]];

    [observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:[SubscriptionManager subscriptionPurchaseSucceededNotification] object:[SubscriptionManager shared] queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notification) {
        [self purchaseSucceeded];
    }]];

    [observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:[SubscriptionManager subscriptionPurchaseDeferredNotification] object:[SubscriptionManager shared] queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notification) {
        [self purchaseDeferred];
    }]];

    [observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:[SubscriptionManager subscriptionPurchaseFailedNotification] object:[SubscriptionManager shared] queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notification) {
        [self purchaseFailed];
    }]];

    if([SubscriptionManager shared].subscription) {
        [self updateSubscriptionDetails];
    }
    else {
        [[SubscriptionManager shared] requestSubscriptionDetails];
    }
}

-(void)windowWillClose:(NSNotification *)notification {

    //window object is released automatically when closes (see setReleasedWhenClosed: method)
    //dont forget to bind delegate and window outlet

    [subscribeCtrl release]; subscribeCtrl = nil;
}

-(void)updateSubscriptionDetails {
    if([SubscriptionManager shared].subscription) {
        NSNumberFormatter *formatter = [[[NSNumberFormatter alloc] init] autorelease];
        formatter.numberStyle = NSNumberFormatterCurrencyStyle;
        formatter.locale = [SubscriptionManager shared].subscription.priceLocale;

        NSString *price = [formatter stringFromNumber:[SubscriptionManager shared].subscription.price];

        if([SubscriptionManager shared].subscriptionEndDate) {
            NSDateFormatter *dateFormatter = [[[NSDateFormatter alloc] init] autorelease];
            [dateFormatter setDateStyle:NSDateFormatterShortStyle];
            [dateFormatter setTimeStyle:NSDateFormatterShortStyle];

            if([[SubscriptionManager shared].subscriptionEndDate
                isGreaterThanOrEqualTo:[NSDate date]]) {
                NSString *message = @"Subscription active until DATE.";
                message = [message stringByReplacingOccurrencesOfString:@"DATE" withString:[dateFormatter stringFromDate:[SubscriptionManager shared].subscriptionEndDate]];

                if([[[NSCalendar currentCalendar] dateByAddingUnit:NSCalendarUnitDay value:7 toDate:[NSDate date] options:0] isGreaterThan:[SubscriptionManager shared].subscriptionEndDate]) {
                    [_statusText setTextColor:[NSColor systemOrangeColor]];
                }
                else {
                    [_statusText setTextColor:[NSColor systemGreenColor]];
                }
                [_statusText setStringValue:message];
            }
            else {
                NSString *message = @"Subscription expired since DATE.";
                message = [message stringByReplacingOccurrencesOfString:@"DATE" withString:[dateFormatter stringFromDate:[SubscriptionManager shared].subscriptionEndDate]];

                [_statusText setTextColor:[NSColor systemRedColor]];
                [_statusText setStringValue:message];
            }
            [_subscribeButton setTitle:@"Renew subscription PRICE for one year"];
        }

        [_subscribeButton setTitle: [[_subscribeButton title] stringByReplacingOccurrencesOfString:@"PRICE" withString:price]];

        [_legalText setStringValue:[_legalText.stringValue stringByReplacingOccurrencesOfString:@"PRICE" withString:price]];

        [self hideProgress];
    }
    else {
        [_loadingMessage setStringValue:@"Failed to retrieve subscription details."];
        [_loadingIndicator stopAnimation:self];
        [_loadingIndicator setHidden:YES];
        [_retryButton setHidden:NO];
    }
}

-(void)purchaseFailed {
    [_subscribeButton setEnabled:YES];
}

-(void)purchaseDeferred {
    [_subscribeButton setEnabled:YES];
}

-(void)purchaseSucceeded {
    [_subscribeButton setEnabled:YES];
    [self close];
}

-(IBAction)subscribe:(id)sender {
    [_subscribeButton setEnabled:NO];
    [[SubscriptionManager shared] purchaseSubscription];
}

-(IBAction)retryRequest:(id)sender {
    [self showProgress];
    [[SubscriptionManager shared] requestSubscriptionDetails];
}

@end
