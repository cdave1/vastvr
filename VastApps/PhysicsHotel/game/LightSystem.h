#ifndef _LIGHT_SYSTEM_H_
#define _LIGHT_SYSTEM_H_

#include "hdk.h"
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd
{
    class EntityPool;
}

class LightSystem : public hd::EntitySystem
{
public:
    
    LightSystem(hd::EntityPool *entityPool);
    
    void Update();
    
    void Begin();
    
    void End();
    
};

#endif
