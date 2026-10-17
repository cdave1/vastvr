#ifndef _VAST_ENTITY_GROUP_H_
#define _VAST_ENTITY_GROUP_H_

#include "core/entity/EntityPool.h"

/*
 EntityGroup contains entity references for entities owned by a particular context.
 
 The group provides access to the underlying entity pool, so the group functions like
 an entity pool, in effect.
 */

namespace hd {
    class EntityGroup {
    private:
        
        hd::Vector<Entity *> m_entities;
        
        
    public:
        
        Entity * Create();
        
        bool Free(Entity * entity);
        
        unsigned int Size() const;
        
        Entity * At(const unsigned int index) const;
        
        
    private:
        
        EntityGroup(EntityPool *entityPool);
        
        ~EntityGroup();
        
        void AddEntity(Entity *entity);
        
        void Clear();
        
        const hd::Vector<Entity *> * Entities() const;
        
        
    protected:
        
        EntityPool *m_entityPool;
        
    };
}


#endif