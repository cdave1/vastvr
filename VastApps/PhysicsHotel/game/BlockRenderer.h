#ifndef _BLOCK_RENDERER_H_
#define _BLOCK_RENDERER_H_

#include "hdk.h"
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

namespace hd {
    class EntityPool;
}

class BlockRenderer : public hd::EntitySystem {
public:
    
    BlockRenderer(hd::EntityPool *entityPool);
    
    void Update();
    
    float m_time;
    
};

#endif