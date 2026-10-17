//
//  TexturePaletteWindow.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "TexturePaletteWindow.h"
#import "TextureImage.h"
#import "LevelEditor.h"
#import "ExternalSettings.h"
#import "NSString+Extensions.h"

@interface TexturePaletteWindow ()
- (void) loadImages;
@end



@implementation TexturePaletteWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"TexturePaletteWindow"]))
	{
		//
		[self loadImages];
	}
	return self;
}


- (void) awakeFromNib
{
	[_scrollView setDocumentView:_imageBrowser];
	[_imageBrowser reloadData];
	[_imageBrowser setDelegate:self];
	[_imageBrowser setAnimates:YES];
}


- (void) dealloc
{
    [_images release];
    [super dealloc];
}


- (void) loadImages
{
	NSArray *allowedExtensions = [NSArray arrayWithObjects:@"tga", @"png", nil];
	
	if (_images)
		[_images release];
	
	NSArray *paths = [ExternalSettings GetTexturePaths];
	_images = [[NSMutableArray alloc] init];
	
	for (NSString *path in paths)
	{
		NSFileManager *localFileManager = [[NSFileManager alloc] init];
		NSDirectoryEnumerator *dirEnum = [localFileManager enumeratorAtPath:path];
		
		NSString *file;
		
		while (file = [dirEnum nextObject]) 
		{
			if ([file hasSuffixes:allowedExtensions])
			{
				TextureImage *image = [[TextureImage alloc] init];
				image.resourceName = file;
				image.path = [NSString stringWithFormat:@"%@/%@", path, file];
				image.title = [NSString stringWithFormat:@"%@/%@", [path lastPathComponent], file];
				image.folderName = [path lastPathComponent];
				
				hdPrintf("Loading texture: %s\n", [image.path UTF8String]);
				[_images addObject:image];
				//[image release];
			}
		}
		[localFileManager release];
	}
}


- (void) imageBrowserSelectionDidChange:(IKImageBrowserView *) aBrowser
{
	NSUInteger selectedIndex = [[aBrowser selectionIndexes] firstIndex];
	if (selectedIndex >= [_images count]) return;
	
	TextureImage *img = [_images objectAtIndex:selectedIndex];
	NSLog(@"Image browser selection changed: %@", img.path);
	
	[LevelEditor sharedInstance]->SetPaletteTexturePath([img.path UTF8String]);
	[LevelEditor sharedInstance]->SetPaletteTextureName([img.title UTF8String]);
	
	[[NSNotificationCenter defaultCenter] postNotificationName:kSelectedTextureImageChanged 
														object:nil];
}


- (void) updateDatasource
{
    [_imageBrowser reloadData];
}


- (NSUInteger) numberOfItemsInImageBrowser:(IKImageBrowserView *) view
{
	hdPrintf("Images: %d\n", [_images count]);
    return [_images count];
}



- (id) imageBrowser:(IKImageBrowserView *) view itemAtIndex:(NSUInteger) index
{
    return [_images objectAtIndex:index];
}


- (IBAction)addTextureFolder:(id)sender
{
	NSOpenPanel *open = [NSOpenPanel openPanel];
	NSURL *fileURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:FileSystem_BaseDir()]];
	
	hdPrintf("Opening folder: %s\n", [[fileURL absoluteString] UTF8String]);
	[open setDirectoryURL:fileURL];
	[open setCanChooseDirectories:YES]; 
	[open setCanChooseFiles:NO]; 
	[open setCanCreateDirectories:NO]; 
	[open setAllowsMultipleSelection:NO];
	[open setAlphaValue:0.95];
	[open setTitle:@"Select a folder"];
	
	if ([open runModal])
	{
		NSArray *urls = [open URLs];
		
		for (NSURL *url in urls)
		{
			NSLog(@"Adding new texture path: %@", [url path]);
			[ExternalSettings AddTexturePath:[url path]];
		}
		
		[self loadImages];
		[self updateDatasource];
	}
}

@end
