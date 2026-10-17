#ifndef _CAMERAS_H_
#define _CAMERAS_H_

#include "hdk.h"
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd {
    class EntityPool;
}

class Cameras : public hd::EntitySystem {
public:
    
    Cameras(hd::EntityPool *entityPool);
    
    void Update();
    
    void Render();
    
    hd::Entity *mainCamera;
    
};

#endif