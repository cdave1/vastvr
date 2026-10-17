// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
#import <AppKit/AppKit.h>
#import <OpenGL/gl3.h>
#import "osx/AppView.h"
#include "Browser.h"
#include "apps/Piano/Piano.h"
#include "apps/MusicPlayer/MusicPlayer.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>
using namespace hd;
using namespace hd::graphics;
using namespace hd::input;
static AppController *controller;
static Browser *browser;
static AppView *view;
static std::string capture, output;
static unsigned int redPixels, colorSample;

void hd::application::Client::Init() {
    controller = new AppController(this);
    browser = RegisterBrowserApps(controller, this);
}
void hd::application::Client::Update() { UpdateEventQueue(); controller->Update(); UpdateTimer(); }
void hd::application::Client::Render() { controller->Draw(); browser->DrawCursor(Cameras::mainCamera); }
void hd::application::Client::WillShutdown() {}
void hd::application::Client::Shutdown() {}
void hd::application::Client::Pause() {}
void hd::application::Client::Resume() {}
static void require(bool ok, const char *message) {
    if (!ok) { std::fprintf(stderr, "BROWSER CHECK FAILED: %s\n", message); std::exit(1); }
    std::printf("PASS: %s\n", message); std::fflush(stdout);
}
@interface BrowserCheckProxy : NSObject { @public hd::application::Client *client; }
- (void)update;
- (void)render;
@end
@implementation BrowserCheckProxy
- (void)update { if (controller) client->Update(); }
- (void)render {
    if (!controller) return;
    client->Render();
    if (capture.empty()) return;
    GLint viewport[4]; glGetIntegerv(GL_VIEWPORT, viewport);
    NSRect backing = [view convertRectToBacking:view.bounds];
    const int width = viewport[2], height = viewport[3];
    require(width == int(backing.size.width) && height == int(backing.size.height), "Browser presents the full native Retina viewport");
    std::vector<unsigned char> pixels(size_t(width) * height * 4), flipped(pixels.size());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    const size_t sampleAt = (size_t(height / 2 + 24) * width + width / 2 + 24) * 4;
    colorSample = (unsigned(pixels[sampleAt]) << 16) | (unsigned(pixels[sampleAt + 1]) << 8) | pixels[sampleAt + 2];
    redPixels = 0;
    for (size_t at = 0; at < pixels.size(); at += 4)
        if (pixels[at] > 100 && pixels[at] > 2 * int(pixels[at + 2])) ++redPixels;
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
        writeToFile:[NSString stringWithUTF8String:capture.c_str()] atomically:YES], "captured Browser frame");
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
static void aim(const Vec3 &point, bool precise = false) {
    if (precise) {
        auto headset = Cameras::mainCamera->Edit<Headset>();
        headset->SetOrientationOffset(Quaternion::FromTwoVectors(Vec3(0, 0, -1), point - headset->GetWorldPosition()));
        tick(0.15);
    } else {
        for (int i = 0; i < 60; ++i) {
            NSPoint p = screenPoint(point);
            double dx = p.x - 640, dy = p.y - 400;
            if (std::fabs(dx) < 18 && std::fabs(dy) < 18) break;
            press(std::fabs(dx) >= 18 ? (dx < 0 ? @"\uf702" : @"\uf703") : (dy > 0 ? @"\uf700" : @"\uf701"), 0.012);
        }
    }
    NSPoint p = screenPoint(point);
    require(std::fabs(p.x - 640) < 18 && std::fabs(p.y - 400) < 18, "gaze aims at the requested control");
}
static void current(const char *name) {
    require(controller->GetCurrentApp()->m_name == name, name);
}
int main(int argc, char **argv) { @autoreleasepool {
    require(argc == 2 || (argc == 3 && std::string(argv[2]) == "piano"), "private resource directory and optional piano mode supplied"); output = argv[1];
    [NSApplication sharedApplication]; [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    NSOpenGLPixelFormatAttribute attributes[] = {NSOpenGLPFAAccelerated, NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24, NSOpenGLPFADepthSize, 24, NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, 0};
    NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(30, 30, 1280, 800)
        styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable backing:NSBackingStoreBuffered defer:NO];
    window.title = @"VastBrowser integration check";
    view = [[AppView alloc] initWithFrame:NSMakeRect(0, 0, 1280, 800) pixelFormat:format];
    window.contentView = view;
    BrowserCheckProxy *proxy = [BrowserCheckProxy new]; [view setRenderProxy:proxy];
    [window makeKeyAndOrderFront:nil]; [NSApp activateIgnoringOtherApps:YES]; [window makeFirstResponder:view];
    [[view openGLContext] makeCurrentContext];
    Device::SetDisplayScreenSize(1280, 800); FileSystem::Init(argv[1]);
    ShaderLoader::Instance()->SetShaderLocation("resources/shaders/gl3/");
    proxy->client = new hd::application::Client(); [view setClient:proxy->client]; proxy->client->Init(); tick(0.5);
    require(window.firstResponder == view, "native keyboard focus belongs to AppView");
    if (argc > 2) {
        controller->SelectApp("Piano"); tick(0.4);
        auto piano = dynamic_cast<const Piano *>(controller->GetCurrentApp());
        aim(Vec3(80, 190, 90), true); press(@" ");
        fprintf(stderr, "Piano note received: %d\n", piano->LastPlayedNote());
        require(piano->LastPlayedNote() == 69, "precise A4 selection");
        std::puts("BROWSER PIANO CHECK PASSED"); return 0;
    }
    current("Browser"); snapshot("browser-home");
    press(@"1"); current("Music & audio"); press(@"2"); current("Browser");
    const char *names[] = {"Teapots", "Shadows", "Geometry", "Piano", "Music & audio"};
    for (int i = 0; i < 5; ++i) {
        aim(Vec3((i % 3 - 1) * 440.0f, 220 - (i / 3) * 270.0f, 0)); press(@" ");
        current(names[i]); tick(0.25);
        std::string shot = "browser-demo-" + std::to_string(i); snapshot(shot.c_str());
        if (i == 0) require(redPixels > 1000, "verified teapot model produces visible red geometry");
        if (i == 2) {
            aim(Vec3(0, 50, 75)); snapshot("browser-geometry-before");
            const unsigned int before = colorSample;
            press(@" "); snapshot("browser-geometry-after");
            require(colorSample != before, "native Space selection changes the rendered block color");
        }
        if (i == 3) {
            const Piano *piano = dynamic_cast<const Piano *>(controller->GetCurrentApp());
            require(piano != nullptr, "selected the real piano app");
            aim(Vec3(80, 190, 90), true); key(@" ", true); tick(0.08);
            std::printf("Piano note received: %d\n", piano->LastPlayedNote());
            require(piano->LastPlayedNote() == 69, "native Space selection plays the A4 piano key");
            require(piano->QueuedAudioBytes() > 0, "procedural piano samples reach the audio device");
            Cameras::mainCamera->Edit<Headset>()->SetOrientationOffset(Quaternion::FromTwoVectors(Vec3(0, 0, -1), Vec3(0, 1, -1)));
            tick(0.08); key(@" ", false); tick(0.15);
            unsigned int before = piano->QueuedAudioBytes();
            aim(Vec3(80, 190, 90), true); key(@" ", true); tick(0.06);
            require(piano->QueuedAudioBytes() > before, "releasing away from a key restores it for the next press");
            key(@" ", false); tick(0.1);
            aim(Vec3(96, 210, 10), true); press(@" ");
            require(piano->LastPlayedNote() == 70, "native selection also plays a raised black piano key");
        }
        if (i == 4) {
            auto audio = const_cast<MusicPlayer *>(dynamic_cast<const MusicPlayer *>(controller->GetCurrentApp()));
            require(audio != nullptr, "selected the real audio app");
            aim(Vec3(0, 150, 0)); press(@" "); require(audio->HasActiveAudio(), "project-sound button starts playback");
            aim(Vec3(440, 150, 0)); press(@" "); require(!audio->HasActiveAudio(), "Stop button ends playback");
            std::ifstream source(FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav"), std::ios::binary);
            std::vector<char> bytes((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());
            require(!bytes.empty(), "licensed project recording is available");
            require(FileSystem::WriteToFile("local café.wav", bytes.data(), unsigned(bytes.size())) == 0, "private local audio path with spaces and Unicode is available");
            require(audio->PlayAudio(FileSystem::FullPathTo("local café.wav")), "local-file playback handler accepts the chosen audio path");
            require(!audio->PlayAudio("/nonexistent-vast-browser.wav"), "invalid local audio fails without replacing current playback");
            press(@"\t"); current("Browser"); require(!audio->HasActiveAudio(), "leaving the audio app stops playback");
            continue;
        }
        press(@"\t"); current("Browser");
    }
    Vec3 position = Cameras::mainCamera->Get<Headset>()->GetPosition(); press(@"w");
    require((Cameras::mainCamera->Get<Headset>()->GetPosition() - position).Length() > 0, "WASD movement reaches the camera");
    press(@"\t");
    [window setContentSize:NSMakeSize(960, 600)]; tick(0.25); snapshot("browser-resized");
    std::puts("BROWSER CHECK PASSED");
}}
