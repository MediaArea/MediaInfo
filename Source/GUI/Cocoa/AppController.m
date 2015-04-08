/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 06.02.12.

#import "AppController.h"
#import "MyWindowController.h"
#import "AboutWindowController.h"
#import "PreferencesWindowController.h"
#import "oMediaInfoList.h"

@implementation AppController

+(void) initialize
{
	
	NSDictionary *defaults = [NSDictionary dictionaryWithObjectsAndKeys:
							  [NSNumber numberWithBool:YES],	@"NSDisabledCharacterPaletteMenuItem", //Disable "Special Characters" in Edit menu
							  [NSNumber numberWithInt:12],		@"treeFontSize",
							  @"__default__",					@"prefLanguage",
							  nil];
	
	// Load default userDefaults
    [[NSUserDefaults standardUserDefaults] registerDefaults:defaults];
}

- (void)awakeFromNib {
	filesToOpenAtStart = nil;
	wc = nil;
}

- (void)dealloc {
	[filesToOpenAtStart release];
	[wc release];
	[super dealloc];
}

- (void)application:(NSApplication *)sender openFiles:(NSArray *)filenames {
	
	//open multiple files at once
	//this is called before applicationDidFinishLaunching: at startup

	//convert to array of NSURLs
	NSMutableArray *a = [NSMutableArray array];
	NSUInteger i, max = [filenames count];
	for(i=0; i<max; i++) {
		[a addObject:[NSURL fileURLWithPath:[filenames objectAtIndex:i]]];
	}
	
	[self doWorkWithFiles:a];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
	// Insert code here to initialize your application 
	
	[NSApp setServicesProvider:self];
	
	//get and set MI language contents
	NSString *langFile = [[NSBundle mainBundle] pathForResource:@"lang" ofType:@"csv"];
	NSString *langContents = [NSString stringWithContentsOfFile:langFile encoding:NSUTF8StringEncoding error:nil];
	if(langContents) {
		[oMediaInfoList setLanguageWithContents:langContents];
	}

}

- (void)applicationWillBecomeActive:(NSNotification *)aNotification {
	//called every time the app will in focus
	
	if(!wc) {
		wc = [[MyWindowController alloc] initWithWindowNibName:@"MyWindow"];
		[[wc window] registerForDraggedTypes:[NSArray arrayWithObjects:NSFilenamesPboardType,nil]];
		[[wc window] makeKeyAndOrderFront:self];
		
		if(filesToOpenAtStart) {
			[wc processFiles:filesToOpenAtStart];
		}
	}
	
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication {
	return YES;
}





/*
- (IBAction)openFile:(id)sender {

	NSOpenPanel *openPanel	= [NSOpenPanel openPanel];
	[openPanel setCanChooseDirectories:YES];
	[openPanel setAllowsMultipleSelection:YES];

	NSInteger retVal = [openPanel runModal];

	if(retVal != NSFileHandlingPanelOKButton){
     	return;
	} 
	
	//create new window and do some processing...
	
}
 */


-(void)analyzeService:(NSPasteboard *)pboard userData:(NSString *)userData error:(NSString **)error {
	
	if([[pboard types] containsObject:NSFilenamesPboardType]) {
		
		NSArray *a = [pboard propertyListForType:NSFilenamesPboardType];
		
		NSMutableArray *r = [NSMutableArray array];
		for(NSString *path in a) {
			[r addObject:[NSURL fileURLWithPath:path]];
		}
		
		[self doWorkWithFiles:r];
		
	}
	else if([[pboard types] containsObject:NSStringPboardType]) {
		
		NSArray *classes = [NSArray arrayWithObject:[NSString class]];
		NSDictionary *options = [NSDictionary dictionary];
		
		if([pboard canReadObjectForClasses:classes options:options]) {
			
			NSString *str =  [pboard stringForType:NSStringPboardType];
			
			[self doWorkWithFiles:[NSArray arrayWithObject:[NSURL fileURLWithPath:str]]];

		}
	}
}



-(void)doWorkWithFiles:(NSArray*)array {

	if(wc) {
		[wc processFiles:array];
	}
	else {
		filesToOpenAtStart = [[NSArray alloc] initWithArray:array];
	}

}


- (IBAction)openAboutPanel:(id)sender {
	
	[[AboutWindowController controller] show];
	
}

- (IBAction)clickAuthorWebsite:(id)sender {
	[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:@"http://mediainfo.sourceforge.net/"]];
}

- (IBAction)openPreferences:(id)sender {
	[[PreferencesWindowController controller] show];	
}


@end
