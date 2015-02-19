//
//  MediaInfoExporter.m
//  MediaInfo
//
//  Created by Max Pozdeev on 14.02.12.
//  Copyright 2012 Jerome Martinez. All rights reserved.
//

#import "MediaInfoExporter.h"


@implementation MediaInfoExporter

@synthesize extensionHidden;

- (id)initWithObj:(oMediaInfoList*)obj {
	if(self = [super init]) {
		mil = [obj retain];
		extensionHidden = NO;
	}
	return self;
}

- (void)dealloc {
	[mil release];
	mil = nil;
	[super dealloc];
}


#pragma mark -

- (BOOL)exportToText:(NSURL*)url {

	NSMutableArray *a = [NSMutableArray array];
	
	NSUInteger i, count = [mil count];
	for(i=0; i<count; i++) {
		
		NSString *info = [mil informAtIndex:i];
		[a addObject:info];
	}

	NSString *text = [a componentsJoinedByString:@"\n\n"];
	
	return [self saveData:[text dataUsingEncoding:NSUTF8StringEncoding] toUrl:url];
	
}


- (BOOL)saveData:(NSData*)aData toUrl:(NSURL*)url {
	
	NSFileManager *fmgr = [[NSFileManager alloc] init]; //thread-safe instance
	NSString *path = [url path];
	BOOL fileExists = [fmgr fileExistsAtPath:path];
	
	if(![aData writeToURL:url atomically:NO]) {
		[fmgr release];
		return NO;
	}
	
	//set attributes only for newly created file
	if(!fileExists && extensionHidden) {
		
		NSDictionary *attrs = [NSDictionary dictionaryWithObjectsAndKeys:
							   [NSNumber numberWithBool:extensionHidden], NSFileExtensionHidden,
							   nil];
		
		[fmgr setAttributes:attrs ofItemAtPath:path error:nil];
	}
	[fmgr release];
	
	return YES;
}

@end
