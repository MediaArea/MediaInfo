/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import "PurchaseReportController.h"

#import "SubscriptionManager.h"
#import <StoreKit/StoreKit.h>

@interface PurchaseReportController ()

@end

@implementation PurchaseReportController
static PurchaseReportController *reportCtrl = nil;

+(instancetype)controller
{
    if (!reportCtrl) {
        reportCtrl = [[PurchaseReportController alloc] initWithWindowNibName:@"PurchaseReport"];
    }

    return reportCtrl;
}

-(void)windowWillClose:(NSNotification *)notification {
    [NSEvent removeMonitor:eventMonitor];

    [reportCtrl release]; reportCtrl = nil;
}

-(void)windowDidLoad {
    [super windowDidLoad];

    NSEvent* (^handler)(NSEvent*) = ^(NSEvent *event) {
        if (event.window!=self.window)
            return event;

        return event;
    };

    eventMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:NSKeyDownMask handler:handler];

    [self createReport];
    [_reportView setString:purchaseReport];
}

-(void)show {
    [[self window] makeKeyAndOrderFront:nil];
}

-(void)createReport {
    NSMutableString* report = [[NSMutableString alloc] init];

    [report appendFormat:@"System date: %@\n\n", [NSDate date]];

    [report appendString:@"Unproceeded purchases in queue:\n"];
    for( SKPaymentTransaction* item in [[SKPaymentQueue defaultQueue] transactions]) {
        [report appendFormat:@"Product: %@, date %@, quantity: %ld, status: %ld\n",
         item.payment.productIdentifier,
         item.transactionDate,
         item.payment.quantity,
         item.transactionState];
    }
    [report appendString:@"\n"];

    [report appendString:@"Purchases in local storage:\n"];
    for (NSDictionary* item in [[NSUserDefaults standardUserDefaults] arrayForKey:@"subscriptions"]) {
        [report appendFormat:@"Product: %@, date: %@, quantity: %@\n",
         item[@"product"],
         item[@"date"],
         item[@"quantity"]
         ];
    }
    [report appendString:@"\n"];

    [report appendString:@"Purchases in remote storage:\n"];
    for (NSDictionary* item in [[NSUbiquitousKeyValueStore defaultStore] arrayForKey:@"subscriptions"]) {
        [report appendFormat:@"Product: %@, date: %@, quantity: %@\n",
         item[@"product"],
         item[@"date"],
         item[@"quantity"]
         ];
    }
    [report appendString:@"\n"];

    NSDate* subscriptionEndDate = [[SubscriptionManager shared] subscriptionEndDate];
    if (subscriptionEndDate != nil) {
        if ([subscriptionEndDate isGreaterThanOrEqualTo: [NSDate date]])
            [report appendFormat:@"Subscription status: active (until %@)\n",
             subscriptionEndDate];
        else
            [report appendFormat:@"Subscription status: expired (since %@)\n",
             subscriptionEndDate];
    }
    else {
        [report appendString:@"Subscription status: none\n"];
    }
    [report appendString:@"\n"];

    [[NSUserDefaults standardUserDefaults] setObject:@"TEST" forKey:@"TEMP"];
    [report appendFormat:@"Local storage is writable: %@\n", [[[NSUserDefaults standardUserDefaults] stringForKey:@"TEMP"] isEqualTo:@"TEST"]?@"YES":@"NO"];
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:@"TEMP"];

    [report appendString:@"\n"];

    [[NSUbiquitousKeyValueStore defaultStore] setObject:@"TEST" forKey:@"TEMP"];
    [report appendFormat:@"Remote storage is writable: %@\n", [[[NSUbiquitousKeyValueStore defaultStore] stringForKey:@"TEMP"] isEqualTo:@"TEST"]?@"YES":@"NO"];
    [[NSUbiquitousKeyValueStore defaultStore] removeObjectForKey:@"TEMP"];

    purchaseReport = report;
}

-(void)showMessage:(NSString *)message {
    NSAlert* dialog = [[NSAlert alloc] init];
    [dialog setMessageText:message];

    [dialog runModal];
}

- (IBAction)validateCode:(id)sender {
}

- (IBAction)submitClicked:(id)sender {
    [self validateCode:sender];
}

- (IBAction)copyClicked:(id)sender {
    [[NSPasteboard generalPasteboard] declareTypes:@[NSPasteboardTypeString] owner:nil];
    [[NSPasteboard generalPasteboard] setString:purchaseReport forType:NSPasteboardTypeString];
}

@end
