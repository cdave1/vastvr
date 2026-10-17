//
//  FontItem.h
//  Tango
//
//  Created by David Petrie on 27/10/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface FontItem : NSObject 
{
@private
    NSString *_path;
	NSString *_resourceName;
	NSString *_title;
	NSString *_subtitle;
	NSString *_folderName;
}

@property (nonatomic, copy) NSString *path;
@property (nonatomic, copy) NSString *resourceName;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *subtitle;
@property (nonatomic, copy) NSString *folderName;

@end
