//
//  IBCheckboxPropertyWindow.h
//  Tango
//
//  Created by David Petrie on 25/07/10.
//  Copyright 2010 n/a. All rights reserved.
//
#import "IBPropertyWindow.h"
#import "hdImageView.h"

@interface IBCheckboxPropertyWindow : IBPropertyWindow 
{
@private
	IBOutlet NSColorWell *_tint;
	IBOutlet hdImageView *_onNormalTexture;
	IBOutlet hdImageView *_onOverTexture;
	IBOutlet hdImageView *_offNormalTexture;
	IBOutlet hdImageView *_offOverTexture;
}

- (IBAction)UpdateOnNormalTexture:(id)sender;

- (IBAction)UpdateOnOverTexture:(id)sender;

- (IBAction)UpdateOffNormalTexture:(id)sender;

- (IBAction)UpdateOffOverTexture:(id)sender;

@end
