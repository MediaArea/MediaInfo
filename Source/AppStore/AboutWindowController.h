//
//  AboutWindowController.h
//  MediaInfo
//
//  Created by Max Pozdeev on 12.03.12.
//  Copyright (c) 2012 Jerome Martinez. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AboutWindowController : NSWindowController {
	IBOutlet NSTextField *versionLabel;
	IBOutlet NSTextField *copyrightLabel;
}

+ (AboutWindowController*)controller;
-(void)show;

- (IBAction)clickAuthorWebsite:(id)sender;
- (IBAction)clickAuthorEmail:(id)sender;
- (IBAction)clickGUIDeveloper:(id)sender;

@end
