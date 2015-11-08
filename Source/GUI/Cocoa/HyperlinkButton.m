/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 16.08.12.

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
		
	NSDictionary *attributes = @{
								 NSUnderlineStyleAttributeName  : [NSNumber numberWithInt:NSUnderlineStyleSingle],
								 NSForegroundColorAttributeName : [NSColor blueColor]
								};
	
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
