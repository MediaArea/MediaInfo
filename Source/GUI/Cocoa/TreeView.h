/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import <Cocoa/Cocoa.h>
#import "TreeOutlineDelegate.h"
#import "oMediaInfoList.h"

NS_ASSUME_NONNULL_BEGIN

@interface TreeView : NSView <NSOutlineViewDataSource>{
    NSMutableArray *fields;
    TreeOutlineDelegate *delegate;
}
@property (strong) IBOutlet NSView *contentView;
@property (strong) IBOutlet NSOutlineView *outlineView;
@property (strong) oMediaInfoList *files;
@property (nonatomic, assign) NSInteger index;
@end

NS_ASSUME_NONNULL_END
