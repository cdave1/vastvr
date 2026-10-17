#import "AppDelegate.h"
#include <OpenGL/OpenGL.h>

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
    //[NSApp setDelegate:self];
    NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
    self.window = [[FullScreenWindow alloc] initWithContentRect: mainDisplayRect styleMask:NSBorderlessWindowMask backing:NSBackingStoreBuffered defer:YES];
    [self.window setLevel:NSMainMenuWindowLevel+1];
    [self.window setOpaque:YES];
    [self.window setHidesOnDeactivate:YES];


    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFANoRecovery,
		NSOpenGLPFAWindow,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADoubleBuffer,
        0
    };

    NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    NSRect viewRect = mainDisplayRect;//NSMakeRect(0.0, 0.0, 1920.0f, 1080.0f);
    self.glView = [[AppView alloc] initWithFrame:viewRect pixelFormat: pixelFormat];
    [self.window setContentView: self.glView];


    [self.glView  setWantsBestResolutionOpenGLSurface:YES];
    viewRect = mainDisplayRect;// NSMakeRect(0.0, 0.0, 1920.0f, 1080.0f);
    [self.glView convertRectToBacking:viewRect];
//    [self.window setAutodisplay:YES];
    [self.window setDelegate:self.glView];
    NSAssert([self.window canBecomeKeyWindow], @"Can beomce key");
    [self.window makeKeyAndOrderFront:self];
}


- (void)applicationDidBecomeActive:(NSNotification *)notification {



    BOOL res = [self.window makeFirstResponder:self];
    NSAssert(res, @"Yeah");
    
}


- (void) update {
}


- (void)StartApplication {
}


- (IBAction) resize:(id)sender {
	@synchronized(_lock) {
        NSLog(@"RESIZE");
	}
}

- (void)keyDown:(NSEvent *)event {
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    NSLog(@"W\n");
  //  exit(1);
}
/*
 * Set full screen.
 */

- (IBAction)setFullScreen:(id)sender {
	/*
     [ _glWindow setContentView:nil ];
     if( [ _glView isFullScreen ] )
     {
     if( ![ _glView setFullScreen:FALSE inFrame:[ _glWindow frame ] ] )
     [ self createFailed ];
     else
     [ _glWindow setContentView:_glView ];
     }
     else
     {
     if( ![ _glView setFullScreen:TRUE
     inFrame:NSMakeRect( 0, 0, 800, 600 ) ] )
     [ self createFailed ];
     }
	 */
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
	
 //   m_client->WillShutdown();
//	m_client->Shutdown();

	[_lock release];
	
	[super dealloc];
}


@end
