// Copyright (c) 2026 David Petrie. SPDX-License-Identifier: MIT
// Drives the real Builder, AppController, and Cocoa AppView in a private scene.
#import <AppKit/AppKit.h>
#import <OpenGL/gl3.h>
#include <vast/Vast.h>
#include "Builder.h"
#include "common/AppController.h"
#import "osx/AppView.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cmath>

using namespace hd;
using namespace hd::graphics;
using namespace hd::input;

class CheckedBuilder : public Builder {
public:
    explicit CheckedBuilder(hd::application::Client *client) : Builder(client) {}
    using Builder::GetEntityPool;
    void Draw(hd::Entity *camera, hd::input::EyePosition eyePosition) override;
};
static AppController *controller;
static CheckedBuilder *builder;
static AppView *view;
static std::string output;
static std::string capturePath;

// Same startup and frame sequence as the app Client; retaining the Builder
// pointer lets the test assert scene state without adding a product test API.
void hd::application::Client::Init() {
    controller = new AppController(this);
    builder = new CheckedBuilder(this);
    controller->AddApp(builder);
}
void hd::application::Client::Update() { UpdateEventQueue(); controller->Update(); UpdateTimer(); }
void hd::application::Client::Render() { controller->Draw(); }
void hd::application::Client::WillShutdown() {}
void hd::application::Client::Shutdown() {}
void hd::application::Client::Pause() {}
void hd::application::Client::Resume() {}

static void require(bool ok, const char *message) {
    if (!ok) { std::fprintf(stderr, "BUILDER CHECK FAILED: %s\n", message); std::exit(1); }
    std::printf("PASS: %s\n", message);
}

void CheckedBuilder::Draw(hd::Entity *camera, hd::input::EyePosition eyePosition) {
    Builder::Draw(camera, eyePosition);
    if (capturePath.empty()) return;
    NSRect backing = [view convertRectToBacking:view.bounds];
    GLint viewport[4], framebuffer, binding;
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &framebuffer);
    require(framebuffer != 0, "scene geometry renders into an offscreen framebuffer");
    require(viewport[2] == int(backing.size.width) && viewport[3] == int(backing.size.height),
            "scene geometry renders at native backing resolution");
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &binding);
    for (GLenum attachment : {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_DEPTH_ATTACHMENT}) {
        GLint texture, width, height;
        glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, attachment,
                                               GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        require(width == viewport[2] && height == viewport[3],
                "scene attachment allocates one texel per backing pixel");
    }
    glBindTexture(GL_TEXTURE_2D, binding);
    std::printf("Scene: %d x %d pixels; view: %.0f x %.0f points\n", viewport[2], viewport[3],
                view.bounds.size.width, view.bounds.size.height);

    // Put a one-pixel checkerboard into the real scene buffer. The complete
    // shadow/composite/presentation path must preserve it without scaling/blur.
    GLboolean scissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
    GLint scissor[4]; glGetIntegerv(GL_SCISSOR_BOX, scissor);
    glEnable(GL_SCISSOR_TEST);
    const GLfloat zero[] = {0, 0, 0, 0};
    for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x) {
        GLfloat level = (x + y) % 2 ? 1.0f : 0.0f;
        const GLfloat color[] = {level, level, level, 1};
        glScissor(32 + x, viewport[3] - 64 + y, 1, 1);
        glClearBufferfv(GL_COLOR, 0, color);
        glClearBufferfv(GL_COLOR, 1, zero);
        glClearBufferfv(GL_COLOR, 2, zero);
    }
    glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
    if (!scissorEnabled) glDisable(GL_SCISSOR_TEST);
}

