#include "ControlSelect.h"

using namespace hd::input;

hd::input::gamepad::ControlSelect::ControlSelect(unsigned int ID) : Component(ID),
    m_deviceId(hd::input::UnspecifiedDeviceId),
    m_control(hd::input::gamepad::Control::UNKNOWN_CONTROL)
{}


int hd::input::gamepad::ControlSelect::GetDeviceId() const {
    return m_deviceId;
}


hd::input::gamepad::Control hd::input::gamepad::ControlSelect::GetControl() const {
    return m_control;
}


void hd::input::gamepad::ControlSelect::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


void hd::input::gamepad::ControlSelect::SetControl(hd::input::gamepad::Control control) {
    m_control = control;
}