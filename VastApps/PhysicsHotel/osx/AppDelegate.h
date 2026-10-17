// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#import <Cocoa/Cocoa.h>
#import "PhysicsView.h"
@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate> {
    NSWindow *window;
    PhysicsView *view;
    hd::application::Client *client;
}
- (void)update;
- (void)render;
@end
