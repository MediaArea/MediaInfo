/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 14.02.12.

#import <Cocoa/Cocoa.h>
#import "oMediaInfoList.h"


@interface MediaInfoExporter : NSObject {
    oMediaInfoList *mil;
    BOOL extensionHidden;
}

@property (nonatomic, assign) BOOL extensionHidden;

- (id)initWithObj:(oMediaInfoList*)obj;

- (BOOL)exportToText:(NSURL*)url;
- (BOOL)exportFormat:(NSString*)format toUrl:(NSURL*)url;

- (BOOL)saveData:(NSData*)aData toUrl:(NSURL*)url;

@end
