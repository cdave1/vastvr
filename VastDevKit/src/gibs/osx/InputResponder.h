#ifndef _VAST_OSX_MOUSE_INPUT
#define _VAST_OSX_MOUSE_INPUT

#import <Cocoa/Cocoa.h>
#include "MouseInput.h"

@interface InputResponder : NSResponder
{
    hd::mousevalues_t mouseValues;
}



@end


#endif