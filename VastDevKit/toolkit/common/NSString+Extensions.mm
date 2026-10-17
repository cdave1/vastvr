//
//  NSString+Extensions.mm
//  Tango
//
//  Created by David Petrie on 21/07/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "NSString+Extensions.h"


@implementation NSString (Extensions)

- (BOOL)hasSuffixes:(NSArray *)suffixes
{
	for (NSString *suffix in suffixes)
	{
		if ([self hasSuffix:suffix])
			return YES;
	}
	return NO;
}

@end
