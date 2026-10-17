#include "InputResponder.h"


using namespace hd;



@implementation InputResponder

- (BOOL)acceptsFirstResponder
{
	return YES;
}


- (void)mouseDown:(NSEvent *)theEvent
{
	if (theEvent.type == NSLeftMouseDown) {
		assert(mouseValues.leftButton.clickState == clickstate_null);
		mouseValues.leftButton.clickState = clickstate_down;
		mouseValues.leftButton.clickDownLocation.Set(theEvent.locationInWindow.x,
                                                     theEvent.locationInWindow.y);
		mouseValues.leftButton.clickDownTimeStamp = theEvent.timestamp;
	}
	else if (theEvent.type == NSRightMouseDown)
	{
		assert(mouseValues.rightButton.clickState == clickstate_null);
		mouseValues.rightButton.clickState = clickstate_down;
		mouseValues.rightButton.clickDownLocation.Set(theEvent.locationInWindow.x,
                                                      theEvent.locationInWindow.y);
		mouseValues.rightButton.clickDownTimeStamp = theEvent.timestamp;		
	}
}


- (void)mouseUp:(NSEvent *)theEvent
{
	if (theEvent.type == NSLeftMouseUp)
	{
		assert(mouseValues.leftButton.clickState == clickstate_down ||
			   mouseValues.leftButton.clickState == clickstate_moving);
		mouseValues.leftButton.clickState = clickstate_up;
		mouseValues.leftButton.clickUpLocation.Set(theEvent.locationInWindow.x,
                                                   theEvent.locationInWindow.y);
		mouseValues.leftButton.clickUpTimeStamp = theEvent.timestamp;	
	}
	else if (theEvent.type == NSRightMouseDown)
	{
		assert(mouseValues.rightButton.clickState == clickstate_down ||
			   mouseValues.rightButton.clickState == clickstate_moving);
		mouseValues.rightButton.clickState = clickstate_up;
		mouseValues.rightButton.clickUpLocation.Set(theEvent.locationInWindow.x,
                                                    theEvent.locationInWindow.y);
		mouseValues.rightButton.clickDownTimeStamp = theEvent.timestamp;		
	}
}


- (void)mouseMoved:(NSEvent *)theEvent
{
	if (theEvent.type == NSMouseMoved)
	{
        mouseValues.previousLocation.Set(mouseValues.currentLocation);
        
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
	}
}


- (void)mouseDragged:(NSEvent *)theEvent
{
}


- (void)scrollWheel:(NSEvent *)theEvent
{
}

@end