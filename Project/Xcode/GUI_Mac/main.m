/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//  Created by Max Pozdeev on 06.02.12.

#import <Cocoa/Cocoa.h>

int main(int argc, char *argv[])
{
	//this is special for wcstombs and mbstowcs functions called in library.
	//setlocale will not help, environment variable is needed.
	setenv("LC_CTYPE", "UTF-8", 0);
	
    return NSApplicationMain(argc,  (const char **) argv);
}
