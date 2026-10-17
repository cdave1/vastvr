#ifndef _VAST_ENTITY_H_
#define _VAST_ENTITY_H_

#include <string>
#include <unordered_map>
#include "core/collections/Collections.h"
#include "core/component/Components.h"
#include "core/component/Component.h"

#define kEntityUUIDSize 40

typedef uint32_t EntityIdentifier;

/**
 * An entity is a collection of components that represents something in the game space.
 *
 * Some restrictions on entities:
 * - Can only be instantiated by an entity pool.
 * - Only components maintain state information.
 * - The entity does not maintain any reference to, or copy of, the proxy data used to
 *   create the entity.
 * - Entity pointers can not be directly destroyed, they can only be marked as "no longer
 *   in use" via the entity pool.
 * - Entities on their own do not do anything.  The components are responsible for
 *   carrying data about the entity in question.  Entity Systems are responsible for
 *   passing messages between components for a particular entity.
 * - Entities should NOT be subclassed.
 */

namespace hd {
    extern const EntityIdentifier NullEntityIdentifier;

    class EntityPool;

    class Entity {
    private:

        EntityIdentifier m_ID;

        std::string m_name;

        hdUuid m_uuid;

        hd::Vector<Component *> m_componentMap;

        hd::Array<Component *> m_components;


    public:

        std::string GetName() const;

        void SetName(const std::string &name);

        const EntityIdentifier GetID() const;

        const hdUuid GetUUID() const;

        void PrintUUID() const;

        void PrintDebugInfo();

        template<typename T>
        T * AddComponent();

        template<typename T>
        T * Add();

        template<typename T>
        bool AddComponent(T *component);

        template<typename T>
        const T * GetComponent() const;

        template<typename T>
        const T * Get() const;

        template<typename T>
        T * Edit() const;

        template<typename T>
        T * EditComponent() const;

        template<typename T>
        bool ContainsComponent();

        template<typename T1, typename T2, typename ... Types>
        bool ContainsComponent();

        template<typename T>
        bool Remove();


    protected:

        friend class EntityPool;

        Entity(EntityIdentifier ID);

        void AddComponent(ComponentTypeValue key, Component *component);

        Component * GetComponent(ComponentTypeValue key) const;

        void Reset();

    };
}


template<typename T>
T * hd::Entity::AddComponent() {
    ComponentTypeValue key = ComponentType::value<T>();
    if (GetComponent(key) == nullptr) {
        T *component = hd::Components::Get<T>();
        AddComponent(key, component);
        return component;
    } else {
        return this->Edit<T>();
    }
}


template<typename T>
T * hd::Entity::Add() {
    ComponentTypeValue key = ComponentType::value<T>();
    if (GetComponent(key) == nullptr) {
        T *component = hd::Components::Get<T>();
        AddComponent(key, component);
        return component;
    } else {
        return this->Edit<T>();
    }
}


template<typename T>
bool hd::Entity::AddComponent(T *component) {
    ComponentTypeValue key = ComponentType::value<T>();

    if (GetComponent(key) == nullptr) {
        AddComponent(key, component);
        return true;
    }
    return false;
}


template<typename T>
const T * hd::Entity::GetComponent() const {
    Component *component = GetComponent(ComponentType::value<T>());
    if (component == nullptr) {
        return nullptr;
    } else {
        return static_cast<T *>(component);
    }
}


template<typename T>
const T * hd::Entity::Get() const {
    Component *component = GetComponent(ComponentType::value<T>());
    if (component == nullptr) {
        return nullptr;
    } else {
        return static_cast<T *>(component);
    }
}


template<typename T>
T * hd::Entity::Edit() const {
    Component *component = GetComponent(ComponentType::value<T>());
    if (component == nullptr) {
        return nullptr;
    } else {
        return static_cast<T *>(component);
    }
}


template<typename T>
T * hd::Entity::EditComponent() const {
    Component *component = GetComponent(ComponentType::value<T>());
    if (component == nullptr) {
        return nullptr;
    } else {
        return static_cast<T *>(component);
    }
}


template<typename T>
bool hd::Entity::ContainsComponent() {
    ComponentTypeValue key = ComponentType::value<T>();
    return m_components.At(uint32_t(key)) != nullptr;
}


template<typename T1, typename T2, typename ... Types>
bool hd::Entity::ContainsComponent() {
    return ContainsComponent<T1>() && ContainsComponent<T2, Types ...>();
}


template<typename T>
bool hd::Entity::Remove() {
    ComponentTypeValue key = ComponentType::value<T>();
    Component *component = GetComponent(key);
    if (component == nullptr) {
        return false;
    } else {
        component->Free();
        m_componentMap.Remove(component);
        m_components.Remove(uint32_t(key));
        return true;
    }
}

#endif
