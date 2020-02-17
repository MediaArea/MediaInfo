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
    if(!_files || index<0 || [_files count]<index)
        return;

    _index = index;
    [self createFields];
    [_outlineView reloadData];

    if(fields) {
        for(id item in fields)
            [_outlineView expandItem:item];
    }
}

- (void)viewWillDraw {
    [_outlineView sizeLastColumnToFit];
}

-(void)createFields {
    if(fields) {
        [fields release];
        fields = nil;
    }

    if(!_files || _index<0 || [_files count]<_index) {
        [_outlineView reloadData];
        return;
    }

    fields = [[NSMutableArray alloc] init];
    [_files setOption:@"File_ExpandSubs" withValue:@"1"];
    for(NSUInteger streamKind=MediaInfo_Stream_General; streamKind<MediaInfo_Stream_Max; streamKind++) {
        NSUInteger streamCount=[_files numberOFStreamsAtIndex:_index ofStreamKind:streamKind];
        for(NSUInteger streamNumber=0; streamNumber<streamCount; streamNumber++) {

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

            NSMutableDictionary *header = [[[NSMutableDictionary alloc] initWithObjectsAndKeys:section, @"name", @(streamKind), @"kind", @(streamNumber), @"number", @"", @"value", [[[NSMutableArray alloc] init] autorelease], @"childs", nil] autorelease];

            NSMutableArray *tree = [[[NSMutableArray alloc] init] autorelease];
            [tree addObject:header];

            for(NSUInteger fieldNumber=0; fieldNumber<fieldCount; fieldNumber++) {
                NSString *field = [_files FieldAtIndex:_index streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber];
                NSString *name = [_files FieldNameAtIndex:_index streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber];
                NSString *value = [_files GetAtIndex:_index streamKind:streamKind streamNumber:streamNumber parameter:field];
                if ((![_files ShowComplete] && ![_files ShowInInform:_index streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber]) || [[_files GetAtIndex:_index streamKind:streamKind streamNumber:streamNumber parameter:field] isEqual:@""])
                        continue;

                NSUInteger level;
                for(level=0; level<[name length]; level++) {
                    if(![@([name characterAtIndex:level]) isEqual:@' '])
                        break;
                }

                if (level)
                    name = [name substringFromIndex:level];

                if(level==[tree count] && [[tree lastObject][@"childs"] count]) {
                    [tree addObject:[[tree lastObject][@"childs"] lastObject]];

                    if ([[tree lastObject][@"value"] isEqual:@"Yes"])
                        [tree lastObject][@"value"] = @"";
                }
                else if (level+1<[tree count]) {
                    while ([tree count]>level+1)
                        [tree removeLastObject];
                }

                NSMutableDictionary *entry = [[[NSMutableDictionary alloc] initWithObjectsAndKeys:name, @"name",@(streamKind), @"kind", @(streamNumber), @"number", @(fieldNumber), @"order", value, @"value", [[[NSMutableArray alloc] init] autorelease], @"childs", nil] autorelease];
                [[tree lastObject][@"childs"] addObject:entry];
            }

            if([header[@"childs"] count]>0) {
                if([_outlineView.sortDescriptors count]>0)
                    [self sort:header descriptors:_outlineView.sortDescriptors];

                [fields addObject:header];
            }
        }
    }
    [_files setOption:@"File_ExpandSubs" withValue:@"0"];
}

-(id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item {
    if(item && [item[@"childs"] count]>=index)
        return item[@"childs"][index];
    else if (!item && fields && [fields count]>=index)
        return fields[index];
    else
        return nil;
}

-(BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item {
    return item && [item[@"childs"] count]>0;
}

-(NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item {
    if (item)
        return [item[@"childs"] count];
    else if (fields)
        return [fields count];
    else
        return 0;
}

-(id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item {
    if (!item)
        return @"";

    NSInteger column = [[outlineView tableColumns] indexOfObject:tableColumn];
    if(column==0)
        return item[@"name"];

    return item[@"value"];
}

-(void)outlineView:(NSOutlineView *)outlineView sortDescriptorsDidChange:(NSArray<NSSortDescriptor *> *)oldDescriptors {
    if(!fields)
        return;

    for(id item in fields)
        [self sort:item descriptors:outlineView.sortDescriptors];

    [outlineView reloadData];
}

-(void)sort:(id)item descriptors:(NSArray<NSSortDescriptor *> *)descriptors {
    if(!item || [item[@"childs"] count]==0 || !descriptors || [descriptors count]==0)
        return;

    for (id child in item[@"childs"]) {
        if([item[@"childs"] count] > 0)
            [self sort:child descriptors:descriptors];
    }

    [item[@"childs"] sortUsingDescriptors:descriptors];
}
@end
