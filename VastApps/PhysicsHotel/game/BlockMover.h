#ifndef _BLOCK_MOVER_H_
#define _BLOCK_MOVER_H_

#include "hdk.h"
//#include "core/entity/Entity.h"
//#include "physics/2d/components/Body.h"
//#include "graphics/components/Polygon.h"

using namespace hd;

class BlockMover : public EntitySystem
{
public:
    
    BlockMover(EntityPool *entityPool);
    
    void Update();
    
};

#endif