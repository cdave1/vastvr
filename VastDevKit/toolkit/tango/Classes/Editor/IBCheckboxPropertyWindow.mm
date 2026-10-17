//
//  IBCheckboxPropertyWindow.mm
//  Tango
//
//  Created by David Petrie on 25/07/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBCheckboxPropertyWindow.h"
#import "InterfaceBuilder.h"
#import "NSImageView+Extensions.h"
#import "NSWindowController+Extensions.h"
#include "hdInterface/hdIBCheckbox.h"

@interface IBCheckboxPropertyWindow ()
-(void) SetCheckboxTexture:(const char *)textureName type:(hdIBCheckboxTextureType_t)type;
@end


@implementation IBCheckboxPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"IBCheckboxPropertyWindow"]))
	{
		NSArray *draggedTextureFileTypes = [NSArray arrayWithObjects:
											NSCreateFileContentsPboardType(@"tga"),
											NSCreateFileContentsPboardType(@"png"),nil];
		
		[_onNormalTexture registerForDraggedTypes:draggedTextureFileTypes];
		[_onOverTexture registerForDraggedTypes:draggedTextureFileTypes];
		[_offNormalTexture registerForDraggedTypes:draggedTextureFileTypes];
		[_offOverTexture registerForDraggedTypes:draggedTextureFileTypes];
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
	
	if (selected->GetInterfaceBuilderProxyType() != hdInterfaceBuilderProxyTypeCheckbox)
	{
		[self.window orderOut:self];
		return;
	}
	
	[self.window orderBack:self];
	[self setAllEnabled:YES];
	
	[_onNormalTexture setEnabled:YES];
	[_onNormalTexture setImageAtContentRepositoryPath:((hdIBCheckbox *)selected)->GetTexture(hdIBCheckboxTextureType_OnNormal)];
	
	[_onOverTexture setEnabled:YES];
	[_onOverTexture setImageAtContentRepositoryPath:((hdIBCheckbox *)selected)->GetTexture(hdIBCheckboxTextureType_OnOver)];
	
	[_offNormalTexture setEnabled:YES];
	[_offNormalTexture setImageAtContentRepositoryPath:((hdIBCheckbox *)selected)->GetTexture(hdIBCheckboxTextureType_OffNormal)];

	[_offOverTexture setEnabled:YES];
	[_offOverTexture setImageAtContentRepositoryPath:((hdIBCheckbox *)selected)->GetTexture(hdIBCheckboxTextureType_OffOver)];
	
	/*
	 if (((hdIBImage *)selected)->GetTint())
	 {
	 NSColor *color = [NSColor colorWithDeviceRed:((hdIBImage *)selected)->GetTint()[0]
	 green:((hdIBImage *)selected)->GetTint()[1]
	 blue:((hdIBImage *)selected)->GetTint()[2]
	 alpha:((hdIBImage *)selected)->GetTint()[3]];
	 [_tint setColor:color];
	 } 
	 else 
	 {
	 [_tint setColor:[NSColor whiteColor]];
	 }*/
	
	[_proxyTag setStringValue:[NSString stringWithUTF8String:selected->GetTag()]];
	[_proxyGroup setStringValue:[NSString stringWithUTF8String:selected->GetGroup()]];
	[_zOffset setFloatValue:((hdIBImage *)selected)->GetZOffset()];
	
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


- (NSDragOperation)draggingEntered:(id < NSDraggingInfo >)sender
{
	return NSDragOperationGeneric;
}


- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
	return YES;
}


- (hdInterfaceBuilderProxyType_t)getInterfaceBuilderProxyType
{
	return hdInterfaceBuilderProxyTypeCheckbox;
}


- (IBAction)UpdateSlidersFromText:(id)sender
{
	[_zOffset setFloatValue:[_zOffsetText floatValue]];
	[self UpdateZOffset:nil];
}


- (IBAction)UpdateOnNormalTexture:(id)sender
{
	[self SetCheckboxTexture:[_onNormalTexture getResourcePathOfImage] type:hdIBCheckboxTextureType_OnNormal];
}


- (IBAction)UpdateOnOverTexture:(id)sender
{
	[self SetCheckboxTexture:[_onOverTexture getResourcePathOfImage] type:hdIBCheckboxTextureType_OnOver];
}


- (IBAction)UpdateOffNormalTexture:(id)sender
{
	[self SetCheckboxTexture:[_offNormalTexture getResourcePathOfImage] type:hdIBCheckboxTextureType_OffNormal];
}


- (IBAction)UpdateOffOverTexture:(id)sender
{
	[self SetCheckboxTexture:[_offOverTexture getResourcePathOfImage] type:hdIBCheckboxTextureType_OffOver];
}


-(void) SetCheckboxTexture:(const char *)textureName type:(hdIBCheckboxTextureType_t)type
{
	if (!textureName) return;
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			hdIBCheckbox * selected = (hdIBCheckbox *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			if (selected)
			{
				selected->SetTexture(textureName, type);
				selected->ResetTextureCoords();
			}
		}
	}
}


- (void)updateTextInterface
{
	[_zOffsetText setFloatValue:[_zOffset floatValue]];
}

@end
