//
//  TreeOutline.m
//  MediaInfo
//
//  Created by Max Pozdeev on 08.02.12.
//  Copyright 2012 Jerome Martinez. All rights reserved.
//

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
