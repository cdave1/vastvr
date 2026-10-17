//
//  MainWindowController.m
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "MainWindowController.h"
#import "LevelEditor.h"
#import "ExternalSettings.h"

@interface MainWindowController ()
- (void)SetButtonStates:(id)sender;
@end


@implementation MainWindowController

- (id) init
{
	if ((self = [super initWithWindowNibName:@"MainWindow"]))
	{
		//[self.window makeMainWindow];
		[self.window setAcceptsMouseMovedEvents:YES];
		
		_worldPropertyWindow = [[WorldPropertyWindow alloc] init];
		[_worldPropertyWindow showWindow:self];
		
		_texturePaletteWindow = [[TexturePaletteWindow alloc] init];
		[_texturePaletteWindow showWindow:self];
		
		_blockPropertyWindow = [[BlockPropertyWindow alloc] init];
		_revoluteJointPropertyWindow = [[RevoluteJointPropertyWindow alloc] init];
		_prismaticJointPropertyWindow = [[PrismaticJointPropertyWindow alloc] init];
		_prefabsPaletteWindow = [[PrefabsPaletteWindow alloc] init];
		[_prefabsPaletteWindow.window setFrameOrigin:NSMakePoint(0, 0)];
		//[_blockPropertyWindow showWindow:self];
		

		
		[_texturePaletteWindow.window
		 setFrameOrigin:NSMakePoint([NSScreen mainScreen].frame.size.width - _texturePaletteWindow.window.frame.size.width, 1600)];
		
		[_toolPanel setFrameOrigin:NSMakePoint(_worldPropertyWindow.window.frame.origin.x + 
											   _worldPropertyWindow.window.frame.size.width, 
											   1600)];
		
		[self.window setFrameOrigin:NSMakePoint(_worldPropertyWindow.window.frame.size.width, 5)];
		NSRect mainWindowFrame = self.window.frame;
		mainWindowFrame.size = NSMakeSize([NSScreen mainScreen].frame.size.width - 
										  (_texturePaletteWindow.window.frame.size.width +
										   _worldPropertyWindow.window.frame.size.width),
										  [NSScreen mainScreen].frame.size.height - _toolPanel.frame.size.height - 30);
		[self.window setFrame:mainWindowFrame display:YES];
		[self.window addChildWindow:_toolPanel ordered:NSWindowAbove];
		
		//[_toolPanel setFloatingPanel:YES];
		
		[_blockMaterialComboBox selectItemAtIndex:0];
		[_blockTypeComboBox selectItemAtIndex:0];
		[_jointTypeComboBox selectItemAtIndex:0];
		[_blockShapeComboBox selectItemAtIndex:0];
		
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(levelWillBeDeleted)
		 name:kLevelWasDeletedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(paletteTextureChanged)
		 name:kSelectedTextureImageChanged
		 object:nil];
		
		if ([ExternalSettings GetLastLoadedFilePath])
		{
			if ([LevelEditor sharedInstance]->LoadWorld([[ExternalSettings GetLastLoadedFilePath] UTF8String]))
			{
				[[NSNotificationCenter defaultCenter] 
				 postNotificationName:kWorldWasLoadedNotification
				 object:nil];
			}
			else 
			{
				//[self fileSystemError:@"Could not load the specified totem world file." withFileName:[ExternalSettings GetLastLoadedFilePath]];
				NSLog(@"Could not load last loaded file.");
			}
		}
	}
	return self;
}


- (void)windowDidResize:(NSNotification *)notification
{
	[_glEditorView handleResize];
}


- (void)mouseMoved:(NSEvent *)mouseOver
{
	//hdPrintf("Mouse moved!\n");
	if (![self.window isKeyWindow])
		[self.window makeKeyWindow];
}


