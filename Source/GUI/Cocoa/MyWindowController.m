//
//  MyWindowController.m
//  MediaInfo
//
//  Created by Max Pozdeev on 07.02.12.
//  Copyright 2012 Jerome Martinez. All rights reserved.
//

#import "MyWindowController.h"
#import "oMediaInfoList.h"
#import "MediaInfoExporter.h"

#define kEasyTabIndex 0
#define kTreeTabIndex 1
#define kTextTabIndex 2
#define kOtherViewsSelectorIndex 3

NSString* TextKindToNSString(ViewMenu_Kind kind)
{
	NSString *_ret = nil;
	switch (kind)
	{
		case Kind_MPEG7:		_ret = @"MPEG-7"; break;
		case Kind_PBCore: 		_ret = @"PBCore"; break;
		case Kind_PBCore2:		_ret = @"PBCore2"; break;
		case Kind_EBUCore_1_5:	_ret = @"EBUCore_1.5"; break;
		case Kind_EBUCore_1_6:	_ret = @"EBUCore_1.6"; break;
		case Kind_FIMS_1_1:		_ret = @"FIMS_1.1"; break;
		case Kind_FIMS_1_2:		_ret = @"FIMS_1.2"; break;
		case Kind_FIMS_1_3:		_ret = @"FIMS_1.3"; break;
		case Kind_reVTMD:		_ret = @"reVTMD"; break;
		case Kind_Text:
								_ret = @"";
		default:				break;
	}
	return _ret;
}

@implementation MyWindowController

-(void)dealloc {
	[mediaList release];
	[super dealloc];
}

-(void)awakeFromNib {
	//make transparent background for table in easy tab
	[easyTable setBackgroundColor:[NSColor clearColor]];
	[easyGeneralLinkButton setHidden:YES];
	_lastTextKind = Kind_Text;
	[tabSelector setMenu:otherViewsMenu forSegment:kOtherViewsSelectorIndex];
	_exportSavePanel = nil;
}

#pragma mark -
#pragma mark GUI routines

- (IBAction)openFile:(id)sender {
	
	NSOpenPanel *openPanel	= [NSOpenPanel openPanel];
	[openPanel setCanChooseDirectories:YES];
	[openPanel setAllowsMultipleSelection:YES];
	
	[openPanel beginSheetModalForWindow:[self window] completionHandler:^(NSInteger result){
		if(result == NSFileHandlingPanelOKButton) {
			[self processFiles:[openPanel URLs]];
		}
	}];
}


-(IBAction)clickOnViewSelector:(id)sender {
	int segment = [(NSSegmentedControl*)sender selectedSegment];
	if(segment == 3) {
		//[NSMenu popUpContextMenu:otherViewsMenu withEvent:[NSApp currentEvent] forView:tabSelector];
	}
	else if(segment == 2) {
		[self selectTextTab:nil];
	}
	else if(segment == 1) {
		[tabs selectTabViewItemAtIndex:kTreeTabIndex];
	}
	else {
		[tabs selectTabViewItemAtIndex:kEasyTabIndex];
	}
}


-(IBAction)selectEasyTab:(id)sender {
	[tabSelector setSelectedSegment:kEasyTabIndex];
	[tabs selectTabViewItemAtIndex:kEasyTabIndex];
}

-(IBAction)selectTreeTab:(id)sender {
	[tabSelector setSelectedSegment:kTreeTabIndex];
	[tabs selectTabViewItemAtIndex:kTreeTabIndex];
}

-(IBAction)selectTextTab:(id)sender {
	
	if (_lastTextKind != Kind_Text)
	{
		_lastTextKind = Kind_Text;
		[self updateTextTabWithFileAtIndex:selectedFileIndex];
	}
	[tabSelector setSelectedSegment:kTextTabIndex];
	[tabs selectTabViewItemAtIndex:kTextTabIndex];
}

-(void)_selectViewOFKind:(ViewMenu_Kind)_kind
{
	_lastTextKind = _kind;
	[tabSelector setSelectedSegment:kOtherViewsSelectorIndex];
	[self updateTextTabWithFileAtIndex:selectedFileIndex];
	[tabs selectTabViewItemAtIndex:kTextTabIndex];
}

-(IBAction)selectViewMPEG7:(id)sender
{
	[self _selectViewOFKind:Kind_MPEG7];
}

-(IBAction)selectViewPBCore:(id)sender
{
	[self _selectViewOFKind:Kind_PBCore];
}

