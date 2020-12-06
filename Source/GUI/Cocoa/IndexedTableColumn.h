/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface IndexedTableColumn : NSTableColumn
@property (readwrite) NSUInteger index;
@end

NS_ASSUME_NONNULL_END
