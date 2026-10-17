#import "AppDelegate.h"


static bool keepRenderThreadAlive = true;


@interface AppDelegate (Private)
- (void)StartApplication;
- (void)RenderThread;
- (void) createFailed;
@end


@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {}


- (void) update {
	m_client->Update();
    m_client->Render();
}


static BOOL clientInitted = NO;
- (void) render {
    if (clientInitted == NO) {
        m_client->Init();
        clientInitted = YES;
    }
    
    @synchronized(_lock) {
        m_client->Render();
    }
}


- (void)RenderThread {
	if (clientInitted == NO) {
        return;
    }
    
    @synchronized(_lock) {
        m_client->Update();
    }
    
    [_glView nextFrame];
}


- (void)StartRenderThread {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	[[_glView openGLContext] makeCurrentContext];
    [NSTimer scheduledTimerWithTimeInterval:0.016 target:self selector:@selector(RenderThread) userInfo:nil repeats:TRUE];
    [pool drain];
}


- (void)StartApplication {
    m_client = new hd::application::Client();
    [_glView setClient:m_client];
    
    //[_glView enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
    
    hd::Device::SetDisplayScreenSize([_glView frame].size.width, [_glView frame].size.height);
    
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	
	[[_glView openGLContext] makeCurrentContext];
    [NSTimer scheduledTimerWithTimeInterval:0.001 target:self selector:@selector(RenderThread) userInfo:nil repeats:TRUE];
    [pool drain];
    
	//[NSThread detachNewThreadSelector:@selector(StartRenderThread) toTarget:self withObject:nil];
}


- (void) awakeFromNib {
	_lock = [[NSObject alloc] init];
	

    [_glView setRenderProxy:self];
    
	[NSApp setDelegate:self];
    
    [self StartApplication];
    
}


- (IBAction) resize:(id)sender {
	@synchronized(_lock) {
        NSLog(@"RESIZE");
	}
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
	keepRenderThreadAlive = false;
	
    m_client->WillShutdown();
	m_client->Shutdown();
	
	[_glView release];
	[_glWindow release];
	[_lock release];
	
	[super dealloc];
}


@end
