// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#import "AppDelegate.h"
#import <OpenGL/gl3.h>

@implementation AppDelegate
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
    NSRect available = NSScreen.mainScreen.visibleFrame;
    NSRect frame = NSMakeRect(0, 0, MIN(1280, available.size.width * 0.9), MIN(800, available.size.height * 0.85));
    window = [[NSWindow alloc] initWithContentRect:frame
        styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
        backing:NSBackingStoreBuffered defer:NO];
    window.title = @"PhysicsHotel"; window.delegate = self;
    window.contentMinSize = NSMakeSize(640, 480);
    NSOpenGLPixelFormatAttribute attributes[] = {NSOpenGLPFAAccelerated, NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24, NSOpenGLPFADepthSize, 24, NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, 0};
    NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    view = [[PhysicsView alloc] initWithFrame:frame pixelFormat:format]; [format release];
    window.contentView = view; [window center]; [window makeKeyAndOrderFront:nil];
    [window makeFirstResponder:view]; [[view openGLContext] makeCurrentContext];
    hd::Device::SetDisplayScreenSize(frame.size.width, frame.size.height);
#ifdef DEVELOPER_FILE_SYSTEM_ROOT
    hd::FileSystem::Init(DEVELOPER_FILE_SYSTEM_ROOT);
#else
    hd::FileSystem::Init([NSBundle.mainBundle.resourcePath UTF8String]);
#endif
    hd::graphics::ShaderLoader::Instance()->SetShaderLocation("resources/shaders/gl3/");
    client = new hd::application::Client(); [view setClient:client]; client->Init();
    [view setRenderProxy:self]; [NSApp activateIgnoringOtherApps:YES];
}
- (void)update { if (client) client->Update(); }
- (void)render { if (client) client->Render(); }
- (void)windowDidResignKey:(NSNotification *)notification { if (client) client->Pause(); }
- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)application { return YES; }
- (void)applicationWillTerminate:(NSNotification *)notification {
    if (client) { client->WillShutdown(); client->Shutdown(); }
}
@end
