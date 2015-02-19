//
//  MediaInfoExporter.h
//  MediaInfo
//
//  Created by Max Pozdeev on 14.02.12.
//  Copyright 2012 Jerome Martinez. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "oMediaInfoList.h"


@interface MediaInfoExporter : NSObject {
	oMediaInfoList *mil;
	BOOL extensionHidden;
}

@property (nonatomic, assign) BOOL extensionHidden;

- (id)initWithObj:(oMediaInfoList*)obj;

- (BOOL)exportToText:(NSURL*)url;

- (BOOL)saveData:(NSData*)aData toUrl:(NSURL*)url;

@end