@interface BuilderCheckProxy : NSObject { @public hd::application::Client *client; }
- (void)update;
- (void)render;
@end
@implementation BuilderCheckProxy
- (void)update { if (controller) client->Update(); }
- (void)render {
    if (!controller) return;
    client->Render();
    if (capturePath.empty()) return;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int width = viewport[2], height = viewport[3];
    std::vector<unsigned char> pixels(size_t(width) * height * 4), flipped(pixels.size());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    NSRect backing = [view convertRectToBacking:[view bounds]];
    require(width == int(backing.size.width) && height == int(backing.size.height),
            "final viewport covers the complete Retina backing surface");
    bool sharp = true;
    for (int y = 0; y < 8; ++y) for (int x = 0; x < 8; ++x) {
        size_t at = (size_t(height - 64 + y) * width + 32 + x) * 4;
        int expected = (x + y) % 2 ? 255 : 0;
        for (int channel = 0; channel < 3; ++channel)
            sharp &= std::abs(int(pixels[at + channel]) - expected) <= 1;
    }
    require(sharp, "one-pixel scene detail survives final presentation without blur or upscaling");
    for (int quarter = 0; quarter < 4; ++quarter) {
        int x = width * ((quarter & 1) ? 3 : 1) / 4;
        int y = height * ((quarter & 2) ? 3 : 1) / 4;
        bool painted = false;
        for (int dy = -4; dy < 4; ++dy) for (int dx = -4; dx < 4; ++dx) {
            size_t at = (size_t(y + dy) * width + x + dx) * 4;
            painted |= pixels[at] > 2 || pixels[at + 1] > 2 || pixels[at + 2] > 2;
        }
        require(painted, "scene content reaches this window quadrant");
    }
    for (int row = 0; row < height; ++row)
        std::copy(pixels.begin() + size_t(height - row - 1) * width * 4,
                  pixels.begin() + size_t(height - row) * width * 4,
                  flipped.begin() + size_t(row) * width * 4);
    NSBitmapImageRep *bitmap = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nullptr
        pixelsWide:width pixelsHigh:height bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES
        isPlanar:NO colorSpaceName:NSDeviceRGBColorSpace bytesPerRow:width * 4 bitsPerPixel:32];
    std::copy(flipped.begin(), flipped.end(), [bitmap bitmapData]);
    [[bitmap representationUsingType:NSBitmapImageFileTypePNG properties:@{}]
        writeToFile:[NSString stringWithUTF8String:capturePath.c_str()] atomically:YES];
    [bitmap release]; capturePath.clear();
}
@end

static void tick(double duration) {
    NSDate *until = [NSDate dateWithTimeIntervalSinceNow:duration];
    while ([until timeIntervalSinceNow] > 0)
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode
            beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.01]];
}
static void snapshot(const char *name) {
    capturePath = output + "/" + name + ".png"; tick(0.2);
    require(capturePath.empty(), "captured the builder's rendered frame");
}
static void key(NSString *characters, bool down) {
    NSEvent *event = [NSEvent keyEventWithType:down ? NSEventTypeKeyDown : NSEventTypeKeyUp
        location:NSZeroPoint modifierFlags:0 timestamp:[NSProcessInfo processInfo].systemUptime
        windowNumber:view.window.windowNumber context:nil characters:characters
        charactersIgnoringModifiers:characters isARepeat:NO keyCode:0];
    [NSApp sendEvent:event];
}
static void press(NSString *characters, double seconds = 0.08) {
    key(characters, true); tick(seconds); key(characters, false); tick(0.12);
}
static std::vector<Entity *> blocks() {
    std::vector<Entity *> values;
    EntityQuery *query = builder->GetEntityPool()->Query<LookAtResponder>("Block");
    for (int i = 0; i < query->Size(); ++i) values.push_back(query->At(i));
    return values;
}
static Entity *menuItem(const std::string &label) {
    EntityQuery *labels = builder->GetEntityPool()->Query<ui::Text, data::Parent>();
    for (int i = 0; i < labels->Size(); ++i) {
        Entity *item = labels->At(i);
        if (item->Get<ui::Text>()->GetText() == label)
            return const_cast<Entity *>(item->Get<data::Parent>()->GetEntity());
    }
    return nullptr;
}
static NSPoint screenPoint(Entity *item) {
    Vec3 world;
    Matrix::Vec3Multiply(world, Vec3(), item->Get<Transform>()->GetMatrix());
    auto headset = Cameras::mainCamera->Get<Headset>();
    Vec4 eye, clip;
    Matrix::Vec4Multiply(eye, Vec4(world[0], world[1], world[2], 1), headset->GetLookAtMatrix());
    Matrix::Vec4Multiply(clip, eye, headset->GetProjection());
    return NSMakePoint((clip[0] / clip[3] + 1) * view.bounds.size.width * 0.5,
                       (clip[1] / clip[3] + 1) * view.bounds.size.height * 0.5);
}
static void click(Entity *item) {
    require(item != nullptr, "found the requested menu control");
    if (item->GetName() == "Block") {
        // Tiny distant blocks need a finer pose than a single desktop arrow
        // step. Supply a tracked-head pose, then use the normal Space event.
        Vec3 center;
        Matrix::Vec3Multiply(center, Vec3(), item->Get<Transform>()->GetMatrix());
        auto headset = Cameras::mainCamera->Edit<Headset>();
        headset->SetOrientationOffset(Quaternion::FromTwoVectors(Vec3(0, 0, -1), center - headset->GetWorldPosition()));
        tick(0.3);
    }
    // The desktop fallback uses arrow keys for head direction and Space for
    // gaze selection. Mouse buttons are assigned to camera dragging.
    for (int turn = 0; turn < 70; ++turn) {
        NSPoint point = screenPoint(item);
        double dx = point.x - view.bounds.size.width * 0.5;
        double dy = point.y - view.bounds.size.height * 0.5;
        if (std::fabs(dx) < 24 && std::fabs(dy) < 24) break;
        if (std::fabs(dx) >= 24) press(dx < 0 ? @"\uf702" : @"\uf703", 0.012);
        else press(dy > 0 ? @"\uf700" : @"\uf701", 0.012);
    }
    NSPoint point = screenPoint(item);
    std::printf("Gaze selection at %.1f, %.1f\n", point.x, point.y);
    require(std::fabs(point.x - 640) < 24 && std::fabs(point.y - 400) < 24,
            "menu control is under the gaze cursor");
    press(@" ");
}

