/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Maxim Pozdeev on 20.07.12.

#import <Foundation/Foundation.h>

@interface TreeOutlineDelegate : NSObject <NSOutlineViewDelegate>
- (void)outlineViewColumnDidResize:(NSNotification *)notification;
- (CGFloat)outlineView:(NSOutlineView *)outlineView heightOfRowByItem:(id)item;
@end
