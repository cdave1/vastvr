#include "EntityGroup.h"

static const int kEntityGroupSize = 1024;

hd::EntityGroup::EntityGroup(hd::EntityPool *entityPool) :
    m_entityPool(entityPool),
    m_entities(kEntityGroupSize) {}


hd::EntityGroup::~EntityGroup() {}


hd::Entity * hd::EntityGroup::Create() {
    hd::Entity *entity = m_entityPool->Create(this);
    hdAssert(m_entities.Contains(entity) == false);
    m_entities.Add(entity);
    return entity;
}


bool hd::EntityGroup::Free(hd::Entity *entity) {
    m_entities.Remove(entity);
    return m_entityPool->Free(entity);
}


unsigned int hd::EntityGroup::Size() const {
     return m_entities.Size();   
}


hd::Entity * hd::EntityGroup::At(const unsigned int index) const {
     return m_entities.At(index);
}
