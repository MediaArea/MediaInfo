/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

extern NSString * const TextSizeDidChangeNotification;

CGFloat TextSizeFactor(void);
NSFont *UserTextFont(void);
NSFont *BoldUserTextFont(void);
NSFont *FixedPitchTextFont(void);
void ApplyTableHeaderFont(NSTableView *tableView);

NS_ASSUME_NONNULL_END
