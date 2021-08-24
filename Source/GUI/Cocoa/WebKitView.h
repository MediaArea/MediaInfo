//
//  WebKitView.h
//  MediaInfo
//
//  Created by Maxime Gervais on 20/04/2021.
//  Copyright © 2021 MediaArea.net. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <WebKit/WebKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface WebKitView : NSView <WKNavigationDelegate, WKUIDelegate> {
    WKWebView *webView;
}
-(void)setContent:(NSString*)content;
@end

NS_ASSUME_NONNULL_END
