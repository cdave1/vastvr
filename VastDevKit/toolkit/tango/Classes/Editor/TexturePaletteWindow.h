//
//  TexturePaletteWindow.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import "hdk.h"



@interface TexturePaletteWindow : NSWindowController
{
    IBOutlet id _imageBrowser;
	IBOutlet NSScrollView *_scrollView;
	
    NSMutableArray * _images;
	
    NSMutableArray * _importedImages;
}


- (IBAction)addTextureFolder:(id)sender;

@end
