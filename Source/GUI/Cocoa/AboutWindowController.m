//
//  AboutWindowController.m
//  MediaInfo
//
//  Created by Max Pozdeev on 12.03.12.
//  Copyright (c) 2012 Jerome Martinez. All rights reserved.
//

#import "AboutWindowController.h"

@implementation AboutWindowController

static AboutWindowController *aboutCtrl = nil;

+ (AboutWindowController*)controller
{
	if (!aboutCtrl) {
		aboutCtrl = [[AboutWindowController alloc] initWithWindowNibName:@"About"];
	}
	
	return aboutCtrl;
}


-(void)show {
	[[self window] makeKeyAndOrderFront:nil];	
}


- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
	
	[versionLabel setStringValue:[NSString stringWithFormat:@"v%s", APP_VERSION_STRING]];
}

-(void)windowWillClose:(NSNotification *)notification {
	
	//window object is released automatically when closes (see setReleasedWhenClosed: method)
	//dont forget to bind delegate and window outlet
	
	[aboutCtrl release], aboutCtrl = nil;
}


- (IBAction)clickAuthorWebsite:(id)sender {
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://mediainfo.sourceforge.net/"]];
}

- (IBAction)clickAuthorEmail:(id)sender {
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"mailto:info@mediaarea.net"]];	
}

- (IBAction)clickGUIDeveloper:(id)sender {
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://www.pozdeev.com/"]];
}


@end
