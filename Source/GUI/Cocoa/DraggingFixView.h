/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 06.11.15.

#import <Cocoa/Cocoa.h>

/*
 
 Use this class for the root NSView in NSTabViewItem. In El Capitan (OSX 10.11) this should fix the file dragging.
 
 */

@interface DraggingFixView : NSView

@end
