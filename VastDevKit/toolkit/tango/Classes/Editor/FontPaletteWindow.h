//
//  FontPaletteWindow.h
//  Tango
//
//  Created by David Petrie on 27/10/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import "hdk.h"



@interface FontPaletteWindow : NSWindowController <NSTableViewDataSource, NSTableViewDelegate> 
{
	IBOutlet NSTableView *_fontTableView;
	NSMutableArray * _fonts;
}

-(IBAction)addFontFolder:(id)sender;

@end