-(IBAction)selectViewPBCore2:(id)sender
{
	[self _selectViewOFKind:Kind_PBCore2];
}

-(IBAction)selectViewEBUCore15:(id)sender
{
	[self _selectViewOFKind:Kind_EBUCore_1_5];
}

-(IBAction)selectViewEBUCore16:(id)sender
{
	[self _selectViewOFKind:Kind_EBUCore_1_6];
}


-(IBAction)selectViewFIMS11:(id)sender
{
	[self _selectViewOFKind:Kind_FIMS_1_1];
}

-(IBAction)selectViewFIMS12:(id)sender
{
	[self _selectViewOFKind:Kind_FIMS_1_2];
}

-(IBAction)selectViewFIMS13:(id)sender
{
	[self _selectViewOFKind:Kind_FIMS_1_3];
}

-(IBAction)selectViewReVTMD:(id)sender
{
	[self _selectViewOFKind:Kind_reVTMD];
}


- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender {
	
	return NSDragOperationCopy;
	//return NSDragOperationLink;
	//return NSDragOperationGeneric;
}


- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {
	
	NSPasteboard *pboard = [sender draggingPasteboard];
	NSDragOperation sourceDragMask = [sender draggingSourceOperationMask];
	
	if([[pboard types] containsObject:NSFilenamesPboardType]) {
		
		if(sourceDragMask & NSDragOperationCopy) {
			
			NSArray *a = [pboard propertyListForType:NSFilenamesPboardType];
			
			NSMutableArray *r = [NSMutableArray array];
			for(NSString *path in a) {
				[r addObject:[NSURL fileURLWithPath:path]];
			}
			
			[self performSelector:@selector(processFiles:) withObject:r afterDelay:0.1];
		}
	}
	
	return YES;
}


-(IBAction)changeExportFormat:(id)sender
{
	if (!_exportSavePanel)
		return;
	
	NSInteger tag = exportFormatButton.selectedTag;
	
	if (tag > 0)
		[_exportSavePanel setAllowedFileTypes:@[@"xml"]];
	else
		[_exportSavePanel setAllowedFileTypes:@[@"txt"]];
}

-(IBAction)export:(id)sender {
	
	//save panel
	NSSavePanel *savePanel	= [NSSavePanel savePanel];
	_exportSavePanel = savePanel;
	
	[savePanel setCanCreateDirectories:YES];
	//[savePanel setCanSelectHiddenExtension:YES];
	[savePanel setExtensionHidden:NO];
	[savePanel setAccessoryView:formatSelectionAccView];

	[savePanel setNameFieldStringValue:@"MediaInfo.txt"];
	[self changeExportFormat:nil];
	
	[savePanel beginSheetModalForWindow:[self window] completionHandler:^(NSInteger result){
		
		if (result == NSFileHandlingPanelOKButton)
		{
			MediaInfoExporter *exporter = [[[MediaInfoExporter alloc] initWithObj:mediaList] autorelease];
			exporter.extensionHidden = [savePanel isExtensionHidden];
			
			BOOL result;
			NSString *format = nil;
			switch (exportFormatButton.selectedTag)
			{
				case 1:
					format = TextKindToNSString(Kind_MPEG7);
					break;
				case 2:
					format = TextKindToNSString(Kind_PBCore);
					break;
				case 3:
					format = TextKindToNSString(Kind_PBCore2);
					break;
				case 4:
					format = TextKindToNSString(Kind_EBUCore_1_5);
					break;
				case 5:
					format = TextKindToNSString(Kind_EBUCore_1_6);
					break;
				case 6:
					format = TextKindToNSString(Kind_FIMS_1_1);
					break;
				case 7:
					format = TextKindToNSString(Kind_FIMS_1_2);
					break;
				case 8:
					format = TextKindToNSString(Kind_FIMS_1_3);
					break;
				case 9:
					format = TextKindToNSString(Kind_reVTMD);
					break;
					
				case 0:
				default:
					//Text or Unknown tag
					break;
			}

			if (format)
				result = [exporter exportFormat:format toUrl:savePanel.URL];
			else
				result = [exporter exportToText:savePanel.URL];
				
			if (!result)
			{
				[[NSAlert alertWithMessageText:NSLocalizedString(@"Error", @"Error header") 
								 defaultButton:nil 
							   alternateButton:nil 
								   otherButton:nil 
					 informativeTextWithFormat:NSLocalizedString(@"Can not save file", @"Error text while export")] runModal];
			}
			
			_exportSavePanel = nil;
		}
	}];
	
}


