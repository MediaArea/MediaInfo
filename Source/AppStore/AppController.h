//
//  MediaInfoAppDelegate.h
//  MediaInfo
//
//  Created by Max Pozdeev on 06.02.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MyWindowController.h"

@interface AppController : NSObject <NSApplicationDelegate> {
	MyWindowController *wc;
	NSArray *filesToOpenAtStart;
}

-(void)doWorkWithFiles:(NSArray*)array;
-(void)analyzeService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error;

//used when no windows present
//- (IBAction)openFile:(id)sender;
- (IBAction)openAboutPanel:(id)sender;
- (IBAction)clickAuthorWebsite:(id)sender;
- (IBAction)openPreferences:(id)sender;

@end
