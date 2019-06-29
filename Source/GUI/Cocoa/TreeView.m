/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import "TreeView.h"

@implementation TreeView

-(void)initialize {
    _files = nil;
    _index = -1;
    [self loadNib];
    [_outlineView setDataSource:self];
}

-(instancetype)initWithCoder:(NSCoder *)decoder {
    self = [super initWithCoder:decoder];

    if(self) {
        [self initialize];
    }

    return self;
}

-(instancetype)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];

    if(self) {
        [self initialize];
    }

    return self;
}

-(void)loadNib {
    [NSBundle loadNibNamed:@"TreeView" owner:self];

    [self addSubview:_contentView];
    self.contentView.frame = self.bounds;
}

-(NSInteger)getIndex {
    return _index;
}

-(void)setIndex:(NSInteger)index
{
    if(!_files || index < 0 || [_files count] < index)
        return;

    _index = index;
    [self createFields];
    [_outlineView reloadData];
    [_outlineView expandItem:nil expandChildren:YES];
}

- (void)viewWillDraw {
    [_outlineView sizeLastColumnToFit];
}

-(void)createFields {
    if(_fields) {
        [_fields release];
        _fields = nil;
    }

    if(!_files || _index < 0 || [_files count] < _index) {
        [_outlineView reloadData];
        return;
    }

    NSMutableArray* fields = [[[NSMutableArray alloc] init] autorelease];
    for(NSUInteger streamKind=MediaInfo_Stream_General; streamKind<MediaInfo_Stream_Max; streamKind++) {
        NSUInteger streamCount=[_files numberOFStreamsAtIndex:_index ofStreamKind:streamKind];
        for(NSUInteger streamNumber=0; streamNumber<streamCount; streamNumber++) {
            NSMutableArray *toAdd = [[[NSMutableArray alloc] init] autorelease];
            NSUInteger fieldCount=[_files FieldCountAtIndex:_index streamKind:streamKind streamNumber:streamNumber];

            NSString* section=nil;
            if(streamKind==MediaInfo_Stream_General) section=@"General";
            else if(streamKind==MediaInfo_Stream_Video) section=@"Video";
            else if(streamKind==MediaInfo_Stream_Audio) section=@"Audio";
            else if(streamKind==MediaInfo_Stream_Text) section=@"Text";
            else if(streamKind==MediaInfo_Stream_Menu) section=@"Menu";
            else if(streamKind==MediaInfo_Stream_Image) section=@"Image";
            else section=@"Other";

            if(streamNumber)
                section=[NSString stringWithFormat:@"%@ #%lu", section, streamNumber+1];

            NSMutableDictionary *header = [[[NSMutableDictionary alloc] initWithObjectsAndKeys:section, @"name", [[[NSNumber alloc] initWithUnsignedInteger:streamKind] autorelease], @"kind", [[[NSNumber alloc] initWithUnsignedInteger:streamNumber] autorelease], @"number", [[[NSNumber alloc] initWithUnsignedInt:0] autorelease], @"order", @"", @"value", nil] autorelease];

            [toAdd addObject:header];
            for(NSUInteger fieldNumber=0; fieldNumber<fieldCount; fieldNumber++) {
                NSString *field = [_files FieldAtIndex:_index streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber];
                NSString *name = [_files FieldNameAtIndex:_index streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber];
                NSString *value = [_files GetAtIndex:_index streamKind:streamKind streamNumber:streamNumber parameter:field];
                if (![_files ShowInInform:_index streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber] || [[_files GetAtIndex:_index streamKind:streamKind streamNumber:streamNumber parameter:field] isEqual:@""])
                        continue;

                NSMutableDictionary *entry = [[[NSMutableDictionary alloc] initWithObjectsAndKeys:name, @"name", [[[NSNumber alloc] initWithUnsignedInteger:streamKind] autorelease], @"kind", [[[NSNumber alloc] initWithUnsignedInteger:streamNumber] autorelease], @"number", [[[NSNumber alloc] initWithUnsignedInt:fieldNumber] autorelease], @"order", value, @"value", nil] autorelease];
                [toAdd addObject:entry];
            }

            if([toAdd count] > 1)
                [fields addObjectsFromArray:toAdd];
        }
    }

    NSArray* descriptors = _outlineView.sortDescriptors;
    if([descriptors count])
        _fields = [[fields sortedArrayUsingDescriptors:descriptors] copy];
    else
        _fields = [fields copy];
}

-(id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item {
    if(!_fields || index < 0)
        return nil;

    NSPredicate *predicate = nil;
    if(item) {
        NSMutableString *filter = [[[NSMutableString alloc] init] autorelease];
        NSMutableArray *arguments = [[[NSMutableArray alloc] init] autorelease];
        [filter appendString:@"kind == %@ AND number == %@ AND order != 0"];
        [arguments addObject:item[@"kind"]];
        [arguments addObject:item[@"number"]];
        predicate = [NSPredicate predicateWithFormat:filter argumentArray:arguments];
    }
    else {
        predicate = [NSPredicate predicateWithFormat:@"order == 0"];
    }

    NSArray *result = [_fields filteredArrayUsingPredicate:predicate];
    if(index >= [result count])
        return nil;

    return result[index];
}

-(BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item {
    return item && [item[@"order"] isEqualTo:@0];
}

-(NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item {
    if(!_fields)
        return 0;

    NSPredicate *predicate = nil;
    if(item) {
        NSMutableString *filter = [[[NSMutableString alloc] init] autorelease];
        NSMutableArray *arguments = [[[NSMutableArray alloc] init] autorelease];
        [filter appendString:@"kind == %@ AND number == %@ AND order != 0"];
        [arguments addObject:item[@"kind"]];
        [arguments addObject:item[@"number"]];
        predicate = [NSPredicate predicateWithFormat:filter argumentArray:arguments];
    }
    else {
        predicate = [NSPredicate predicateWithFormat:@"order == 0"];
    }

    return [[_fields filteredArrayUsingPredicate:predicate] count];
}

-(id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item {
    if (!item)
        return @"";

    NSInteger column = [[outlineView tableColumns] indexOfObject:tableColumn];
    if(column == 0)
        return item[@"name"];

    return item[@"value"];
}

-(void)outlineView:(NSOutlineView *)outlineView sortDescriptorsDidChange:(NSArray<NSSortDescriptor *> *)oldDescriptors {
    if(!_fields)
        return;

    NSArray *fields = [_fields copy];
    [_fields release];
    _fields = nil;

    NSArray* descriptors = outlineView.sortDescriptors;
    _fields = [[fields sortedArrayUsingDescriptors:descriptors] copy];
    [outlineView reloadData];
}
@end
