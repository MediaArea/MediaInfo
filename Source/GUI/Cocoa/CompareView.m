/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import "CompareView.h"

#import "IndexedTableColumn.h"

@implementation CompareView
- (IBAction)changeViewMode:(NSButton *)sender {
    if(sender)
        _mode = (CompareViewMode)sender.tag;

    if(_outlineView)
        [_outlineView reloadData];
}

-(void)initialize {
    _files = nil;
    _fields = nil;
    _mode = CompareViewModeAll;
    _selectedIndex = -1;
    [self loadNib];
    [_outlineView setDataSource:self];
    [_outlineView setDelegate:self];
    [_closeMenu setDelegate:self];

    NSRect frame;
    [_showLabel sizeToFit];
    [_allFieldsRadio sizeToFit];
    [_onlyDifferingRadio sizeToFit];
    [_onlyIdenticalRadio sizeToFit];

    frame = _allFieldsRadio.frame;
    frame.origin.x = _showLabel.frame.origin.x + _showLabel.frame.size.width + 5;
    [_allFieldsRadio setFrame:frame];

    frame = _onlyDifferingRadio.frame;
    frame.origin.x = _allFieldsRadio.frame.origin.x + _allFieldsRadio.frame.size.width + 5;
    [_onlyDifferingRadio setFrame:frame];

    frame = _onlyIdenticalRadio.frame;
    frame.origin.x = _onlyDifferingRadio.frame.origin.x + _onlyDifferingRadio.frame.size.width + 5;
    [_onlyIdenticalRadio setFrame:frame];
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
    [NSBundle loadNibNamed:@"CompareView" owner:self];

    [self addSubview:_contentView];
    self.contentView.frame = self.bounds;
}

-(oMediaInfoList *)getFiles {
    return _files;
}

-(void)setFiles:(oMediaInfoList *)files {
    if(_files) {
        [_files release];
        _files = nil;
    }
    _files = [files retain];
    [self createFields];
}

