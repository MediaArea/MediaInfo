/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 06.11.15.

#import "DraggingFixView.h"

@implementation DraggingFixView

-(void) awakeFromNib
{
	[super awakeFromNib];

	//dragged types are the same like in window
	[self registerForDraggedTypes:@[NSFilenamesPboardType]];	
}

-(NSDragOperation)draggingEntered:(id<NSDraggingInfo>)sender
{
	return [self.window.windowController draggingEntered:sender];
}

- (BOOL)performDragOperation:(id < NSDraggingInfo >)sender
{
	return [self.window.windowController performDragOperation:sender];
}

@end
