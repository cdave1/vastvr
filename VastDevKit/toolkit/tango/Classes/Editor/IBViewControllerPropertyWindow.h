//
//  IBViewControllerPropertyWindow.h
//  Tango
//
//  Created by David Petrie on 20/07/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface IBViewControllerPropertyWindow : NSWindowController 
{
	IBOutlet NSTextField *_width;
	IBOutlet NSTextField *_height;
}

- (void)viewControllerWasLoaded;

- (IBAction)UpdateWidth:(id)sender;

- (IBAction)UpdateHeight:(id)sender;

@end
