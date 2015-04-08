/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 08.02.12.

#import "TreeOutline.h"


@implementation TreeOutline

- (BOOL)validateUserInterfaceItem:(id <NSValidatedUserInterfaceItem>)item {
	
	SEL action = [item action];
	
	if(action == @selector(copy:)) {
		return ([self numberOfSelectedRows] > 0 || [self clickedRow] != -1);
	}
	
	return [super validateUserInterfaceItem:item];
	
}


-(IBAction)copy:(id)sender {
	
	NSIndexSet *indexes = [self selectedRowIndexes];
	int clickedRow = [self clickedRow];
	
	NSMutableArray *textLines = [NSMutableArray array];
	
	// muliple rows selection (clicked on it or not clicked at all)
	if([indexes count] >= 1 
	   && (clickedRow == -1 || [indexes containsIndex:clickedRow]))
	{
		[indexes enumerateIndexesUsingBlock:
		 ^(NSUInteger idx, BOOL *stop) {
			 //extract from tree node
			 [textLines addObject:[[[self itemAtRow:idx] representedObject] objectForKey:@"extValue"]];
		 }];
	}
	else {
		//no selection, from right-clicked context menu
		//extract from tree node
		[textLines addObject:[[[self itemAtRow:clickedRow] representedObject] objectForKey:@"extValue"]];
	}
	
	NSString *textToCopy = [textLines componentsJoinedByString:@"\n"];
	NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
	[pasteboard clearContents];
	NSArray *copiedObjects = [NSArray arrayWithObject:textToCopy];
	[pasteboard writeObjects:copiedObjects];
	
}


@end
