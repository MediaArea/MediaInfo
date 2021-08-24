/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Maxim Pozdeev on 10.04.12.

#import <Cocoa/Cocoa.h>

@interface PreferencesWindowController : NSWindowController {
    IBOutlet NSTabView *tabView;
    IBOutlet NSPopUpButton *langsCombo;
    IBOutlet NSTextField *viewLabel;
    IBOutlet NSPopUpButton *viewCombo;
    IBOutlet NSButton *graphAdmShowTrackUIDs;
    IBOutlet NSButton *graphAdmShowChannelFormats;
    IBOutlet NSButton *subscribeButton;

    NSMutableArray *observers;
}


+ (PreferencesWindowController*)controller;
-(void)show;

@end
