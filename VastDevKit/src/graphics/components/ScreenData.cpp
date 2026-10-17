#include "ScreenData.h"
#include "math/Geometry.h"
#include "core/device/Device.h"

using namespace hd::graphics;

ScreenData::ScreenData(unsigned int ID) : Component(ID),
    m_aabb() {
    Reset();
}


void ScreenData::Reset() {
    AABB aabb(0, 0, hd::Device::GetDisplayScreenWidth(), hd::Device::GetDisplayScreenHeight());
    m_aabb = aabb;
}


void ScreenData::SetAABB(const AABB &aabb) {
    m_aabb = aabb;
}


const AABB& ScreenData::GetAABB() const {
    return m_aabb;
}


void ScreenData::RenderOrthographic() const {
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    
    hd::gl::Orthof(m_aabb.lower[0], m_aabb.upper[0],
                   m_aabb.lower[1], m_aabb.upper[1],
                   -10000.0f, 10000.0f);
    
    //glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
}
