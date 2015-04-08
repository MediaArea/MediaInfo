//
//  NSString+wchar.m
//  Invisor
//
//  Created by Max Pozdeev on 20.06.11.
//  Copyright 2011 Max Pozdeev. All rights reserved.
//

#import "NSString+wchar.h"
#include "wchar.h"

#define WCHAR_4BYTE_ENCODING NSUTF32LittleEndianStringEncoding

@implementation NSString (NSString_wchar)

+ (NSString*) stringFromWCHAR:(const wchar_t*) inStr {
	return [[[NSString alloc] initWithBytes:(char*)inStr length:wcslen(inStr)*4 encoding:WCHAR_4BYTE_ENCODING] autorelease];
}

- (const wchar_t *) WCHARString {
	return (wchar_t*)[self cStringUsingEncoding:WCHAR_4BYTE_ENCODING];
}

@end
