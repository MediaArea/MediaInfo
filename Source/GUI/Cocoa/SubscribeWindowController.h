/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface SubscribeWindowController : NSWindowController {
    NSMutableArray *observers;
}
@property (assign) IBOutlet NSBox *loadingLayer;
@property (assign) IBOutlet NSTextField *loadingMessage;
@property (assign) IBOutlet NSProgressIndicator *loadingIndicator;
@property (assign) IBOutlet NSButton *retryButton;
@property (assign) IBOutlet NSTextField *statusText;
@property (assign) IBOutlet NSButton *subscribeButton;
@property (assign) IBOutlet NSButton *lifetimeSubscribeButton;
@property (assign) IBOutlet NSButton *restoreButton;
@property (assign) IBOutlet NSTextField *legalText;

+(instancetype)controller;
-(void)show;

-(void)updateSubscriptionDetails;
@end

NS_ASSUME_NONNULL_END
