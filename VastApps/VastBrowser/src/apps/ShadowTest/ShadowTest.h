#ifndef _APPS_SHADOW_TEST_H_
#define _APPS_SHADOW_TEST_H_

#include "AppPrototype.h"

class ShadowTest : public AppPrototype {
public:
    ShadowTest(hd::application::Client *client);
    
    ~ShadowTest();
    
    void Create();
    
};

#endif
