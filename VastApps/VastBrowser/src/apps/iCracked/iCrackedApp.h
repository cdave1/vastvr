#ifndef _VAST_BROWSER_ICRACKED_APP_H_
#define _VAST_BROWSER_ICRACKED_APP_H_

#include "AppPrototype.h"

class iCrackedApp : public AppPrototype {
public:
    iCrackedApp(hd::application::Client *client);
        
    ~iCrackedApp();
    
    void CreateApp();
    
};

#endif