//
//  IBFontPropertyWindow.mm
//  Tango
//
//  Created by David Petrie on 27/10/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBFontPropertyWindow.h"
#import "InterfaceBuilder.h"
#import "NSImageView+Extensions.h"
#import "NSWindowController+Extensions.h"


@implementation IBFontPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"IBFontPropertyWindow"]))
	{		
		
	}
	return self;
}


- (void)itemWasSelected
{
	hdIBProxy *selected;
	if (!(selected = (hdIBProxy *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectSingle()))
	{
		hdPrintf("No game object was selected.");
		return;
	}
	
	if (selected->GetInterfaceBuilderProxyType() != hdInterfaceBuilderProxyTypeFontPolygon)
	{
		[self.window orderOut:self];
		return;
	}
	[_tint setColor:[NSColor whiteColor]];
	[self.window orderBack:self];
	[self setAllEnabled:YES];
	
	[_tint setEnabled:YES];
	
	[_text setEnabled:YES];
	[_text setStringValue:[NSString stringWithUTF8String:((hdIBFontPolygon *)selected)->GetText()]];
	
	if ([NSColorPanel sharedColorPanel])
		[[NSColorPanel sharedColorPanel] setShowsAlpha:YES];
	
	NSColor *color = [NSColor colorWithDeviceRed:((hdIBFontPolygon *)selected)->GetTint().r
											   green:((hdIBFontPolygon *)selected)->GetTint().g
												blue:((hdIBFontPolygon *)selected)->GetTint().b
											   alpha:((hdIBFontPolygon *)selected)->GetTint().a];
		[_tint setColor:color];
	
	[_proxyTag setStringValue:[NSString stringWithUTF8String:selected->GetTag()]];
	[_proxyGroup setStringValue:[NSString stringWithUTF8String:selected->GetGroup()]];
	[_zOffset setFloatValue:((hdIBFontPolygon *)selected)->GetZOffset()];
	
	[self updateTextInterface];
}


- (void)multipleItemsSelected
{
	[self.window orderOut:self];
}


- (void)itemWasUnselected
{
	[super itemWasUnselected];
}


- (hdInterfaceBuilderProxyType_t)getInterfaceBuilderProxyType
{
	return hdInterfaceBuilderProxyTypeFontPolygon;
}


- (IBAction)updateText:(id)sender
{
	hdIBProxy *selected;
	if (!(selected = (hdIBProxy *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectSingle()))
	{
		hdPrintf("No game object was selected.");
		return;
	}
	
	if (selected->GetInterfaceBuilderProxyType() != hdInterfaceBuilderProxyTypeFontPolygon) return;
	
	((hdIBFontPolygon *)selected)->SetText([[_text stringValue] UTF8String]);
}


- (IBAction)updateAlignment:(id)sender
{
	hdIBProxy *selected;
	if (!(selected = (hdIBProxy *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectSingle()))
	{
		hdPrintf("No game object was selected.");
		return;
	}
	if (selected->GetInterfaceBuilderProxyType() != hdInterfaceBuilderProxyTypeFontPolygon) return;
	
	if ([_alignmentButton selectedSegment] >= hdIBFontPolygonAlignmentLeft &&
		[_alignmentButton selectedSegment] < hdIBFontPolygonAlignmentCount)
	{
		((hdIBFontPolygon *)selected)->SetFontAlignment((hdIBFontPolygonAlignment_t)[_alignmentButton selectedSegment]);
	}
}


- (IBAction)updateTint:(id)sender
{
	hdIBProxy *selected;
	if (!(selected = (hdIBProxy *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectSingle()))
	{
		hdPrintf("No game object was selected.");
		return;
	}
	
	if (selected->GetInterfaceBuilderProxyType() != hdInterfaceBuilderProxyTypeFontPolygon) return;
	
	NSColor *color = [_tint color];
	hdColor4f tint([color redComponent], 
				   [color greenComponent], 
				   [color blueComponent], 
				   [color alphaComponent]);
	
	((hdIBFontPolygon *)selected)->SetTint(tint);
}

@end
