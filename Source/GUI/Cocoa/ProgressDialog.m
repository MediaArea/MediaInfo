/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

#import "ProgressDialog.h"

@interface ProgressDialog ()
@end

@implementation ProgressDialog

-(void)dealloc {
    for(id observer in observers) {
        [[NSNotificationCenter defaultCenter] removeObserver:observer];
    }
    [observers release];
    [coordinator release];

    [super dealloc];
}

-(void)windowDidLoad {
    [super windowDidLoad];

    coordinator = [[NSFileCoordinator alloc] initWithFilePresenter:nil];
    canceled = NO;

    _progressBar.indeterminate=YES;
    [_progressBar startAnimation:self];

    if(_mediaList && _items && [_items count]) {
        [self queryItem:0];
    }
    else {
        [self cancel];
    }
}

-(void)cancel {
    canceled = YES;
    [coordinator cancel];
    [_progressBar stopAnimation:self];

    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [NSApp abortModal];
    });
}

-(void)next {
    if(!canceled && currentItemIndex < [_items count] - 1) {
        [self queryItem:currentItemIndex + 1];
    }
    else {
        [_progressBar stopAnimation:self];
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [NSApp stopModal];
        });
    }
}

-(IBAction)cancelAction:(id)sender {
    [self cancel];
}

-(void) queryItem:(size_t)index {
    if(index >= [_items count]) {
        [self cancelAction:self];
        return;
    }

    currentItemIndex = index;

    NSURL *url = _items[index];
    if([[url absoluteString] hasSuffix:@".icloud"])
        url = [NSURL URLWithString:[[url absoluteString] stringByDeletingPathExtension]];

    if([[NSFileManager defaultManager] fileExistsAtPath:[url path]]) {
        [_progressMessage setStringValue:[NSString stringWithFormat:NSLocalizedString(@"Opening file(s) %lu of %lu ...", @"Opening File"), index + 1,  [_items count]]];
        if(![_mediaList openURL:url]) {
            [[NSAlert alertWithMessageText:NSLocalizedString(@"Error", @"Error header")
                defaultButton:nil alternateButton:nil otherButton:nil
                informativeTextWithFormat:NSLocalizedString(@"Can not open %@", @"Open error with filename"), url.absoluteString] runModal];
        }

        [self next];
    }
    else if(@available(macOS 10.7, *) && [[NSFileManager defaultManager] isUbiquitousItemAtURL:url]) {
        [_progressMessage setStringValue:[NSString stringWithFormat:NSLocalizedString(@"Downloading file(s) %lu of %lu ...", @"Downloading File"), index + 1,  [_items count]]];
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            NSError *error = nil;
            [coordinator coordinateReadingItemAtURL:url options:0 error:&error byAccessor:^(NSURL *newURL) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [_progressMessage setStringValue:[NSString stringWithFormat:NSLocalizedString(@"Opening file(s) %lu of %lu ...", @"Opening File"), index + 1,  [_items count]]];
                    if(!canceled && ![_mediaList openURL:newURL]) {
                        [[NSAlert alertWithMessageText:NSLocalizedString(@"Error", @"Error header")
                            defaultButton:nil alternateButton:nil otherButton:nil
                            informativeTextWithFormat:NSLocalizedString(@"Can not open %@", @"Open error with filename"), url.absoluteString] runModal];
                    }
                });

                dispatch_async(dispatch_get_main_queue(), ^{
                    if(!canceled && error) {
                        [[NSAlert alertWithMessageText:NSLocalizedString(@"Error", @"Error header")
                            defaultButton:nil alternateButton:nil otherButton:nil
                            informativeTextWithFormat:NSLocalizedString(@"Can not download %@", @"Download error with filename"), url.absoluteString] runModal];
                    }
                    [self next];
                });
            }];
        });
    }
    else {
        [[NSAlert alertWithMessageText:NSLocalizedString(@"Error", @"Error header")
            defaultButton:nil alternateButton:nil otherButton:nil
            informativeTextWithFormat:NSLocalizedString(@"Can not open %@", @"Open error with filename"), url.absoluteString] runModal];
    }
}
@end
