//
//  MyWindowController.m
//  MediaInfo
//
//  Created by Max Pozdeev on 07.02.12.
//  Copyright 2012-2020 Jerome Martinez. All rights reserved.
//

#import "MyWindowController.h"
#import "oMediaInfoList.h"
#import "MediaInfoExporter.h"
#import "SubscriptionManager.h"
#import "SubscribeWindowController.h"
#import "ProgressDialog.h"
#define kEasyTabIndex 0
#define kTreeTabIndex 1
#define kTextTabIndex 2
#define kCompareTabIndex 3

#define kApplicationMenuTag 10
#define kSubscribeMenuItemTag 11
#define kViewMenuTag 50
#define kCompareMenuItemTag 51
#define kAdvancedMenuItemTag 52
#define kAdvancedMenuItemSeparatorTag 53

NSString* TextKindToNSString(ViewMenu_Kind kind)
{
	NSString *_ret = nil;
	switch (kind)
	{
		case Kind_HTML:			_ret = @"HTML"; break;
		case Kind_XML:			_ret = @"XML"; break;
		case Kind_CSV:			_ret = @"CSV"; break;
		case Kind_JSON:			_ret = @"JSON"; break;
		case Kind_MPEG7:		_ret = @"MPEG-7"; break;
		case Kind_PBCore:		_ret = @"PBCore"; break;
		case Kind_PBCore2:		_ret = @"PBCore2"; break;
		case Kind_EBUCore_1_5:	_ret = @"EBUCore_1.5"; break;
		case Kind_EBUCore_1_6:	_ret = @"EBUCore_1.6"; break;
		case Kind_EBUCore_1_8_ps:	_ret = @"EBUCore_1.8_ps"; break;
		case Kind_EBUCore_1_8_sp:	_ret = @"EBUCore_1.8_sp"; break;
		case Kind_EBUCore_1_8_ps_json:	_ret = @"EBUCore_1.8_ps_JSON"; break;
		case Kind_EBUCore_1_8_sp_json:	_ret = @"EBUCore_1.8_sp_JSON"; break;
		case Kind_FIMS_1_1:		_ret = @"FIMS_1.1"; break;
		case Kind_FIMS_1_2:		_ret = @"FIMS_1.2"; break;
		case Kind_FIMS_1_3:		_ret = @"FIMS_1.3"; break;
		case Kind_reVTMD:		_ret = @"reVTMD"; break;
		case Kind_NISO_Z39_87:		_ret = @"NISO_Z39.87"; break;
		case Kind_Text:
								_ret = @"";
		default:				break;
	}
	return _ret;
}

@implementation MyWindowController

-(void)dealloc {
	[mediaList release];
    for(id observer in observers) {
        [[NSNotificationCenter defaultCenter] removeObserver:observer];
    }
    [observers release];
	[super dealloc];
}

