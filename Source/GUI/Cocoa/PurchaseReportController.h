/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface PurchaseReportController : NSWindowController {
    id eventMonitor;
    NSString *purchaseReport;
}
@property (assign) IBOutlet NSTextView *reportView;
@property (assign) IBOutlet NSTextField *codeField;
@property (assign) IBOutlet NSBox *codeBox;

+(instancetype)controller;
-(void)show;
@end

NS_ASSUME_NONNULL_END