static std::string readFile(const std::string &path) {
    std::ifstream file(path); std::ostringstream content; content << file.rdbuf(); return content.str();
}

int main(int argc, char **argv) { @autoreleasepool {
    require(argc == 4, "resource root, output directory, and edit/reload mode supplied");
    output = argv[2];
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    NSOpenGLPixelFormatAttribute attributes[] = {NSOpenGLPFAAccelerated, NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24, NSOpenGLPFADepthSize, 24, NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, 0};
    NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    NSWindow *window = [[NSWindow alloc] initWithContentRect:NSMakeRect(30, 30, 1280, 800)
        styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable backing:NSBackingStoreBuffered defer:NO];
    [window setTitle:@"VastBuilder release check"];
    view = [[AppView alloc] initWithFrame:NSMakeRect(0, 0, 1280, 800) pixelFormat:format];
    require(view.wantsBestResolutionOpenGLSurface, "AppView requests a native-resolution OpenGL surface");
    [window setContentView:view];
    BuilderCheckProxy *proxy = [BuilderCheckProxy new];
    [view setRenderProxy:proxy];
    [window makeKeyAndOrderFront:nil];
    [NSApp activateIgnoringOtherApps:YES];
    [window makeFirstResponder:view];
    [[view openGLContext] makeCurrentContext];
    hd::Device::SetDisplayScreenSize(1280, 800);
    hd::FileSystem::Init(argv[1]);
    const char *file = "write-check/nested/café.txt";
    require(hd::FileSystem::WriteToFile(file, "long first save", 15) == 0, "writer creates directories and UTF-8 paths");
    require(hd::FileSystem::WriteToFile(file, "ok", 2) == 0, "writer replaces an existing file");
    require(readFile(hd::FileSystem::FullPathTo(file)) == "ok", "shorter save truncates old data");
    ShaderLoader::Instance()->SetShaderLocation("resources/shaders/gl3/");
    proxy->client = new hd::application::Client();
    [view setClient:proxy->client];
    proxy->client->Init();
    tick(0.6);
    require(window.firstResponder == view, "Cocoa keyboard focus belongs to AppView");
    std::string save = hd::FileSystem::FullPathTo("saves/MySaveFile.json");
    if (std::string(argv[3]) == "reload") {
        require(blocks().size() == 1, "fresh process restored exactly the saved block, without the brush preview");
        auto color = blocks()[0]->Get<Material>()->GetAmbient();
        auto origin = blocks()[0]->Get<Transform>()->GetOrigin();
        std::ifstream expected(output + "/expected-scene.txt");
        float saved[7]; for (float &v : saved) expected >> v;
        require(bool(expected), "saved scene expectation is available");
        for (int i = 0; i < 3; ++i) require(std::fabs(origin[i] - saved[i]) < 0.0001f, "restored block position matches saved scene");
        for (int i = 0; i < 4; ++i) require(std::fabs(color[i] - saved[i + 3]) < 0.0001f, "restored block color matches saved scene");
        snapshot("builder-reloaded");
    } else {
        require(blocks().empty(), "private test scene starts empty");
        Vec3 before = Cameras::mainCamera->Get<Headset>()->GetPosition();
        press(@"w");
        require((Cameras::mainCamera->Get<Headset>()->GetPosition() - before).Length() > 0, "native keyboard input moves the camera");
        press(@" ");
        require(blocks().size() == 1, "Space places a block through the app input path");
        snapshot("builder-painted");
        press(@"\t"); tick(0.8);
        snapshot("builder-menu");
        click(menuItem("Save"));
        require(!readFile(save).empty(), "Save menu control writes the scene file");
        press(@"\t"); tick(0.4);
        press(@"\t"); tick(0.6);
        Entity *huePicker = nullptr;
        auto items = builder->GetEntityPool()->Query<CollisionData>(DASHBOARD_MENU_ITEM_NAME);
        for (int i = 0; i < items->Size(); ++i) {
            const AABB bounds = items->At(i)->Get<CollisionData>()->GetMeshData()->GetAABB();
            if (bounds.Width() <= 10.1f && bounds.Height() > 50.0f) huePicker = items->At(i);
        }
        click(huePicker);
        press(@"\t"); tick(0.4);
        // Move the camera before painting a second brush session.
        press(@"d"); press(@" ");
        std::printf("Block count after second paint: %zu\n", blocks().size());
        snapshot("builder-colored");
        require(blocks().size() == 2, "a second keyboard paint session adds another block");
        auto second = blocks().back();
        auto chosen = second->Get<Material>()->GetAmbient();
        require(std::max(chosen[0], std::max(chosen[1], chosen[2])) > 0.95f &&
                std::min(chosen[0], std::min(chosen[1], chosen[2])) < 0.05f,
                "hue menu selection changes new paint from white to a saturated color");
        press(@"\t"); tick(0.7);
        Entity *first = blocks()[0];
        click(menuItem("Erase"));
        press(@"\t"); tick(0.4);
        click(first);
        require(blocks().size() == 1, "Erase control and gaze selection delete a block");
        press(@"\t"); tick(0.6);
        click(menuItem("Save"));
        require(readFile(save).find("entities") != std::string::npos, "save rewrite remains readable");
        auto block = blocks()[0];
        auto origin = block->Get<Transform>()->GetOrigin();
        auto color = block->Get<Material>()->GetAmbient();
        std::ofstream expected(output + "/expected-scene.txt");
        expected.precision(9);
        for (int i = 0; i < 3; ++i) expected << origin[i] << ' ';
        for (int i = 0; i < 4; ++i) expected << color[i] << ' ';
        expected.close();
        unsigned int sound = hd::sound::SoundController::PlayEffect(hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav").c_str());
        require(sound != 0, "project replacement sound loads and starts playback");
        hd::sound::SoundController::StopEffect(sound);
        unsigned int loop = hd::sound::SoundController::PlayEffectWithLoop(hd::FileSystem::FullPathTo("resources/sfx/converted/blop_002.wav").c_str());
        require(loop != 0, "looping effect starts");
        hd::sound::SoundController::StopEffect(loop);
        require(hd::sound::SoundController::PlayEffect("/nonexistent-vast-builder-check.wav") == 0,
                "invalid audio path reports failure");
        snapshot("builder-saved");
    }
    [window setContentSize:NSMakeSize(960, 600)]; tick(0.3);
    require(view.bounds.size.width == 960 && view.bounds.size.height == 600,
            "test window changed drawable size");
    snapshot("builder-resized");
    std::puts("BUILDER CHECK PASSED");
    return 0;
}}
