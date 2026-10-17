#ifndef _VAST_ENTITY_POOL_H_
#define _VAST_ENTITY_POOL_H_

#include <list>
#include "core/entity/Entity.h"
#include "core/entity/EntityQuery.h"
#include "core/Collections/Collections.h"

namespace hd {
    class EntityGroup;

    class EntityPool {
    private:

        hd::Vector<Entity *> m_entities;
        hd::Vector<Entity *> m_freeEntities;
        hd::Vector<Entity *> m_usedEntities;
        uint32_t m_capacity;
        Entity *m_nullEntityRef;

        hd::Vector<EntityQuery *> m_queries;
        hd::Vector<EntityQuery *> m_freeQueries;

        hd::Vector<EntityGroup *> m_entityGroups;
        hd::Vector<EntityGroup *> m_freeEntityGroups;

        EntityQuery *m_nullQuery;

    public:

        EntityPool(const uint32_t capacity);

        void Update();

        Entity * Create(hd::EntityGroup *group = nullptr);

        bool Free(Entity * entity);

        void FreeAll();

        bool Available();

        bool Empty();

        bool IsNull(Entity * ref);

        void PrintDebugInfo();

        uint32_t GetCapacity() const { return m_capacity; }

        const hd::Vector<Entity *> * GetEntities() const;

        ///
        /// Optimization note: queries by name or component type only need change when an entity
        /// is created, or one of the current entities has a component added or removed.  Because
        /// of this, we can possibly cache entity queries.
        ///
        hd::EntityQuery * QueryByName(const std::string &name);

        template <typename T>
        hd::EntityQuery * Query(const std::string &name = "");

        template <typename T1, typename T2, typename ... Types>
        hd::EntityQuery * Query(const std::string &name = "");

        EntityGroup * CreateEntityGroup(void *owner);

        bool FreeEntityGroup(EntityGroup *entityGroup);


    private:

        EntityQuery * GetQuery();

        bool FreeQuery(EntityQuery *query);

    };
}

template <typename T>
hd::EntityQuery * hd::EntityPool::Query(const std::string &name) {
    hd::EntityQuery *query = GetQuery();
    if (query == m_nullQuery) {
        return m_nullQuery;
    }

    hdAssert(query->Size() == 0);

    for (uint32_t i = 0; i < m_usedEntities.Size(); ++i) {
        Entity *entity = m_usedEntities.At(i);
        if (entity->ContainsComponent<T>()) {
            if (name.empty()) {
                query->AddEntity(entity);
            } else if (entity->GetName() == name) {
                query->AddEntity(entity);
            }
        }
    }
    return query;
}


template <typename T1, typename T2, typename ... Types>
hd::EntityQuery * hd::EntityPool::Query(const std::string &name) {
    hd::EntityQuery *query = GetQuery();
    if (query == m_nullQuery) {
        return m_nullQuery;
    }

    hdAssert(query->Size() == 0);

    for (uint32_t i = 0; i < m_usedEntities.Size(); ++i) {
        Entity *entity = m_usedEntities.At(i);
        if (entity->ContainsComponent<T1, T2, Types ...>()) {
            if (name.empty()) {
                query->AddEntity(entity);
            } else if (entity->GetName() == name) {
                query->AddEntity(entity);
            }
        }
    }
    return query;
}


#endif
