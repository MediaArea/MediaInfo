//
//  SegmentedControlWithMenu.m
//  MediaInfo
//
//  Created by Max Pozdeev on 13.05.16.
//  Copyright © 2016-2020 MediaArea.net. All rights reserved.
//

#import "SegmentedControlWithMenu.h"
#import "SegmentedCellWithMenu.h"

@implementation SegmentedControlWithMenu

-(void)mouseDown:(NSEvent *)theEvent
{
	//Determine if click on the last segment
	CGFloat w = [self widthForSegment:self.segmentCount-1];
	NSPoint point = [self convertPoint:theEvent.locationInWindow fromView:nil];
	
	BOOL _flag = NO;
	if (point.x >= self.bounds.size.width - w) {
		_flag = YES;
	}
	[(SegmentedCellWithMenu*)self.cell setFlag:_flag];
	
	[super mouseDown:theEvent];
}

@end
