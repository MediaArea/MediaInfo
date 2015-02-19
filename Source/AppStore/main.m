//
//  main.m
//  MediaInfo
//
//  Created by Max Pozdeev on 06.02.12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

int main(int argc, char *argv[])
{
	//this is special for wcstombs and mbstowcs functions called in library.
	//setlocale will not help, environment variable is needed.
	setenv("LC_CTYPE", "UTF-8", 0);
	
    return NSApplicationMain(argc,  (const char **) argv);
}
