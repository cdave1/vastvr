#ifndef _VAST_ENTITY_QUERY_H_
#define _VAST_ENTITY_QUERY_H_

#include "core/Collections/Collections.h"

/**
 * Returned by an entity pool, containing all entities in the pool that
 * satisfy the query result.
 */
namespace hd {
    class Entity;
    
    class EntityQuery {
    private:
        
        friend class EntityPool;
        
        EntityQuery(uint32_t capacity);
        
        ~EntityQuery();
        
        hd::Vector<Entity *> m_entities;
        
        void AddEntity(Entity *entity);
        
        void Clear();
        
        const hd::Vector<Entity *> * Entities() const;
        
    public:
        
        unsigned int Size() const;
        
        Entity * At(const uint32_t index) const;
        
    };
}


#endif