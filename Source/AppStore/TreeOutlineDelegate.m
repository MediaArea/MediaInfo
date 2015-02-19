//
//  TreeOutlineDelegate.m
//  MediaInfo
//
//  Created by Maxim Pozdeev on 20.07.12.
//  Copyright (c) 2012 MediaArea.net. All rights reserved.
//

#import "TreeOutlineDelegate.h"

@implementation TreeOutlineDelegate

/**
 * word wrapping 
 * column cell's layout should be set to Wrap
 */

- (void)outlineViewColumnDidResize:(NSNotification *)notification {
	
	NSOutlineView *o = [notification object];
	
	// Tell the outline that we will have changed the row heights
    [o noteHeightOfRowsWithIndexesChanged:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, o.numberOfRows)]];    
}

//this method called for non-empty cells
- (CGFloat)outlineView:(NSOutlineView *)outlineView heightOfRowByItem:(id)item {
	
	int row = [outlineView rowForItem:item];
	//calc max height of cell in every column
	CGFloat maxHeight = [outlineView rowHeight];
	int max = [[outlineView tableColumns] count];
	for(int i=0; i<max; i++) {
		
		NSTableColumn *col = [[outlineView tableColumns] objectAtIndex:i];
		
		if(col.isHidden) {
			continue;	
		}

		//offset is calculated for the 1st column of outlineview
		CGFloat offset = i == 0 ? ([outlineView levelForRow:row]+1) * [outlineView indentationPerLevel] : 0; //+triangle?
		
		NSCell *cell = [outlineView preparedCellAtColumn:i row:row];
		NSRect constrainedBounds = NSMakeRect(0, 0, col.width-offset, CGFLOAT_MAX);
		NSSize naturalSize = [cell cellSizeForBounds:constrainedBounds];
		
		if(naturalSize.height > maxHeight) {
			maxHeight = naturalSize.height;
		}
	}
	return maxHeight;	
}

@end
