//
//  BlockPropertyWindow.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "BlockPropertyWindow.h"
#import "LevelEditor.h"
#import "NSImageView+Extensions.h"
#import "NSWindowController+Extensions.h"

@implementation BlockPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"BlockPropertyWindow"]))
	{
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(itemWasSelected)
		 name:kSingleItemSelectedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(itemWasUnselected)
		 name:kSingleItemUnselectedNotification
		 object:nil];
		
		
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(multipleItemsSelected)
		 name:kMultipleItemsSelectedNotification
		 object:nil];
		
		// initially hidden
		[self.window orderOut:self];
		
		if ([self.window respondsToSelector:@selector(setFloatingPanel:)])
		{
			[((NSPanel *)self.window) setFloatingPanel:YES];
		}
		
		[_texture registerForDraggedTypes:[NSArray arrayWithObjects:NSCreateFileContentsPboardType(@"tga"),NSCreateFileContentsPboardType(@"png"),nil]];
		//[self.window setDraggingDestinationDelegate:self];
		
	}
	return self;
}


- (void)itemWasSelected
{
	const hdGameObject *selected;
	if (!(selected = [LevelEditor sharedInstance]->GetSelectedGameObjectSingle()))
	{
		hdPrintf("No game object was selected.");
		return;
	}
	
	if (selected->GetUserType() != e_hd2DTypeBlock)
	{
		[self.window orderOut:self];
		return;
	}
	[self.window orderBack:self];
	[self setAllEnabled:YES];
	if (((hd2DBlock*)selected)->GetMaterial() &&
		((hd2DBlock*)selected)->GetMaterial()->IsEditable())
	{
		[_texture setEnabled:YES];
		[_texture setImageAtContentRepositoryPath:((hd2DBlock*)selected)->GetTextureName()];
		[_textureName setStringValue:[NSString stringWithUTF8String:((hd2DBlock*)selected)->GetTextureName()]];
	}
	else 
	{
		[_texture setEnabled:NO];
	}
	
	NSColor *color = [NSColor colorWithDeviceRed:((hd2DBlock *)selected)->GetTint().r
											green:((hd2DBlock *)selected)->GetTint().g
											blue:((hd2DBlock *)selected)->GetTint().b
											alpha:((hd2DBlock *)selected)->GetTint().a];
		[_tint setColor:color];
	
	/* 
	 The following is a bit pedantic, given the structure of the block type and material
	 enums and menu enums, but I'm paranoid about one of the lists being out of whack and having to
	 spend hours tracking down the bug.
	 */
	NSUInteger materialComboBoxIndex = 0, hd2DBlockTypeComboBoxIndex = 0;
	if (((hd2DBlock *)selected)->GetMaterial())
	{
		for (int i = 0; i < hd2DMaterialsLoader::GetDatabase()->GetMaterialListCount(); i++)
		{
			if (hd2DMaterialsLoader::GetDatabase()->GetMaterialList()[i] == ((hd2DBlock *)selected)->GetMaterial())
			{
				materialComboBoxIndex = i;
				break;
			}
		}
	}
	
	for (int i = 0; i < hdMin((NSInteger)e_hd2DBlockType_Count, [_blockTypeComboBox numberOfItems]); i++)
	{
		if (hd2DBlockTypeMenuItems[i].blockType == ((hd2DBlock *)selected)->GetBlockType())
		{
			hd2DBlockTypeComboBoxIndex = i;
			break;
		}
	}
	
	[_materialComboBox selectItemAtIndex:materialComboBoxIndex];
	[_blockTypeComboBox selectItemAtIndex:hd2DBlockTypeComboBoxIndex];
	
	[_blockTag setIntValue:((hd2DBlock *)selected)->GetTag()];
	[_depth setFloatValue:-((hd2DBlock *)selected)->GetDepth()]; /* keep the minus! */
	[_zOffset setFloatValue:((hd2DBlock *)selected)->GetZOffset()];
	[_tiling setFloatValue:((hd2DBlock *)selected)->GetTextureRepeatX()];
	
	[self updateTextInterface];
}


- (void)multipleItemsSelected
{
	if (![LevelEditor sharedInstance]->SelectedItemsContainsType(e_hd2DTypeBlock)) return;
	
	[self.window orderBack:self];
	[self setAllEnabled:NO];
	
	[_texture setEnabled:YES];
	[_textureName setEnabled:YES];
	[_tint setEnabled:YES];
	[_materialComboBox setEnabled:YES];
	[_blockTypeComboBox setEnabled:YES];
	[_depth setEnabled:YES];
	[_zOffset setEnabled:YES];
	[_blockTag setEnabled:YES];
	[self updateTextInterface];
}


- (void)itemWasUnselected
{
	[self.window orderOut:self];
}


- (IBAction)valueWasChanged:(id)sender
{
	
}


- (NSDragOperation)draggingEntered:(id < NSDraggingInfo >)sender
{
	return NSDragOperationGeneric;
}


- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
	return YES;
}



- (IBAction)UpdateSelectedBlockMaterial:(id)sender
{
	hd2DMaterial * material = hd2DMaterialsLoader::GetDatabase()->GetMaterialList()[[_materialComboBox indexOfSelectedItem]];
	if (!material) return;
	
	hd2DBlock* selected = NULL;
	for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
	{
		if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_hd2DTypeBlock)
		{
			selected = (hd2DBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
			selected->SetMaterial(material);
		}
	}
}


