#include "EntityPool.h"
#include <Vast/VastMath.h>

using namespace hd;

const unsigned int kEntityQueryPoolSize = 256;

EntityPool::EntityPool(const uint32_t capacity) :
  m_entities(capacity),
  m_freeEntities(capacity),
  m_usedEntities(capacity),
  m_capacity(capacity),
  m_nullEntityRef(nullptr),
  m_queries(kEntityQueryPoolSize),
  m_freeQueries(kEntityQueryPoolSize),
  m_nullQuery(nullptr) {
    hdAssert(hd::NullEntityIdentifier > 0);
    m_capacity = hd::clamp(capacity, uint32_t(0), hd::NullEntityIdentifier - 1);

    Entity *entityMemory;
    if (NULL == (entityMemory = (Entity *)calloc(1, sizeof(Entity) * m_capacity))) {
        return;
    }

    for (EntityIdentifier i = 0; i < m_capacity; ++i) {
        Entity *entity = new (&entityMemory[i]) Entity(i);
        m_entities.Add(entity);
        m_freeEntities.Add(entity);
    }

    EntityQuery *entityQueryMemory;
    if (NULL == (entityQueryMemory = (EntityQuery *)calloc(1, sizeof(EntityQuery) * kEntityQueryPoolSize))) {
        return;
    }

    m_nullEntityRef = new Entity(hd::NullEntityIdentifier);

    for (int i = 0; i < kEntityQueryPoolSize; ++i) {
        EntityQuery *query = new (&entityQueryMemory[i]) EntityQuery(capacity);
        m_queries.Add(query);
        m_freeQueries.Add(query);
    }

    m_nullQuery = new EntityQuery(0);
}


//
// Clear all the queries.
//
void EntityPool::Update() {
    m_freeQueries.Clear();
    for (uint32_t i = 0; i < m_queries.Size(); ++i) {
        EntityQuery *query = m_queries.At(i);
        query->Clear();
        m_freeQueries.Add(query);
    }
}


bool EntityPool::Available() {
    return m_freeEntities.Size() > 0;
}


bool EntityPool::Empty() {
    return m_freeEntities.IsEmpty();
}


Entity * EntityPool::Create(hd::EntityGroup *group) {
    hdAssert(m_freeEntities.Size() <= m_entities.Size());
    hdAssert(m_usedEntities.Size() + m_freeEntities.Size() == m_entities.Size());

    if (m_freeEntities.IsEmpty()) {
        return m_nullEntityRef;
    }

    Entity *entity = m_freeEntities.Top();
    m_freeEntities.Pop();
    m_usedEntities.Add(entity);
    return entity;
}


bool EntityPool::Free(Entity * entity) {
    hdAssert(m_freeEntities.Size() <= m_entities.Size());

    if (entity == nullptr) {
        return false;
    }

    if (m_freeEntities.Size() == m_entities.Size()) {
        return false;
    }

    if (entity < m_entities.At(0) ||
        entity > m_entities.At(m_entities.Size() - 1)) {
        hd::log::e("Entity is not part of this pool!");
        return false;
    }
    m_usedEntities.Remove(entity);

    entity->Reset();
    m_freeEntities.Add(entity);

    hdAssert(m_usedEntities.Size() + m_freeEntities.Size() == m_entities.Size());

    return true;
}


void EntityPool::FreeAll() {
    m_usedEntities.Clear();
    m_freeEntities.Clear();

    for (uint32_t i = 0; i < m_entities.Size(); ++i) {
        Entity *entity = m_entities.At(i);
        entity->Reset();
        m_freeEntities.Add(entity);
    }

    hdAssert(m_usedEntities.Size() + m_freeEntities.Size() == m_entities.Size());
}


void EntityPool::PrintDebugInfo() {
    hd::log::i("%d free entities available in this pool with %d free queries.\n",
               m_freeEntities.GetSize(), m_freeQueries.GetSize());
}


const hd::Vector<Entity *> * EntityPool::GetEntities() const {
    return &m_usedEntities;
}


hd::EntityQuery * EntityPool::QueryByName(const std::string &name) {
    hd::EntityQuery *query = GetQuery();
    if (query == m_nullQuery) {
        hd::log::w("[%s] Warning: null query returned.  Are you caching too many queries?\n", __FUNCTION__);
        query = m_nullQuery;
        query->Clear();
    }

    hdAssert(query->Size() == 0);

    for (uint32_t i = 0; i < m_usedEntities.Size(); ++i) {
        Entity *entity = m_usedEntities.At(i);
        if (entity->GetName() == name) {
            query->AddEntity(entity);
        }
    }
    return query;
}


bool EntityPool::FreeQuery(EntityQuery *query) {
    hdAssert(m_freeQueries.Size() <= m_queries.Size());

    if (query == nullptr) {
        return false;
    }

    if (m_freeQueries.Size() == m_queries.Size()) {
        return false;
    }
    if (query < m_queries.At(0) || query > m_queries.Top()) {
        return false;
    }

    query->Clear();
    m_freeQueries.Add(query);

    return true;
}


EntityQuery * EntityPool::GetQuery() {
    hdAssert(m_freeQueries.Size() <= m_queries.Size());

    if (m_freeQueries.IsEmpty()) {
        return m_nullQuery;
    }

    EntityQuery *query = m_freeQueries.Top();
    m_freeQueries.Pop();
    if (m_freeQueries.Size() == 0) {
        hd::log::w("[%s] Warning: 0 queries left on the pool!\n", __FUNCTION__);
    }
    return query;
}
