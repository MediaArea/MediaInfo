//
//  PreferencesWindowController.h
//  MediaInfo
//
//  Created by Maxim Pozdeev on 10.04.12.
//  Copyright (c) 2012 MediaArea.net. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface PreferencesWindowController : NSWindowController {
	//IBOutlet NSArrayController *languagesArrayController;
	IBOutlet NSToolbar *toolbar;
	IBOutlet NSPopUpButton *langsCombo;
}


+ (PreferencesWindowController*)controller;
-(void)show;

@end
