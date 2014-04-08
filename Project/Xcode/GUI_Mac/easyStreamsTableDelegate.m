/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 15.08.12.

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
