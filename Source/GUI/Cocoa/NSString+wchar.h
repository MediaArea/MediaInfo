//
//  NSString+wchar.h
//  Invisor
//
//  Created by Max Pozdeev on 20.06.11.
//  Copyright 2011 Max Pozdeev. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSString (NSString_wchar)

+ (NSString*) stringFromWCHAR:(const wchar_t*) inStr;
- (const wchar_t *) WCHARString;

@end
