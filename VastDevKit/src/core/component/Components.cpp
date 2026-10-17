#include "Components.h"
#include "AbstractComponentPool.h"

static hd::Components *m_componentsInstance = nullptr;

hd::Components * hd::Components::Instance() {
    if (m_componentsInstance == nullptr) {
        m_componentsInstance = new hd::Components();
    }
    return m_componentsInstance;
}


hd::Components::Components() : m_pools() {
}


hd::Components::~Components() {
}


hd::AbstractComponentPool * hd::Components::FindComponentPool(ComponentTypeValue key) {
    if (m_pools.count(key) == 0) {
        return nullptr;
    }
    return m_pools[key];
}


bool hd::Components::AddComponentPoolInternal(ComponentTypeValue key,
                                              AbstractComponentPool *componentPool) {
    if (m_pools.count(key) > 0) {
        //hd::log::e("Already have a pool with the name: %s\n", key.c_str());
        return false;
    }
    
    m_pools[key] = componentPool;
    return true;
}
