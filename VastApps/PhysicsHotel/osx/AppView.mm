#import "AppView.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>

using namespace hd;

@interface AppView (InternalMethods)
- (BOOL) initGL;

@end

@implementation AppView


- (void) awakeFromNib {
    [[self window] setAcceptsMouseMovedEvents:YES];
}


- (id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat*)format {
    
    return self = [super initWithFrame:frameRect pixelFormat:format];
}
-(BOOL)acceptsFirstResponder { return YES; }

/*
 * Enable/disable full screen mode
 */
- (BOOL) setFullScreen:(BOOL)enableFS inFrame:(NSRect)frame {
    /*
	BOOL success = FALSE;
	NSOpenGLPixelFormat *pixelFormat;
	NSOpenGLContext *newContext;
	
	[ [ self openGLContext ] clearDrawable ];
	if( runningFullScreen )
		[ self switchToOriginalDisplayMode ];
	runningFullScreen = enableFS;
	pixelFormat = [ self createPixelFormat:frame ];
	if( pixelFormat != nil )
	{
		newContext = [ [ NSOpenGLContext alloc ] initWithFormat:pixelFormat
												   shareContext:nil ];
		if( newContext != nil )
		{
			[ super setFrame:frame ];
			[ super setOpenGLContext:newContext ];
			[ newContext makeCurrentContext ];
			if( runningFullScreen )
				[ newContext setFullScreen ];
			[ self reshape ];
			if( [ self initGL ] )
				success = TRUE;
		}
		[ pixelFormat release ];
	}
	if( !success && runningFullScreen )
		[ self switchToOriginalDisplayMode ];
	
	return success;*/
    return TRUE;
}


/*
 * Initial OpenGL setup
 */
- (BOOL) initGL { 
	glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
	
	return TRUE;
}



- (void) nextFrame {
    [self setNeedsDisplay: YES];
    [self drawRect:[self bounds]];
}


- (void) setRenderProxy:(id)proxy {
    renderProxy = proxy;
}


- (void) setClient:(hd::application::Client *)client {
    m_client = client;
}


- (void) drawRect: (NSRect) bounds {
    [renderProxy render];
}


- (void)prepareOpenGL {
    glEnable(GL_LINE_SMOOTH);
	glLineWidth(1.0f);
    
    //[NSTimer scheduledTimerWithTimeInterval:0.016 target:self selector:@selector(nextFrame) userInfo:nil repeats:TRUE];
}



/*
 * Are we full screen?
 */
- (BOOL) isFullScreen {
	return runningFullScreen;
}


- (void)keyDown:(NSEvent *)event {
    if (!m_client) {
        return;
    }
    
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    eventQueue->OnButtonDown(event.timestamp, 0, unicodeKey);
}


- (void)keyUp:(NSEvent *)event {
    if (!m_client) {
        return;
    }
    
    unsigned short unicodeKey = [[event characters] characterAtIndex:0];
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    eventQueue->OnButtonUp(event.timestamp, 0, unicodeKey);
}


- (void)mouseDown:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    
	if (event.type == NSLeftMouseDown) {
        eventQueue->OnPointerSelect(event.timestamp, 0, location);
	} else if (event.type == NSRightMouseDown) {
        eventQueue->OnPointerSelect(event.timestamp, 1, location);
	}
}


- (void)mouseUp:(NSEvent *)event {
    if (!m_client) return;
    
    hd::input::EventQueue *eventQueue = m_client->GetEventQueue();
    
    Vec3 location(event.locationInWindow.x, event.locationInWindow.y);
    
    if (event.type == NSLeftMouseUp) {
        eventQueue->OnPointerRelease(event.timestamp, 0, location);
    } else if (event.type == NSRightMouseUp) {
        eventQueue->OnPointerRelease(event.timestamp, 1, location);
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
    /*
    eventQueue->OnPointerMove
    
    if (event.type == NSLeftMouseUp) {
        eventQueue->OnPointerRelease(event.timestamp, 0, location);
    } else if (event.type == NSRightMouseUp) {
        eventQueue->OnPointerRelease(event.timestamp, 1, location);
    }*/
    
    if (location[0] > 0.0f && location[1] > 0.0f) {
       // NSLog(@"Mouse moved. %3.0f %3.0f\n", location[0], location[1]);
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
        eventQueue->OnPointerDrag(event.timestamp, 0, location, delta);
    } else if (event.type == NSRightMouseDragged) {
        eventQueue->OnPointerDrag(event.timestamp, 1, location, delta);
    }
}


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
	if( runningFullScreen )
		//[ self switchToOriginalDisplayMode ];
	[ originalDisplayMode release ];
	[super dealloc];
}
@end
