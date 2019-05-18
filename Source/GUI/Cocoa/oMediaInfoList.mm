/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 06.02.12.

#include "oMediaInfoList.h"
#include "MediaInfoDLL/MediaInfoDLL_Static.h"

#import "NSString+wchar.h"

@implementation oMediaInfoList


- (id)init {
	
	if(self = [super init]) {
		MIL = MediaInfoList_New();
		
	}
	
	return self;
	
}

- (void)dealloc {
	MediaInfoList_Delete(MIL);
	[super dealloc];
}

+(void)setLanguageWithContents:(NSString*)langContents {
	MediaInfoList_Option(NULL, [@"Language" WCHARString], [langContents WCHARString]);
}


- (BOOL)openURL:(NSURL *)fileURL {
	
	NSString *filename = [fileURL path];

	/*
    //this works for non-latin files
	const char *cstr = [filename UTF8String];
	wchar_t *wstr = new wchar_t[strlen(cstr)+1];
	int newlen = mbstowcs(wstr, cstr, strlen(cstr));
	wstr[newlen] = 0;
	
	size_t ret = MediaInfoList_Open(MIL, wstr, MediaInfo_FileOption_Nothing);
	
	delete[] wstr;
	*/
	
	MediaInfoList_Option(MIL, [@"setlocale_LC_CTYPE" WCHARString], [@"UTF-8" WCHARString]);
	size_t ret = MediaInfoList_Open(MIL, [filename WCHARString], MediaInfo_FileOption_Nothing);
	
	return ret ? YES : NO;
}


- (BOOL)openFiles:(NSArray *)files {
	
	NSUInteger number = [files count];
	if(!number) {
		return NO;
	}
	
	for(NSUInteger i=0; i<number; i++) {
		if(![self openURL:[files objectAtIndex:i]]) {
			return NO; //really no? maybe skip?
		}
	}
	return YES;
}


- (NSString *)GetAtIndex:(NSUInteger)fileIndex 
			  streamKind:(oMediaInfoStream)streamKind 
			streamNumber:(int)streamNumber 
			   parameter:(NSString *)parameter {

	
	const wchar_t *s = MediaInfoList_Get(MIL, 
										 (int)fileIndex, 
										 (MediaInfo_stream_t)streamKind, 
										 streamNumber, 
										 [parameter WCHARString],
										 MediaInfo_Info_Text,
										 MediaInfo_Info_Name);
	
	NSString *r = [NSString stringFromWCHAR:s];

	if(r==nil) return @"";
	else return r;
}

- (NSString *)FieldAtIndex:(NSUInteger)fileIndex
              streamKind:(oMediaInfoStream)streamKind
            streamNumber:(NSUInteger)streamNumber
               parameter:(NSUInteger)parameter {


    const wchar_t *s = MediaInfoList_GetI(MIL,
                                         (int)fileIndex,
                                         (MediaInfo_stream_t)streamKind,
                                         (int)streamNumber,
                                         (int)parameter,
                                         MediaInfo_Info_Name);

    NSString *r = [NSString stringFromWCHAR:s];

    if(r==nil) return @"";
    else return r;
}

- (NSString *)FieldNameAtIndex:(NSUInteger)fileIndex
                    streamKind:(oMediaInfoStream)streamKind
                  streamNumber:(NSUInteger)streamNumber
                     parameter:(NSUInteger)parameter {


    const wchar_t *s = MediaInfoList_GetI(MIL,
                                          (int)fileIndex,
                                          (MediaInfo_stream_t)streamKind,
                                          (int)streamNumber,
                                          (int)parameter,
                                          MediaInfo_Info_Name_Text);

    NSString *r = [NSString stringFromWCHAR:s];

    if(r==nil) return [self FieldAtIndex:fileIndex streamKind:streamKind streamNumber:streamNumber parameter:parameter];
    else return r;
}

- (NSUInteger) FieldCountAtIndex:(NSUInteger)fileIndex streamKind:(oMediaInfoStream)streamKind streamNumber:(NSUInteger)streamNumber {
    return MediaInfoList_Count_Get(MIL, fileIndex, (MediaInfo_stream_t)streamKind, streamNumber);
}

