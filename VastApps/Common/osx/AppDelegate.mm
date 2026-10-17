#import "AppDelegate.h"

@interface FullScreenWindow : NSWindow
- (BOOL)canBecomeKeyWindow;
@end


@implementation FullScreenWindow
- (BOOL)canBecomeKeyWindow {
    return YES;
}
@end


@implementation AppDelegate

@synthesize window, glView;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
    NSRect viewRect = NSMakeRect(0.0, 0.0, mainDisplayRect.size.width, mainDisplayRect.size.height);
    self.window = [[FullScreenWindow alloc] initWithContentRect:viewRect styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:YES];
    [self.window setLevel:NSMainMenuWindowLevel+1];
    [self.window setOpaque:YES];
    [self.window setHidesOnDeactivate:YES];

    NSOpenGLPixelFormatAttribute attrs[] = {
		NSOpenGLPFAAccelerated,
        NSOpenGLPFANoRecovery,
		NSOpenGLPFADoubleBuffer,
        //NSOpenGLPFAMultisample,
        NSOpenGLPFAClosestPolicy,
        NSOpenGLPFAAllowOfflineRenderers,
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)24,
        NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute)8,
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
    //    NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)2,
     //   NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)8,
        0
    };

    _lock = [[NSObject alloc] init];

    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    self.glView = [[AppView alloc] initWithFrame:viewRect pixelFormat: pixelFormat];
    [self.window setContentView: self.glView];

    hd::Device::SetDisplayScreenSize(viewRect.size.width, viewRect.size.height);

#ifdef DEVELOPER_FILE_SYSTEM_ROOT
    hd::FileSystem::Init(DEVELOPER_FILE_SYSTEM_ROOT);
#else
    hd::FileSystem::Init([[[NSBundle mainBundle] resourcePath] UTF8String]);
#endif
    hd::graphics::ShaderLoader::Instance()->SetShaderLocation("resources/shaders/gl3/");

    m_client = new hd::application::Client();
    [self.glView setClient:m_client];
    //[self.window setDelegate:self.glView];
    [self.glView setRenderProxy:self];

    [self.window makeKeyAndOrderFront:self];
    [self.window makeFirstResponder:self.glView];
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
}


static BOOL clientInitted = NO;
static int waiting = 0;

- (void) update {
    if (clientInitted == NO) {
        if (waiting++ > 100) {
            m_client->Init();
            clientInitted = YES;
        }
    } else {
        m_client->Update();
    }
}


- (void) render {
    if (clientInitted) {
        m_client->Render();
    }
}


/*
 * Called if we fail to create a valid OpenGL view
 */

- (void) createFailed {
	NSWindow *infoWindow;
	
	infoWindow = NSGetCriticalAlertPanel( @"Initialization failed",
                                         @"Failed to initialize OpenGL",
                                         @"OK", nil, nil );
	[NSApp runModalForWindow:infoWindow];
	[infoWindow close];
	[NSApp terminate:self];
}


- (void) dealloc {
    m_client->WillShutdown();
	m_client->Shutdown();
	
	[self.glView release];
	[_lock release];
	
	[super dealloc];
}

@end
