#ifndef _VAST_COMPONENT_COMPONENTS_H_
#define _VAST_COMPONENT_COMPONENTS_H_

#include <unordered_map>
#include <string>
#include "core/component/Component.h"
#include "core/component/ComponentPool.h"

namespace hd {
    class Components {
    private:
        
        std::unordered_map<ComponentTypeValue, hd::AbstractComponentPool *> m_pools;
        
        
    public:
        template<class T>
        static hd::ComponentPool<T> * GetPool();
        
        template<class T>
        static T * Get();
        
        template<class T>
        static bool CreatePool(uint32_t size);
        
        template<class T>
        static bool CreateSmallPool();
        
        template<class T>
        static bool CreateMediumPool();
        
        template<class T>
        static bool CreateLargePool();

        template<class T>
        static bool IsNull(T *component);
        
        template<class T>
        static bool Free(T *component);
        
        static hd::Components * Instance();
        
        
    private:
        
        Components();
        
        ~Components();
        
        hd::AbstractComponentPool * FindComponentPool(ComponentTypeValue key);
        
        bool AddComponentPoolInternal(ComponentTypeValue key,
                                      hd::AbstractComponentPool *componentPool);
        
    };
}


template<class T>
hd::ComponentPool<T> * hd::Components::GetPool() {
    ComponentTypeValue key = ComponentType::value<T>();
    AbstractComponentPool *pool = hd::Components::Instance()->FindComponentPool(key);
    
    if (pool) {
        return static_cast<ComponentPool<T> *>(pool);
    } else {
        return nullptr;
    }
}


template<class T>
T * hd::Components::Get() {
    hd::ComponentPool<T> *pool = hd::Components::GetPool<T>();
    if (pool) {
        return pool->Get();
    } else {
        return nullptr;
    }
}


template<class T>
bool hd::Components::CreatePool(uint32_t size) {
    ComponentPool<T> *pool = new ComponentPool<T>(size);
    ComponentTypeValue key = ComponentType::value<T>();
    return hd::Components::Instance()->AddComponentPoolInternal(key, static_cast<AbstractComponentPool *>(pool));
}


template<class T>
bool hd::Components::CreateSmallPool() {
    return hd::Components::CreatePool<T>(32);
}


template<class T>
bool hd::Components::CreateMediumPool() {
    return hd::Components::CreatePool<T>(256);
}


template<class T>
bool hd::Components::CreateLargePool() {
    return hd::Components::CreatePool<T>(4096);
}


template<class T>
bool hd::Components::IsNull(T *component) {
    hd::ComponentPool<T> *pool = hd::Components::GetPool<T>();
    if (pool) {
        return pool->IsNull(component);
    } else {
        return false;
    }
}


template<class T>
bool hd::Components::Free(T *component) {
    hd::ComponentPool<T> *pool = hd::Components::GetPool<T>();
    if (pool) {
        return pool->Free(component);
    } else {
        return false;
    }
}

#endif
