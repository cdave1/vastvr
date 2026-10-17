#ifndef _VAST_APPS_TEAPOT_DEMO_H_
#define _VAST_APPS_TEAPOT_DEMO_H_

#include "AppPrototype.h"

class TeapotDemo : public AppPrototype {
public:
    TeapotDemo(hd::application::Client *client);
    
    void CreateTeapotDemo();
    
    void OnUiSelect(hd::Entity *element, const Collision3 &collision);
    
    void OnUiRelease(hd::Entity *element, const Collision3 &collision);
    
};

#endif