-(void)awakeFromNib {
	//make transparent background for table in easy tab
	[easyTable setBackgroundColor:[NSColor clearColor]];
	[easyGeneralLinkButton setHidden:YES];
	_lastTextKind = Kind_Text;
	[tabSelector setMenu:otherViewsMenu forSegment:tabSelector.segmentCount - 1];
	_exportSavePanel = nil;
    fileSelectorIsHidden = NO;
    subscriptionEnabled = NO;

    observers = [[NSMutableArray alloc] init];

    if (@available(macOS 10.9, *)) {
        [subscribeButton setEnabled:YES forSegment:0];
        if([[SubscriptionManager shared] subscriptionActive]) {
            [self enableSubscription];

             NSString* defaultView = [[NSUserDefaults standardUserDefaults] stringForKey:@"defaultView"];

            if (defaultView == nil || [defaultView isEqualToString:@"Easy"])
                [self selectEasyTab:nil];
            else if ([defaultView isEqualToString:@"Tree"])
                [self selectTreeTab:nil];
            else if ([defaultView isEqualToString:@"Compare"])
                [self selectCompareTab:nil];
            else if ([defaultView isEqualToString:@"Text"])
                [self selectTextTab:nil];
            else if ([defaultView isEqualToString:@"XML"])
                [self selectViewXML:nil];
            else if ([defaultView isEqualToString:@"JSON"])
                [self selectViewJSON:nil];
            else if ([defaultView isEqualToString:@"MPEG-7"])
                [self selectViewMPEG7:nil];
            else if ([defaultView isEqualToString:@"PBCore"])
                [self selectViewPBCore:nil];
            else if ([defaultView isEqualToString:@"PBCore2"])
                [self selectViewPBCore2:nil];
            else if ([defaultView isEqualToString:@"EBUCore_1.5"])
                [self selectViewEBUCore15:nil];
             else if ([defaultView isEqualToString:@"EBUCore_1.6"])
                [self selectViewEBUCore16:nil];
            else if ([defaultView isEqualToString:@"EBUCore_1.8_ps"])
                [self selectViewEBUCore18_ps:nil];
            else if ([defaultView isEqualToString:@"EBUCore_1.8_sp"])
                [self selectViewEBUCore18_sp:nil];
            else if ([defaultView isEqualToString:@"EBUCore_1.8_ps_JSON"])
                [self selectViewEBUCore18_ps_json:nil];
            else if ([defaultView isEqualToString:@"EBUCore_1.8_sp_JSON"])
                [self selectViewEBUCore18_sp_json:nil];
            else if ([defaultView isEqualToString:@"FIMS_1.1"])
                [self selectViewFIMS11:nil];
            else if ([defaultView isEqualToString:@"FIMS_1.2"])
                [self selectViewFIMS12:nil];
            else if ([defaultView isEqualToString:@"FIMS_1.3"])
                [self selectViewFIMS13:nil];
            else if ([defaultView isEqualToString:@"reVTMD"])
                [self selectViewReVTMD:nil];
            else if ([defaultView isEqualToString:@"NISO_Z39.87"])
                [self selectViewNISO_Z39_87:nil];
            }
        else {
            [observers addObject:[[NSNotificationCenter defaultCenter] addObserverForName:[SubscriptionManager subscriptionStateChangedNotification] object:[SubscriptionManager shared] queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *notification) {
                [self enableSubscription];
                [self selectCompareTab:nil];
            }]];
        }

        NSMenu *menu = [NSApp mainMenu];
        if(menu) {
            NSMenuItem *main = [menu itemWithTag:kApplicationMenuTag];
            if(main && main.submenu) {
                NSMenuItem *item = [main.submenu itemWithTag:kSubscribeMenuItemTag];
                if(item) {
                    [item setHidden:NO];
                }
            }
        }
    }
    else {
        [tabSelector setEnabled:NO forSegment:kCompareTabIndex];
    }
}

#pragma mark -
#pragma mark GUI routines

- (IBAction)openFile:(id)sender {
	NSOpenPanel *openPanel	= [NSOpenPanel openPanel];
	[openPanel setCanChooseDirectories:YES];
	[openPanel setAllowsMultipleSelection:YES];
	[openPanel setCanDownloadUbiquitousContents:NO];
	[openPanel beginSheetModalForWindow:[self window] completionHandler:^(NSInteger result){
		if(result == NSFileHandlingPanelOKButton) {
			[self processFiles:[openPanel URLs]];
		}
	}];
}


-(IBAction)clickOnViewSelector:(id)sender {
    NSSegmentedControl *control = (NSSegmentedControl *)sender;

    int index = [control selectedSegment];
    if(index == control.segmentCount - 1) {
    }
	else if(index == kCompareTabIndex) {
		[self selectCompareTab:nil];
	}
	else if(index == kTextTabIndex) {
		[self selectTextTab:nil];
	}
	else if(index == kTreeTabIndex) {
		[self selectTreeTab:nil];
	}
	else if(index == kEasyTabIndex) {
		[self selectEasyTab:nil];
	}
}

-(void)showFileSelector {
    if(fileSelectorIsHidden) {
        [comboBox setHidden:NO];
        [hline setHidden:NO];

        NSRect frame = [tabs frame];
        frame.size.height-=42;
        [tabs setFrame:frame];

        fileSelectorIsHidden = NO;
    }
}

