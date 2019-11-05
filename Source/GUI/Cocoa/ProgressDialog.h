/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import <Cocoa/Cocoa.h>
#import "oMediaInfoList.h"

NS_ASSUME_NONNULL_BEGIN

@interface ProgressDialog : NSWindowController {
    NSMutableArray *observers;
    size_t currentItemIndex;
    BOOL canceled;

    NSMetadataQuery *currentItemQuery;
    NSMetadataItem *currentItemMetadata;
    id currentQueryDidUpdateObserver;
    id currentQueryDidFinishGathering;
    id currentItemPercentDownloadedObserver;
    id currentItemIsDownloadedObserver;
    NSFileCoordinator *coordinator;
}

@property (assign) IBOutlet NSTextField *progressMessage;
@property (assign) IBOutlet NSProgressIndicator *progressBar;
@property (assign) IBOutlet NSButton *cancelButton;

@property (assign) NSArray *items;
@property (assign) oMediaInfoList *mediaList;

-(void)cancel;
-(void)next;

@end

NS_ASSUME_NONNULL_END
