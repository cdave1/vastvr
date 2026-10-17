//
//  IBFontPropertyWindow.h
//  Tango
//
//  Created by David Petrie on 27/10/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBPropertyWindow.h"


@interface IBFontPropertyWindow : IBPropertyWindow 
{
@private
	IBOutlet NSColorWell *_tint;
	
	IBOutlet NSTextField *_text;
	
	IBOutlet NSSegmentedControl *_alignmentButton;
}

- (IBAction)updateTint:(id)sender;

- (IBAction)updateText:(id)sender;

- (IBAction)updateAlignment:(id)sender;

@end
