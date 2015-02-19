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

@implementation MyWindowController

-(void)dealloc {
	[mediaList release];
	[super dealloc];
}

-(void)awakeFromNib {
	//make transparent background for table in easy tab
	[easyTable setBackgroundColor:[NSColor clearColor]];
	[easyGeneralLinkButton setHidden:YES];
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
	if(segment == 2) {
		[tabs selectTabViewItemAtIndex:kTextTabIndex];
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
	[tabSelector setSelectedSegment:kTextTabIndex];
	[tabs selectTabViewItemAtIndex:kTextTabIndex];
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


-(IBAction)export:(id)sender {
	
	//save panel
	NSSavePanel *savePanel	= [NSSavePanel savePanel];
	
	[savePanel setCanCreateDirectories:YES];
	//[savePanel setCanSelectHiddenExtension:YES];
	[savePanel setExtensionHidden:NO];

	[savePanel setAllowedFileTypes:[NSArray arrayWithObject:@"txt"]];
	[savePanel setNameFieldStringValue:@"MediaInfo.txt"];
	
	[savePanel beginSheetModalForWindow:[self window] completionHandler:^(NSInteger result){
		
		if(result == NSFileHandlingPanelOKButton) {
			
			MediaInfoExporter *exporter = [[[MediaInfoExporter alloc] initWithObj:mediaList] autorelease];
			exporter.extensionHidden = [savePanel isExtensionHidden];
			BOOL result = [exporter exportToText:[savePanel URL]];
			if(!result) {
				[[NSAlert alertWithMessageText:NSLocalizedString(@"Error", @"Error header") 
								 defaultButton:nil 
							   alternateButton:nil 
								   otherButton:nil 
					 informativeTextWithFormat:NSLocalizedString(@"Can not save file", @"Error text while export")] runModal];
			}
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
	
	
	//prepare font attributes for text view
	
	NSString *fontFamily = [[NSFont systemFontOfSize:13.0] familyName];
	
	NSFontManager *fontManager = [NSFontManager sharedFontManager];
	NSFont *boldFont = [fontManager fontWithFamily:fontFamily
											  traits:NSBoldFontMask
											  weight:0
												size:13];
	NSFont *normalFont = [fontManager fontWithFamily:fontFamily
											traits:0
											weight:0
											  size:13];
	
	NSDictionary *normalAttr = [[NSDictionary alloc] initWithObjectsAndKeys:
								normalFont, NSFontAttributeName,
								nil];
	NSDictionary *boldAttr = [[NSDictionary alloc] initWithObjectsAndKeys:
								boldFont, NSFontAttributeName,
								nil];
	
	//go,go,go
	
	NSString *info = [mediaList informAtIndex:index];
	

	//tree view
	
	NSArray *array = [info componentsSeparatedByString:@"\r"];
	NSMutableArray *finalArray = [NSMutableArray array];
	NSMutableArray *textArray = [NSMutableArray array];

	
	
	NSInteger i, max = [array count];
	NSMutableArray *currentRoot = finalArray;
	
	
	
	for(i=0; i<max; i++) {
		
		NSString *tmp = [array objectAtIndex:i];
		if([tmp isEqualToString:@""]) continue;
		
		NSArray *a = [tmp componentsSeparatedByString:@" : "];
		
		NSAttributedString *strToAdd = nil;
		
		if(2 == [a count]) {
			
			NSString *name = [[a objectAtIndex:0] stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceAndNewlineCharacterSet]];
			NSString *value = [[a objectAtIndex:1] stringByTrimmingCharactersInSet: [NSCharacterSet whitespaceAndNewlineCharacterSet]];
			
			NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
								  name, @"name",
								  value, @"value",
								  [NSString stringWithFormat:@"%@ : %@", name, value], @"extValue",
								  nil];
			
			[currentRoot addObject:dict];
			
			strToAdd = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"\n%@ : %@", name, value]
													   attributes:normalAttr];

		}
		else {
			
			NSMutableArray *children = [NSMutableArray array];
			
			NSDictionary *dict = [NSDictionary dictionaryWithObjectsAndKeys:
								  tmp, @"name",
								  tmp, @"extValue",
								  children, @"children",
								  nil];
			
			[finalArray addObject:dict];
			currentRoot = children;
			
			NSString *prefix = (i>0 ? @"\n\n" : @"");
			strToAdd = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%@%@",prefix,tmp]
													   attributes:boldAttr];
	
			
		}
										   
		[textArray addObject:strToAdd];
										   
										   
	}
	
	[treeOutlineController setContent:finalArray];
	[treeOutline expandItem:nil expandChildren:YES];
	
	
	//text view example
	//to disable word-wrapping see http://snipplr.com/view/2676/
	
	NSMutableAttributedString *attrStr = [[NSMutableAttributedString alloc] initWithString:@""];
	
	max = [textArray count];
	for(i=0; i<max; i++) {
		[attrStr appendAttributedString:[textArray objectAtIndex:i]];
	}
	
	[[textField textStorage] setAttributedString:attrStr];

	
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
		[menuItem setState: ([tabSelector selectedSegment] == kTextTabIndex ? NSOnState : NSOffState)];
	}
	else if(action == @selector(export:)) {
		return (mediaList != nil); //be careful if it's in background processing
	}
	
	return YES;
}


@end
