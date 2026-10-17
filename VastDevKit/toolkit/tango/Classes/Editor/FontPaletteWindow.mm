//
//  FontPaletteWindow.mm
//  Tango
//
//  Created by David Petrie on 27/10/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "FontPaletteWindow.h"
#import "InterfaceBuilder.h"
#import "ApplicationSettings.h"
#import "NSString+Extensions.h"
#import "FontItem.h"

@interface FontPaletteWindow ()
- (void) loadFonts;
@end

@implementation FontPaletteWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"FontPaletteWindow"]))
	{
		[self loadFonts];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(fontDidChange:)
		 name:NSTableViewSelectionDidChangeNotification
		 object:nil];
	}
	return self;
}


- (void) loadFonts
{
	NSArray *allowedExtensions = [NSArray arrayWithObjects:@"f", @"otf", @"ttf", nil];
	
	if (_fonts)
		[_fonts release];
	
	NSArray *paths = [ApplicationSettings GetFontPaths];
	_fonts = [[NSMutableArray alloc] init];
	
	for (NSString *path in paths)
	{
		NSFileManager *localFileManager = [[NSFileManager alloc] init];
		NSDirectoryEnumerator *dirEnum = [localFileManager enumeratorAtPath:path];
		
		NSString *file;
		
		while (file = [dirEnum nextObject]) 
		{
			if ([file hasSuffixes:allowedExtensions])
			{
				FontItem *font = [[FontItem alloc] init];
				font.resourceName = file;
				font.path = [NSString stringWithFormat:@"%@/%@", path, file];
				font.title = [NSString stringWithFormat:@"%@/%@", [path lastPathComponent], file];
				font.folderName = [path lastPathComponent];
				
				hdPrintf("Loading font: %s\n", [font.path UTF8String]);
				[_fonts addObject:font];
				//[image release];
			}
		}
		[localFileManager release];
	}
}


- (IBAction)addFontFolder:(id)sender
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
	[open setTitle:@"Select a font folder"];
	
	if ([open runModal])
	{
		NSArray *urls = [open URLs];
		
		for (NSURL *url in urls)
		{
			NSLog(@"Adding new font path: %@", [url path]);
			[ApplicationSettings AddFontPath:[url path]];
		}
		
		[self loadFonts];
		//[self updateDatasource];
	}
}


- (void)fontDidChange:(NSNotification *)notification
{
	id obj = [notification object];
	if (obj == _fontTableView)
	{
		NSInteger row = [_fontTableView selectedRow];
		if (row >= [_fonts count]) return;
		
		FontItem *font = [_fonts objectAtIndex:row];
		[InterfaceBuilder sharedInstance]->SetPaletteFontName([font.title UTF8String]);
		[InterfaceBuilder sharedInstance]->SetPaletteFontPath([font.path UTF8String]);
	}
}


#pragma mark -
#pragma mark NSTableViewDelegate functions

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return (_fonts) ? [_fonts count] : 0;
}


- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	if (_fonts && rowIndex < [_fonts count])
	{
		FontItem *item = [_fonts objectAtIndex:rowIndex];
		return item.resourceName;
	}
	return nil;
}

@end
