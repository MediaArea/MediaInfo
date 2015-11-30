/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 16.08.12.

#import <Cocoa/Cocoa.h>

@interface HyperlinkButton : NSButton {
    NSURL *url;
}

- (void)setActionUrl:(NSURL*)anUrl;

- (IBAction) openURL:(id) sender;

@end
