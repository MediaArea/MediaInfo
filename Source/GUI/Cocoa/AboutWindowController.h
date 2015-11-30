/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 12.03.12.

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
