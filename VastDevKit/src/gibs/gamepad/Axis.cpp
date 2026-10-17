#include "Axis.h"

hd::input::gamepad::Axis::Axis() :
    m_control(hd::input::gamepad::Control::UNKNOWN_CONTROL),
    m_direction(),
    m_state(hd::input::gamepad::AxisState::Inactive) {}


hd::input::gamepad::Axis::Axis(hd::input::gamepad::Control control, const Vec2 &direction) :
    m_control(control),
    m_direction(direction),
    m_state(hd::input::gamepad::AxisState::Inactive) {}


hd::input::gamepad::Axis::~Axis() {
}


void hd::input::gamepad::Axis::SetDirection (const Vec2 &direction) {
    m_direction = direction;
}


hd::input::gamepad::Control hd::input::gamepad::Axis::GetControl() const {
    return m_control;
}


const Vec2 & hd::input::gamepad::Axis::GetDirection() const {
    return m_direction;
}


hd::input::gamepad::AxisState hd::input::gamepad::Axis::GetState() const {
    return m_state;
}


void hd::input::gamepad::Axis::SetState(hd::input::gamepad::AxisState state) {
    m_state = state;
}
