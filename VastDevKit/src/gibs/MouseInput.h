#ifndef _VAST_MOUSE_INPUT
#define _VAST_MOUSE_INPUT

#include <Vast/VastMath.h>

namespace hd
{
    typedef enum clickstate
    {
        clickstate_null = 0,
        clickstate_down = 1,
        clickstate_moving = 2,
        clickstate_up = 3,
        clickstate_scrollup = 4,
        clickstate_scrolldown = 5,
    }
    clickstate_t;
    
    
    typedef struct clickinfo
    {
        clickinfo() :
		clickDownLocation(),
		clickUpLocation(),
		clickDownTimeStamp(0),
		clickUpTimeStamp(0),
		clickState(clickstate_null),
		clickCount(0)
        {}
        
        Vec2 clickDownLocation;
        Vec2 clickUpLocation;
        
        double clickDownTimeStamp;
        double clickUpTimeStamp;
        clickstate_t clickState;
        uint32 clickCount;
    }
    clickinfo_t;
    
    
    typedef struct mousevalues
    {
        mousevalues() :
		currentLocation(),
		previousLocation()
        {}
        
        Vec2 currentLocation;
        Vec2 previousLocation;
        
        clickinfo_t leftButton;
        
        clickinfo_t rightButton;
        
        clickinfo_t scrollWheel;
    } 
    mousevalues_t;
    
    
    extern mousevalues_t GetMouseValues();
    
}



#endif
