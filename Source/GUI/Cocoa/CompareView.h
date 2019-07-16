/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import <Cocoa/Cocoa.h>

#import "oMediaInfoList.h"

NS_ASSUME_NONNULL_BEGIN

typedef enum CompareViewMode {
    CompareViewModeAll,
    CompareViewModeDiffers,
    CompareViewModeEquals
} CompareViewMode;

@interface CompareView : NSView <NSTableViewDataSource, NSTableViewDelegate, NSOutlineViewDataSource, NSOutlineViewDelegate>
@property (strong) IBOutlet NSView *contentView;
@property (strong) IBOutlet NSOutlineView *outlineView;
@property (nonatomic, strong) oMediaInfoList *files;
@property (strong) NSArray *fields;
@property (readonly) CompareViewMode mode;

-(void)reload;
@end

NS_ASSUME_NONNULL_END
