//
//  LevelPropertyWindow.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "WorldPropertyWindow.h"
#import "LevelEditor.h"
#import "NSImageView+Extensions.h"
#import "AppDelegate.h"

@implementation WorldPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"WorldPropertyWindow"]))
	{
		[_levelName setEnabled:NO];
		
		/*
		_extendLevelAABBCheckbox;
		_skyCheckbox;
		
		_skyInfoBox;
		_skyTexture;
		_horizonTexture;
		_middleTexture;
		_nearTexture;
		
		_hasFloor;
		_hasFloorBox;
		_floorTexture1;
		_floorTexture2;
		_floorLevel;
		_isWater;
		_isReflective;
		 */
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(levelWasLoaded)
		 name:kLoadedLevelWasChangedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(worldWasLoaded)
		 name:kWorldWasLoadedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(levelDidChange:)
		 name:NSTableViewSelectionDidChangeNotification
		 object:nil];
		
		[[_hasFloorBox contentView] setAllControlsEnabled:NO];
		NSPoint o = NSMakePoint(0, 1600);
		[self.window setFrameTopLeftPoint:o];
	}
	return self;
}


- (void)worldWasLoaded
{	
	const hd2DWorld *world;
	if ((world = [LevelEditor sharedInstance]->GetCurrentTotemWorld()))
	{
		[_worldName setStringValue:[NSString stringWithUTF8String:world->GetName().c_str()]];
		[self levelWasLoaded];
		[_levelsTableView reloadData]; 
	}
	
}


- (void)worldWasUnloaded
{
}


- (void)levelDidChange:(NSNotification *)notification
{
	id obj = [notification object];
	if (obj == _levelsTableView)
	{
		NSInteger row = [_levelsTableView selectedRow];
		if ([LevelEditor sharedInstance]->SetLevel(row))
		{
			[[NSNotificationCenter defaultCenter] 
			 postNotificationName:kLoadedLevelWasChangedNotification
			 object:nil];
		}
	}
}


