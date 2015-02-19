//
//  PreferencesWindowController.m
//  MediaInfo
//
//  Created by Maxim Pozdeev on 10.04.12.
//  Copyright (c) 2012 MediaArea.net. All rights reserved.
//

#import "PreferencesWindowController.h"

@interface PreferencesWindowController () 

- (void)addLanguageToComboWithTitle:(NSString*)title andValue:(NSString*)value;
- (IBAction)languageChanged:(id)sender;
- (IBAction)clickToSelectPrefTab:(id)sender;

@end

@implementation PreferencesWindowController

static PreferencesWindowController *prefsCtrl = nil;

+ (PreferencesWindowController*)controller
{
	if (!prefsCtrl) {
		prefsCtrl = [[PreferencesWindowController alloc] initWithWindowNibName:@"Preferences"];
	}
	
	return prefsCtrl;
}


-(void)show {
	[[self window] makeKeyAndOrderFront:nil];	
}


- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
	
	[toolbar setSelectedItemIdentifier:@"General"];
	
	
	//Load languages...
	
	[self addLanguageToComboWithTitle:@"العربية" andValue:@"ar"];
	//[self addLanguageToComboWithTitle:@"Български" andValue:@"bg"];
	[self addLanguageToComboWithTitle:@"Català" andValue:@"ca"];
	[self addLanguageToComboWithTitle:@"简体中文" andValue:@"zh-Hans"];
	[self addLanguageToComboWithTitle:@"華語 (台灣)" andValue:@"zh-Hant"];
	[self addLanguageToComboWithTitle:@"Hrvatski" andValue:@"hr"];
	[self addLanguageToComboWithTitle:@"Czech" andValue:@"cs"];
	[self addLanguageToComboWithTitle:@"Dansk" andValue:@"da"];
	[self addLanguageToComboWithTitle:@"Nederlands" andValue:@"nl"];
	[self addLanguageToComboWithTitle:@"Deutsch" andValue:@"de"];
	[self addLanguageToComboWithTitle:@"Ελληνικά" andValue:@"el"];
	[self addLanguageToComboWithTitle:@"magyar" andValue:@"hu"];
	[self addLanguageToComboWithTitle:@"English" andValue:@"en"];
	[self addLanguageToComboWithTitle:@"Français" andValue:@"fr"];
	//[self addLanguageToComboWithTitle:@"ქართული" andValue:@"ka"];
	[self addLanguageToComboWithTitle:@"日本語" andValue:@"ja"];
	[self addLanguageToComboWithTitle:@"Italiano" andValue:@"it"];
	[self addLanguageToComboWithTitle:@"한국어" andValue:@"ko"];
	[self addLanguageToComboWithTitle:@"Polski" andValue:@"pl"];
	[self addLanguageToComboWithTitle:@"Português" andValue:@"pt"];
	[self addLanguageToComboWithTitle:@"Português (Europeu)" andValue:@"pt-PT"];
	[self addLanguageToComboWithTitle:@"Romana" andValue:@"ro"];
	[self addLanguageToComboWithTitle:@"Русский" andValue:@"ru"];
	[self addLanguageToComboWithTitle:@"Slovak" andValue:@"sk"];
	[self addLanguageToComboWithTitle:@"Español" andValue:@"es"];
	[self addLanguageToComboWithTitle:@"Svenska" andValue:@"sv"];
	//[self addLanguageToComboWithTitle:@"ไทย" andValue:@"th"];
	[self addLanguageToComboWithTitle:@"Türkçe" andValue:@"tr"];
	//[self addLanguageToComboWithTitle:@"Українська" andValue:@"uk"];
	
	// Set language checkmark
	NSString *savedLang = [[NSUserDefaults standardUserDefaults] objectForKey:@"prefLanguage"];
	
	if(![savedLang isEqualToString:@"__default__"]) {
		for(NSMenuItem *i in [[langsCombo menu] itemArray]) {
			if([[i representedObject] isEqualToString:savedLang]) {
				[langsCombo selectItem:i];
				break;
			}
		}
	}
	
	
}

-(void)windowWillClose:(NSNotification *)notification {
	
	//window object is released automatically when closes (see setReleasedWhenClosed: method)
	//dont forget to bind delegate and window outlet
	
	[prefsCtrl release], prefsCtrl = nil;
}

- (void)addLanguageToComboWithTitle:(NSString*)title andValue:(NSString*)value {
	NSMenuItem *item = [[NSMenuItem alloc] initWithTitle:title action:nil keyEquivalent:@""];
	[item setRepresentedObject:value];
	[[langsCombo menu] addItem:item];
	[item release];
}

- (IBAction)languageChanged:(id)sender {
	
	NSMenuItem *obj = [langsCombo selectedItem];
	NSString *value = [obj representedObject];
	NSUserDefaults *ud = [NSUserDefaults standardUserDefaults];
	
	if(!value) {
		//set to default
		[ud setObject:@"__default__" forKey:@"prefLanguage"];
		[ud removeObjectForKey:@"AppleLanguages"];
	}
	else {
		//set non-default language
		[ud setObject:value forKey:@"prefLanguage"];
		[ud setObject:[NSArray arrayWithObjects:value,@"en",nil] forKey:@"AppleLanguages"];
	}
	
}

- (IBAction)clickToSelectPrefTab:(id)sender {
	
}

@end
