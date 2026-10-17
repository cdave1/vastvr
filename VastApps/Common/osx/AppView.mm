#import "AppView.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#import <OpenGL/glu.h>

using namespace hd;


@implementation AppView

- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format {
    self = [super initWithFrame:frameRect pixelFormat:format];
    if (self) {
        [self setWantsBestResolutionOpenGLSurface:YES];
    }
    return self;
}


- (void)prepareOpenGL {

    // this sets swap interval for double buffering
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

    [[self window] setAcceptsMouseMovedEvents:YES];

    timer = [NSTimer timerWithTimeInterval:0.001
                                    target:self
                                  selector:@selector(timerEvent:)
                                  userInfo:nil
                                   repeats:YES];

    updateTimer = [NSTimer timerWithTimeInterval:0.001
                                          target:self
                                        selector:@selector(updateTimerEvent:)
                                        userInfo:nil
                                         repeats:YES];

    [[NSRunLoop mainRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
    //[[NSRunLoop mainRunLoop] addTimer:updateTimer forMode:NSDefaultRunLoopMode];

    glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(1.0f);
    //glEnable(GL_MULTISAMPLE);
    glEnable(GL_VERTEX_ARRAY);
     glEnable(GL_DEPTH_TEST);
     glDepthFunc(GL_LEQUAL);
     glDepthMask(TRUE);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}


- (void)timerEvent:(NSTimer *)t {
    //[self setNeedsDisplay:YES];
    [renderProxy update];
    [self render];
}


- (void) updateTimerEvent:(NSTimer *)t {
    //[self setNeedsDisplay:YES];

}


- (BOOL)acceptsFirstResponder
{
    return NO;
}
- (BOOL)becomeFirstResponder
{
    return YES;
}
///-(BOOL)acceptsFirstResponder { return YES; }


- (void) setRenderProxy:(id)proxy {
    renderProxy = proxy;
}


- (void) setClient:(hd::application::Client *)client {
    m_client = client;
}


- (void) render {
    NSRect backingBounds = [self convertRectToBacking:[self bounds]];
    GLsizei backingPixelWidth  = (GLsizei)(backingBounds.size.width),
    backingPixelHeight = (GLsizei)(backingBounds.size.height);

    [[self openGLContext] clearDrawable];
    [[self openGLContext] setView:self];
    [[self openGLContext] makeCurrentContext];
    
    // Set viewport
    glViewport(0, 0, backingPixelWidth, backingPixelHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    [renderProxy render];

    [[self openGLContext] flushBuffer];
}


static bool r = false;
- (void) drawRect: (NSRect) bounds {
    [self render];
}


- (void)keyDown:(NSEvent *)event {
    if (!m_client) {
        return;
    }
    
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    eventQueue->OnKeySelect(0, unicodeKey);
}


- (void)keyUp:(NSEvent *)event {
    if (!m_client) {
        return;
    }
    
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    eventQueue->OnKeyRelease(0, unicodeKey);
}


- (void)mouseDown:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    
	if (event.type == NSLeftMouseDown) {
        eventQueue->OnPointerSelect(hd::input::pointer::Button::LEFT_BUTTON, hd::input::pointer::Control::MOUSE, location);
	} else if (event.type == NSRightMouseDown) {
        eventQueue->OnPointerSelect(hd::input::pointer::Button::RIGHT_BUTTON, hd::input::pointer::Control::MOUSE, location);
	}
}


- (void)mouseUp:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    
    if (event.type == NSLeftMouseUp) {
        eventQueue->OnPointerRelease(hd::input::pointer::Button::LEFT_BUTTON, hd::input::pointer::Control::MOUSE, location);
    } else if (event.type == NSRightMouseUp) {
        eventQueue->OnPointerRelease(hd::input::pointer::Button::RIGHT_BUTTON, hd::input::pointer::Control::MOUSE, location);
    }
}


- (void) mouseEntered:(NSEvent *)theEvent {
    
}


- (void) mouseExited:(NSEvent *)theEvent {
    
}


- (void)mouseMoved:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);

    Vec3 delta(event.deltaX, -event.deltaY, event.deltaZ);

    if (event.type == NSMouseMoved) {
        eventQueue->OnPointerMove(hd::input::pointer::Control::MOUSE, location, delta);
    }
    
    
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


- (void)mouseDragged:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    Vec3 delta(event.deltaX, -event.deltaY, event.deltaZ);
    
    if (event.type == NSLeftMouseDragged) {
        eventQueue->OnPointerDrag(hd::input::pointer::Button::LEFT_BUTTON, hd::input::pointer::Control::MOUSE, location, delta);
    } else if (event.type == NSRightMouseDragged) {
        eventQueue->OnPointerDrag(hd::input::pointer::Button::RIGHT_BUTTON, hd::input::pointer::Control::MOUSE, location, delta);
    }
}


- (void)scrollWheel:(NSEvent *)event {
    if (!m_client) return;

    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();

    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    Vec3 delta(event.deltaX, -event.deltaY, event.deltaZ);

	if (event.type == NSScrollWheel) {
        NSLog(@"Scroll Wheel");

        eventQueue->OnPointerMove(hd::input::pointer::Control::PRIMARY_SCROLL_WHEEL, location, delta);

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