- (void)levelWasLoaded
{
	const hd2DLevel *updatedLevel;
	
	if (!(updatedLevel = [LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	
	[_levelName setStringValue:[NSString stringWithUTF8String:updatedLevel->GetLevelName()]];
	[_extendLevelAABBCheckbox setState:(((hd2DLevel *)updatedLevel)->GetExtendAABB() ? NSOnState : NSOffState)];
	if (updatedLevel->GetFloorInfo() == NULL)
	{
		[_hasFloor setState:NSOffState];
		[[_hasFloorBox contentView] setAllControlsEnabled:NO];
	}
	else
	{
		[_hasFloor setState:NSOnState];	
		[[_hasFloorBox contentView] setAllControlsEnabled:YES];
		
		const hd2DFloorInfo *floorInfo = updatedLevel->GetFloorInfo();
		
		if (((hd2DFloorInfo *)floorInfo)->GetFirstTexture())
		{
			[_floorTexture1 setImageAtContentRepositoryPath:((hd2DFloorInfo *)floorInfo)->GetFirstTexture()->name];
		}
		
		if (((hd2DFloorInfo *)floorInfo)->GetSecondTexture())
		{
			[_floorTexture2 setImageAtContentRepositoryPath:((hd2DFloorInfo *)floorInfo)->GetSecondTexture()->name];
		}
		
		[_floorLevel setFloatValue:((hd2DFloorInfo *)floorInfo)->m_floorLevel];
		[_floorLevelText setFloatValue:((hd2DFloorInfo *)floorInfo)->m_floorLevel];
		
		[_isWater setState:(((hd2DFloorInfo *)floorInfo)->m_isWater ? NSOnState : NSOffState)];
		[_isReflective setState:(((hd2DFloorInfo *)floorInfo)->m_isReflective ? NSOnState : NSOffState)];
	}
	
	[_skyTexture setImageAtContentRepositoryPath:updatedLevel->GetSkyTextureName()];
	[_horizonTexture setImageAtContentRepositoryPath:updatedLevel->GetDistantBackgroundTextureName()];
	[_middleTexture setImageAtContentRepositoryPath:updatedLevel->GetFarBackgroundTextureName()];
	[_nearTexture setImageAtContentRepositoryPath:updatedLevel->GetNearBackgroundTextureName()];
}


- (IBAction)UpdateHasFloor:(id)sender
{
	const hd2DLevel *level;
	
	[[_hasFloorBox contentView] setAllControlsEnabled:[_hasFloor state] == NSOnState];
	
	if (!(level = [LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	
	if ([_hasFloor state] == NSOffState)
	{
		hd2DFloorInfo *floorInfo = (hd2DFloorInfo *)(level->GetFloorInfo());
		hdDeleteSafely(floorInfo);
		((hd2DLevel *)level)->SetFloorInfo(NULL);
	}
}


- (IBAction)UpdateLevelFloorInfo:(id)sender
{
	const hd2DLevel *level;
	const char *textureName;
	
	if (!(level = [LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	
	hd2DFloorInfo *floorInfo = (hd2DFloorInfo *)(level->GetFloorInfo());
	
	if (floorInfo == NULL)
	{
		floorInfo = new hd2DFloorInfo();
		((hd2DLevel *)level)->SetFloorInfo(floorInfo);
	}
	
	
	textureName = [_floorTexture1 getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		snprintf(floorInfo->m_firstTextureName, kMaxTexturePathSize, "%s", textureName);
	}
	
	textureName = [_floorTexture2 getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		snprintf(floorInfo->m_secondTextureName, kMaxTexturePathSize, "%s", textureName);
	}
	
	floorInfo->m_floorLevel = [_floorLevel floatValue];
	floorInfo->m_isWater = [_isWater state] == NSOnState;
	floorInfo->m_isReflective = [_isReflective state] == NSOnState;
	floorInfo->ResetTextures();
}


- (IBAction)levelAddRemoveSegment:(id)sender
{
	if ([_addRemoveSegment selectedSegment] == 0)
	{
		[self addNewLevel:sender];
	}
	else if ([_addRemoveSegment selectedSegment] == 1)
	{
		[self deleteSelectedLevel:sender];
	}
}


- (IBAction)addNewLevel:(id)sender
{
	[LevelEditor sharedInstance]->GenerateNewLevel();
	[_levelsTableView reloadData];
	[_levelsTableView selectRowIndexes:[NSIndexSet indexSetWithIndex:[LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevelCount()-1]
				  byExtendingSelection:NO];
	[[NSNotificationCenter defaultCenter] 
	 postNotificationName:kLoadedLevelWasChangedNotification
	 object:nil];
}



/**
 * Needs an alert.
 */
- (IBAction)deleteSelectedLevel:(id)sender
{
	const hd2DLevel *level;
	if ((level = (hd2DLevel *)[LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		[[NSNotificationCenter defaultCenter] 
		 postNotificationName:kLevelWasDeletedNotification
		 object:nil];
	}
}





- (IBAction)UpdateLevelName:(id)sender
{
	const hd2DLevel *level;
	if (!(level = (hd2DLevel *)[LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	((hd2DLevel *)level)->SetLevelName([[_levelName stringValue] UTF8String]);
	[_levelsTableView reloadData];
}


- (IBAction)UpdateWorldName:(id)sender
{
	const hd2DWorld *world;
	if ((world = [LevelEditor sharedInstance]->GetCurrentTotemWorld()))
	{
		NSString *worldName = [_worldName stringValue];
		if ([worldName length] == 0)
			((hd2DWorld *)world)->SetName("(NONAME)");
		else 
			((hd2DWorld *)world)->SetName([worldName UTF8String]);
	}
}


- (IBAction)UpdateLevelAABB:(id)sender
{
	const hd2DLevel *level;
	if (!(level = (hd2DLevel *)[LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	
	((hd2DLevel *)level)->SetExtendAABB([_extendLevelAABBCheckbox state] == NSOnState);
}


- (IBAction)UpdateLevelBackgrounds:(id)sender
{
	const hd2DLevel *level;
	const char *textureName;
	if (!(level = [LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	
	textureName = [_skyTexture getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		((hd2DLevel *)level)->SetSkyTextureName(textureName);
	}
	else
	{
		((hd2DLevel *)level)->SetSkyTextureName("");
	}
	
	textureName = [_horizonTexture getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		((hd2DLevel *)level)->SetDistantBackgroundTextureName(textureName);
	}
	else
	{
		((hd2DLevel *)level)->SetDistantBackgroundTextureName("");
	}
	
	textureName = [_middleTexture getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		((hd2DLevel *)level)->SetFarBackgroundTextureName(textureName);
	}
	else
	{
		((hd2DLevel *)level)->SetFarBackgroundTextureName("");
	}
	
	textureName = [_nearTexture getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		((hd2DLevel *)level)->SetNearBackgroundTextureName(textureName);
	}
	else
	{
		((hd2DLevel *)level)->SetNearBackgroundTextureName("");
	}
	((hd2DLevel *)level)->ResetTextureCache();
}


#pragma mark -
#pragma mark NSTableViewDelegate functions

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	const hd2DWorld *world = [LevelEditor sharedInstance]->GetCurrentTotemWorld();
	return (world) ? world->GetLevelCount() : 0;
	
}


- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	const hd2DWorld *world;
	const hd2DLevel *level;
	if ((world = [LevelEditor sharedInstance]->GetCurrentTotemWorld()))
	{
		if (rowIndex >= 0 && rowIndex < world->GetLevelCount())
		{
			if ((level = world->GetLevels()[rowIndex]))
			{
				return [NSString stringWithUTF8String: level->GetLevelName()];
			}
			else {
				return nil;
			}

		}
	}
	return nil;
}

@end
