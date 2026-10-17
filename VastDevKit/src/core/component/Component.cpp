#include "Component.h"
#include "core/component/AbstractComponentPool.h"

const ComponentIdentifier hd::NullComponentIdentifier = ULONG_MAX;


ComponentTypeValue hd::ComponentType::typeValueCounter = 0;


ComponentIdentifier hd::Component::GetID() const {
    return m_ID;
}


void hd::Component::PrintDebugInfo() const {
    hd::log::i("Component with ID: %lu\n", m_ID);
}


void const hd::Component::Free() {
    m_isFree = true;
    if (m_componentPool) {
        m_componentPool->Free(this);
    }
}


bool hd::Component::IsFree() const {
    return m_isFree;
}


bool hd::Component::IsNullComponent() const {
    return m_ID == NullComponentIdentifier;
}


hd::Component::Component(ComponentIdentifier ID) :
    m_ID(ID),
    m_isFree(true),
    m_componentPool(nullptr) {
}


hd::Component::~Component() {}


void hd::Component::Reset() {}



void hd::Component::SetComponentPool(AbstractComponentPool *componentPool) {
    m_componentPool = componentPool;
}


void hd::Component::SetBusy() {
    m_isFree = false;
}
