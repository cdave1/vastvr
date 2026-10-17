//
//  ViewController.h
//  test
//
//  Created by David Petrie on 15/04/13.
//  Copyright (c) 2013 David Petrie. SPDX-License-Identifier: MIT
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#include "hdk.h"

@interface ViewController : GLKViewController
{
    hd::application::Client *m_client;
}
- (void) setClient:(hd::application::Client *)client;

@end
