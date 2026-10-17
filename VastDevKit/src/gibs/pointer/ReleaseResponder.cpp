#include "ReleaseResponder.h"

using namespace hd::input;

pointer::ReleaseResponder::ReleaseResponder(unsigned int ID) : Component(ID),
    m_control(hd::input::pointer::Control::MOUSE),
    m_button(hd::input::pointer::Button::UNKNOWN_BUTTON),
    m_onRelease(),
    m_onReleaseInside(),
    m_onReleaseOutside()
{}


void pointer::ReleaseResponder::SetButton(hd::input::pointer::Button button) {
    m_button = button;
}


void pointer::ReleaseResponder::SetControl(hd::input::pointer::Control control) {
    m_control = control;
}


hd::input::pointer::Control pointer::ReleaseResponder::GetControl() const {
    return m_control;
}


hd::input::pointer::Button pointer::ReleaseResponder::GetButton() const {
    return m_button;
}


void pointer::ReleaseResponder::OnRelease(const std::function<void(hd::Entity *entity, const Collision3 &collision)> &callback) {
    m_onRelease = callback;
}


void pointer::ReleaseResponder::OnReleaseInside(const std::function<void(hd::Entity *entity, const Collision3 &collision)> &callback) {
    m_onReleaseInside = callback;
}


void pointer::ReleaseResponder::OnReleaseOutside(const std::function<void(hd::Entity *entity, const Collision3 &collision)> &callback) {
    m_onReleaseOutside = callback;
}


void pointer::ReleaseResponder::Release(hd::Entity *entity, const Collision3 &collision) {
    if (m_onRelease) {
        m_onRelease(entity, collision);
    }
}


void pointer::ReleaseResponder::ReleaseInside(hd::Entity *entity, const Collision3 &collision) {
    if (m_onReleaseInside) {
        m_onReleaseInside(entity, collision);
    }
}


void pointer::ReleaseResponder::ReleaseOutside(hd::Entity *entity, const Collision3 &collision) {
    if (m_onReleaseOutside) {
        m_onReleaseOutside(entity, collision);
    }
}