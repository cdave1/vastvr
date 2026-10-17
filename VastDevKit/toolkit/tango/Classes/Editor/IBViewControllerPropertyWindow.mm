//
//  IBViewControllerPropertyWindow.mm
//  Tango
//
//  Created by David Petrie on 20/07/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBViewControllerPropertyWindow.h"
#import "InterfaceBuilder.h"

@implementation IBViewControllerPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"IBViewControllerPropertyWindow"]))
	{
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(viewControllerWasLoaded)
		 name:kViewControllerWasLoadedNotification
		 object:nil];
	}
	return self;
}


- (IBAction)UpdateWidth:(id)sender
{
	if ([InterfaceBuilder sharedInstance]->GetCurrentViewController())
	{
		[InterfaceBuilder sharedInstance]->GetCurrentViewController()->SetWidth([_width floatValue]);
		[InterfaceBuilder sharedInstance]->DidUpdateCurrentViewController();
	}
}


- (IBAction)UpdateHeight:(id)sender
{
	if ([InterfaceBuilder sharedInstance]->GetCurrentViewController())
	{
		[InterfaceBuilder sharedInstance]->GetCurrentViewController()->SetHeight([_height floatValue]);
		[InterfaceBuilder sharedInstance]->DidUpdateCurrentViewController();
	}
}


- (void)viewControllerWasLoaded
{
	// Update width/height props.
	if ([InterfaceBuilder sharedInstance]->GetCurrentViewController())
	{
		[_width setFloatValue:[InterfaceBuilder sharedInstance]->GetCurrentViewController()->GetWidth()];
		[_height setFloatValue:[InterfaceBuilder sharedInstance]->GetCurrentViewController()->GetHeight()];
		[InterfaceBuilder sharedInstance]->DidUpdateCurrentViewController();
	}
}

@end
