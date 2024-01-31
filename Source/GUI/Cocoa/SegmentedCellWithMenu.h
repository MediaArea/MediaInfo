//
//  SegmentedCellWithMenu.h
//  MediaInfo
//
//  Created by Max Pozdeev on 13.05.16.
//  Copyright © 2016-2024 MediaArea.net SARL. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface SegmentedCellWithMenu : NSSegmentedCell {
	BOOL _flag;
}

-(void)setFlag:(BOOL)flag;

@end
