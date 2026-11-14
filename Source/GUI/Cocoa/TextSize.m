/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#include <math.h>

#import "TextSize.h"

NSString * const TextSizeDidChangeNotification = @"TextSizeDidChangeNotification";

CGFloat TextSizeFactor(void) {
	static const CGFloat factors[] = { 0.60, 0.75, 0.89, 1.0, 1.2, 1.5, 2.0 };
	NSInteger textSize = [[NSUserDefaults standardUserDefaults] integerForKey:@"textSize"];
	textSize = MAX(-3, MIN(3, textSize));
	return factors[textSize + 3];
}

NSFont *UserTextFont(void) {
	NSFont *baseFont = [NSFont userFontOfSize:0];
	return [NSFont userFontOfSize:baseFont.pointSize * TextSizeFactor()];
}

NSFont *BoldUserTextFont(void) {
	return [[NSFontManager sharedFontManager] convertFont:UserTextFont()
											 toHaveTrait:NSBoldFontMask];
}

NSFont *FixedPitchTextFont(void) {
	NSFont *baseFont = [NSFont userFixedPitchFontOfSize:0];
	return [NSFont userFixedPitchFontOfSize:baseFont.pointSize * TextSizeFactor()];
}

static CGFloat ApplyTableHeaderCellFonts(NSTableView *tableView, NSFont *font) {
	CGFloat requiredHeight = 0;
	for (NSTableColumn *column in [tableView tableColumns]) {
		NSTableHeaderCell *headerCell = [column headerCell];
		[headerCell setFont:font];
		NSMutableAttributedString *title = [[headerCell attributedStringValue] mutableCopy];
		if ([title length])
			[title addAttribute:NSFontAttributeName value:font range:NSMakeRange(0, [title length])];
		[headerCell setAttributedStringValue:title];
		[title release];
		requiredHeight = MAX(requiredHeight, ceil([headerCell cellSize].height) + 8);
	}
	return requiredHeight;
}

void ApplyTableHeaderFont(NSTableView *tableView) {
	NSFont *font = BoldUserTextFont();
	CGFloat requiredHeight = ApplyTableHeaderCellFonts(tableView, font);
	NSTableHeaderView *headerView = [tableView headerView];
	if (!headerView)
		return;

	NSRect frame = headerView.frame;
	frame.size.height = MAX(25, requiredHeight);
	headerView.frame = frame;
	[tableView tile];
	ApplyTableHeaderCellFonts(tableView, font);
	frame = headerView.frame;
	frame.size.height = MAX(25, requiredHeight);
	headerView.frame = frame;
	[headerView setNeedsDisplay:YES];
}
