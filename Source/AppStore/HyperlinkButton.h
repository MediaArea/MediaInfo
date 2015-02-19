//
//  HyperlinkButton.h
//  MediaInfo
//
//  Created by Max Pozdeev on 16.08.12.
//  Copyright (c) 2012 MediaArea.net. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface HyperlinkButton : NSButton {
	NSURL *url;
}

- (void)setActionUrl:(NSURL*)anUrl;

- (IBAction) openURL:(id) sender;

@end