-(void) createFields {
    _selectedIndex = -1;

    if (_fields) {
        [_fields release];
        _fields = nil;
    }

   while([[_outlineView tableColumns] count]>1)
       [_outlineView removeTableColumn:[_outlineView tableColumns][1]];

    [[_outlineView tableColumns][0] setWidth:80]; // reset first column size to avoid "infinite growing" bug

    if(!_files || ![_files count]) {
        [_outlineView reloadData];
        return;
    }

    NSMutableArray* fields = [[[NSMutableArray alloc] init] autorelease];
    for(NSUInteger streamKind=MediaInfo_Stream_General; streamKind<MediaInfo_Stream_Max; streamKind++) {
        NSMutableSet* toAdd = [[[NSMutableSet alloc] init] autorelease];
        for(NSUInteger fileIndex=0; fileIndex<[_files count]; fileIndex++) {
            NSUInteger streamCount=[_files numberOFStreamsAtIndex:fileIndex ofStreamKind:streamKind];
            for(NSUInteger streamNumber=0; streamNumber<streamCount; streamNumber++) {
                NSUInteger fieldCount=[_files FieldCountAtIndex:fileIndex streamKind:streamKind streamNumber:streamNumber];
                for(NSUInteger fieldNumber=0; fieldNumber<fieldCount; fieldNumber++) {
                    NSString *field = [_files FieldAtIndex:fileIndex streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber];
                    NSString *name = [_files FieldNameAtIndex:fileIndex streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber];
                    if ((![_files ShowComplete] && ![_files ShowInInform:fileIndex streamKind:streamKind streamNumber:streamNumber parameter:fieldNumber]) || [[_files GetAtIndex:fileIndex streamKind:streamKind streamNumber:streamNumber parameter:field] isEqual:@""] || [field isEqual:@"CompleteName"])
                        continue;


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

                    NSMutableDictionary *dic1 = [[NSMutableDictionary alloc] initWithObjectsAndKeys:section, @"field", section, @"name", [[NSNumber alloc] initWithUnsignedInteger:streamKind], @"kind", [[NSNumber alloc] initWithUnsignedInteger:streamNumber], @"number", [[NSNumber alloc] initWithUnsignedInt:0], @"order", [[NSMutableArray alloc] initWithCapacity:0], @"entries", [[NSNumber alloc] initWithBool:NO], @"equal", nil];
                    [toAdd addObject:dic1];

                    NSMutableDictionary *dic2 = [[NSMutableDictionary alloc] initWithObjectsAndKeys:field, @"field", name, @"name", [[NSNumber alloc] initWithUnsignedInteger:streamKind], @"kind", [[NSNumber alloc] initWithUnsignedInteger:streamNumber], @"number", [[NSNumber alloc] initWithUnsignedInteger:fieldNumber], @"order", [[NSMutableArray alloc] initWithCapacity:[_files count]], @"entries",[[NSNumber alloc] initWithBool:NO], @"equal", nil];
                    [toAdd addObject:dic2];
                }
            }
        }
        if([toAdd count])
            [fields addObjectsFromArray:[toAdd allObjects]];
    }

    NSSortDescriptor *firstDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"kind" ascending:YES] autorelease];
    NSSortDescriptor *secondDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"number" ascending:YES] autorelease];
    NSSortDescriptor *thirdDescriptor = [[[NSSortDescriptor alloc] initWithKey:@"order" ascending:YES] autorelease];

    NSArray *sortDescriptors = [NSArray arrayWithObjects:firstDescriptor, secondDescriptor, thirdDescriptor, nil];

    NSArray* sorted = [fields sortedArrayUsingDescriptors:sortDescriptors];

    for(NSUInteger pos=0; pos<[sorted count]; pos++) {
        NSMutableDictionary *field=sorted[pos];
        if([field[@"order"] unsignedIntegerValue] == 0)
            continue;

        for(NSUInteger fileIndex=0; fileIndex<[_files count]; fileIndex++) {
            NSString *value = [_files GetAtIndex:fileIndex streamKind:[(NSNumber*)field[@"kind"] unsignedIntegerValue] streamNumber:[(NSNumber*)field[@"number"] unsignedIntegerValue] parameter:field[@"field"]];
            [field[@"entries"] addObject:value];
        }

        if([[[[NSSet alloc] initWithArray:field[@"entries"]] autorelease] count]<2) {
            field[@"equal"]=[[NSNumber alloc] initWithBool:YES];
        }
    }

    for(NSUInteger fileIndex=0; fileIndex<[_files count]; fileIndex++) {
        IndexedTableColumn *column = [[[IndexedTableColumn alloc] init] autorelease];

        NSString* fileName=[_files GetAtIndex:fileIndex streamKind:0 streamNumber:0 parameter:@"FileName"];
        NSString *fileExtension=[_files GetAtIndex:fileIndex streamKind:0 streamNumber:0 parameter:@"FileExtension"];

        if([fileExtension length]) {
            fileName=[NSString stringWithFormat:@"%@.%@", fileName, fileExtension];
        }

        [column setIndex: fileIndex];
        [[column headerCell] setTitle:fileName];
        [_outlineView addTableColumn:column];
    }

    NSArray* descriptors = _outlineView.sortDescriptors;
    if([descriptors count])
        _fields = [[sorted sortedArrayUsingDescriptors:descriptors] copy];
    else
        _fields = [sorted copy];

    [_outlineView reloadData];
    [_outlineView sizeToFit];
    [_outlineView expandItem:nil expandChildren:YES];
}

-(void)outlineView:(NSOutlineView *)outlineView willDisplayCell:(nonnull id)cell forTableColumn:(nullable NSTableColumn *)tableColumn item:(nonnull id)item {
    if([[outlineView tableColumns] indexOfObject:tableColumn] == 0)
        [cell setTextColor:[NSColor colorWithRed:0.5 green:0.5 blue:0.5 alpha:1]];
    else if([item[@"equal"] boolValue] == YES)
        [cell setTextColor:NSColor.textColor];
    else
        [cell setTextColor:[NSColor colorWithRed:1 green:0 blue:0 alpha:1]];

     if([[outlineView tableColumns] indexOfObject:tableColumn] != 0)
        [cell setMenu:_closeMenu];
}