-(void)hideFileSelector {
    if(!fileSelectorIsHidden) {
        [comboBox setHidden:YES];
        [hline setHidden:YES];

        NSRect frame = [tabs frame];
        frame.size.height+=42;
        [tabs setFrame:frame];

        fileSelectorIsHidden = YES;
    }
}

-(IBAction)selectCompareTab:(id)sender {
    if (@available(macOS 10.9, *)) {
        [self hideFileSelector];
        [tabSelector setSelectedSegment:kCompareTabIndex];
        [tabs selectTabViewItemAtIndex:kCompareTabIndex];
    }
}

-(IBAction)selectEasyTab:(id)sender {
    [self showFileSelector];
	[tabSelector setSelectedSegment:kEasyTabIndex];
	[tabs selectTabViewItemAtIndex:kEasyTabIndex];
}

-(IBAction)selectTreeTab:(id)sender {
    [self showFileSelector];
	[tabSelector setSelectedSegment:kTreeTabIndex];
	[tabs selectTabViewItemAtIndex:kTreeTabIndex];
}

-(IBAction)selectTextTab:(id)sender {
    [self showFileSelector];
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
    [self showFileSelector];
	_lastTextKind = _kind;
	[tabSelector setSelectedSegment:tabSelector.segmentCount - 1];
	[self updateTextTabWithFileAtIndex:selectedFileIndex];
	[tabs selectTabViewItemAtIndex:kTextTabIndex];
}

-(IBAction)selectViewXML:(id)sender
{
	[self _selectViewOFKind:Kind_XML];
}