- (void) keyDown:(NSEvent *)theEvent
{
	unichar unicodeKey;
	
	NSString *chars = [theEvent charactersIgnoringModifiers];
	unicodeKey = [chars characterAtIndex:0];
	unsigned short keyCode = [theEvent keyCode];
	
	switch( unicodeKey )
	{
		case NSDeleteCharacter:
		case NSDeleteFunctionKey:
			[LevelEditor sharedInstance]->DeleteKey();
			break;
		case ' ':
			[LevelEditor sharedInstance]->TogglePerspectiveProjection();
			break;
		case 'p':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[_prefabsPaletteWindow makeNewPrefabFromSelected];
			}
			break;
		case 'a':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->SelectAll();
			}
			break;
		case 'c':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->CopySelectedObjects();
			}
			break;
			
		case 'v':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->PasteCopiedObjects();
			}
			break;
		case 't':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->ApplyCurrentTextureToSelected();
			}
			break;
		case 'j':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->RepairSelectedShape();
			}
			break;
		case 'd':
			if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
			{
				[LevelEditor sharedInstance]->DumpUniqueTextureList();
			}
			break;
		case 'g':
			if (([theEvent modifierFlags] & (NSControlKeyMask) == (NSControlKeyMask)))
			{
				[LevelEditor sharedInstance]->ApplyBGValuesToAllFollowingLevels();
			}
			break;
		case '-':
			if (([theEvent modifierFlags] & (NSControlKeyMask) == (NSControlKeyMask)))
			{
				[LevelEditor sharedInstance]->SubtractMesh();
			}
			break;
        case 'k':
            if (([theEvent modifierFlags] & (NSControlKeyMask) == (NSControlKeyMask)))
			{
				[LevelEditor sharedInstance]->GenerateBlocksFromNoise();
			}
			break;
		case NSLeftArrowFunctionKey:
			[LevelEditor sharedInstance]->Pan(-20.0f, 0);
			break;
			
			// Press right to pan right.
		case NSRightArrowFunctionKey:
			[LevelEditor sharedInstance]->Pan(20.0f, 0);
			break;
			
			// Press down to pan down.
		case NSDownArrowFunctionKey:
			[LevelEditor sharedInstance]->Pan(0.0f, -20.0f);
			break;
			
			// Press up to pan up.
		case NSUpArrowFunctionKey:
			[LevelEditor sharedInstance]->Pan(0.0f, 20.0f);
			break;
		default:
			break;
	}
	[super keyDown:theEvent];
}


- (IBAction)togglePhysics:(id)sender
{
	if ([LevelEditor sharedInstance]->isPhysicsOn())
	{
		[LevelEditor sharedInstance]->PhysicsOff();
		[_togglePhysics setTitle:@"Start Physics"];
		[_progress stopAnimation:self];
	}
	else
	{
		[LevelEditor sharedInstance]->PhysicsOn();
		[_togglePhysics setTitle:@"Stop Physics"];
		[_progress startAnimation:self];
	}
}


- (IBAction)setCursorMode:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeCursor;
}


- (IBAction)setFilter:(id)sender
{
	[LevelEditor sharedInstance]->settings.showCosmetics = [_cosmeticsFilterCheckbox state] == NSOnState;
	[LevelEditor sharedInstance]->settings.showGround = [_groundFilterCheckbox state] == NSOnState;
	[LevelEditor sharedInstance]->settings.showPhysics = [_physicsFilterCheckbox state] == NSOnState;
}


- (IBAction)commitBlockStates:(id)sender
{
	const hd2DLevel *level;
	if (!(level = [LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	// Current shapes: make the current positions of the 
	// blocks into the start positions.
	((hd2DLevel *)level)->CommitBlockStates();
}


- (IBAction)updatePaletteTexture:(id)sender
{
	const char *textureName = [_currentTexture getResourcePathOfImage];
	if (textureName)
	{
		[LevelEditor sharedInstance]->SetPaletteTexturePath(textureName);
	}
}


- (IBAction)updatePaletteTint:(id)sender
{
	NSColor *color = _currentTint.color;
	
	[LevelEditor sharedInstance]->SetPaletteTint([color redComponent], 
												 [color greenComponent], 
												 [color blueComponent], 
												 [color alphaComponent]);
}


- (void)paletteTextureChanged
{
	[_currentTexture setImageAtContentRepositoryPath:[LevelEditor sharedInstance]->GetPaletteTexturePath()];
}


/*
 - (IBAction)UpdateNewEventType(int)
 {
 levelEditorController->settings.newTotemEventType = (e_hd2DEventType)newEventTypesList->get_int_val();	
 }*/


- (IBAction)SetNewEventMode:(id)sender
{
	//[LevelEditor sharedInstance]->
}




- (IBAction)SetNewBlockMaterial:(id)sender
{
	[LevelEditor sharedInstance]->SetNewBlockMaterial(hd2DMaterialsLoader::GetDatabase()->GetMaterialList()[[_blockMaterialComboBox indexOfSelectedItem]]); //(totemMaterialMenuItems[[_blockMaterialComboBox indexOfSelectedItem]].material);
}


- (IBAction)SetNewBlockType:(id)sender
{
	[LevelEditor sharedInstance]->SetNewBlockType(hd2DBlockTypeMenuItems[[_blockTypeComboBox indexOfSelectedItem]].blockType);
}


// TEMPORARY
- (IBAction)SetNewShapeMesh:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeMesh);
}


/**
 * toolbar buttons
 */
- (IBAction)SetNewShapeRectangle:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeRectangle);
}