#pragma mark -
#pragma mark Processing

-(void)processFiles:(NSArray *)URLs {
	
	//Process files...
	
	if(mediaList != nil) {
		[mediaList release];
		mediaList = nil;
	}
	
	mediaList = [[oMediaInfoList alloc] init]; //dont care about release
	
	if([mediaList openFiles:URLs]) {
		
		//Update GUI
		
		NSArray *files = [mediaList files];
		
		NSUInteger max = [files count];
		
		NSMutableArray *array = [NSMutableArray array];
		
		for(NSUInteger i=0; i<max; i++) {
			
			NSDictionary *listElement = [NSDictionary dictionaryWithObjectsAndKeys:
										 [files objectAtIndex:i], @"value",
										 [files objectAtIndex:i], @"title",
										 nil];
			
			[array addObject:listElement];
		}
		
		[comboController setContent:array];
		
		//display first file
		[self setSelectedFileIndex:0];
		
	}
	else {
		//Report about some error while opening?
		
		[[NSAlert alertWithMessageText:NSLocalizedString(@"Error", @"Error header")
						 defaultButton:nil 
					   alternateButton:nil 
						   otherButton:nil 
			 informativeTextWithFormat:NSLocalizedString(@"Can not open file(s)", @"Error text while open")] runModal];
	}
	
	
}


-(void)showFileAtIndex:(NSUInteger)index {
	
	// Easy view
	
	[self updateEasyTabWithFileAtIndex:index];
	
	
	//Text View
	
	[self updateTextTabWithFileAtIndex:index];

	
	//go,go,go
	
	NSString *info = [mediaList informAtIndex:index];
	

	//tree view
	
	NSArray *array = [info componentsSeparatedByString:@"\n"];
	NSInteger max = [array count];
	if (max == 1)
	{
		array = [info componentsSeparatedByString:@"\r"];
         max = [array count];
	}
	NSMutableArray *finalArray = [NSMutableArray array];
	NSInteger i;
	NSMutableArray *currentRoot = finalArray;
	
	for(i=0; i<max; i++) {
		
		NSString *tmp = [array objectAtIndex:i];
		if([tmp isEqualToString:@""]) continue;
		
		NSArray *a = [tmp componentsSeparatedByString:@" : "];
		
		if(2 == [a count])
		{
			NSString *name = [[a objectAtIndex:0] stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceAndNewlineCharacterSet]];
			NSString *value = [[a objectAtIndex:1] stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceAndNewlineCharacterSet]];
			
			NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
								  name, @"name",
								  value, @"value",
								  [NSString stringWithFormat:@"%@ : %@", name, value], @"extValue",
								  nil];
			
			[currentRoot addObject:dict];
			
		}
		else
		{
			NSMutableArray *children = [NSMutableArray array];
			
			NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
								  tmp, @"name",
								  tmp, @"extValue",
								  children, @"children",
								  nil];
			
			[finalArray addObject:dict];
			currentRoot = children;
		}
										   
	}
	
	[treeOutlineController setContent:finalArray];
	[treeOutline expandItem:nil expandChildren:YES];
	
	
	//recent items
	NSString *filename = [mediaList filenameAtIndex:index];
	[[NSDocumentController sharedDocumentController] noteNewRecentDocumentURL:[NSURL fileURLWithPath:filename]];

}