-(id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item {
    NSPredicate *predicate = nil;
    if(item) {
        NSMutableString *filter = [[[NSMutableString alloc] init] autorelease];
        NSMutableArray *arguments = [[[NSMutableArray alloc] init] autorelease];
        [filter appendString:@"kind == %@ AND number == %@ AND order != 0"];
        [arguments addObject:item[@"kind"]];
        [arguments addObject:item[@"number"]];
        if(_mode == CompareViewModeEquals)
            [filter appendString:@" AND equal == YES"];
        else if (_mode == CompareViewModeDiffers)
            [filter appendString:@" AND equal == NO"];

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
        if(_mode == CompareViewModeEquals)
            [filter appendString:@" AND equal == YES"];
        else if (_mode == CompareViewModeDiffers)
            [filter appendString:@" AND equal == NO"];

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

    if([[outlineView tableColumns] indexOfObject:tableColumn] == 0)
        return item[@"name"];

    if (![tableColumn isKindOfClass:[IndexedTableColumn class]])
        return @"";

    NSInteger index = [(IndexedTableColumn *)tableColumn index];
    if(index >= [item[@"entries"] count])
        return @"";

    return item[@"entries"][index];
}

-(void)outlineView:(NSOutlineView *)outlineView sortDescriptorsDidChange:(NSArray<NSSortDescriptor *> *)oldDescriptors {
    if(!_fields)
        return;

    NSArray *fields = [_fields copy];
    [_fields release];
    _fields = nil;

    NSArray* descriptors =outlineView.sortDescriptors;
    _fields = [[fields sortedArrayUsingDescriptors:descriptors] copy];
    [outlineView reloadData];
}

-(BOOL)outlineView:(NSOutlineView *)outlineView shouldSelectTableColumn:(nullable NSTableColumn *)tableColumn {
    return [[outlineView tableColumns] indexOfObject:tableColumn] > 0;
}

-(BOOL)outlineView:(NSOutlineView *)outlineView shouldReorderColumn:(NSInteger)columnIndex toColumn:(NSInteger)newColumnIndex {
    if (columnIndex==0 || newColumnIndex==0)
        return NO;

    return YES;
}

-(void)outlineViewSelectionDidChange:(NSNotification *)notification {
    NSUInteger index = [_outlineView clickedColumn];
    if (index >= [[_outlineView tableColumns] count])
        return;

    NSTableColumn *column = [[_outlineView tableColumns] objectAtIndex:index];
    if (![column isKindOfClass: [IndexedTableColumn class]])
        return;

    _selectedIndex = [(IndexedTableColumn*)column index];
}

-(void)menuWillOpen:(NSMenu *)menu {
    NSInteger column = [_outlineView clickedColumn];
    [_outlineView selectColumnIndexes:[NSIndexSet indexSetWithIndex:column] byExtendingSelection:NO];
}

-(void)menuDidClose:(NSMenu *)menu {
    [_outlineView selectColumnIndexes:[NSIndexSet indexSet] byExtendingSelection:NO];
}

-(IBAction)closeFileInColumn:(id)sender {
    if(![NSApplication sharedApplication] ||
       ![[NSApplication sharedApplication] mainWindow] ||
       ![[[NSApplication sharedApplication] mainWindow] windowController])
        return;

    id mainWindowController = [[[NSApplication sharedApplication] mainWindow] windowController];

    NSUInteger index = [_outlineView clickedColumn];
    if (index >= [[_outlineView tableColumns] count])
        return;

    NSTableColumn *column = [[_outlineView tableColumns] objectAtIndex:index];
    if (![column isKindOfClass: [IndexedTableColumn class]])
        return;

    _selectedIndex = [(IndexedTableColumn*)column index];

    [mainWindowController performSelector:@selector(closeFile:)];
    [self reload];
}

-(void)reload {
    [self createFields];
}
@end