- (IBAction)SetNewShapeCircle:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeCylinder);
}


- (IBAction)SetNewShapeFivePointStar:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeStar);
}


- (IBAction)SetNewShapeHexagon:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeHexagon);
}


- (IBAction)SetNewShapeTriangle:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeTriangle);
}


- (IBAction)SetNewShapeMultiStar:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeMultiStar);
}


- (IBAction)SetNewShapeTriangleFan:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeCustom);
}


- (IBAction)SetNewShapeTriangleStrip:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	//[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeTriStrip);
}


- (IBAction)SetNewShapeSpikes:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_hd2DShapeTypeSpikes);
}


- (IBAction)SetNewJointDistance:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeTotemJoint;
	[LevelEditor sharedInstance]->SetNewJointType(e_hd2DJointTypeDistance);
}


- (IBAction)SetNewJointPrismatic:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeTotemJoint;
	[LevelEditor sharedInstance]->SetNewJointType(e_hd2DJointTypePrismatic);
}


- (IBAction)SetNewJointRevolute:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeTotemJoint;
	[LevelEditor sharedInstance]->SetNewJointType(e_hd2DJointTypeRevolute);
}


- (void)SetButtonStates:(id)sender
{
	for(NSView *view in [[_toolPanel contentView] subviews])
	{
		if (view != sender &&
			[view class] == [NSButton class] && 
			[view tag] == kToolbarToggleButtonTag)
		{
			[((NSButton *)view) setState:NSOffState];
		}
	}
	[((NSButton *)sender) setState:NSOnState];
}



- (void)levelWillBeDeleted
{
	NSAlert *alert = [[[NSAlert alloc] init] autorelease];
	
	[alert addButtonWithTitle:@"OK"];
	[alert addButtonWithTitle:@"Cancel"];
	[alert setMessageText:@"Delete this level?"];
	[alert setInformativeText:@"Deleted levels cannot be restored."];
	[alert setAlertStyle:NSWarningAlertStyle];
	[alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:@selector(deleteAlertEnded:returnCode:contextInfo:) contextInfo:nil];
}


- (void)deleteAlertEnded:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo 
{
    if (returnCode == NSAlertFirstButtonReturn)
	{
		// Do a backup save
		[LevelEditor sharedInstance]->SaveCurrentWorld();
		
		// Remove the level
		int index = -1;
		for (int i = 0; i < [LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevelCount(); i++)
		{
			if ([LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevels()[i] == [LevelEditor sharedInstance]->GetCurrentLevel())
			{
				index = i;
				break;
			}
		}
		
		if (index == -1) return;
		
		if (index == ([LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevelCount() - 1))
		{
			[LevelEditor sharedInstance]->SetLevel(hdMax(0, index - 1));
		}
		else
		{
			[LevelEditor sharedInstance]->SetLevel(index + 1);
		}
		
		
		[LevelEditor sharedInstance]->GetCurrentTotemWorld()->RemoveLevelAtIndex(index);
		

		
		[[NSNotificationCenter defaultCenter] 
		 postNotificationName:kWorldWasLoadedNotification
		 object:nil];
    }
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
	if (aComboBox == _blockMaterialComboBox)
	{
		return [NSString stringWithUTF8String:hd2DMaterialsLoader::GetDatabase()->GetMaterialList()[index]->GetDisplayName()];
	}
	if (aComboBox == _blockTypeComboBox)
	{
		return [NSString stringWithUTF8String:hd2DBlockTypeMenuItems[index].blockTypeName];
	}
	if (aComboBox == _jointTypeComboBox)
	{
		return [NSString stringWithUTF8String:hd2DJointTypeMenuItems[index].jointTypeName];
	}
	if (aComboBox == _blockShapeComboBox)
	{
		return [NSString stringWithUTF8String:hd2DShapeTypeMenuItems[index].shapeTypeName];
	}
	return nil;
}


- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)aComboBox
{
	if (aComboBox == _blockMaterialComboBox)
	{
		return hd2DMaterialsLoader::GetDatabase()->GetMaterialListCount(); //e_hd2DMaterial_Count;
	}
	if (aComboBox == _blockTypeComboBox)
	{
		return e_hd2DBlockType_Count;
	}
	if (aComboBox == _jointTypeComboBox)
	{
		return e_hd2DJoinType_Count;
	}
	if (aComboBox == _blockShapeComboBox)
	{
		return e_hd2DShapeType_Count;
	}
	return 0;
}


@end
