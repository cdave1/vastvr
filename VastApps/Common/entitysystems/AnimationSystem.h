#ifndef _ANIMATION_SYSTEM_H_
#define _ANIMATION_SYSTEM_H_

#include <Vast/Vast.h>
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

using namespace hd;

class AnimationSystem : public EntitySystem
{
public:
    
    AnimationSystem(EntityPool *entityPool);
    
    void Update();
    
};

#endif