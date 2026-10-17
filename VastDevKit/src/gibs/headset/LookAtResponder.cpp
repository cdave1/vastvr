#include "LookAtResponder.h"

hd::input::LookAtResponder::LookAtResponder(unsigned int ID) : Component(ID),
m_onLookAt(),
m_onLookAtBegin(),
m_onLookAtHover(),
m_onLookAtEnd(),
m_pointerId(hd::input::UnspecifedPointerId),
m_isEnabled(true) {}


void hd::input::LookAtResponder::SetPointerId(unsigned int pointerId) {
    m_pointerId = pointerId;
}


int hd::input::LookAtResponder::GetPointerId() const {
    return m_pointerId;
}


void hd::input::LookAtResponder::Enable() {
    m_isEnabled = true;
}


void hd::input::LookAtResponder::Disable() {
    m_isEnabled = false;
}


bool hd::input::LookAtResponder::IsEnabled() const {
    return m_isEnabled;
}


void hd::input::LookAtResponder::OnLookAt(const std::function<void(const hd::Entity *entity,
                                                                   const hd::input::Headset *headset,
                                                                   const Collision3 &collision)> &callback) {
    m_onLookAt = callback;
}


void hd::input::LookAtResponder::LookAt(const hd::Entity *entity,
                                        const hd::input::Headset *headset,
                                        const Collision3 &collision) {
    if (m_isEnabled) {
        if (m_onLookAt) {
            m_onLookAt(entity, headset, collision);
        }
    }
}


void hd::input::LookAtResponder::LookAtBegin(hd::Entity *entity, hd::input::Headset *headset, const Collision3 &collision) {
    if (m_isEnabled) {
        if (m_onLookAtBegin) {
            m_onLookAtBegin(entity, headset, collision);
        }
    }
}


void hd::input::LookAtResponder::LookAtHover(hd::Entity *entity, hd::input::Headset *headset, const Collision3 &collision) {
    if (m_isEnabled) {
        if (m_onLookAtHover) {
            m_onLookAtHover(entity, headset, collision);
        }
    }
}


void hd::input::LookAtResponder::LookAtEnd(hd::Entity *entity, hd::input::Headset *headset, const Collision3 &collision) {
    if (m_isEnabled) {
        if (m_onLookAtEnd) {
            m_onLookAtEnd(entity, headset, collision);
        }
    }
}