-(IBAction)selectViewJSON:(id)sender
{
	[self _selectViewOFKind:Kind_JSON];
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

-(IBAction)selectViewEBUCore18_ps:(id)sender
{
	[self _selectViewOFKind:Kind_EBUCore_1_8_ps];
}

-(IBAction)selectViewEBUCore18_sp:(id)sender
{
	[self _selectViewOFKind:Kind_EBUCore_1_8_sp];
}

-(IBAction)selectViewEBUCore18_ps_json:(id)sender
{
	[self _selectViewOFKind:Kind_EBUCore_1_8_ps_json];
}

-(IBAction)selectViewEBUCore18_sp_json:(id)sender
{
	[self _selectViewOFKind:Kind_EBUCore_1_8_sp_json];
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

-(IBAction)selectViewNISO_Z39_87:(id)sender
{
	[self _selectViewOFKind:Kind_NISO_Z39_87];
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

	if (tag == 4 || tag == 12 || tag == 13)
		[_exportSavePanel setAllowedFileTypes:@[@"json"]];
	else if (tag == 3)
		[_exportSavePanel setAllowedFileTypes:@[@"csv"]];
	else if (tag == 1)
		[_exportSavePanel setAllowedFileTypes:@[@"html"]];
	else if (tag > 1)
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
					format = TextKindToNSString(Kind_HTML);
					break;
				case 2:
					format = TextKindToNSString(Kind_XML);
					break;
				case 3:
					format = TextKindToNSString(Kind_CSV);
					break;
				case 4:
					format = TextKindToNSString(Kind_JSON);
					break;
				case 5:
					format = TextKindToNSString(Kind_MPEG7);
					break;
				case 6:
					format = TextKindToNSString(Kind_PBCore);
					break;
				case 7:
					format = TextKindToNSString(Kind_PBCore2);
					break;
				case 8:
					format = TextKindToNSString(Kind_EBUCore_1_5);
					break;
				case 9:
					format = TextKindToNSString(Kind_EBUCore_1_6);
					break;
				case 10:
					format = TextKindToNSString(Kind_EBUCore_1_8_ps);
					break;
				case 11:
					format = TextKindToNSString(Kind_EBUCore_1_8_sp);
					break;
				case 12:
					format = TextKindToNSString(Kind_EBUCore_1_8_ps_json);
					break;
				case 13:
					format = TextKindToNSString(Kind_EBUCore_1_8_sp_json);
					break;
				case 14:
					format = TextKindToNSString(Kind_FIMS_1_1);
					break;
				case 15:
					format = TextKindToNSString(Kind_FIMS_1_2);
					break;
				case 16:
					format = TextKindToNSString(Kind_FIMS_1_3);
					break;
				case 17:
					format = TextKindToNSString(Kind_reVTMD);
					break;
				case 18:
					format = TextKindToNSString(Kind_NISO_Z39_87);
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

-(void)enableSubscription {
    if(subscriptionEnabled)
        return;

    NSMenu *menu = [NSApp mainMenu];
    if(menu) {
        NSMenuItem *main = [menu itemWithTag:kApplicationMenuTag];
        if(main && main.submenu) {
            NSMenuItem *item = [main.submenu itemWithTag:kSubscribeMenuItemTag];
            if(item) {
                [item setTitle:NSLocalizedString(@"Manage Subscription", @"Manage Subscription")];
            }
        }

        NSMenuItem *view = [menu itemWithTag:kViewMenuTag];
        if(view && view.submenu) {
            NSMenuItem *compare = [view.submenu itemWithTag:kCompareMenuItemTag];
            [compare setHidden:NO];

            NSMenuItem *advanced = [view.submenu itemWithTag:kAdvancedMenuItemTag];
            [advanced setHidden:NO];

            NSMenuItem *separator = [view.submenu itemWithTag:kAdvancedMenuItemSeparatorTag];
            [separator setHidden:NO];
        }
    }

    if (@available(macOS 10.13, *)) {
        [tabSelector setToolTip:NSLocalizedString(@"Compare View", @"Compare View") forSegment:kCompareTabIndex];
        [subscribeButton setToolTip:NSLocalizedString(@"Manage subscription", @"Manage Subscription") forSegment:0];
    }

    NSInteger index = [[mainToolbar items] indexOfObject:subscribeButtonItem];
    if (index != NSNotFound)
        [mainToolbar removeItemAtIndex:index];

    [tabSelector setEnabled:YES forSegment:kCompareTabIndex];

    /* NSUInteger lastSegment = tabSelector.segmentCount - 1;
    NSString *segmentLabel = [tabSelector labelForSegment:lastSegment];
    NSImage *segmentImage = [tabSelector imageForSegment:lastSegment];
    NSMenu *segmentMenu = [tabSelector menuForSegment:lastSegment];
    CGFloat segmentWidth = [tabSelector widthForSegment:lastSegment];
    NSImageScaling segmentScaling = [tabSelector imageScalingForSegment:lastSegment];
    NSTextAlignment segmentAlignment = NSTextAlignmentCenter;
    NSString *segmentTooltip = nil;
    if (@available(macOS 10.13, *)) {
        segmentAlignment = [tabSelector alignmentForSegment:lastSegment];
        segmentTooltip = [tabSelector toolTipForSegment:lastSegment];
    }

    [tabSelector setLabel:@"⇆" forSegment:lastSegment];
    [tabSelector setImage:nil forSegment:lastSegment];
    [tabSelector setMenu:nil forSegment:lastSegment];
    [tabSelector setWidth:28 forSegment:lastSegment];
    [tabSelector setImageScaling:NSImageScaleProportionallyDown forSegment:lastSegment];
    if (@available(macOS 10.13, *)) {
        [tabSelector setAlignment:NSTextAlignmentCenter forSegment:lastSegment];
        [tabSelector setToolTip:@"Compare View" forSegment:lastSegment];
    }

    [tabSelector setSegmentCount:tabSelector.segmentCount + 1];
    lastSegment++;

    [tabSelector setLabel:segmentLabel forSegment:lastSegment];
    [tabSelector setImage:segmentImage forSegment:lastSegment];
    [tabSelector setMenu:segmentMenu forSegment:lastSegment];
    [tabSelector setWidth:segmentWidth forSegment:lastSegment];
    [tabSelector setImageScaling:segmentScaling forSegment:lastSegment];
    if(@available(macOS 10.13, *)) {
        [tabSelector setAlignment:segmentAlignment forSegment:lastSegment];
        [tabSelector setToolTip:segmentTooltip forSegment:lastSegment];
    } */

    subscriptionEnabled = YES;
}

#pragma mark -
#pragma mark Processing

-(void)processFiles:(NSArray *)URLs {
	
	//Process files...
	if(!mediaList)
		mediaList = [[oMediaInfoList alloc] init]; //dont care about release

	NSInteger oldCount = [mediaList count];

	if([URLs count]==1 && [[NSFileManager defaultManager] fileExistsAtPath:[URLs[0] path]]) {
		if(![mediaList openFiles:URLs]) {
			//Report about some error while opening?
			[[NSAlert alertWithMessageText:NSLocalizedString(@"Error", @"Error header")
				defaultButton:nil
				alternateButton:nil
				otherButton:nil
				informativeTextWithFormat:NSLocalizedString(@"Can not open file(s)", @"Error text while open")] runModal];
		}
	}
	else {
		ProgressDialog *wc = [[ProgressDialog alloc] initWithWindowNibName:@"ProgressDialog"];
		[wc setItems:URLs];
		[wc setMediaList:mediaList];

		[NSApp runModalForWindow:wc.window];
		[wc.window close];
		wc = nil;
	}

	if([mediaList count]>oldCount) {
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
		[compareView setFiles:mediaList];
		[treeView setFiles:mediaList];

		//display first added file
		[self setSelectedFileIndex:oldCount];
		[comboController setSelectionIndex:oldCount];
	}
}


-(void)showFileAtIndex:(NSUInteger)index {
	// Easy view
	[self updateEasyTabWithFileAtIndex:index];

	//Text View
	[self updateTextTabWithFileAtIndex:index];

	//tree view
	[treeView setIndex:index];

	// compare view
	[compareView reload];

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
                                NSColor.textColor, NSForegroundColorAttributeName,
								nil];
	NSDictionary *boldAttr = [NSDictionary dictionaryWithObjectsAndKeys:
							  boldFont, NSFontAttributeName,
                              NSColor.textColor, NSForegroundColorAttributeName,
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
								normalFont, NSFontAttributeName, NSColor.textColor, NSForegroundColorAttributeName,
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

    if(action == @selector(selectEasyTab:)) {
        [menuItem setState: ([tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kEasyTabIndex ? NSOnState : NSOffState)];
    }
    else if(action == @selector(selectTreeTab:)) {
		[menuItem setState: ([tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTreeTabIndex ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectTextTab:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_Text ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
    else if(action == @selector(selectCompareTab:)) {
		[menuItem setState: ([tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kCompareTabIndex ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewXML:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_XML ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewJSON:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_JSON ? YES : NO;
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
	else if(action == @selector(selectViewEBUCore18_ps:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_EBUCore_1_8_ps ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewEBUCore18_sp:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_EBUCore_1_8_sp ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewEBUCore18_ps_json:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_EBUCore_1_8_ps_json ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(selectViewEBUCore18_sp_json:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_EBUCore_1_8_sp_json ? YES : NO;
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
	else if(action == @selector(selectViewNISO_Z39_87:)) {
		BOOL state = [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kTextTabIndex && _lastTextKind == Kind_NISO_Z39_87 ? YES : NO;
		[menuItem setState: (state ? NSOnState : NSOffState)];
	}
	else if(action == @selector(export:)) {
		return (mediaList != nil); //be careful if it's in background processing
	}
    else if(action == @selector(selectNextTab:)) {
        return mediaList && [tabs indexOfTabViewItem:tabs.selectedTabViewItem] != kCompareTabIndex && [mediaList count] && selectedFileIndex < [mediaList count] - 1;
    }
    else if(action == @selector(selectPreviousTab:)) {
        return mediaList && [tabs indexOfTabViewItem:tabs.selectedTabViewItem] != kCompareTabIndex && [mediaList count] && selectedFileIndex > 0;
    }
    else if(action == @selector(closeFile:) || action == @selector(closeAllFiles:)) {
        if(action == @selector(closeFile:) && [tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kCompareTabIndex)
            return mediaList && compareView.selectedIndex > 0 && compareView.selectedIndex - 1 < [mediaList count];

        return mediaList && [mediaList count];
    }

	return YES;
}

-(void)closeFileAtIndex:(NSUInteger)index {
    if(mediaList && index < [mediaList count]) {
        NSUInteger oldSelectedFileIndex = selectedFileIndex;
        NSUInteger oldComboControllerIndex = [comboController selectionIndex];

        NSUInteger newSelectedFileIndex = oldSelectedFileIndex;
        NSUInteger newComboControllerIndex = oldComboControllerIndex;

        if((selectedFileIndex > 0) && ((index < selectedFileIndex) || (index == selectedFileIndex && index == [mediaList count] - 1))) {
            newSelectedFileIndex = oldSelectedFileIndex - 1;
            newComboControllerIndex = oldComboControllerIndex - 1;
        }

        NSMutableArray* array = [comboController content];
        [array removeObjectAtIndex:index];
        [comboController setContent:array];

        [mediaList closeAtIndex:index];

        [comboController setSelectionIndex:newComboControllerIndex];
        [self setSelectedFileIndex:newSelectedFileIndex];
    }
}

-(IBAction)selectNextTab:(id)sender {
    if(mediaList && [tabs indexOfTabViewItem:tabs.selectedTabViewItem] != kCompareTabIndex && [mediaList count] && selectedFileIndex < [mediaList count] - 1) {
        [comboController selectNext:nil];
        [self setSelectedFileIndex:selectedFileIndex + 1];
    }
}

-(IBAction)selectPreviousTab:(id)sender {
    if(mediaList && [tabs indexOfTabViewItem:tabs.selectedTabViewItem] != kCompareTabIndex && [mediaList count] && selectedFileIndex > 0) {
        [comboController selectPrevious:nil];
        [self setSelectedFileIndex:selectedFileIndex - 1];
    }
}

-(IBAction)closeFile:(id)sender {
    if(mediaList && [mediaList count]) {

        if ([tabs indexOfTabViewItem:tabs.selectedTabViewItem] == kCompareTabIndex) {
            if(compareView.selectedIndex > 0 && (compareView.selectedIndex - 1) < [mediaList count])
                [self closeFileAtIndex:(compareView.selectedIndex - 1)];

            return;
        }

        NSUInteger oldSelectedFileIndex = selectedFileIndex;
        NSUInteger oldComboControllerIndex = [comboController selectionIndex];
        NSUInteger newSelectedFileIndex = oldSelectedFileIndex;
        NSUInteger newComboControllerIndex = oldComboControllerIndex;

       if(selectedFileIndex > 0 && [mediaList count] > 1) {
           newSelectedFileIndex = oldSelectedFileIndex - 1;
           newComboControllerIndex = oldComboControllerIndex - 1;
        }

        NSMutableArray* array = [comboController content];
        [array removeObjectAtIndex:oldSelectedFileIndex];

        [comboController setContent:array];
        [mediaList closeAtIndex:oldSelectedFileIndex];

        [comboController setSelectionIndex:newComboControllerIndex];
        [self setSelectedFileIndex:newSelectedFileIndex];
    }
}

-(IBAction)closeAllFiles:(id)sender {
    if(mediaList && [mediaList count]) {
        while([mediaList count]) {
            [mediaList closeAtIndex:0];
        }
        [comboController setContent:nil];
        [self setSelectedFileIndex:0];
    }
}

- (IBAction)clickOnSubscribe:(id)sender {
    [[SubscribeWindowController controller] show];
}

-(IBAction)advancedMode:(id)sender {
    if([sender isKindOfClass: [NSMenuItem class]]) {
        NSMenuItem* item = sender;

        BOOL newState = !item.state;
        [item setState:newState];

        [oMediaInfoList setOptionStatic:@"Complete" withValue:(newState?@"1":@"")];

        if(mediaList && selectedFileIndex < [mediaList count]) {
            [self showFileAtIndex:selectedFileIndex];
        }
    }
}
@end
