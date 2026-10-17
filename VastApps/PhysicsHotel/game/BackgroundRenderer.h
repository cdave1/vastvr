#ifndef _BACKGROUND_RENDERER_H_
#define _BACKGROUND_RENDERER_H_

#include "hdk.h"
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd {
    class EntityPool;
}

class BackgroundRenderer : public hd::EntitySystem {
public:
    
    BackgroundRenderer(hd::EntityPool *entityPool);
    
    void Update();
    
    float m_time;
    
};

#endif