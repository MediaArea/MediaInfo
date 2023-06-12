/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 06.02.12.

#import <Cocoa/Cocoa.h>

//do not remember why this shoud be defined
#define _UNICODE

#include "MediaInfoDLL/MediaInfoDLL_Static.h"

typedef enum oMediaInfoStream_t
{
    oMediaInfoStream_General = MediaInfo_Stream_General,
    oMediaInfoStream_Video = MediaInfo_Stream_Video,
    oMediaInfoStream_Audio = MediaInfo_Stream_Audio,
    oMediaInfoStream_Text = MediaInfo_Stream_Text,
    oMediaInfoStream_Chapters = MediaInfo_Stream_Chapters,
    oMediaInfoStream_Image = MediaInfo_Stream_Image,
    oMediaInfoStream_Menu = MediaInfo_Stream_Menu
} oMediaInfoStream;


@interface oMediaInfoList : NSObject {
    void *MIL;
}

- (BOOL)openURL:(NSURL *)fileURL;

// open NSArray of NSURL
- (BOOL)openFiles:(NSArray *)files;

- (NSString *)GetAtIndex:(NSUInteger)fileIndex streamKind:(oMediaInfoStream)streamKind streamNumber:(int)streamNumber parameter:(NSString *)parameter;
- (NSString *)FieldNameAtIndex:(NSUInteger)fileIndex streamKind:(oMediaInfoStream)streamKind streamNumber:(NSUInteger)streamNumber parameter:(NSUInteger)parameter;
- (NSString *)FieldAtIndex:(NSUInteger)fileIndex streamKind:(oMediaInfoStream)streamKind streamNumber:(NSUInteger)streamNumber parameter:(NSUInteger)parameter;
- (NSUInteger)FieldCountAtIndex:(NSUInteger)fileIndex streamKind:(oMediaInfoStream)streamKind streamNumber:(NSUInteger)streamNumber;
- (bool)ShowInInform:(NSUInteger)fileIndex streamKind:(oMediaInfoStream)streamKind streamNumber:(NSUInteger)streamNumber parameter:(NSUInteger)parameter;
- (bool)ShowComplete;
- (NSUInteger)count;
- (NSString*)filenameAtIndex:(NSInteger)index;
- (NSArray*)files;
- (NSString*)inform;
- (NSString*)informAtIndex:(NSUInteger)fileIndex;
- (NSInteger)numberOFStreamsAtIndex:(NSUInteger)fileIndex ofStreamKind:(oMediaInfoStream)streamKind;
- (void)setOption:(NSString*)option withValue:(NSString*)value;
- (NSString*)generalTagsAtIndex:(NSUInteger)index limit:(NSUInteger)limit;
- (NSString*)getConformanceURLForIndex:(NSUInteger)index;
- (void)closeAtIndex:(NSUInteger)fileIndex;

+(void)setLanguageWithContents:(NSString*)langContents;
+(void)setOptionStatic:(NSString*)option withValue:(NSString*)value;

@end
