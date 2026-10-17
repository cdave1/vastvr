#import "AppView.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>


@implementation AppView

static NSTimer *timer = nil;

- (void)prepareOpenGL {

    // this sets swap interval for double buffering
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

    // this enables alpha in the frame buffer (commented now)
    //  GLint opaque = 0;
    //  [[self openGLContext] setValues:&opaque forParameter:NSOpenGLCPSurfaceOpacity];
    /*
}



- (void)windowDidResignKey:(NSNotification *)notification {
    //    NSLog(@"window did resign main");
    [timer invalidate];

    //game_deactivate();      // freeze, pause
    [self setNeedsDisplay:YES];
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
    //    NSLog(@"window did become main");
*/
    //game_activate();
    [self setNeedsDisplay:YES];

    timer = [NSTimer timerWithTimeInterval:0.001
                                    target:self
                                  selector:@selector(timerEvent:)
                                  userInfo:nil
                                   repeats:YES];

    [[NSRunLoop mainRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
}

- (void)timerEvent:(NSTimer *)t {
    //run_game();
//    [self setNeedsDisplay:YES];
    [self blbl];
}



//-(BOOL)acceptsFirstResponder { return YES; }

/*
 * Enable/disable full screen mode
 */
- (BOOL) setFullScreen:(BOOL)enableFS inFrame:(NSRect)frame {
    return TRUE;
}

- (void)reshape {
}


- (BOOL)canBecomeKeyView
{
    return  YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)becomeFirstResponder
{
    return YES;
}

- (BOOL)resignFirstResponder
{
    return YES;
}
- (void) update {
   // [self setNeedsDisplay:YES];
}

static float f1 = 0.0f;
static float m_currentTime = 0.0f;
static float m_LastFPSUpdateTime = 0.0f;
static float m_LastFrameTime = 0.0f;
static unsigned m_frames = 0;
static unsigned m_totalFrames = 0;
static unsigned m_totalSecs = 0;

- (void) drawRect: (NSRect) bounds {
}

- (void) blbl {
//    NSRect viewRect = NSMakeRect(0.0, 0.0, 1920.0f, 1080.0f);

   // [[self openGLContext] makeCurrentContext];

    m_currentTime = CACurrentMediaTime();


    if (m_LastFPSUpdateTime == 0) {
        m_LastFPSUpdateTime = m_currentTime;
    }

    if (m_LastFrameTime == 0) {
        m_LastFrameTime = m_currentTime;
    }

    if ((m_currentTime - m_LastFPSUpdateTime) > 1.0f) {
        fprintf(stderr, "fps: %d\n", m_frames);
        m_totalFrames += m_frames;
        ++m_totalSecs;
        if (m_totalSecs == 10) {
            fprintf(stderr, "Frame rate: %d\n", m_totalFrames/m_totalSecs);
            m_totalSecs =0;
            m_totalFrames = 0;
        }
        m_frames = 0;
        m_LastFPSUpdateTime = m_currentTime;
    }
    m_frames++;



    f1 += 0.08f;
    float f = sinf(f1);

    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
    GLsizei backingPixelWidth  = (GLsizei)(backingBounds.size.width),
    backingPixelHeight = (GLsizei)(backingBounds.size.height);

    // Set viewport
    glViewport(0, 0, backingPixelWidth, backingPixelHeight);
    
    glClearColor(0, 0, f, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(f * 0.5f + 0.5f, 0.85f, 0.35f);
    glBegin(GL_TRIANGLES);

    {
        glVertex3f(  f,  0.6, 0.0);
        glVertex3f( -0.2, -0.3, 0.0);
        glVertex3f(  0.2, -0.3 ,0.0);
    }
    glEnd();

    [[self openGLContext] flushBuffer];
//    [self setNeedsDisplay:YES];
}



- (void)keyDown:(NSEvent *)event {
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    NSLog(@"W\n");
}
/*
- (void)keyDown:(NSEvent *)event {
    if (!m_client) {
        return;
    }
    
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    eventQueue->OnButtonDown(0, unicodeKey);
}


- (void)keyUp:(NSEvent *)event {
    if (!m_client) {
        return;
    }
    
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    eventQueue->OnButtonUp(0, unicodeKey);
}


- (void)mouseDown:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    
	if (event.type == NSLeftMouseDown) {
        eventQueue->OnPointerSelect(0, location);
	} else if (event.type == NSRightMouseDown) {
        eventQueue->OnPointerSelect(1, location);
	}
}


- (void)mouseUp:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    
    if (event.type == NSLeftMouseUp) {
        eventQueue->OnPointerRelease(0, location);
    } else if (event.type == NSRightMouseUp) {
        eventQueue->OnPointerRelease(1, location);
    }
}


- (void) mouseEntered:(NSEvent *)theEvent {
    
}


- (void) mouseExited:(NSEvent *)theEvent {
    
}
*/

- (void)mouseMoved:(NSEvent *)event {
    /*
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
     */


    /*
    eventQueue->OnPointerMove
    
    if (event.type == NSLeftMouseUp) {
        eventQueue->OnPointerRelease(event.timestamp, 0, location);
    } else if (event.type == NSRightMouseUp) {
        eventQueue->OnPointerRelease(event.timestamp, 1, location);
    }*/
    
    //if (location[0] > 0.0f && location[1] > 0.0f) {
       // NSLog(@"Mouse moved. %3.0f %3.0f\n", location[0], location[1]);
    //}
    
    
    
    /*
	if (theEvent.type == NSMouseMoved)
	{
        NSLog(@"Mouse moved");
        
		mouseValues.previousLocation.Set(mouseValues.currentLocation.x,
                                         mouseValues.currentLocation.y);
        
		mouseValues.currentLocation.Set(theEvent.locationInWindow.x,
                                        theEvent.locationInWindow.y);
		
		if (mouseValues.leftButton.clickState == clickstate_down)
		{
			mouseValues.leftButton.clickState = clickstate_moving;
		}
		
		if (mouseValues.rightButton.clickState == clickstate_down)
		{
			mouseValues.rightButton.clickState = clickstate_moving;
		}
	}*/
}

/*
- (void)mouseDragged:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    Vec3 delta(event.deltaX, -event.deltaY, event.deltaZ);
    
    if (event.type == NSLeftMouseDragged) {
        eventQueue->OnPointerDrag(0, location, delta);
    } else if (event.type == NSRightMouseDragged) {
        eventQueue->OnPointerDrag(1, location, delta);
    }
}
*/

- (void)scrollWheel:(NSEvent *)theEvent {
	if (theEvent.type == NSScrollWheel) {
        NSLog(@"Scroll Wheel");
        
       //mouseValues.scrollWheel.clickState = clickstate_moving;
		//mouseValues.scrollWheel.xClickDown = [theEvent deltaX];
		//mouseValues.scrollWheel.yClickDown = [theEvent deltaY];
		
		//printf("%3.2f, %3.2f\n", [theEvent deltaX], [theEvent deltaY]);
	}
}


/*
 * Cleanup
 */
- (void) dealloc {
	[super dealloc];
}
@end
