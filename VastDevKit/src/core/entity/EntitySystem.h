#ifndef _VAST_ENTITY_SYSTEM_H_
#define _VAST_ENTITY_SYSTEM_H_

#include "core/entity/EntityPool.h"

namespace hd {
    class EntitySystem {
    public:
        
        EntitySystem(EntityPool *entityPool);
        
        ~EntitySystem();

        virtual void Update() {};
        
        
    protected:
        
        EntityPool *m_entityPool;
        
    };
}


#endif