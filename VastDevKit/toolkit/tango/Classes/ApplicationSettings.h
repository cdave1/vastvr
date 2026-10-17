//
//  TextureCollections.h
//  Tango
//
//  Created by David Petrie on 21/07/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/*
 * Global interface for loading/adding/removing/getting settings
 * from the Settings.plist file.
 */
@interface ApplicationSettings : NSObject 

+ (void)Load;

+ (void)Save;

+ (NSArray *)GetTexturePaths;

+ (void)AddTexturePath:(NSString *)path;

+ (void)RemoveTexturePath:(NSString *)path;



+ (NSArray *)GetFontPaths;

+ (void)AddFontPath:(NSString *)path;

+ (void)RemoveFontPath:(NSString *)path;



+ (NSString *)GetLastLoadedFilePath;

+ (void)SetLastLoadedFilePath:(NSString *)path;


+ (void)SetLastLoadedFileName:(NSString *)name;

+ (NSString *)GetLastLoadedFileName;

@end
