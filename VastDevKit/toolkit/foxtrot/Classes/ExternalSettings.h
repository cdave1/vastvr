//
//  TexturePaths.h
//  Foxtrot
//
//  Created by David Petrie on 27/07/10.
//  Copyright 2010 n/a. All rights reserved.
//

/*
 * Global interface for loading/adding/removing/getting file system paths
 * from the TexturePaths.plist file.
 */
@interface ExternalSettings : NSObject 

+ (void)LoadSettings;

+ (void)SaveSettings;

+ (NSArray *)GetTexturePaths;

+ (void)AddTexturePath:(NSString *)path;

+ (void)RemoveTexturePath:(NSString *)path;

+ (void)ClearTexturePaths;

+ (void)SetLastLoadedFilePath:(NSString *)path;

+ (NSString *)GetLastLoadedFilePath;

+ (void) SetCurrentPrefabFilePath:(NSString *)path;

+ (NSString *)GetCurrentPrefabFilePath;

@end

