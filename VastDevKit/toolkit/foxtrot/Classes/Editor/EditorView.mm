//
//  EditorView.mm
//  Smashed_OSX
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "EditorView.h"
#import "LevelEditor.h"

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>

id idResponder;

mousevalues_t GetMouseValues()
{
	return [idResponder GetMouseValues];
}

@interface EditorView (InternalMethods)
- (NSOpenGLPixelFormat *) createPixelFormat:(NSRect)frame;
- (void) switchToOriginalDisplayMode;
- (BOOL) initGL;
- (void) nextFrame;

@end


@implementation EditorView


- (id)initWithFrame:(NSRect)frame pixelFormat:(NSOpenGLPixelFormat*)format
{
    if ((self = [super initWithFrame:frame pixelFormat:format]))
    {
        _lock = [[NSLock alloc] init];
        [self handleResize];
        [LevelEditor sharedInstance]->Resize(frame.size.width, frame.size.height);
        
    }
    return self;
}


/*
 * Cleanup
 */
- (void) dealloc
{
	if( runningFullScreen )
		[ self switchToOriginalDisplayMode ];
	[ originalDisplayMode release ];
	[super dealloc];
}


- (void) nextFrame
{
    [self setNeedsDisplay: YES];
    [self drawRect:[self bounds]];
}


- (void)drawRect:(NSRect)rect
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	LOCK_GL_CONTEXT;
	[[self openGLContext] makeCurrentContext];	
	[LevelEditor sharedInstance]->Draw();
	[[self openGLContext] flushBuffer];
	UNLOCK_GL_CONTEXT;
	[pool drain];
}


- (void)prepareOpenGL
{
	LOCK_GL_CONTEXT;
    glShadeModel( GL_SMOOTH );                // Enable smooth shading
	//glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );   // Black background
	//glClearDepth( 1.0f );                     // Depth buffer setup
	//glEnable( GL_DEPTH_TEST );                // Enable depth testing
	//glDepthFunc( GL_LEQUAL );                 // Type of depth test to do
	// Really nice perspective calculations
	//glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
//	[super prepareOpenGL];
    [NSTimer scheduledTimerWithTimeInterval:0.001 target:self selector:@selector(nextFrame) userInfo:nil repeats:TRUE];
	UNLOCK_GL_CONTEXT;
}


- (void)clearGLContext
{
	LOCK_GL_CONTEXT;
	[super clearGLContext];
	UNLOCK_GL_CONTEXT;
}


- (void)update
{
	LOCK_GL_CONTEXT;
	[super update];
	UNLOCK_GL_CONTEXT;
}


- (void)reshape
{
	LOCK_GL_CONTEXT;
	[LevelEditor sharedInstance]->Resize(self.frame.size.width, self.frame.size.height);
	[super reshape];
	UNLOCK_GL_CONTEXT;
}


- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent
{
	return YES;
}


- (void) handleResize
{
	[self reshape];
}


/*
 * Switch to the display mode in which we originally began
 */
- (void) switchToOriginalDisplayMode
{
	CGDisplaySwitchToMode( kCGDirectMainDisplay,
                          (CFDictionaryRef) originalDisplayMode );
	CGDisplayShowCursor( kCGDirectMainDisplay );
	CGDisplayRelease( kCGDirectMainDisplay );
}


/*
 * Are we full screen?
 */
- (BOOL) isFullScreen
{
	return runningFullScreen;
}


- (mousevalues_t)GetMouseValues
{
	memcpy(&cpyMouseValues, &mouseValues, sizeof(mousevalues_t));
	
	if (mouseValues.leftButton.isUp)
	{
		memset(&(mouseValues.leftButton), 0, sizeof(clickinfo_t));
	}
	
	if (mouseValues.rightButton.isUp)
	{
		mouseValues.rightButton.isDown = false;
		mouseValues.rightButton.isMoving = false;
		mouseValues.rightButton.isUp = false;
	}
	
	if (mouseValues.scrollWheel.isMoving)
	{
		memset(&(mouseValues.scrollWheel), 0, sizeof(clickinfo_t));
	}
	
	mouseValues.xPrevious = mouseValues.xCurrent;
	mouseValues.yPrevious = mouseValues.yCurrent;
	
	
	return cpyMouseValues;
}


- (void)rightMouseDown:(NSEvent *)theEvent
{
	LOCK_GL_CONTEXT;
	hdPrintf("Right Mouse down\n");
	[LevelEditor sharedInstance]->StartPan(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
	[super rightMouseDown:theEvent];
	UNLOCK_GL_CONTEXT;
}



- (void)rightMouseDragged:(NSEvent *)theEvent
{
	LOCK_GL_CONTEXT;
	[LevelEditor sharedInstance]->Pan(theEvent.deltaX, -theEvent.deltaY);
	[super rightMouseDragged:theEvent];
	UNLOCK_GL_CONTEXT;
}
									  

- (void)mouseDown:(NSEvent *)theEvent
{
	uint32 selectedCount = [LevelEditor sharedInstance]->GetSelectedGameObjectsCount();
	bool shiftKeyDown = (([theEvent modifierFlags] & NSShiftKeyMask) == NSShiftKeyMask);
	[LevelEditor sharedInstance]->MouseDown(shiftKeyDown, theEvent.locationInWindow.x, theEvent.locationInWindow.y);
	
	if (([LevelEditor sharedInstance]->GetSelectedGameObjectSingle()))
	{
		[[NSNotificationCenter defaultCenter] 
			 postNotificationName:kSingleItemSelectedNotification
			 object:nil];
	}
	else if (selectedCount == 1)
	{
		[[NSNotificationCenter defaultCenter] 
		 postNotificationName:kSingleItemUnselectedNotification
		 object:nil];
	}
}



- (void)mouseDragged:(NSEvent *)theEvent
{
	if (([theEvent modifierFlags] & (NSShiftKeyMask | NSControlKeyMask)) == 
			(NSShiftKeyMask | NSControlKeyMask))
	{
		[LevelEditor sharedInstance]->SetDragStyleScale();
	}
	else if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
	{
		[LevelEditor sharedInstance]->SetDragStyleRotate();
	}
	else 
	{
		[LevelEditor sharedInstance]->SetDragStyleMove();
	}
	
	[LevelEditor sharedInstance]->MouseDrag(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
}


- (void)mouseUp:(NSEvent *)theEvent
{
	bool shiftKeyDown = (([theEvent modifierFlags] & NSShiftKeyMask) == NSShiftKeyMask);
	[LevelEditor sharedInstance]->MouseUp(shiftKeyDown);
	
	if ([LevelEditor sharedInstance]->GetSelectedGameObjectsCount() > 0)
	{
		if (([LevelEditor sharedInstance]->GetSelectedGameObjectSingle()))
		{
			[[NSNotificationCenter defaultCenter] 
			 postNotificationName:kSingleItemSelectedNotification
			 object:nil];
		}
		else
		{
			[[NSNotificationCenter defaultCenter] 
			 postNotificationName:kMultipleItemsSelectedNotification
			 object:nil];	
		}
	}
}



- (void)scrollWheel:(NSEvent *)theEvent
{
	hdPrintf("Mouse scroll\n");
	if (theEvent.type == NSScrollWheel)
	{
		[LevelEditor sharedInstance]->Zoom([theEvent deltaY]);
		
		//printf("%3.2f, %3.2f\n", [theEvent deltaX], [theEvent deltaY]);
	}
}



@end
