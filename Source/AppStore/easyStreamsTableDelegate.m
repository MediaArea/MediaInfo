//
//  easyStreamsTableDelegate.m
//  MediaInfo
//
//  Created by Max Pozdeev on 15.08.12.
//  Copyright (c) 2012 MediaArea.net. All rights reserved.
//

#import "easyStreamsTableDelegate.h"

@implementation easyStreamsTableDelegate

- (void)tableViewColumnDidResize:(NSNotification *)aNotification {
	[table noteHeightOfRowsWithIndexesChanged:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, table.numberOfRows)]];
}


- (CGFloat)tableView:(NSTableView *)tableView heightOfRow:(NSInteger)row {

	CGFloat maxHeight = [tableView rowHeight];
	
	NSTableColumn *col = [[tableView tableColumns] objectAtIndex:1]; //2nd column vith value
	NSCell *cell = [tableView preparedCellAtColumn:1 row:row];
	NSRect constrainedBounds = NSMakeRect(0, 0, col.width, CGFLOAT_MAX);
	NSSize naturalSize = [cell cellSizeForBounds:constrainedBounds];
	
	if(naturalSize.height > maxHeight) {
		maxHeight = naturalSize.height;
	}

	return maxHeight;
	
}

@end
