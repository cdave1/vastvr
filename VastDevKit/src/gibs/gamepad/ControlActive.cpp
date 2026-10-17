#include "ControlActive.h"

using namespace hd::input;

hd::input::gamepad::ControlActive::ControlActive(unsigned int ID) : Component(ID),
    m_deviceId(hd::input::UnspecifiedDeviceId),
    m_control(hd::input::gamepad::Control::UNKNOWN_CONTROL)
{}


int hd::input::gamepad::ControlActive::GetDeviceId() const {
    return m_deviceId;
}


hd::input::gamepad::Control hd::input::gamepad::ControlActive::GetControl() const {
    return m_control;
}


void hd::input::gamepad::ControlActive::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


void hd::input::gamepad::ControlActive::SetControl(hd::input::gamepad::Control control) {
    m_control = control;
}