-(bool) ShowInInform:(NSUInteger)fileIndex streamKind:(oMediaInfoStream)streamKind streamNumber:(NSUInteger)streamNumber parameter:(NSUInteger)parameter {
    return MediaInfoList_GetI(MIL, fileIndex, (MediaInfo_stream_t)streamKind, streamNumber, parameter, MediaInfo_Info_Options)[MediaInfo_InfoOption_ShowInInform]==L'Y';
}

- (NSUInteger)count {
	
	size_t ret = MediaInfoList_Count_Get_Files(MIL);
	
	return (NSUInteger)ret;
	
}

- (NSInteger)numberOFStreamsAtIndex:(NSUInteger)fileIndex ofStreamKind:(oMediaInfoStream)streamKind {
	
	return (int)MediaInfoList_Count_Get(MIL, (int)fileIndex, (MediaInfo_stream_t)streamKind, -1);
}

- (NSString*)filenameAtIndex:(NSInteger)index {

/*
	int numberOfFiles = (int)MediaInfoList_Count_Get_Files(MIL);
	
	if(index >= numberOfFiles) {
		return nil;
	}
 */
	
	return [self GetAtIndex:index
				streamKind:oMediaInfoStream_General
			   streamNumber:0
				  parameter:@"CompleteName"];
}

- (NSArray*)files {
	
	NSMutableArray *a = [NSMutableArray array];
	
	int max = MediaInfoList_Count_Get_Files(MIL);
	
	for(int i=0; i<max; i++) {
		
		NSString *filename = [self GetAtIndex:i
								   streamKind:oMediaInfoStream_General
								 streamNumber:0
									parameter:@"CompleteName"];
		
		[a addObject:filename];
		
	}
	
	return a;
	
}

- (NSString*)inform
{
	const wchar_t *s = MediaInfoList_Inform(MIL, -1, 0);
	return [NSString stringFromWCHAR:s];
}

- (NSString*)informAtIndex:(NSUInteger)fileIndex {
	
	const wchar_t *s = MediaInfoList_Inform(MIL, fileIndex, 0);
	return [NSString stringFromWCHAR:s];
}


- (void)setOption:(NSString*)option withValue:(NSString*)value {
	
	MediaInfoList_Option(MIL, [option WCHARString], [value WCHARString]);
}


- (NSString*)generalTagsAtIndex:(NSUInteger)index limit:(NSUInteger)limit {
	
	NSMutableArray *r = [NSMutableArray array];
	
	int i = 94;
	int max = MediaInfoList_Count_Get(MIL, (int)index, MediaInfo_Stream_General, 0);
	int lines = 0;
	
	while(i < max) {
		
		NSString *value = [NSString stringFromWCHAR:MediaInfoList_GetI(MIL, (int)index, MediaInfo_Stream_General, 0, i, MediaInfo_Info_Text)];
		NSString *options = [NSString stringFromWCHAR:MediaInfoList_GetI(MIL, (int)index, MediaInfo_Stream_General, 0, i, MediaInfo_Info_Options)];
		
		if(![value isEqualToString:@""] && [options characterAtIndex:(int)MediaInfo_InfoOption_ShowInInform] == 'Y') {
			
			NSString *name = [NSString stringFromWCHAR:MediaInfoList_GetI(MIL, (int)index, MediaInfo_Stream_General, 0, i, MediaInfo_Info_Name_Text)];
			if(name == nil || [name isEqualToString:@""]) {
				name = [NSString stringFromWCHAR:MediaInfoList_GetI(MIL, (int)index, MediaInfo_Stream_General, 0, i, MediaInfo_Info_Name)];
			}
			
			[r addObject:[NSString stringWithFormat:@"%@: %@", name, value]];
			
			lines++;
			if(lines >= limit) {
				break; //exit while
			}
			
		}
		
		i++;
	}

	return [r componentsJoinedByString:@"\n"];
}

-(void)closeAtIndex:(NSUInteger)fileIndex {
    MediaInfoList_Close(MIL, fileIndex);
}

@end
