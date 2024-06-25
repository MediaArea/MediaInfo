/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Maxim Pozdeev on 10.04.12.

#import "PreferencesWindowController.h"
#import "SubscribeWindowController.h"
#import "SubscriptionManager.h"

@interface PreferencesWindowController () 

- (void)addLanguageToComboWithTitle:(NSString*)title andValue:(NSString*)value;
- (IBAction)languageChanged:(id)sender;

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

    observers = [[NSMutableArray alloc] init];

    if (@available(macOS 10.9, *)) {
        if([[SubscriptionManager shared] subscriptionActive]) {
            [self enableSubscription];
        }
        else {
            [observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:[SubscriptionManager subscriptionStateChangedNotification] object:[SubscriptionManager shared] queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notification) {
                [self enableSubscription];
            }]];
        }
    }
    else {
        [subscribeButton setEnabled:NO];
        [subscribeButton setHidden:YES];
    }
	
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
	
	// Load views
	[self addViewToComboWithTitle:@"Easy" andValue:@"Easy"];
	[self addViewToComboWithTitle:@"Tree" andValue:@"Tree"];
	[self addViewToComboWithTitle:@"Compare" andValue:@"Compare"];
	[self addViewToComboWithTitle:@"Text" andValue:@"Text"];
    [self addViewToComboWithTitle:@"HTML" andValue:@"HTML"];
	[self addViewToComboWithTitle:@"XML" andValue:@"XML"];
	[self addViewToComboWithTitle:@"JSON" andValue:@"JSON"];
	[self addViewToComboWithTitle:@"Graph" andValue:@"Graph_Svg"];
	[self addViewToComboWithTitle:@"MPEG-7 (strict)" andValue:@"MPEG-7_Strict"];
	[self addViewToComboWithTitle:@"MPEG-7 (relaxed)" andValue:@"MPEG-7_Relaxed"];
	[self addViewToComboWithTitle:@"MPEG-7 (extended)" andValue:@"MPEG-7_Extended"];
	[self addViewToComboWithTitle:@"PBCore" andValue:@"PBCore"];
	[self addViewToComboWithTitle:@"PBCore2" andValue:@"PBCore2"];
	[self addViewToComboWithTitle:@"EBUCore 1.5" andValue:@"EBUCore 1.5"];
	[self addViewToComboWithTitle:@"EBUCore 1.6" andValue:@"EBUCore 1.6"];
	[self addViewToComboWithTitle:@"EBUCore 1.8 (acq. metadata: parameter then segment)" andValue:@"EBUCore_1.8_ps"];
	[self addViewToComboWithTitle:@"EBUCore 1.8 (acq. metadata: segment then parameter)" andValue:@"EBUCore_1.8_sp"];
	[self addViewToComboWithTitle:@"EBUCore 1.8 (acq. metadata: parameter then segment, json output)" andValue:@"EBUCore_1.8_ps_JSON"];
	[self addViewToComboWithTitle:@"EBUCore 1.8 (acq. metadata: segment then parameter, json output)" andValue:@"EBUCore_1.8_sp_JSON"];
	[self addViewToComboWithTitle:@"FIMS 1.1" andValue:@"FIMS_1.1"];
	[self addViewToComboWithTitle:@"FIMS 1.2" andValue:@"FIMS_1.2"];
	[self addViewToComboWithTitle:@"FIMS 1.3" andValue:@"FIMS_1.3"];
	[self addViewToComboWithTitle:@"reVTMD" andValue:@"reVTMD"];
	[self addViewToComboWithTitle:@"NISO Z39.87" andValue:@"NISO_Z39.87"];

	NSString *savedView = [[NSUserDefaults standardUserDefaults] objectForKey:@"defaultView"];
	
	if(savedView == nil)
		savedView = @"Easy";

	for(NSMenuItem *i in [[viewCombo menu] itemArray]) {
		if([[i representedObject] isEqualToString:savedView]) {
				[viewCombo selectItem:i];
				break;
		}
	}

    //DisplayCaptions
    [[[displayCaptionsCombo menu] itemWithTag:1] setRepresentedObject:@"Content"];
    [[[displayCaptionsCombo menu] itemWithTag:2] setRepresentedObject:@"Command"];
    [[[displayCaptionsCombo menu] itemWithTag:3] setRepresentedObject:@"Stream"];
    NSString *savedCaptionsOption = [[NSUserDefaults standardUserDefaults] objectForKey:@"displayCaptions"];
    if(savedCaptionsOption == nil)
        savedCaptionsOption = @"Command";

    for(NSMenuItem *i in [[displayCaptionsCombo menu] itemArray]) {
        if([[i representedObject] isEqualToString:savedCaptionsOption]) {
                [displayCaptionsCombo selectItem:i];
                break;
        }
    }

    // Graph options
    BOOL savedGraphAdmShowTrackUIDs = [[NSUserDefaults standardUserDefaults] boolForKey:@"graphAdmShowTrackUIDs"];
    [graphAdmShowTrackUIDs setState:savedGraphAdmShowTrackUIDs?NSControlStateValueOn:NSControlStateValueOff];

    BOOL savedGraphAdmShowChannelFormats = [[NSUserDefaults standardUserDefaults] boolForKey:@"graphAdmShowChannelFormats"];
    [graphAdmShowChannelFormats setState:savedGraphAdmShowChannelFormats?NSControlStateValueOn:NSControlStateValueOff];
}

