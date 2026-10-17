// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#import <AppKit/AppKit.h>
#import <OpenGL/gl3.h>
#import "PhysicsView.h"
#include "PhysicsSandbox.h"
#include "common/AppController.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>
using namespace hd;
using namespace hd::graphics;
using namespace hd::input;
static AppController *controller;
static PhysicsSandbox *sandbox;
static PhysicsView *view;
static std::string capture, output;
static unsigned int redPixels;

void hd::application::Client::Init() {
    controller = new AppController(this);
    sandbox = new PhysicsSandbox(this);
    controller->AddApp(sandbox); controller->SelectApp("PhysicsHotel");
}
void hd::application::Client::Update() { UpdateEventQueue();
    sandbox->ProcessInput(); controller->Update(); UpdateTimer(); }
void hd::application::Client::Render() { controller->Draw(); }
void hd::application::Client::WillShutdown() {}
void hd::application::Client::Shutdown() {}
void hd::application::Client::Pause() { sandbox->OnDeactivate(); }
void hd::application::Client::Resume() {}
static void require(bool ok, const char *message) {
    if (!ok) { std::fprintf(stderr, "PHYSICS CHECK FAILED: %s\n", message); std::exit(1); }
    std::printf("PASS: %s\n", message); std::fflush(stdout);
}
@interface PhysicsCheckProxy : NSObject { @public hd::application::Client *client; }
- (void)update;
- (void)render;
@end
@implementation PhysicsCheckProxy
- (void)update { if (controller) client->Update(); }
- (void)render {
    if (!controller) return;
    client->Render();
    if (capture.empty()) return;
    GLint viewport[4]; glGetIntegerv(GL_VIEWPORT, viewport);
    NSRect backing = [view convertRectToBacking:view.bounds];
    const int width = viewport[2], height = viewport[3];
    require(width == int(backing.size.width) && height == int(backing.size.height), "PhysicsHotel presents the full native Retina viewport");
    std::vector<unsigned char> pixels(size_t(width) * height * 4), flipped(pixels.size());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    redPixels = 0;
    for (size_t at = 0; at < pixels.size(); at += 4)
        if (pixels[at] > 100 && pixels[at] > 2 * int(pixels[at + 2])) ++redPixels;
    if (sandbox->ShapeCount()) {
        auto headset = Cameras::mainCamera->Get<Headset>();
        Vec4 eye, clip;
        Matrix::Vec4Multiply(eye, Vec4(sandbox->ShapePosition(0), 1.0f), headset->GetLookAtMatrix());
        Matrix::Vec4Multiply(clip, eye, headset->GetProjection());
        int x = int((clip[0] / clip[3] + 1) * width / 2), y = int((clip[1] / clip[3] + 1) * height / 2);
        bool orange = false;
        for (int dy = -10; dy <= 10; ++dy) for (int dx = -10; dx <= 10; ++dx) {
            if (x + dx < 0 || x + dx >= width || y + dy < 0 || y + dy >= height) continue;
            size_t at = (size_t(y + dy) * width + x + dx) * 4;
            orange |= pixels[at] > 80 && pixels[at] > 1.4f * pixels[at + 1] && pixels[at] > 2 * int(pixels[at + 2]);
        }
        require(orange, "the rendered orange ball follows its Box2D position");
    }
    bool quadrants = true;
    for (int q = 0; q < 4; ++q) {
        int x = width * ((q & 1) ? 3 : 1) / 4, y = height * ((q & 2) ? 3 : 1) / 4;
        size_t at = (size_t(y) * width + x) * 4;
        quadrants &= pixels[at] || pixels[at + 1] || pixels[at + 2];
    }
    for (int row = 0; row < height; ++row)
        std::copy(pixels.begin() + size_t(height - row - 1) * width * 4,
                  pixels.begin() + size_t(height - row) * width * 4, flipped.begin() + size_t(row) * width * 4);
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nullptr
        pixelsWide:width pixelsHigh:height bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES
        isPlanar:NO colorSpaceName:NSDeviceRGBColorSpace bytesPerRow:width * 4 bitsPerPixel:32];
    std::copy(flipped.begin(), flipped.end(), bitmap.bitmapData);
    require([[bitmap representationUsingType:NSBitmapImageFileTypePNG properties:@{}]
        writeToFile:[NSString stringWithUTF8String:capture.c_str()] atomically:YES], "captured PhysicsHotel frame");
    [bitmap release]; capture.clear();
    require(quadrants, "scene reaches all four window quadrants");
}
@end
static void tick(double seconds) {
    NSDate *until = [NSDate dateWithTimeIntervalSinceNow:seconds];
    while (until.timeIntervalSinceNow > 0)
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.01]];
}
static void snapshot(const char *name) {
    capture = output + "/" + name + ".png"; tick(0.25);
    require(capture.empty(), "render callback completed the capture");
}
static void key(NSString *characters, bool down) {
    [NSApp sendEvent:[NSEvent keyEventWithType:down ? NSEventTypeKeyDown : NSEventTypeKeyUp
        location:NSZeroPoint modifierFlags:0 timestamp:NSProcessInfo.processInfo.systemUptime
        windowNumber:view.window.windowNumber context:nil characters:characters
        charactersIgnoringModifiers:characters isARepeat:NO keyCode:0]];
}
static void press(NSString *characters, double seconds = 0.06) {
    key(characters, true); tick(seconds); key(characters, false); tick(0.1);
}
static NSPoint screenPoint(const Vec3 &world) {
    auto headset = Cameras::mainCamera->Get<Headset>();
    Vec4 eye, clip;
    Matrix::Vec4Multiply(eye, Vec4(world, 1.0f), headset->GetLookAtMatrix());
    Matrix::Vec4Multiply(clip, eye, headset->GetProjection());
    return NSMakePoint((clip[0] / clip[3] + 1) * view.bounds.size.width / 2,
                       (clip[1] / clip[3] + 1) * view.bounds.size.height / 2);
}
static void mouse(NSEventType type, NSPoint point) {
    NSEvent *event = [NSEvent mouseEventWithType:type location:point modifierFlags:0
        timestamp:NSProcessInfo.processInfo.systemUptime windowNumber:view.window.windowNumber
        context:nil eventNumber:0 clickCount:1 pressure:1];
    if (type == NSEventTypeLeftMouseDown) [view mouseDown:event];
    else if (type == NSEventTypeLeftMouseDragged) [view mouseDragged:event];
    else [view mouseUp:event];
    tick(0.04);
}
int main(int argc, char **argv) { @autoreleasepool {
    require(argc == 2, "private resource directory supplied"); output = argv[1];
    [NSApplication sharedApplication]; [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    NSOpenGLPixelFormatAttribute attributes[] = {NSOpenGLPFAAccelerated, NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24, NSOpenGLPFADepthSize, 24, NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, 0};
    NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(30, 30, 1280, 800)
        styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
        backing:NSBackingStoreBuffered defer:NO];
    window.title = @"PhysicsHotel integration check";
    view = [[PhysicsView alloc] initWithFrame:NSMakeRect(0, 0, 1280, 800) pixelFormat:format];
    window.contentView = view;
    PhysicsCheckProxy *proxy = [PhysicsCheckProxy new]; [view setRenderProxy:proxy];
    [window makeKeyAndOrderFront:nil]; [NSApp activateIgnoringOtherApps:YES]; [window makeFirstResponder:view];
    [[view openGLContext] makeCurrentContext];
    Device::SetDisplayScreenSize(1280, 800); FileSystem::Init(argv[1]);
    ShaderLoader::Instance()->SetShaderLocation("resources/shaders/gl3/");
    proxy->client = new hd::application::Client(); [view setClient:proxy->client]; proxy->client->Init();
    require(window.firstResponder == view, "native keyboard focus belongs to AppView");
    require(sandbox->ShapeCount() == 16, "sandbox starts with sixteen dynamic shapes");
    Vec3 initial = sandbox->ShapePosition(0); unsigned int steps = sandbox->PhysicsSteps();
    tick(0.75);
    require(sandbox->ShapePosition(0)[1] < initial[1] - 100, "gravity makes the rendered sandbox bodies fall");
    require(sandbox->PhysicsSteps() - steps >= 20 && sandbox->PhysicsSteps() - steps <= 65,
        "physics advances at a bounded fixed rate independent of render callbacks");
    press(@"p"); require(sandbox->IsPaused(), "P pauses through the native keyboard path");
    Vec3 paused = sandbox->ShapePosition(0); float angle = sandbox->ShapeAngle(1);
    tick(0.3);
    require((sandbox->ShapePosition(0) - paused).Length() < 0.001f && sandbox->ShapeAngle(1) == angle,
        "paused physics preserves both position and rotation");
    snapshot("physics-paused"); require(redPixels > 500, "colored physics geometry is visible in the frame");
    mouse(NSEventTypeLeftMouseDown, screenPoint(paused));
    require(sandbox->IsDragging(), "native mouse down picks the physics body under the pointer");
    Vec3 destination(180, 370, 0);
    mouse(NSEventTypeLeftMouseDragged, screenPoint(destination));
    press(@"g"); require(!sandbox->HasGravity(), "G disables gravity");
    press(@"p"); require(!sandbox->IsPaused(), "P resumes the simulation"); tick(0.65);
    require((sandbox->ShapePosition(0) - destination).Length() < 35, "mouse joint pulls the chosen body to the dragged position");
    mouse(NSEventTypeLeftMouseUp, NSMakePoint(10, 10));
    require(!sandbox->IsDragging(), "releasing away from the body always ends dragging");
    snapshot("physics-dragged");
    size_t count = sandbox->ShapeCount(); press(@" ");
    require(sandbox->ShapeCount() == count + 1, "Space adds a dynamic shape");
    press(@"\177"); require(sandbox->ShapeCount() == count, "Backspace removes a dynamic shape");
    press(@"r"); require(sandbox->ShapeCount() == 16 && sandbox->HasGravity() && !sandbox->IsPaused(),
        "R restores the initial scene and simulation settings");
    tick(2.8);
    bool contained = true;
    for (size_t i = 0; i < sandbox->ShapeCount(); ++i) {
        Vec3 p = sandbox->ShapePosition(i);
        contained &= std::isfinite(p[0]) && std::isfinite(p[1]) && p[0] > -510 && p[0] < 510 && p[1] > 20 && p[1] < 510;
    }
    require(contained, "floor and walls contain every simulated shape after collisions");
    press(@"p"); paused = sandbox->ShapePosition(0);
    mouse(NSEventTypeLeftMouseDown, screenPoint(paused)); require(sandbox->IsDragging(), "body can be picked again");
    proxy->client->Pause(); require(!sandbox->IsDragging(), "pausing the client cancels an active mouse joint");
    press(@"r");
    [window setContentSize:NSMakeSize(960, 600)]; tick(0.25); snapshot("physics-resized");
    press(@"p");
    mouse(NSEventTypeLeftMouseDown, screenPoint(sandbox->ShapePosition(0)));
    require(sandbox->IsDragging(), "pointer picking remains aligned after resizing the window");
    mouse(NSEventTypeLeftMouseUp, NSMakePoint(10, 10));
    [window setContentSize:NSMakeSize(800, 900)]; tick(0.25); snapshot("physics-portrait");
    mouse(NSEventTypeLeftMouseDown, screenPoint(sandbox->ShapePosition(0)));
    require(sandbox->IsDragging(), "pointer picking also follows a changed aspect ratio");
    press(@"r"); require(!sandbox->IsDragging(), "reset cancels the selected mouse joint");
    press(@"p");
    for (int i = 0; i < 70; ++i) { key(@" ", true); key(@" ", false); tick(0.015); }
    require(sandbox->ShapeCount() == 64, "adding shapes stops safely at the capacity limit");
    for (int i = 0; i < 70; ++i) { key(@"\177", true); key(@"\177", false); tick(0.015); }
    require(sandbox->ShapeCount() == 0, "removing shapes stops safely at an empty scene");
    press(@"r"); require(sandbox->ShapeCount() == 16, "reset rebuilds a scene after all bodies have been removed");
    press(@"p");
    mouse(NSEventTypeLeftMouseDown, screenPoint(Vec3(0, 500, 0)));
    require(!sandbox->IsDragging(), "pressing empty space does not create a mouse joint");
    mouse(NSEventTypeLeftMouseUp, NSMakePoint(10, 10));
    const size_t last = sandbox->ShapeCount() - 1;
    mouse(NSEventTypeLeftMouseDown, screenPoint(sandbox->ShapePosition(last)));
    require(sandbox->IsDragging(), "the last box can be selected before removal");
    press(@"\177");
    require(sandbox->ShapeCount() == last && !sandbox->IsDragging(),
        "removing a selected body destroys its mouse joint safely");
    mouse(NSEventTypeLeftMouseUp, NSMakePoint(10, 10));
    press(@"r"); press(@"p"); press(@"g");
    mouse(NSEventTypeLeftMouseDown, screenPoint(sandbox->ShapePosition(0)));
    require(sandbox->IsDragging(), "a body can be selected for dragging outside the board");
    mouse(NSEventTypeLeftMouseDragged, screenPoint(Vec3(1500, 1500, 0)));
    press(@"p"); tick(0.8);
    Vec3 clamped = sandbox->ShapePosition(0);
    require(!sandbox->HasGravity() && !sandbox->IsPaused() && clamped[0] > 430 && clamped[0] < 510 &&
        clamped[1] > 430 && clamped[1] < 510,
        "dragging beyond the window keeps the moving body near the clamped board corner");
    mouse(NSEventTypeLeftMouseUp, NSMakePoint(-50, -50));
    require(!sandbox->IsDragging(), "release outside the window ends the clamped drag");
    press(@"r");
    [window setContentSize:NSMakeSize(1280, 800)]; tick(0.15); snapshot("physics-sandbox");
    std::puts("PHYSICS CHECK PASSED");
}}
