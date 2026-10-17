#include "Viewport.h"
#include "math/Geometry.h"

using namespace hd::graphics;

Viewport::Viewport(unsigned int ID) : Component(ID),
    m_aabb() {
    Reset();
}


void Viewport::Reset() {
    m_aabb = AABB(0, 0, 1, 1);
}


void Viewport::SetAABB(const AABB &aabb) {
    m_aabb = aabb;
}


const AABB& Viewport::GetAABB() const {
    return m_aabb;
}
