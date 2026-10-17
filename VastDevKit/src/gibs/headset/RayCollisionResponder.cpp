#include "RayCollisionResponder.h"

hd::input::RayCollisionResponder::RayCollisionResponder(unsigned int ID) : Component(ID),
    m_onRayCollision(),
    m_pointerId(hd::input::UnspecifedPointerId),
    m_isEnabled(true) {}


void hd::input::RayCollisionResponder::SetPointerId(unsigned int pointerId) {
    m_pointerId = pointerId;
}


int hd::input::RayCollisionResponder::GetPointerId() const {
    return m_pointerId;
}


void hd::input::RayCollisionResponder::Enable() {
    m_isEnabled = true;
}


void hd::input::RayCollisionResponder::Disable() {
    m_isEnabled = false;
}


bool hd::input::RayCollisionResponder::IsEnabled() const {
    return m_isEnabled;
}


void hd::input::RayCollisionResponder::RayCollision(hd::Entity *entity,
                                                    hd::input::Headset *headset,
                                                    const Collision3 &collision) {
    if (m_isEnabled) {
        if (m_onRayCollision) {
            m_onRayCollision(entity, headset, collision);
        }
    }
}
