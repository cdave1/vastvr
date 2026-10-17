//
//  TextureCollections.mm
//  Tango
//
//  Created by David Petrie on 21/07/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "ApplicationSettings.h"

static NSMutableDictionary * _settings = nil;

@implementation ApplicationSettings

+ (void)Load
{
	if (_settings)
	{
		[_settings release];
	}
	
	NSString *errorDesc = nil;
	NSPropertyListFormat format;
	NSString *plistPath;
	NSString *rootPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
															  NSUserDomainMask, YES) objectAtIndex:0];
	
	plistPath = [rootPath stringByAppendingPathComponent:@"ApplicationSettings.plist"];
	if (![[NSFileManager defaultManager] fileExistsAtPath:plistPath]) 
	{
		plistPath = [[NSBundle mainBundle] pathForResource:@"ApplicationSettings" ofType:@"plist"];
	}
	
	NSData *plistXML = [[NSFileManager defaultManager] contentsAtPath:plistPath];
	NSDictionary *temp = (NSDictionary *)[NSPropertyListSerialization
										  propertyListFromData:plistXML
										  mutabilityOption:NSPropertyListMutableContainersAndLeaves
										  format:&format
										  errorDescription:&errorDesc];
	
	if (!temp) 
	{
		NSLog(@"Error reading plist: %@, format: %d", errorDesc, format);
	}
	
	_settings = [[NSMutableDictionary dictionaryWithDictionary:temp] retain];
	
	for (NSString *key in [_settings allKeys])
	{
		NSLog(key);
	}
}


+ (void)Save
{
	if (!_settings) return;
	
	NSString *error;
    NSString *rootPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *plistPath = [rootPath stringByAppendingPathComponent:@"ApplicationSettings.plist"];
	
    NSData *plistData = [NSPropertyListSerialization dataFromPropertyList:_settings
																   format:NSPropertyListXMLFormat_v1_0
														 errorDescription:&error];
	
    if (plistData) 
	{
        [plistData writeToFile:plistPath atomically:YES];
    }
    else 
	{
        NSLog(error);
        [error release];
    }
}


+ (void)SetLastLoadedFilePath:(NSString *)path
{
	if (!_settings) return;
	[_settings setObject:path forKey:@"LastLoadedFilePath"];
	[self Save];
}


+ (NSString *)GetLastLoadedFilePath
{
	if (!_settings) return nil;
	NSString *path;
	if ((path = [_settings objectForKey:@"LastLoadedFilePath"]))
	{
		return path;
	}
	return nil;
}


+ (void)SetLastLoadedFileName:(NSString *)name
{
	if (!_settings) return;
	[_settings setObject:name forKey:@"LastLoadedFileName"];
	[self Save];
}


+ (NSString *)GetLastLoadedFileName
{
	if (!_settings) return nil;
	NSString *name;
	if ((name = [_settings objectForKey:@"LastLoadedFileName"]))
	{
		return name;
	}
	return nil;
}


+ (NSArray *)GetTexturePaths
{
	if (!_settings) return nil;
	
	NSArray *texturePaths;
	if ((texturePaths = [_settings objectForKey:@"TexturePaths"]))
	{
		return texturePaths;
	}
	return nil;
}


+ (void)AddTexturePath:(NSString *)path
{
	if (!_settings) return;
	
	NSArray *texturePaths;
	if ((texturePaths = [_settings objectForKey:@"TexturePaths"]))
	{
		texturePaths = [texturePaths arrayByAddingObject:path];
	}
	else 
	{
		texturePaths = [NSArray arrayWithObject:path];
	}
	[_settings setObject:texturePaths forKey:@"TexturePaths"];
	[self Save];
}


+ (void)RemoveTexturePath:(NSString *)path
{
}


+ (NSArray *)GetFontPaths
{
	if (!_settings) return nil;
	
	NSArray *fontPaths;
	if ((fontPaths = [_settings objectForKey:@"FontPaths"]))
	{
		return fontPaths;
	}
	return nil;
}


+ (void)AddFontPath:(NSString *)path
{
	if (!_settings) return;
	
	NSArray *fontPaths;
	if ((fontPaths = [_settings objectForKey:@"FontPaths"]))
	{
		fontPaths = [fontPaths arrayByAddingObject:path];
	}
	else 
	{
		fontPaths = [NSArray arrayWithObject:path];
	}
	[_settings setObject:fontPaths forKey:@"FontPaths"];
	[self Save];
}


+ (void)RemoveFontPath:(NSString *)path
{
}

@end
