//
//  AppDelegate.h
//  test
//
//  Created by David Petrie on 15/04/13.
//  Copyright (c) 2013 David Petrie. SPDX-License-Identifier: MIT
//

#import <UIKit/UIKit.h>
#include "Core/Application/Client.h"

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>
{
    hd::application::Client *m_client;
}

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) ViewController *viewController;

@end
