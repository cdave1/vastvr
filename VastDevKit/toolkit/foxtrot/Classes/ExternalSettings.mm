//
//  TextureCollections.mm
//  Tango
//
//  Created by David Petrie on 21/07/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "ExternalSettings.h"


static NSMutableDictionary * _settings = nil;

@implementation ExternalSettings

+ (void)LoadSettings
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
	
	plistPath = [rootPath stringByAppendingPathComponent:@"Settings.plist"];
	if (![[NSFileManager defaultManager] fileExistsAtPath:plistPath]) 
	{
		plistPath = [[NSBundle mainBundle] pathForResource:@"Settings" ofType:@"plist"];
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
}


+ (void)SaveSettings
{
	if (!_settings) return;
	
	NSString *error;
    NSString *rootPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *plistPath = [rootPath stringByAppendingPathComponent:@"Settings.plist"];
	
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
		[_settings setObject:texturePaths forKey:@"TexturePaths"];
		[self SaveSettings];
	}
}


+ (void)RemoveTexturePath:(NSString *)path
{
}


+ (void)SetLastLoadedFilePath:(NSString *)path
{
	if (!_settings) return;
	[_settings setObject:path forKey:@"LastLoadedFilePath"];
	[self SaveSettings];
}


+ (void)ClearTexturePaths
{
	if (!_settings) return;
	NSArray *texturePaths;
	if ((texturePaths = [_settings objectForKey:@"TexturePaths"]))
	{
		texturePaths = [NSArray array];
		[_settings setObject:texturePaths forKey:@"TexturePaths"];
		[self SaveSettings];
	}
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


+ (void) SetCurrentPrefabFilePath:(NSString *)path
{
	if (!_settings) return;
	[_settings setObject:path forKey:@"CurrentPrefabFilePath"];
	[self SaveSettings];
}


+ (NSString *)GetCurrentPrefabFilePath
{
	if (!_settings) return nil;
	NSString *path;
	if ((path = [_settings objectForKey:@"CurrentPrefabFilePath"]))
	{
		return path;
	}
	return nil;
}

@end