-(void)windowWillClose:(NSNotification *)notification {
	for(id observer in observers) {
		[[NSNotificationCenter defaultCenter] removeObserver:observer];
	}
	[observers release];

	//window object is released automatically when closes (see setReleasedWhenClosed: method)
	//dont forget to bind delegate and window outlet
	
	[prefsCtrl release], prefsCtrl = nil;
}

- (void)addViewToComboWithTitle:(NSString*)title andValue:(NSString*)value {
	NSMenuItem *item = [[NSMenuItem alloc] initWithTitle:title action:nil keyEquivalent:@""];
	[item setRepresentedObject:value];
	[[viewCombo menu] addItem:item];
	[item release];
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

- (IBAction)viewChanged:(id)sender {
	NSMenuItem *obj = [viewCombo selectedItem];
	NSString *value = [obj representedObject];

	if(!value)
		value = @"Easy";

	[[NSUserDefaults standardUserDefaults] setObject:value forKey:@"defaultView"];
}

- (IBAction)captionsOptionChanged:(id)sender {
    NSMenuItem *obj = [displayCaptionsCombo selectedItem];
    NSString *value = [obj representedObject];

    if (!value)
        value = @"Command";

    [[NSUserDefaults standardUserDefaults] setObject:value forKey:@"displayCaptions"];
}

- (IBAction)subscribeClicked:(id)sender {
    if (@available(macOS 10.9, *)) {
        [[SubscribeWindowController controller] show];
    }
}

- (void)enableSubscription {
    [viewCombo setEnabled:YES];
    [subscribeButton setEnabled:NO];
    [subscribeButton setHidden:YES];

}
- (IBAction)graphAdmShowTrackUIDsClicked:(id)sender {
    BOOL newGraphAdmShowTrackUIDs = [graphAdmShowTrackUIDs state]==NSControlStateValueOn?TRUE:FALSE;
    [[NSUserDefaults standardUserDefaults] setBool:newGraphAdmShowTrackUIDs forKey:@"graphAdmShowTrackUIDs"];
}

- (IBAction)graphAdmShowChannelFormatsClicked:(id)sender {
    BOOL newGraphAdmShowChannelFormats = [graphAdmShowChannelFormats state]==NSControlStateValueOn?TRUE:FALSE;
    [[NSUserDefaults standardUserDefaults] setBool:newGraphAdmShowChannelFormats forKey:@"graphAdmShowChannelFormats"];
}

@end
