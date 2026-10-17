#include "EntityQuery.h"
#include "core/entity/Entity.h"

using namespace hd;

EntityQuery::EntityQuery(uint32_t capacity) : m_entities(capacity) {}


EntityQuery::~EntityQuery() {}


void EntityQuery::AddEntity(Entity *entity) {
    //hdAssert(m_entities.Contains(entity) == false);
    m_entities.Add(entity);
}


void EntityQuery::Clear() {
    m_entities.Clear();
}


unsigned int EntityQuery::Size() const {
    return m_entities.Size();
}


Entity * EntityQuery::At(const uint32_t index) const {
    return m_entities.At(index);
}


const hd::Vector<Entity *> * EntityQuery::Entities() const {
    return &m_entities;
}
