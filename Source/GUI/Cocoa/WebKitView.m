//
//  WebKitView.m
//  MediaInfo
//
//  Created by Maxime Gervais on 20/04/2021.
//  Copyright © 2021 MediaArea.net SARL. All rights reserved.
//

#import "WebKitView.h"

@implementation WebKitView

-(void)initialize {
    WKWebViewConfiguration *configuration=[[WKWebViewConfiguration alloc] init];

    webView = [[WKWebView alloc] initWithFrame:CGRectZero configuration:configuration];
    [self addSubview:webView];

    webView.UIDelegate = self;

    // Fix bug with WKWebKit on macOS < 12 where background color
    // doesn't follows the system theme
    if (@available(macOS 12, *)) {}
    else if (@available(macOS 10.14, *)) { // First macOS version that support dark theme
        [webView setValue:@(NO) forKey:@"drawsBackground"];
        [self setValue: NSColor.controlBackgroundColor forKey:@"backgroundColor"];
    }

    [webView setTranslatesAutoresizingMaskIntoConstraints:false];
    [NSLayoutConstraint constraintWithItem:self attribute: NSLayoutAttributeTop relatedBy: NSLayoutRelationEqual toItem:webView attribute:NSLayoutAttributeTop multiplier:1.0 constant:0.0].active = true;
    [NSLayoutConstraint constraintWithItem:self attribute: NSLayoutAttributeBottom relatedBy: NSLayoutRelationEqual toItem:webView attribute:NSLayoutAttributeBottom multiplier:1.0 constant:0.0].active = true;
    [NSLayoutConstraint constraintWithItem:self attribute: NSLayoutAttributeLeading relatedBy: NSLayoutRelationEqual toItem:webView attribute:NSLayoutAttributeLeading multiplier:1.0 constant:0.0].active = true;
    [NSLayoutConstraint constraintWithItem:self attribute: NSLayoutAttributeTrailing relatedBy: NSLayoutRelationEqual toItem:webView attribute:NSLayoutAttributeTrailing multiplier:1.0 constant:0.0].active = true;
}

-(void)dealloc {
    [webView release];
    [super dealloc];
}

-(instancetype)initWithCoder:(NSCoder *)coder {
    self = [super initWithCoder:coder];

    if (self)
        [self initialize];

    return self;
}

-(void)setContent:(NSString *)content {
    [webView loadHTMLString:content baseURL:nil];
}

@end
