#include "Children.h"

hd::data::Children::Children(unsigned int ID) : hd::Component(ID),
m_children(),
m_childrenCache(){
}


void hd::data::Children::Reset() {
}


void hd::data::Children::SetChildren() {

}


hd::Vector<EntityIdentifier> hd::data::Children::GetChildren() const {
    return m_children;
}


hd::Vector<hd::Entity *> * hd::data::Children::GetCache() const {
    return m_childrenCache;
}