-(void)updateEasyTabWithFileAtIndex:(NSUInteger)index {
	
	NSMutableArray *generalLeft = [NSMutableArray array];
	NSMutableArray *streamsContent = [NSMutableArray array];
	
	//left side of general
	
	[mediaList setOption:@"Inform" withValue:@"Summary"];
	NSString *generalInfo = [mediaList GetAtIndex:index streamKind:oMediaInfoStream_General streamNumber:0 parameter:@"Inform"];
	[mediaList setOption:@"Inform" withValue:@""];
	
	[generalLeft addObject:generalInfo];
	
	//url for general
	NSString *generalUrlStr = [mediaList GetAtIndex:index streamKind:oMediaInfoStream_General streamNumber:0 parameter:@"CodecID/Url"];
	if([generalUrlStr isEqualToString:@""])
		generalUrlStr = [mediaList GetAtIndex:index streamKind:oMediaInfoStream_General streamNumber:0 parameter:@"Format/Url"];
	
	if(![generalUrlStr isEqualToString:@""]) {
		[easyGeneralLinkButton setActionUrl:[NSURL URLWithString:generalUrlStr]];
		[easyGeneralLinkButton setHidden:NO];
	}
	else {
		[easyGeneralLinkButton setHidden:YES];
	}
	
	//streams
	
	oMediaInfoStream kinds[6] = {oMediaInfoStream_Video, oMediaInfoStream_Audio, oMediaInfoStream_Text,
								oMediaInfoStream_Menu, oMediaInfoStream_Chapters, oMediaInfoStream_Image};
	
	for(int i=0; i<6; i++) {
		
		NSInteger numberOfStreams = [mediaList numberOFStreamsAtIndex:index ofStreamKind:kinds[i]];
		NSString *streamName = [mediaList GetAtIndex:index streamKind:kinds[i] streamNumber:0 parameter:@"StreamKind"];
		
		if(numberOfStreams > 0) {

			NSString *streamFormatListParameter = [streamName stringByAppendingString:@"_Format_WithHint_List"];
			NSString *hint = [mediaList GetAtIndex:index streamKind:oMediaInfoStream_General streamNumber:0 parameter:streamFormatListParameter];
			
			if(kinds[i] != oMediaInfoStream_Chapters && kinds[i] != oMediaInfoStream_Image) { //skip Chapters and Image streams in general box
				[generalLeft addObject:[NSString stringWithFormat:@"%ld %@ stream%@: %@", (long)numberOfStreams, streamName, numberOfStreams>1?@"s":@"", hint]];
			}
			
			for(int streamNum=0; streamNum<numberOfStreams; streamNum++) {
				
				NSString *contentHeader = numberOfStreams > 1 ? [NSString stringWithFormat:@"%@ #%ld:", streamName, (long)streamNum+1] : [streamName stringByAppendingString:@":"];
				
				
				[mediaList setOption:@"Inform" withValue:@"Summary"];
				NSString *streamInfo = [mediaList GetAtIndex:index streamKind:kinds[i] streamNumber:streamNum parameter:@"Inform"];
				[mediaList setOption:@"Inform" withValue:@""];
				
				NSString *streamTitle = [mediaList GetAtIndex:index streamKind:kinds[i] streamNumber:streamNum parameter:@"Title"];
				
				NSString *contentValue = [streamTitle isEqualToString:@""] ? streamInfo : [NSString stringWithFormat:@"%@\n%@", streamInfo, streamTitle];
				
				[streamsContent addObject:[NSDictionary dictionaryWithObjectsAndKeys:
										   contentHeader,@"header",
										   contentValue,@"value",
										   nil]];
			}

		}
		else {
			[generalLeft addObject:@""]; //empty line like in original MI
		}
	}
	
	[easyGeneralTextLeft setStringValue:[generalLeft componentsJoinedByString:@"\n"]];
	[easyGeneralTextRight setStringValue:[mediaList generalTagsAtIndex:index limit:5]];
	[easyStreamsController setContent:streamsContent];

}


-(void)updateTextTabWithFileAtIndex:(NSUInteger)index
{
	NSString *_inform = TextKindToNSString(_lastTextKind);
	if (!_inform)
	{
		[mediaList setOption:@"Inform" withValue:@""];
		[self _updateTextTabWithContentOfAttributedTextAtIndex:index];
	}
	else
	{
		[mediaList setOption:@"Inform" withValue:_inform];
		[self _updateTextTabWithContentOfSimpleTextAtIndex:index];
		[mediaList setOption:@"Inform" withValue:@""];
	}
}

