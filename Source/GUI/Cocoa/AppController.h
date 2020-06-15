/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 06.02.12.

#import <Cocoa/Cocoa.h>
#import <StoreKit/StoreKit.h>
#import "MyWindowController.h"
#import "SubscriptionManager.h"

@interface AppController : NSObject <NSApplicationDelegate, SKPaymentTransactionObserver> {
    MyWindowController *wc;
    NSArray *filesToOpenAtStart;
    SubscriptionManager* subscriptionManager;
}

-(void)doWorkWithFiles:(NSArray*)array;
-(void)analyzeService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error;
-(void)didChangeExternally;

//used when no windows present
//- (IBAction)openFile:(id)sender;
- (IBAction)openAboutPanel:(id)sender;
- (IBAction)openSubscribePanel:(id)sender;
- (IBAction)openSubscriptionReportPanel:(id)sender;
- (IBAction)clickAuthorWebsite:(id)sender;
- (IBAction)openPreferences:(id)sender;

@end
