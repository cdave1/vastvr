#include "ControlRelease.h"

using namespace hd::input;

hd::input::gamepad::ControlRelease::ControlRelease(unsigned int ID) : Component(ID),
    m_deviceId(hd::input::UnspecifiedDeviceId),
    m_control(hd::input::gamepad::Control::UNKNOWN_CONTROL)
{}


int hd::input::gamepad::ControlRelease::GetDeviceId() const {
    return m_deviceId;
}


hd::input::gamepad::Control hd::input::gamepad::ControlRelease::GetControl() const {
    return m_control;
}


void hd::input::gamepad::ControlRelease::SetDeviceId(unsigned int deviceId) {
    m_deviceId = deviceId;
}


void hd::input::gamepad::ControlRelease::SetControl(hd::input::gamepad::Control control) {
    m_control = control;
}