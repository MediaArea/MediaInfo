/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 20.06.11.

#import "NSString+wchar.h"
#include "wchar.h"

#define WCHAR_4BYTE_ENCODING NSUTF32LittleEndianStringEncoding

@implementation NSString (NSString_wchar)

+ (NSString*) stringFromWCHAR:(const wchar_t*) inStr {
	return [[[NSString alloc] initWithBytes:(char*)inStr length:wcslen(inStr)*4 encoding:WCHAR_4BYTE_ENCODING] autorelease];
}

- (const wchar_t *) WCHARString {
	NSMutableData *data = [NSMutableData dataWithData:[self dataUsingEncoding:WCHAR_4BYTE_ENCODING]];
	[data setLength:[data length] + 4]; //add 4 null-terminating bytes
	return (const wchar_t*)[data bytes];
}

@end
