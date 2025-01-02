//
//  SegmentedCellWithMenu.m
//  MediaInfo
//
//  Created by Max Pozdeev on 13.05.16.
//  Copyright © 2016-2025 MediaArea.net SARL. All rights reserved.
//

#import "SegmentedCellWithMenu.h"

@implementation SegmentedCellWithMenu

-(SEL)action
{
	if(_flag)
		return nil;
	
	return [super action];
}

-(void)setFlag:(BOOL)flag
{
	_flag = flag;
}


@end