-(void)_updateTextTabWithContentOfAttributedTextAtIndex:(NSUInteger)index
{
	NSString *info = [mediaList informAtIndex:index];
	if (!info) return;
	
	//prepare font attributes for text view
	
	NSString *fontFamily = [[NSFont userFontOfSize:13.0] familyName];
	
	NSFontManager *fontManager = [NSFontManager sharedFontManager];
	NSFont *boldFont = [fontManager fontWithFamily:fontFamily
											traits:NSBoldFontMask
											weight:0
											  size:13];
	NSFont *normalFont = [fontManager fontWithFamily:fontFamily
											  traits:0
											  weight:0
												size:13];
	
	NSDictionary *normalAttr = [NSDictionary dictionaryWithObjectsAndKeys:
								normalFont, NSFontAttributeName,
								nil];
	NSDictionary *boldAttr = [NSDictionary dictionaryWithObjectsAndKeys:
							  boldFont, NSFontAttributeName,
							  nil];
	
	//go
	
	
	NSArray *array = [info componentsSeparatedByString:@"\n"];
	NSInteger max = [array count];
	if (max == 1)
	{
		array = [info componentsSeparatedByString:@"\r"];
		max = [array count];
	}
	
	NSMutableArray *textArray = [NSMutableArray array];
	
	for(NSInteger i=0; i<max; i++)
	{
		NSString *tmp = [array objectAtIndex:i];
		if([tmp isEqualToString:@""]) continue;
		
		NSArray *a = [tmp componentsSeparatedByString:@" : "];
		
		NSAttributedString *strToAdd = nil;
		
		if(2 == [a count])
		{
			NSString *name = [[a objectAtIndex:0] stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceAndNewlineCharacterSet]];
			NSString *value = [[a objectAtIndex:1] stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceAndNewlineCharacterSet]];
			
			strToAdd = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"\n%@ : %@", name, value]
													   attributes:normalAttr];
			
		}
		else
		{
			NSString *prefix = (i>0 ? @"\n\n" : @"");
			strToAdd = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%@%@",prefix,tmp]
													   attributes:boldAttr];
		}
		
		[textArray addObject:strToAdd];
		[strToAdd release];
	}
	
	//text view example
	//to disable word-wrapping see http://snipplr.com/view/2676/
	
	NSMutableAttributedString *attrStr = [[NSMutableAttributedString alloc] initWithString:@""];
	
	max = [textArray count];
	for(NSInteger i=0; i<max; i++) {
		[attrStr appendAttributedString:[textArray objectAtIndex:i]];
	}
	
	[[textField textStorage] setAttributedString:attrStr];
	[attrStr release];
}


-(void)_updateTextTabWithContentOfSimpleTextAtIndex:(NSUInteger)index
{
	NSString *info = [mediaList informAtIndex:index];
	if (!info) return;
	
	//prepare font attributes for text view

	CGFloat fontSize = 12.0;
	NSString *fontFamily = [[NSFont userFixedPitchFontOfSize:fontSize] familyName];
	
	//CGFloat fontSize = 13.0;
	//NSString *fontFamily = [[NSFont userFontOfSize:fontSize] familyName];

	
	NSFontManager *fontManager = [NSFontManager sharedFontManager];
	NSFont *normalFont = [fontManager fontWithFamily:fontFamily
											  traits:0
											  weight:0
												size:fontSize];
	
	NSDictionary *normalAttr = [NSDictionary dictionaryWithObjectsAndKeys:
								normalFont, NSFontAttributeName,
								nil];

	
	//fix \r and \n
	NSArray *array = [info componentsSeparatedByString:@"\n"];
	if (array.count == 1)
		array = [info componentsSeparatedByString:@"\r"];
	
	NSAttributedString *as = [[NSAttributedString alloc] initWithString:[array componentsJoinedByString:@"\n"]
															 attributes:normalAttr];
	

	[textField.textStorage setAttributedString:as];
	[as release];
}


#pragma mark -
#pragma mark comboController friends

-(NSInteger)selectedFileIndex {
	return selectedFileIndex;
}

-(void)setSelectedFileIndex:(NSInteger)index {
	selectedFileIndex = index;
	[self showFileAtIndex:index];
}


#pragma mark -
#pragma mark delegates of <NSWindowDelegate> 
- (void)windowWillClose:(NSNotification *)notification {
	[self release];
}

#pragma mark -
#pragma mark menu delegates

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem {

	SEL action = [menuItem action];
	
	if(action == @selector(selectTreeTab:)) {
		[menuItem setState: ([tabSelector selectedSegment] == kTreeTabIndex ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectTextTab:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_Text ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewMPEG7:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_MPEG7 ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewPBCore:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_PBCore ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewPBCore2:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_PBCore2 ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewEBUCore15:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_EBUCore_1_5 ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewEBUCore16:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_EBUCore_1_6 ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewFIMS11:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_FIMS_1_1 ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewFIMS12:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_FIMS_1_2 ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewFIMS13:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_FIMS_1_3 ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewReVTMD:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_reVTMD ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(export:)) {
		return (mediaList != nil); //be careful if it's in background processing
	}
	
	return YES;
}


@end
