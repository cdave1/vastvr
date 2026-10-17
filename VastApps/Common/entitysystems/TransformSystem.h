#ifndef _TRANSFORM_SYSTEM_H_
#define _TRANSFORM_SYSTEM_H_

#include <Vast/Vast.h>
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

using namespace hd;

class TransformSystem : public EntitySystem {
public:
    
    TransformSystem(EntityPool *entityPool);
    
    void Update();
    
};

#endif