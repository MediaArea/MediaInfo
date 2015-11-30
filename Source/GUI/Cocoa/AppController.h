/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 06.02.12.

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
