#ifndef _HUD_RENDERER_H_
#define _HUD_RENDERER_H_

#include "hdk.h"
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd
{
    class EntityPool;
}

class HudRenderer : public hd::EntitySystem
{
public:
    
    HudRenderer(hd::EntityPool *entityPool);
    
    void Update();
    
};

#endif