- (IBAction)UpdateSelectedBlockType:(id)sender
{
	hd2DBlock* selected = NULL;
	for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
	{
		if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_hd2DTypeBlock)
		{
			selected = (hd2DBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
			selected->SetBlockType(hd2DBlockTypeMenuItems[[_blockTypeComboBox indexOfSelectedItem]].blockType);
			
			selected->ResetAABB();
			selected->ResetOBB();
		}
	}
}


- (IBAction)UpdateSelectedBlockZOffset:(id)sender
{
	float zOffset;
	hd2DBlock* selected = NULL;
	for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
	{
		if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_hd2DTypeBlock)
		{
			selected = (hd2DBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
			zOffset = [_zOffset floatValue];
			zOffset = hdClamp(zOffset, -10.0f, 10.0f);
			selected->SetZOffset(zOffset);
		}
	}
	[self updateTextInterface];
}


- (IBAction)UpdateSlidersFromText:(id)sender
{
	[_zOffset setFloatValue:[_zOffsetText floatValue]];
	[self UpdateSelectedBlockZOffset:nil];
	[_tiling setFloatValue:[_tilingText floatValue]];
	[self UpdateSelectedBlockTiling:nil];
	[_depth setFloatValue:[_depthText floatValue]];
	[self UpdateSelectedBlockDepth:nil];
}


/**
 * Fired by drag and drop event onto imageview
 */
- (IBAction)UpdateSelectedBlockTexture:(id)sender
{
	hd2DBlock* selected = NULL;
	const char *textureName = [_texture getResourcePathOfImage];
	if (!textureName) return;
	
	for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
	{
		if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_hd2DTypeBlock)
		{
			selected = (hd2DBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
			if (selected && selected->GetMaterial()->IsEditable())
			{
				selected->SetTextureName(textureName);
				selected->ResetTextureCoords();
			}
		}
	}
	[_textureName setStringValue:[NSString stringWithUTF8String:textureName]];
}


- (IBAction)UpdateSelectedBlockTint:(id)sender
{
	hd2DBlock *selected = NULL;
	NSColor *color = _tint.color;
	for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
	{
		if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_hd2DTypeBlock)
		{
			selected = (hd2DBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
			if (selected && selected->GetMaterial()->IsEditable())
			{
				selected->SetTint([color redComponent], 
								  [color greenComponent], 
								  [color blueComponent], 
								  [color alphaComponent]);
				selected->ResetTextureCoords();
			}				
		}
	}
}


- (IBAction)UpdateSelectedBlockTiling:(id)sender
{
	hdVec3 dim;
	float hwRatio, xTiling;
	hd2DBlock* selected = NULL;
	
	for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
	{
		if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_hd2DTypeBlock)
		{
			selected = (hd2DBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
			dim = (selected->GetAABB().upper - selected->GetAABB().lower);
			
			if (dim.x <= 0.0f) return;
			
			hwRatio = (dim.y / dim.x);
			xTiling		= [_tiling floatValue];
			
			if (xTiling < 0.01f) xTiling = 1.0f;
			
			selected->SetTextureRepeatX(xTiling);
			selected->SetTextureRepeatY(xTiling);
			
			selected->ResetTextureCoords();
		}
	}
	[self updateTextInterface];
}


- (IBAction)UpdateSelectedBlockDepth:(id)sender
{
	hd2DBlock* selected = NULL;
	for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
	{
		if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_hd2DTypeBlock)
		{
			selected = (hd2DBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
			if ([_depth floatValue] == 0)
			{
				selected->SetDepth(-0.02f);
			}
			else
			{
				selected->SetDepth(-[_depth floatValue]);
			}
		}
	}
	[self updateTextInterface];
}


- (IBAction)UpdateSelectedBlockTag:(id)sender
{
	hd2DBlock* selected = NULL;
	for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
	{
		if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_hd2DTypeBlock)
		{
			selected = (hd2DBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
			selected->SetTag([_blockTag intValue]);
		}
	}
}


- (void)updateTextInterface
{
	[_zOffsetText setFloatValue:[_zOffset floatValue]];
	[_tilingText setFloatValue:[_tiling floatValue]];
	[_depthText setFloatValue:[_depth floatValue]];
}


#pragma mark -
#pragma mark NSComboBoxDelegate functions

- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
	NSLog(@"%@", [notification name]);
}


- (void)comboBoxWillDismiss:(NSNotification *)notification
{
	NSLog(@"%@", [notification name]);
}


#pragma mark -
#pragma mark NSComboBoxDataSource functions

- (id)comboBox:(NSComboBox *)aComboBox objectValueForItemAtIndex:(NSInteger)index
{
	if (aComboBox == _materialComboBox)
	{
		return [NSString stringWithUTF8String:hd2DMaterialsLoader::GetDatabase()->GetMaterialList()[index]->GetDisplayName()];
	}
	else if (aComboBox == _blockTypeComboBox)
	{
		return [NSString stringWithUTF8String:hd2DBlockTypeMenuItems[index].blockTypeName];
	}
	return nil;
}


- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)aComboBox
{
	if (aComboBox == _materialComboBox)
	{
		return hd2DMaterialsLoader::GetDatabase()->GetMaterialListCount();
	}
	else if (aComboBox == _blockTypeComboBox)
	{
		return e_hd2DBlockType_Count;
	}
	return 0;
}


@end
