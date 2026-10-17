#include "Entity.h"

const EntityIdentifier hd::NullEntityIdentifier = ULONG_MAX;

std::string hd::Entity::GetName() const {
    return m_name;
}


void hd::Entity::SetName(const std::string &name) {
    m_name = name;
}


const EntityIdentifier hd::Entity::GetID() const {
    return m_ID;
}


const hdUuid hd::Entity::GetUUID() const {
    return m_uuid;
}


void hd::Entity::PrintUUID() const {
#ifdef WIN32

#else
    char uuidStr[40];
    uuid_unparse(* m_uuid, uuidStr);
    hd::log::i("%s\n", uuidStr);
#endif
}


void hd::Entity::PrintDebugInfo() {
    hd::log::i("Entity with ID: %lu\n", m_ID);
    //for (std::unordered_map<ComponentTypeValue, Component *>::iterator it = m_components.begin();
    //    it != m_components.end(); ++it) {
    //    Component *component = it->second;
        //hd::log::i("Component (type: %s) with ID: %lu\n", it->first.c_str(), component->GetID());
    //}
}


#pragma mark - Private constructors

hd::Entity::Entity(EntityIdentifier ID) :
  m_ID(ID),
  m_name(),
  m_uuid(),
  m_components(64),
  m_componentMap(64) {
    // TODO: Put UUID creation stuff into common/compat
#ifdef WIN32
    UuidCreate(&m_uuid);
#else
    if (!(m_uuid = (uuid_t *)calloc(1, sizeof(uuid_t)))) {
        hd::log::e("Calloc\n");
        hdAssert(false);
    }
    uuid_generate(* m_uuid);
#endif
}


void hd::Entity::AddComponent(ComponentTypeValue key, hd::Component *component) {
    //m_components.insert(m_components.begin(), std::pair<ComponentTypeValue, Component *>(key, component));
    m_components.Add(component, uint32_t(key));
    m_componentMap.Add(component);
}


hd::Component * hd::Entity::GetComponent(ComponentTypeValue key) const {
    return m_components.At(uint32_t(key));
}


void hd::Entity::Reset() {
    for (uint32_t i = 0; i < m_componentMap.Size(); ++i) {
        Component *component = m_componentMap.At(i);
        component->Free();
    }
    m_name = "";
    m_componentMap.Clear();
    m_components.Clear();
}
