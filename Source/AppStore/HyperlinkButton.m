//
//  HyperlinkButton.m
//  MediaInfo
//
//  Created by Max Pozdeev on 16.08.12.
//  Copyright (c) 2012 MediaArea.net. All rights reserved.
//

#import "HyperlinkButton.h"

@implementation HyperlinkButton

- (void)dealloc {
	[url release];
	[super dealloc];
}

- (void) awakeFromNib {
	
	[self setBordered:NO];
	[self setBezelStyle:NSRegularSquareBezelStyle];
	[self setButtonType:NSMomentaryChangeButton];
		
	NSString *fontFamily = [[NSFont systemFontOfSize:12.0] familyName];
	NSFontManager *fontManager = [NSFontManager sharedFontManager];
	NSFont *normalFont = [fontManager fontWithFamily:fontFamily
											  traits:0
											  weight:0
												size:12];
	
	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:
								[NSNumber numberWithInt:NSUnderlineStyleSingle], NSUnderlineStyleAttributeName,
								/*@"go", NSLinkAttributeName,*/
								normalFont, NSFontAttributeName,
								[NSColor blueColor], NSForegroundColorAttributeName,
								nil];

	self.attributedTitle = [[NSAttributedString alloc] initWithString:self.title attributes:attributes];
	[self sizeToFit]; //will align to left
	
	//calc parent bounds to center the button's frame
	//button autosize should be set to center with fixed size
	NSRect parentSize = [[self superview] bounds];
	NSRect newSize = [self frame];
	newSize.origin.x = (parentSize.size.width - newSize.size.width)/2;
	[self setFrame:newSize];
	
	self.action = @selector(openURL:);
	self.target = self;
}

- (void)setActionUrl:(NSURL*)anUrl {
	
	[url release];
	url = [anUrl retain];

	[self setToolTip:[url absoluteString]];
}

- (IBAction) openURL:(id) sender {
	[[NSWorkspace sharedWorkspace] openURL:url];
}


- (void)resetCursorRects {
	[self addCursorRect:[self bounds] cursor:[NSCursor pointingHandCursor]];
}

@end
