#include "Parent.h"

hd::data::Parent::Parent(unsigned int ID) : hd::Component(ID),
m_id(hd::NullEntityIdentifier),
m_entity(nullptr) {
}


void hd::data::Parent::Reset() {
    m_id = hd::NullEntityIdentifier;
    m_entity = nullptr;
}


void hd::data::Parent::SetId(EntityIdentifier id) {
    m_id = id;
}


void hd::data::Parent::SetEntity(hd::Entity *parent) {
    m_entity = parent;
}


EntityIdentifier hd::data::Parent::GetId() const {
    return m_id;
}


hd::Entity * hd::data::Parent::GetEntity() const {
    return m_entity;
}